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
            << "or other double typed parameters.\n";

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


  std::cout << std::endl << std::endl;
  std::cout << "Each parameter can have an alternative alias name. This parementers value\n"
            << "can either be accessed through the alias or the primary name, they both\n"
            << "return the same value when used with a get function. There are important\n" 
            << "distinction between the two though: values may only be set through the primary\n"
            << "name. Using a set function with an alias will create a new primary name,\n"
            << "something that, generally, is probably not intended. Secondly, alias names\n" 
            << "have expanded access to values when it comes to the tree structure of a bin.\n"
            << "The alias values are found via a recursive search from the root level bin\n"
            << "and therefore they do not need to be accessed through subbins as with primary names.\n"
            << "For parameters that are scaled by other parameters, short names and global access\n" 
            << "are convenient as can be seen in the GridSize example below.\n";
  std::cout << std::endl << std::endl;

  ParamBin grid;
  ParamBin T_group;
  T_group << NamedParam<std::string>("GridSize","16.0 [tp]");
  T_group << NamedParam<int>("NumberPoints",2048);

  T_group.setAlias("NumberPoints","nt");

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
  std::cout << "PulseWidth(tp) = " << bin.getDbl("tp") << std::endl;
  std::cout << "GridSize = " <<  bin.getBin("GRID").getBin("T").getDbl("GridSize") << std::endl;
  std::cout << "NumberPoints(nt) = " << bin.getInt("nt") << std::endl;
  std::cout << "Its also possible to access NumberPoints through nt in the INPUT bin\n"
            << "even though GRID is not a subbin of INPUT. They have the same root bin.\n";
  std::cout << "NumberPoints(nt) = " << bin.getBin("INPUT").getInt("nt");


  bin.clear();
  bin.set("A","10 [B]");
  bin.set("B","15 [C]");
  bin.set("C","2 [D]");
  bin.set("D","3");

  std::cout << bin << std::endl << std::endl;
  std::cout << "A = " << bin.getDbl("A") << std::endl;



  return 0;
}







