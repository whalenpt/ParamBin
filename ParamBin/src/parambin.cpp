

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <filesystem>

#include "parambin.hpp"
#include <pwutils/pwstrings.h>


template<>
void ParamBin::get(const std::string& name,double& val) const
{
    std::string strval = getStrParam(name);
    std::string rawval = pw::eatWhiteSpace(pw::removeSubString(strval,'[',']'));
    convertFromString<double>(rawval,val);
}

template<>
void ParamBin::get(const std::string& name,std::vector<double>& vals) const
{
    std::string strval = getStrParam(name);
    std::vector<std::string> strvec = pw::parseString(strval,',');
    vals.clear();
    for(auto strval : strvec){
        double val;
        std::string rawval = pw::eatWhiteSpace(pw::removeSubString(strval,'[',']'));
        convertFromString<double>(rawval,val);
        vals.push_back(val);
    }
}

// Only children are copied, parent bin is considered root of the tree
ParamBin::ParamBin(const ParamBin& bin) : parent(nullptr),
    params(bin.params),
    children(),
    alias_map(bin.alias_map),
    reverse_alias_map(bin.reverse_alias_map)
{
    for(auto it = bin.children.cbegin() ; it != bin.children.cend(); it++)
    {
        std::string bin_name = it->first;
        std::unique_ptr<ParamBin> child_bin(new ParamBin(*it->second));
        child_bin->parent = this;
        children[bin_name] = std::move(child_bin);
    }
}

// Assignment
ParamBin& ParamBin::operator = (const ParamBin& bin)
{
    parent = nullptr;
    params = bin.params;
    alias_map =  bin.alias_map;
    reverse_alias_map = bin.reverse_alias_map;
    {
        for(auto it = bin.children.cbegin() ; it != bin.children.cend(); it++)
        {
            std::string bin_name = it->first;
            std::unique_ptr<ParamBin> child_bin(new ParamBin(*it->second));
            child_bin->parent = this;
            children[bin_name] = std::move(child_bin);
        }
    }
    return *this;
}

ParamBin::ParamBin() : 
    parent(nullptr)
{
}

ParamBin::ParamBin(const char* FILE) : 
    parent(nullptr)
{
    loadParamFile(FILE);
}

ParamBin::ParamBin(std::string fileName) : 
    parent(nullptr)
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

std::string ParamBin::getStrParam(const std::string& name) const
{
    std::string strval;
    std::string key = pw::eatWhiteSpace(name);
    if(searchParamMap(key,strval))
        return strval;
    if(rootAliasSearch(key,strval)) 
        return strval;
    throw ParamBinKeyException(key);
}

void ParamBin::set(const NamedBin& named_bin) 
{
    setBin(named_bin.name(),named_bin.bin());
}

void ParamBin::setAlias(const std::string& name,const std::string& alias)
{
    std::string key = pw::eatWhiteSpace(name);
    if(params.count(key) > 0){
        alias_map[alias] = key;
        reverse_alias_map[key] = alias;
    } else
        throw ParamBinKeyException(key);
}

std::string ParamBin::setParamKey(const std::string& name)
{
    std::string key = pw::eatWhiteSpace(pw::removeSubString(name,'(',')'));
    std::string alias = pw::eatWhiteSpace(pw::subString(name,'(',')')); 
    if(!alias.empty()){
        alias_map[alias] = key;
        reverse_alias_map[key] = alias;
    }
    return key;
}

bool ParamBin::searchParamMap(const std::string& key,std::string& strval) const
{
    auto it = params.find(key);
    if(it != params.cend()){
        strval = (*it).second;
        return true;
    }
    return false;
}

bool ParamBin::searchAliasTree(const std::string& alias_key,std::string& strval) const
{
    // Check current bin alias'
    auto it = alias_map.find(alias_key);
    if(it != alias_map.cend()){
        std::string key = (*it).second;
        return searchParamMap(key,strval);
    }
    // Check child bins
    for(const auto& kv : children) {
        if(kv.second->searchAliasTree(alias_key,strval))
            return true;
    }
    return false;
}

bool ParamBin::rootAliasSearch(const std::string& alias_key,std::string& strval) const 
{
    const ParamBin* top = this;
    while(top->parent != nullptr)
        top = top->parent;
    return top->searchAliasTree(alias_key,strval);
}


template <>
void convertFromString(const std::string& str,char& val)
{
    val = *str.c_str();
}

template <>
void convertFromString(const std::string& str,std::string& val)
{
    val = str;
}

