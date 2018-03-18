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
  bin.set("var1","100.0 [cm]");
  bin.set("var2","'Commas, there are a few, in this string'");
  bin.set("var3","0.4 [mum], 0.5 [mum], 0.6 [mum]");

  std::cout << "var1 = " << bin.getDbl("var1") << std::endl;
  std::cout << "var2 = " << bin.getStr("var2") << std::endl;
  std::vector<double> dbl_vec = bin.getDblVec("var3");
  for(int i = 0; i < dbl_vec.size(); i++)
      std::cout << "var3[" << i << "] = " << dbl_vec[i] << std::endl;

  bin.set("var4",3.0);
  std::cout << "var4 = " << bin.getStr("var4") << std::endl;
  std::cout << "Setting var4 to a new value..." << std::endl;
  bin.set("var4",4.0);
  std::cout << "var4 = " << bin.getStr("var4") << std::endl;


  ParamBin grid;
  ParamBin T_group;
  T_group.set("GridSize","16.0 [tp]");
  T_group.set("NumberPoints",2048);
  grid.set("T",T_group);
  bin.set("GRID",grid);

  ParamBin input;
  input.set("Intensity",5.0e16);
  ParamBin tin;
  //tin.set("CarrierWavelength","0.4 [mum], 0.5 [mum], 0.6 [mum]");
  tin.set("CarrierWavelength","0.4 [mum], 0.5 [mum], 0.6 [mum]");
  tin.set("PulseWidth (tp)","20.0 [fs]");
  input.set("T",tin);

  bin.set("INPUT",input);

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







