
#ifndef PARAMBIN_HPP_ 
#define PARAMBIN_HPP_

#include <vector>
#include <map>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <exception>
#include <memory>
#include <pwutils/pwstrings.h>

std::ifstream& readNextLine(std::ifstream& fin,std::string& line_feed);
void lineToNameVal(const std::string& line_feed,std::string& name,std::string& vals);

class ParamBinException : public std::exception
{
    public:
        ParamBinException() {}
        virtual ~ParamBinException() {};
        virtual const char* what() const noexcept = 0;
};

class ParamBinKeyException : public ParamBinException
{
    public:
        explicit ParamBinKeyException(const std::string& nm) {
            msg = "The key " + nm + " was not found in the ParamBin.";
        }
        ~ParamBinKeyException() {};
        const char* what() const noexcept override {
            return msg.c_str();
        }
    private:
				std::string msg;
};

class ParamBinFileException : public ParamBinException
{
    public:
        explicit ParamBinFileException(const std::string& nm) {
            msg = "Failed to open the ParamBin parameter file " + nm + ".";
        }
        ~ParamBinFileException() {};
        const char* what() const noexcept override {
            return msg.c_str();
        }
    private:
				std::string msg;
};

class ParamBinScaleException : public ParamBinException
{
    public:
        explicit ParamBinScaleException(const std::string& scale){
            msg = "The scale " + scale + " could not be processed.";
        } 
        ~ParamBinScaleException() {};
        const char* what() const noexcept override {
            return msg.c_str();
        }
    private:
				std::string msg;
};


class ParamBin;
class NamedBin;
using ParamMap = std::map<std::string,std::string>; 
using AliasMap = std::map<std::string,std::string>;
using BinMap = std::map<std::string,std::unique_ptr<ParamBin>>;

template <class T>
class NamedParam
{
    public:
        NamedParam(const std::string& name,T val) :
            m_name(name), m_val(val) {}
        std::string name() const {return m_name;}
        T value() const {return m_val;}
    private:
        std::string m_name;
        T m_val;
};


class ParamBin{

    public:
        ParamBin();
        ~ParamBin(); 
        ParamBin(const ParamBin& bin);
        ParamBin(const char* FILE);
        ParamBin(std::string fileName);
        ParamBin& operator = (const ParamBin& bin);

        void loadParamFile(std::string FILE); 
        void loadParamFile(const char* FILE) {loadParamFile(std::string(FILE));}
  
        double getDbl(const std::string&) const;
        int getInt(const std::string&) const;
        bool getBool(const std::string&) const;
        bool getBoolF(const std::string&) const;
        bool getBoolT(const std::string&) const;
        std::string getStr(const std::string&) const;
        std::string getStrL(const std::string& nm) const;
        std::string getStrU(const std::string& nm) const;
        char getChar(const std::string&) const;

        // get a child bin
        ParamBin& getBin(const std::string& name);
        const ParamBin& getBin(const std::string& name) const;

        // get the parent bin
        ParamBin& parentBin() {return *parent;} 
        const ParamBin& getParentBin() const {return *parent;}

        // get map of all params
        ParamMap getParamMap() const; 

        // get map of all child bins
        const BinMap& getChildBins() const {return children;}

        std::vector<double> getDblVec(const std::string&) const;
        std::vector<int> getIntVec(const std::string&) const;
        std::vector<std::string> getStrVec(const std::string&) const;
  
        template<typename T>
        void get(const std::string& nm,T& val) const;  // get a param of type T 
  
        template<typename T>
        void get(const std::string& nm,std::vector<T>& val) const;  // get a param of type T 

        template<typename T>
        void set(const NamedParam<T>& named_param);

        template<typename T>
        void set(const std::string&,T val);

        template<typename T>
        void set(const std::string&,const std::vector<T>&);

        void set(const std::string&,const ParamBin& bin);

        void setBool(const std::string& nm,bool);
        void set(const NamedBin& named_bin);

        // Bins are deep copied into the tree structure
        void setBin(const std::string& nm,const ParamBin& bin);
        // Transfer ownership of bin 
        void setBin(const std::string& name,std::unique_ptr<ParamBin> bin);

        void setAlias(const std::string& name,const std::string& alias);