template <>
void convertFromString(const std::string& str,std::vector<std::string>& vals)
{
    vals.clear();
    vals = pw::parseString(str,',');
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

void lineToNameVal(const std::string& line_feed,std::string& name,std::string& vals)
{
    std::vector<std::string> parsedParam = pw::parseString(line_feed,':');
    name = parsedParam[0];
    vals = parsedParam[1];
    name = pw::eatWhiteSpace(name);
    vals = pw::eatWhiteSpace(vals);
}

std::ifstream& readNextLine(std::ifstream& fin,std::string& line_feed) 
{
    getline(fin,line_feed);
    line_feed = pw::decommentString(line_feed,"#");
    // Check that line_feed is not whitespace
    while(fin && pw::isWhitespace(line_feed)){
        getline(fin,line_feed);
        line_feed = pw::decommentString(line_feed,"#");
    }
    return fin;
}

void ParamBin::scanYAML(std::ifstream& fin)
{
    // Levels determine parent/child relationships, root level is -1 with a parent=nullptr
    int level = -1;
    std::vector<int> levels;
    std::vector<ParamBin*> parents;
    levels.push_back(level);

    parents.push_back(this);
		std::string line_feed;

    while(readNextLine(fin,line_feed))
    {
				std::vector<std::string> parsed_line_feed = pw::parseString(line_feed,':');
				if(parsed_line_feed.size() == 0)
						throw std::exception(); 
				else if(parsed_line_feed.size() == 1){
						throw std::exception(); 
				} else if(parsed_line_feed.size() == 2){
						std::string name(pw::eatWhiteSpace(parsed_line_feed[0]));
						std::string val(pw::eatWhiteSpace(parsed_line_feed[1]));
						// Found a group
						if(val.empty()){
								std::string group_name(parsed_line_feed[0]);
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
								parents.push_back(bin->children[group_name].get());
						}
						else
                parents.back()->set(NamedParam<std::string>(name,val));
				} else {
						throw std::exception(); 
				}
    }
}

void ParamBin::loadParamFile(std::string FILE)
{
		namespace fs = std::filesystem;
		fs::path local_path = fs::path(FILE);
		if(!fs::exists(local_path)){
				fs::path current_dir = fs::current_path();
				fs::path full_path = current_dir / FILE;
				FILE = full_path.string();
		}
    std::ifstream fin(FILE);
    if(!fin.is_open())
    {
        fin.clear();
        throw ParamBinFileOpenException(FILE);
    }
		try{
		  	scanYAML(fin);
		} catch(...){
				fin.close();
        throw ParamBinFileReadException(FILE);
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
        auto ait = reverse_alias_map.find(it->first);
        if(ait != reverse_alias_map.cend())
            name += '(' + ait->second + ')'; 

        os << std::setiosflags(std::ios::left) << std::setw(40) << name + ":";
        os << std::setw(16) << it->second << std::endl;
        it++;
    }
    os << std::endl;
    auto bit = children.cbegin();
    while(bit != children.cend()){
        std::string group_name;
				// add empty chars before starting group name
        for(int i = 0; i < depth; i++)
            group_name += EMPTY_CHARS;
        group_name += bit->first;
				group_name += ':';
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
    if(params.count(name) > 0 || children.count(name) > 0) 
        return true;
    return false;
}

int ParamBin::size(const std::string& name) const
{
    auto it = params.find(name);
    if(it != params.cend())
        return pw::countCharacters(it->second,',') + 1;
    return 0;
}

bool ParamBin::empty() const
{
    return (params.empty() && children.empty() ? true : false);
}

ParamBin& ParamBin::getBin(const std::string& name) 
{
    auto it = children.find(name);
    if(it != children.cend())
        return *it->second;
    else
        throw ParamBinKeyException(name);
}

const ParamBin& ParamBin::getBin(const std::string& name) const
{
    auto it = children.find(name);
    if(it != children.cend())
        return *it->second;
    else
        throw ParamBinKeyException(name);
}


bool ParamBin::clear(const std::string& name)
{
    if(inBin(name)){
        params.erase(name);
        children.erase(name);
        return true;
    }
    else 
        return false;
}

void ParamBin::clear() 
{
    params.clear();
    children.clear();
    alias_map.clear();
    reverse_alias_map.clear();
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
    return (val == "on" ? true : false);
}

bool ParamBin::getBoolF(const std::string& name) const
{ return (inBin(name) ? getBool(name) : false); }

bool ParamBin::getBoolT(const std::string& name) const
{ return (inBin(name) ? getBool(name) : true); }

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
    child_bin->parent = this;
    children[name] = std::move(child_bin);
}

// Pass by pointers simply transfers ownership of pointer
void ParamBin::setBin(const std::string& name,std::unique_ptr<ParamBin> bin)
{
    bin->parent = this;
    children[name] = std::move(bin);
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




