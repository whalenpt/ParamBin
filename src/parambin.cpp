

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <set>

#include "siscales.h"
#include "parambin.hpp"
#include <pwutils/pwstrings.h>



// Only children are copied, parent bin is considered root of the tree
ParamBin::ParamBin(const ParamBin& bin) : parent_bin(nullptr),
    params(bin.params),
    child_bins(),
    si_obj(bin.si_obj)
{
    for(auto it = bin.child_bins.cbegin() ; it != bin.child_bins.cend(); it++)
    {
        std::string bin_name = it->first;
        std::unique_ptr<ParamBin> child_bin(new ParamBin(*it->second));
        child_bin->parent_bin = this;
        child_bins[bin_name] = std::move(child_bin);
    }
}

ParamBin::ParamBin() : 
    parent_bin(nullptr),
    si_obj(new scales::SIscalings())
{
}


ParamBin::ParamBin(const char* FILE) : 
    parent_bin(nullptr),
    si_obj(new scales::SIscalings()) 
{
    loadParamFile(FILE);
}

ParamBin::ParamBin(std::string fileName) : 
    parent_bin(nullptr),
    si_obj(new scales::SIscalings()) 
{
    loadParamFile(fileName);
}

ParamBin::~ParamBin() 
{ }

ParamMap ParamBin::getParamMap() const {
    return params;
}

ParamBin& ParamBin::operator<<(const NamedBin& named_bin)
{
    setBin(named_bin.name(),named_bin.bin());
    return *this;
}

void ParamBin::set(const NamedBin& named_bin) 
{
    setBin(named_bin.name(),named_bin.bin());
}



//ParamMap ParamBin::getParamMap() const {
//    ParamMap map(params);
//    for(auto it = scaleMap.cbegin(); it != scaleMap.cend(); it++)
//    {
//        auto search = map.find(it->first);
//        if(search != map.end()){
//            std::string name = it->first;
//            std::string val = search->second + " [" + it->second + "]" ;
//            map.erase(search);
//            map[name] = val;
//        }
//    }
//    return map;
//}
                     

std::string ParamBin::processKey(const std::string& name)
{
    std::string key = pw::eatWhiteSpace(pw::removeSubString(name,'(',')'));
    return key;

//    std::string scale = pw::subString(name,'[',']'); 
//    if(!scale.empty())
//        scaleMap[key] = pw::eatWhiteSpace(scale);

}

template<>
void ParamBin::get(const std::string& key,double& val) const
{
    std::string strval = getStrParam(key);
    val = convertFromString<double>(strval);
//    auto it = scaleMap.find(key);
//    if(it != scaleMap.cend()){
//        val = processScale(key,it->second,val);
//    }
}

//double ParamBin::processScale(const std::string& key,const std::string& scale,double val) const
//{
//    if(si_obj->ValidScaling(scale))
//        return si_obj->ProcessScaling(scale,val);
//    if(inBin(scale))
//        return val*getDbl(scale);
//    return val;
//}


template <>
const char* convertFromString(const std::string& str)
{
    const char* val = str.c_str();
    return val;
}

template <>
std::string convertFromString(const std::string& str)
{
    return str;
}

template <>
std::string convertToString(std::string str)
{
    return str;
}

template <>
std::string convertToString(const char* cstr)
{
    std::string str(cstr);
    return str;
}


template <>
void convertFromString(const std::string& str,std::vector<std::string>& vals)
{
    vals.clear();
    vals = pw::parseString(str,',');
}

