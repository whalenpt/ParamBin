/*------------------------------------------------------------------------------
 *   
 *    Author: Patrick Townsend Whalen   
 *    Email: pwhalen@email.arizona.edu
 *    Status: Development
 *    Date: (Enter Date, eg. 06/30/15)
 *    Description: (Enter Program Description, e.g. 
 *
------------------------------------------------------------------------------*/

// HEADERS, INCLUDES, GLOBAL VARS/DECLARATIONS, ETC. 

#include <iostream>
#include <string>
#include "../include/parambin.hpp"

//------------------------------------------------------------------------------

int main(int argc,char* argv[])
{

  using namespace pw;
  ParamBin bin;
  bin << NamedParam<std::string>("var1","100.0 [cm]");
  bin << NamedParam<std::string>("var2","'Commas, there are a few, in this string'");
  bin << NamedParam<std::string>("var3","0.4 [mum], 0.5 [mum], 0.6 [mum]");
  bin << NamedParam<int>("A",1) << NamedParam<int>("B",2);

  std::cout << "var1 = " << bin.getDbl("var1") << std::endl;
  std::cout << "var2 = " << bin.getStr("var2") << std::endl;

  auto dbl_vec = bin.getDblVec("var3");
  std::cout << "var3 = ";
  for(auto v : dbl_vec)
      std::cout << v << ' ';
  std::cout << std::endl;

  // += operator equivalent to bin.set(name,val)
  bin << NamedParam<double>("var4",3.0);
  std::cout << "var4 = " << bin.getDbl("var4") << std::endl;

  std::cout << "Setting var4 to a new value..." << std::endl;
  bin << NamedParam<double>("var4",4.0);
  std::cout << "var4 = " << bin.getDbl("var4") << std::endl;

  bin << NamedParam<std::vector<int>>("var5",{0,1,2,3});
  auto int_vec = bin.getIntVec("var5");
  std::cout << "Heres the int vector [0,1,2,3].\n";
  for(auto item : int_vec)
      std::cout << item << ' ';
  std::cout << std::endl;


  ParamBin grid;
  ParamBin T_group;
  T_group << NamedParam<std::string>("GridSize","16.0 [tp]");
  T_group << NamedParam<int>("NumberPoints",2048);
  grid << NamedBin("T",T_group);
  bin << NamedBin("GRID",grid);

  ParamBin input;
  input << NamedParam<double>("Intensity",5.0e16);

  ParamBin tin;
  tin.set("CarrierWavelength","0.4 [mum], 0.5 [mum], 0.6 [mum]");
  tin.set("PulseWidth (tp)","20.0 [fs]");

  input << NamedBin("T",tin);
  bin << NamedBin("INPUT",input);

  std::cout << bin << std::endl << std::endl;

  std::cout << "PulseWidth(tp) is stored as the parameter named PulseWidth with \n \
      typical access through the group bins INPUT and T. However, there is now an \n \
      additional way to access the parameter through the name 'tp' that bypasses \n \
      the group access points. This also gives the parameter a potentially much shorter \n \
      name to reference.\n\n";
//  std::cout << "PulseWidth = " << bin.getBin("INPUT").getBin("T").getDbl("PulseWidth") << std::endl;
//  std::cout << "tp = " << bin.getDbl("tp");

  return 0;
}







