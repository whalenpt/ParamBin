

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <set>

#include "siscales.h"
#include "parambin.hpp"
#include <pwutils/pwstrings.h>

namespace pw{


const std::string ParamBin::EMPTY_CHARS = std::string(2,' ');

ParamMap ParamBin::getParamMap() const {
    ParamMap map(params);
    for(auto it = scaleMap.cbegin(); it != scaleMap.cend(); it++)
    {
        auto search = map.find(it->first);
        if(search != map.end()){
            std::string name = it->first;
            std::string val = search->second + " [" + it->second + "]" ;
            map.erase(search);
            map[name] = val;
        }
    }
    return map;
}


ParamBin::ParamBin() : si_obj(new scales::SIscalings()) {} 
ParamBin::ParamBin(const char* FILE) : 
    si_obj(new scales::SIscalings()) 
{
    loadParamFile(FILE);
}
ParamBin::ParamBin(std::string fileName) : 
    si_obj(new scales::SIscalings()) 
{
    loadParamFile(fileName);
}

std::string ParamBin::processKey(const std::string& name)
{
    std::string key = pw::removeSubString(name,'[',']');
    key = pw::removeSubString(key,'(',')');
    key = pw::eatWhiteSpace(key);

    auto pit = params.find(key);
    if(pit != params.cend())
        params.erase(pit);
    auto scaleit = scaleMap.find(key);
    if(scaleit != scaleMap.cend())
        scaleMap.erase(scaleit);

    std::string scale = pw::subString(name,'[',']'); 
    if(!scale.empty())
        scaleMap[key] = pw::eatWhiteSpace(scale);

    return key;
}

template<>
void ParamBin::get(const std::string& key,double& val) const
{
    std::string strval = getStrParam(key);
    val = convertFromString<double>(strval);
    auto it = scaleMap.find(key);
    if(it != scaleMap.cend()){
        val = processScale(key,it->second,val);
    }
}

double ParamBin::processScale(const std::string& key,const std::string& scale,double val) const
{
    if(si_obj->ValidScaling(scale))
        return si_obj->ProcessScaling(scale,val);
    if(inBin(scale))
        return val*getDbl(scale);
    return val;
}


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
    vals = parseString(str,',');
}

std::string ParamBin::readNextLine(std::ifstream& fin) const
{
    std::string line_feed;
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
    return line_feed;
}

ParamBin ParamBin::readNextGroup(std::string& line_feed,std::ifstream& fin){
    ParamBin group;
    line_feed = readNextLine(fin);
    while(fin && pw::countCharacters(line_feed,':') > 0){
        group.setParam(line_feed);
        line_feed = readNextLine(fin);
    }
    return group;
}


void ParamBin::setParam(std::string& line_feed)
{
    std::vector<std::string> parsedParam = pw::parseString(line_feed,':');
    std::string name = parsedParam[0];
    std::string vals = parsedParam[1];
    name = pw::eatWhiteSpace(name);
    vals = pw::eatWhiteSpace(vals);
    set(name,vals);
}

void ParamBin::loadParamFile(const char* FILE)
{
    std::ifstream fin(FILE);
    if(!fin.is_open())
    {
        fin.clear();
        throw ParamBinFileException(FILE);
    }

    BinMap gn_map;
    using mIntStrMap = std::multimap<int,std::string>;

    mIntStrMap gn_level_map;
    std::set<int> gn_levels;
    std::vector<std::string> group_names;

    std::vector<int> levels;
    levels.push_back(-1);
    int cLev;

    std::string line_feed = readNextLine(fin);
    while(fin){
        if(pw::countCharacters(line_feed,':') > 0){
            setParam(line_feed);
            line_feed = readNextLine(fin);
        }
        else{
            
            cLev = countFirstChar(line_feed," ");

            std::string group_name(line_feed);
            group_name = pw::eatWhiteSpace(group_name);

            if(cLev > levels.back())
                levels.push_back(cLev);
            else if(cLev == levels.back())
                group_names.pop_back();
            else if(cLev < levels.back()){
                group_names.pop_back();
                while(levels.back() > cLev){
                    group_names.pop_back();
                    levels.pop_back();
                }
            }
            group_names.push_back(group_name);
            std::string gname = pw::joinVector(group_names,'/'); 

            ParamBin group = readNextGroup(line_feed,fin);
            gn_map[gname] = group;

            using intStrPair = std::pair<int,std::string>;
            gn_level_map.insert(intStrPair(group_names.size(),gname));
            gn_levels.insert(group_names.size());
        }
    }
    fin.close();

    // Start with children groups that have the most parents first
    for(auto rit=gn_levels.crbegin(); rit != gn_levels.crend(); rit++){
        int level = *rit;
        auto it_pair = gn_level_map.equal_range(level);
        for(auto it = it_pair.first; it != it_pair.second; it++){
            // Check for child groups and set to the parents
            if(std::next(rit) != gn_levels.rend()){
                std::string gname = it->second;
                std::string parent_name = stripFirst(gname,'/');
                std::string child_name = stripLast(gname,'/');
                ParamBin child_bin = gn_map[gname];
                ParamBin parent_bin = gn_map[parent_name];
                parent_bin.set(child_name,child_bin);
                gn_map[parent_name] = parent_bin;
            } //If a group is not a child, then set it to the ParamBin
            else
                set(it->second,gn_map[it->second]);
        }
    }
}

void ParamBin::printBin(std::ostream& os) const{
    static int depth = 0;
    os << std::endl;
    auto it = params.cbegin();
    while(it != params.cend()){
        std::string name;
        for(int i = 0; i < depth; i++)
            name += EMPTY_CHARS;
        name += it->first;
        auto scaleit = scaleMap.find(it->first);
        if(scaleit != scaleMap.cend())
            name += '[' + scaleit->second + ']'; 

        os << std::setiosflags(std::ios::left) << std::setw(40) << name + ":";
        os << std::setw(16) << it->second << std::endl;
        it++;
    }
    os << std::endl;
    auto bit = parambins.cbegin();
    while(bit != parambins.cend()){
        std::string group_name;
        for(int i = 0; i < depth; i++)
            group_name += EMPTY_CHARS;
        group_name += bit->first;
        os << group_name << std::endl;
        depth++;
        bit->second.printBin(os);
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
    if(params.count(name) > 0 || parambins.count(name) > 0) 
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
    if(params.empty()  && parambins.empty())
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
    auto it = parambins.find(name);
    if(it != parambins.cend())
        return it->second;
    else
        throw ParamBinKeyException(name);
}

const ParamBin& ParamBin::getBin(const std::string& name) const
{
    auto it = parambins.find(name);
    if(it != parambins.cend())
        return it->second;
    else
        throw ParamBinKeyException(name);
}

bool ParamBin::clear(const std::string& name)
{
    if(inBin(name)){
        params.erase(name);
        parambins.erase(name);
        scaleMap.erase(name);
        return true;
    }
    else 
        return false;
}

void ParamBin::clearAll() 
{
    params.clear();
    parambins.clear();
    scaleMap.clear();
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
    if(val)
        set(name,"on");
    else
        set(name,"off");
}

void ParamBin::setBin(const std::string& name,ParamBin bin)
{
    parambins[name] = bin;
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


}