std::ifstream& readNextLine(std::ifstream& fin,std::string& line_feed) 
{
    getline(fin,line_feed);
    line_feed = pw::decommentString(line_feed,"#");
    line_feed = pw::decommentString(line_feed,"//");
    // Check that line_feed is not whitespace
    while(fin && pw::isWhitespace(line_feed)){
        getline(fin,line_feed);
        line_feed = pw::decommentString(line_feed,"#");
        line_feed = pw::decommentString(line_feed,"//");
    }
    // Check for line continuation
    while(line_feed.back() == '\\'){
        std::string st(line_feed.substr(0,line_feed.size()-1));
        std::string linecont;
        getline(fin,linecont);
        linecont = pw::decommentString(linecont,"#");
        linecont = pw::decommentString(linecont,"//");
        linecont = pw::eatWhiteSpace(linecont);
        line_feed = st + " " + linecont;
    }
    return fin;
}

void lineToNameVal(const std::string& line_feed,std::string& name,std::string& vals)
{
    std::vector<std::string> parsedParam = pw::parseString(line_feed,':');
    name = parsedParam[0];
    vals = parsedParam[1];
    name = pw::eatWhiteSpace(name);
    vals = pw::eatWhiteSpace(vals);
}


void ParamBin::loadParamFile(const char* FILE)
{
    std::ifstream fin(FILE);
    if(!fin.is_open())
    {
        fin.clear();
        throw ParamBinFileException(FILE);
    }

    // Levels determine parent/child relationships, root level is -1 with a parent=nullptr

    int level = -1;
    std::vector<int> levels;
    std::vector<ParamBin*> parents;

    levels.push_back(level);
    parents.push_back(this);

    std::string line_feed;
    while(readNextLine(fin,line_feed))
    {
        // If a colon is found, then the line contains a paramater and not a group
        if(pw::countCharacters(line_feed,':') > 0){
            std::string name,vals;
            lineToNameVal(line_feed,name,vals);
            parents.back()->set(NamedParam<std::string>(name,vals));
        } else {
            // Group found.
            std::string group_name(line_feed);
            group_name = pw::eatWhiteSpace(group_name);

            // Calculate amount of line whitespace to start string to
            // determine parent/child relationship
            level = pw::countFirstChar(line_feed," ");
            while(level <= levels.back() && parents.size() > 0){
                parents.pop_back();
                levels.pop_back();
            }
            levels.push_back(level);
            ParamBin* bin = parents.back();
            bin->setBin(group_name,std::unique_ptr<ParamBin>(new ParamBin));
            parents.push_back(bin->child_bins[group_name].get());
        }
    }
}

void ParamBin::printBin(std::ostream& os) const{

    static const std::string EMPTY_CHARS = std::string(2,' ');

    static int depth = 0;
    os << std::endl;
    auto it = params.cbegin();
    while(it != params.cend()){
        std::string name;
        for(int i = 0; i < depth; i++)
            name += EMPTY_CHARS;
        name += it->first;
//        auto scaleit = scaleMap.find(it->first);
//        if(scaleit != scaleMap.cend())
//            name += '[' + scaleit->second + ']'; 

        os << std::setiosflags(std::ios::left) << std::setw(40) << name + ":";
        os << std::setw(16) << it->second << std::endl;
        it++;
    }
    os << std::endl;
    auto bit = child_bins.cbegin();
    while(bit != child_bins.cend()){
        std::string group_name;
        for(int i = 0; i < depth; i++)
            group_name += EMPTY_CHARS;
        group_name += bit->first;
        os << group_name << std::endl;
        depth++;
        bit->second->printBin(os);
        depth--;
        bit++;
    }
}

std::ostream& operator<<(std::ostream& os,const ParamBin& bin) 
{
    bin.printBin(os);
    return os;
}

bool ParamBin::inBin(const std::string& name) const
{
    if(params.count(name) > 0 || child_bins.count(name) > 0) 
        return true;
    return false;
}

int ParamBin::size(const std::string& name) const
{
    auto it = params.find(name);
    if(it != params.cend())
        return pw::countCharacters(it->second,',') + 1;
    else
        return 0;
}

bool ParamBin::empty() const
{
    if(params.empty()  && child_bins.empty())
        return true;
    else
        return false;
}