        friend std::ostream& operator<<(std::ostream&,const ParamBin& bin);

        template<typename T>
        ParamBin& operator<<(const NamedParam<T>& named_param);
        ParamBin& operator<<(const NamedBin& named_bin);

        bool inBin(const std::string&) const;
        std::vector<std::string> inBin(const std::vector<std::string>&) const;
        std::vector<std::string> notInBin(const std::vector<std::string>&) const;
        int size(const std::string& name) const;
        bool empty() const;
        void clear();
        bool clear(const std::string& name);

    private:

        ParamBin* parent;
        ParamMap params; 
        BinMap children;
        AliasMap alias_map;
        AliasMap reverse_alias_map;

        std::string setParamKey(const std::string& name);
        std::string getStrParam(const std::string& name) const;
        void printBin(std::ostream& os) const;
				void scanYAML(std::ifstream& fin);

        bool searchAliasTree(const std::string& key,std::string& strval) const;
        bool searchParamMap(const std::string& key,std::string& strval) const;

        bool rootAliasSearch(const std::string& alias_key,std::string& strval) const;
};

class NamedBin
{
    public:
        NamedBin(const std::string& name,const ParamBin& bin) :
            m_name(name), m_bin(bin) {}
        std::string name() const {return m_name;}
        ParamBin bin() const {return m_bin;}
    private:
        std::string m_name;
        ParamBin m_bin;
};

template<typename T>
ParamBin& ParamBin::operator<<(const NamedParam<T>& named_param)
{
    set(named_param);
    return *this;
}

template<typename T> 
std::string convertToString(T val) 
{
    std::ostringstream stm;
    stm.setf(std::ios_base::showpoint);
    stm.precision(12);
    stm << val;
    std::string valstr = stm.str();
    std::size_t found_dot = valstr.find_last_of(".");
    std::size_t found_e = valstr.find_last_of("e");
    if(found_dot != std::string::npos){
        std::string substr = valstr.substr(found_dot+1,found_e - found_dot - 1);
        std::string replace_str(substr); 
        while(replace_str[replace_str.size()-1] == '0')
            replace_str = replace_str.substr(0,replace_str.size()-1);
        if(replace_str.empty())
            replace_str = "0";
        valstr.replace(found_dot+1,found_e - found_dot - 1,replace_str);
    }
    return valstr;
}

template<typename T> 
std::string convertToString(const std::vector<T>& val) 
{
    if(val.empty())
        return "";

    std::string vecstr = convertToString(val[0]);
    for(unsigned int i = 1; i < val.size(); i++){
        std::string valstr = convertToString(val[i]);
        vecstr = vecstr + "," + valstr;
    }
    return vecstr;
}

template<> 
std::string convertToString(std::string val);

template<> 
std::string convertToString(const char* val);

template <typename T>
void convertFromString(const std::string& str,T& val)
{
    std::stringstream ss;
    ss.setf(std::ios_base::showpoint);
    ss.precision(12);
    ss << str;
    ss >> val;
}

template <>
void convertFromString(const std::string& str,char& val);

template <>
void convertFromString(const std::string& str,std::string& val);

// ADD A PARAM TO THE PARAM BIN
template<typename T>
void ParamBin::set(const std::string& name,T val) 
{
    set(NamedParam<T>(name,val));
}

// ADD A PARAM SET TO THE PARAM BIN
template<typename T>
void ParamBin::set(const std::string& name,const std::vector<T>& val) 
{
    set(NamedParam<std::vector<T>>(name,val));
}

template<typename T>
void ParamBin::set(const NamedParam<T>& named_param)
{
    std::string key = setParamKey(named_param.name());
    std::string strVal = convertToString(named_param.value());
    params[key] = strVal;
}

template<typename T>
void ParamBin::get(const std::string& name,T& val) const
{
    std::string strval = getStrParam(name);
    convertFromString<T>(strval,val);
}

template<typename T>
void ParamBin::get(const std::string& name,std::vector<T>& vals) const
{
    std::string strval = getStrParam(name);
    std::vector<std::string> strvec = pw::parseString(strval,',');
    vals.clear();
    for(auto strval : strvec){
        T val;
        convertFromString<T>(strval,val);
        vals.push_back(val);
    }
}



#endif



    
 

