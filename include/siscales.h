
#ifndef SISCALES_H_ 
#define SISCALES_H_  

#include<vector>
#include<map>
#include<string>
#include<iterator>
#include<iostream>
#include <pwutils/pwstrings.h>

namespace scales{

  typedef std::pair<std::string,double> dblPair;
  typedef std::map<std::string,double> dblMap;
  void ParseString(const std::string& str,std::string& num,std::string& den);

  const std::string YOTTA = "Y";
  const std::string ZETTA = "Z";
  const std::string EXA = "E";
  const std::string PETA = "P";
  const std::string TERA = "T";
  const std::string GIGA = "G";
  const std::string MEGA = "M";
  const std::string KILO = "k";
  const std::string HECTO = "h";
  const std::string DECA = "da";
  const std::string BASE = "";
  const std::string DECI = "d";
  const std::string CENTI = "c";
  const std::string MILLI = "m";
  const std::string MICRO = "mu";
  const std::string NANO = "n";
  const std::string PICO = "p";
  const std::string FEMTO = "f";
  const std::string ATTO = "a";
  const std::string ZEPTO = "z";
  const std::string YOCTO = "y";

  const int UNIT_TYPES = 5;
  const int NUM_PREFIXES = 21;
  const int NUM_CONST = 2;
  const std::string NO_SCALING = "noScaling";

  // SI unit strings are case sensitive
  const std::string UNIT_LIST[UNIT_TYPES] = {"m","s","W","sec","rad"}; 
  const std::string SI_PREFIXES[NUM_PREFIXES] = {YOTTA,ZETTA,EXA,PETA,
                       TERA,GIGA,MEGA,KILO,HECTO,DECA,BASE,DECI,CENTI,MILLI,
                       MICRO,NANO,PICO,FEMTO,ATTO,ZEPTO,YOCTO};

  const double SI_SCALES[NUM_PREFIXES] = {1.0e24,1.0e21,1.0e18,1.0e15,1.0e12,1.0e9,
                        1.0e6,1.0e3,1.0e2,1.0e1,1.0,1.0e-1,1.0e-2,1.0e-3,1.0e-6,1.0e-9,
                        1.0e-12,1.0e-15,1.0e-18,1.0e-21,1.0e-24};

  // Const strings are case insensitive
  const std::string CONST_STR[NUM_CONST] = {"PI","eV"};
  const double CONST_VALS[NUM_CONST] = {3.14159265358979323846,1.602176565e-19};

  class SIscalings{
    public:
      SIscalings();
      ~SIscalings() {};
      double ProcessScaling(const std::string& str,double val) const;
      bool ValidScaling(const std::string& str) const;
      double getScale(const std::string& str) const;
     private:
      dblMap siMap;
      dblMap conMap;
      std::vector<std::string> unitList;
      std::vector<dblPair> prefixList;
      double ProcessDiv(const std::string& str,double val) const;
      double ProcessMult(const std::string& str,double val) const;
      bool CheckScale(const std::string& str) const;
  };
}

#endif