std::string ParamBin::getStrParam(const std::string& name) const
{
    std::string strval;
    auto it = params.find(name);
    if(it != params.cend())
        strval = (*it).second;
    else
        throw ParamBinKeyException(name);
    return strval;
}

ParamBin& ParamBin::getBin(const std::string& name) 
{
    auto it = child_bins.find(name);
    if(it != child_bins.cend())
        return *it->second;
    else
        throw ParamBinKeyException(name);
}

const ParamBin& ParamBin::getBin(const std::string& name) const
{
    auto it = child_bins.find(name);
    if(it != child_bins.cend())
        return *it->second;
    else
        throw ParamBinKeyException(name);
}


bool ParamBin::clear(const std::string& name)
{
    if(inBin(name)){
        params.erase(name);
        child_bins.erase(name);
        //scaleMap.erase(name);
        return true;
    }
    else 
        return false;
}

void ParamBin::clearAll() 
{
    params.clear();
    child_bins.clear();
    //scaleMap.clear();
}


double ParamBin::getDbl(const std::string& name) const
{
    double val;
    get(name,val);
    return val;
}

int ParamBin::getInt(const std::string& nm) const
{
    int val;
    get(nm,val);
    return val;
}

std::string ParamBin::getStr(const std::string& nm) const
{
    std::string val;
    get(nm,val);
    return val;
}

std::string ParamBin::getStrL(const std::string& name) const
{
    return pw::stringLowerCase(getStr(name));
}

std::string ParamBin::getStrU(const std::string& name) const
{
    return pw::stringUpperCase(getStr(name));
}

char ParamBin::getChar(const std::string& name) const
{
    char val;
    get(name,val);
    return val;
}

bool ParamBin::getBool(const std::string& name) const
{
    std::string val;
    get(name,val);
    if(val == "on")
        return true;
    else
        return false;
}

bool ParamBin::getBoolF(const std::string& name) const
{
    if(inBin(name))
        return getBool(name);
    else
        return false;
}

bool ParamBin::getBoolT(const std::string& name) const
{
    if(inBin(name))
        return getBool(name);
    else
        return true;
}

std::vector<double> ParamBin::getDblVec(const std::string& name) const
{
    std::vector<double> vals;
    get(name,vals);
    return vals;
}

std::vector<int> ParamBin::getIntVec(const std::string& name) const
{
    std::vector<int> vals;
    get(name,vals);
    return vals;
}

std::vector<std::string> ParamBin::getStrVec(const std::string& name) const
{
    std::vector<std::string> vals;
    get(name,vals);
    return vals;
}

void ParamBin::setBool(const std::string& name,bool val){
    set(NamedParam<std::string>(name,(val ? "on" : "off")));
}

void ParamBin::set(const std::string& name,const ParamBin& bin)
{
    setBin(name,bin);
} 
 
// Pass by value requires constructing a new ParamBin with values copied in
void ParamBin::setBin(const std::string& name,const ParamBin& bin)
{
    std::unique_ptr<ParamBin> child_bin(new ParamBin(bin));
    child_bin->parent_bin = this;
    child_bins[name] = std::move(child_bin);
}

// Pass by pointers simply transfers ownership of pointer
void ParamBin::setBin(const std::string& name,std::unique_ptr<ParamBin> bin)
{
    bin->parent_bin = this;
    child_bins[name] = std::move(bin);
}

std::vector<std::string> ParamBin::inBin(const std::vector<std::string>& nameVec) const
{
    std::vector<std::string> good_names;
    for(unsigned int i = 0; i < nameVec.size(); i++){
        if(inBin(nameVec[i]))
            good_names.push_back(nameVec[i]);
    }
    return good_names;
}


std::vector<std::string> ParamBin::notInBin(const std::vector<std::string>& nameVec) const
{
    std::vector<std::string> bad_names;
    for(unsigned int i = 0; i < nameVec.size(); i++){
        if(!inBin(nameVec[i]))
            bad_names.push_back(nameVec[i]);
    }
    return bad_names;
}




