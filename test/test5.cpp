/*------------------------------------------------------------------------------
 *   
 *    Author: Patrick Townsend Whalen   
 *    Status: Development
 *    Date: (Enter Date, eg. 03/21/18)
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
  std::cout << "Parameter scalings are allowed for double types.\n"
            << "Scales are placed in brackets ([...]) and can be simple SI scalings\n"
            << "or other double typed parameters. If scaling by a double value, the\n"
            << "parameter can be in the root bin or any of its children/grandchildren etc.\n";

  ParamBin bin;
  bin << NamedParam<std::string>("var1","100.0 [cm]");
  bin << NamedParam<std::string>("var2","String with commas, brackets([blah]), and parenthesis");
  bin << NamedParam<std::string>("var3","0.4 [mum], 0.5 [mum], 0.6 [mum]");
  bin << NamedParam<int>("A",1) << NamedParam<int>("B",2);

  std::cout << bin << std::endl << std::endl;

  std::cout << "var1 = " << bin.getDbl("var1") << std::endl;
  std::cout << "var2 = " << bin.getStr("var2") << std::endl;
  auto dbl_vec = bin.getDblVec("var3");
  std::cout << "var3 = ";
  for(auto v : dbl_vec)
      std::cout << v << ' ';
  std::cout << std::endl;

  // operator << equivalent to bin.set(name,val)
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
  //T_group << NamedParam<std::string>("GridSize","16.0");
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
  std::cout << "tp = " << tin.getDbl("tp") << std::endl;
  std::cout << "GridSize = " <<  bin.getBin("GRID").getBin("T").getDbl("GridSize") << std::endl;


  return 0;
}







