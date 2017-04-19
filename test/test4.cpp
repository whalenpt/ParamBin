/*------------------------------------------------------------------------------
 *   
 *    Author: Patrick Townsend Whalen   
 *    Email: pwhalen@email.arizona.edu
 *    Status: Development
 *    Date: (Enter Date, eg. 06/30/15)
 *    Description: Testing of exceptions
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
  ParamBin input;
  input.set("var1",1.3);
  input.set("var2",10);
  input.set("var3","string");

  std::cout << "HERE'S YOUR BIN " << std::endl;
  std::cout << input << std::endl << std::endl;

  try{
      std::cout << "Try to access var1." << std::endl;
      std::cout << "var1 = " << input.getDbl("var1") << std::endl;
      std::cout << "Try to access non-existent var4." << std::endl;
      std::cout << std::endl;
      input.getDbl("var4");
  } catch(ParamBinKeyException& e) {
      std::cout << "A ParamBinKeyException was caught. Here's the message:" << std::endl;
      std::cout << e.what() << std::endl;
  }

  std::cout << std::endl << std::endl;
  try{
      std::string filename = "bleblabblueblah.cows";
      std::cout << "Try to open the nonexistant file " + filename << std::endl;
      std::cout << std::endl;
      input.setParams(filename);
  } catch(ParamBinFileException& e) {
      std::cout << "A ParamBinFileException was caught. Here's the message:" << std::endl;
      std::cout << e.what() << std::endl;
  }
  std::cout << std::endl << std::endl;
  

  return 0;
}










