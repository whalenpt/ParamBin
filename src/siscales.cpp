
#include <iostream>
#include <cmath>
#include <string> 
#include <cstdlib>
#include <pwutils/pwstrings.h>
#include "siscales.h"

namespace scales{
  
double SIscalings::getScale(const std::string& str) const
{
    std::string num_str,den_str;
    parseString(str,num_str,den_str);
    if(!checkScale(num_str) || !checkScale(den_str)){
        std::string error_msg = "Scaling of " + str + " not recognized.";
        throw ScalingException(error_msg);
    }
    double num = processScale(num_str);
    double den = processScale(den_str);
    return num/den;
}


bool SIscalings::isValid(const std::string& str) const
{
    std::string num_str,den_str;
    parseString(str,num_str,den_str);
    if(!checkScale(num_str) || !checkScale(den_str))
        return false;
    return true;
}

bool SIscalings::checkScale(const std::string& str) const
{
    if(str.empty())
        return true;

    // Check SI units
    dblMap::const_iterator d_iter = siMap.find(str);
    if(d_iter != siMap.end())
        return true;

    // Upper case string
    std::string upper_str = pw::stringUpperCase(str);

    // Check Constant List (PI,...) 
    dblMap::const_iterator c_iter = conMap.find(upper_str);
    if(c_iter != conMap.end())
        return true;

    return false;
}

void parseString(const std::string& str,std::string& num,std::string& den)
{
    size_t pos1,pos2;
    pos1 = str.find('/');
    pos2 = str.find('/',pos1+1);
    if(pos1 != std::string::npos){
        num.assign(str,0,pos1);
        den.assign(str,pos1+1,pos2);
    }
    else{
        num = str;
        den = "";
    }
    pw::eatWhiteSpace(den);
    pw::eatWhiteSpace(num);
}

double SIscalings::processScale(const std::string& str) const
{
    if(str.empty() || str == "1" || str == "1.0")
        return 1.0;

    auto d_iter = siMap.find(str);
    if(d_iter != siMap.cend())   
        return (*d_iter).second;

    std::string upper_str = pw::stringUpperCase(str);
    auto c_iter = conMap.find(upper_str);
    if(c_iter != conMap.cend())
        return (*c_iter).second;

    throw ScalingException("Did not recognize scaling " + str);
}


SIscalings::SIscalings()
{
    for(int i = 0; i < NUM_CONST; i++){
        std::string ustr(CONST_STR[i]);
        ustr = pw::stringUpperCase(ustr);
        conMap.insert(dblPair(ustr,CONST_VALS[i]));
    }
    std::string str;
    for(int i = 0; i < UNIT_TYPES; i++)
        unitList.push_back(UNIT_LIST[i]);

    for(int i = 0; i < NUM_PREFIXES; i++)
    {
        dblPair myPair(SI_PREFIXES[i],SI_SCALES[i]);
        prefixList.push_back(myPair);
    }

    std::vector<std::string>::const_iterator u_iter;
    std::vector<dblPair>::const_iterator p_iter;
    for(u_iter = unitList.begin(); u_iter != unitList.end(); ++u_iter){
        for(p_iter = prefixList.begin(); p_iter != prefixList.end(); ++p_iter){
            str = (*p_iter).first+(*u_iter);
            siMap.insert(dblPair(str,(*p_iter).second));
            str = (*p_iter).first+(*u_iter) + "2";
            siMap.insert(dblPair(str,pow((*p_iter).second,2)));
            str = (*p_iter).first+(*u_iter) + "3";
            siMap.insert(dblPair(str,pow((*p_iter).second,3)));
            str = (*p_iter).first+(*u_iter) + "4";
            siMap.insert(dblPair(str,pow((*p_iter).second,4)));
        }
    }
}

}





