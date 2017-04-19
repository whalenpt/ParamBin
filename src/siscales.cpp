
#include <iostream>
#include <cmath>
#include <string> 
#include <cstdlib>
#include <pwutils/pwstrings.h>
#include "siscales.h"

namespace pw{
namespace scales{
  
  double SIscalings::ProcessScaling(const std::string& str,double val) const
  {
    std::string num_str,den_str;
    ParseString(str,num_str,den_str);
    if(!CheckScale(num_str)){
      std::cerr << "Scaling of " << str << " not recognized."
                << std::endl;
      exit(EXIT_FAILURE);
    }
    if(!CheckScale(den_str)){
      std::cerr << "Scaling of " << str << " not recognized."
                << std::endl;
      exit(EXIT_FAILURE);
    }
    val = ProcessMult(num_str,val);
    val = ProcessDiv(den_str,val);
    return val;
  }

  bool SIscalings::ValidScaling(const std::string& str) const
  {
    std::string num_str,den_str;
    ParseString(str,num_str,den_str);
    if(!CheckScale(num_str))
      return false;
    if(!CheckScale(den_str))
      return false;
    return true;
  }

  bool SIscalings::CheckScale(const std::string& str) const
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

    // Check Special (dB)
    if(upper_str == "DB")
      return true;

    return false;
  }

  //void ParseString(const std::string& str,dblMap& scalePairs)
  void ParseString(const std::string& str,std::string& num,std::string& den)
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

  double SIscalings::ProcessMult(const std::string& str,double val) const
  {
    if(str.empty())
      return val;
    if(str == "1" || str == "1.0")
      return val;

    dblMap::const_iterator d_iter;
    d_iter = siMap.find(str);
    if(d_iter != siMap.end())
      return (*d_iter).second*val;
    dblMap::const_iterator c_iter;

    std::string upper_str = pw::stringUpperCase(str);
    c_iter = conMap.find(upper_str);
    if(c_iter != conMap.end())
      return (*c_iter).second*val;

    if(upper_str == "DB"){
      val = pow(10.0,0.1*val);
      return val;
    }
    std::cerr << "Scaling of " << str << " not recognized."
              << std::endl;
    exit(EXIT_FAILURE);
  }


  double SIscalings::ProcessDiv(const std::string& str,double val) const
  {
    if(str.empty())
      return val;
    if(str == "1" || str == "1.0")
      return val;
    dblMap::const_iterator d_iter;
    d_iter = siMap.find(str);
    if(d_iter != siMap.end())
      return val/(*d_iter).second;
    dblMap::const_iterator c_iter;

    std::string upper_str = pw::stringUpperCase(str);
    c_iter = conMap.find(upper_str);
    if(c_iter != conMap.end())
      return val/(*c_iter).second*val;
    std::cerr << "Scaling of " << str << " not recognized."
              << std::endl;
    exit(EXIT_FAILURE);
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
}





