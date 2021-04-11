/*------------------------------------------------------------------------------
 *   
 *    Author: Patrick Whalen   
 *    Email: whalenpt@gmail.com
 *    Status: Development
 *    Date: 04/09/2021
 *    Description: "Testing of ParamBin class."
 *
------------------------------------------------------------------------------*/

// HEADERS, INCLUDES, GLOBAL VARS/DECLARATIONS, ETC. 

#include <iostream>
#include <vector>
#include <string>
#include <parambin.hpp>

//------------------------------------------------------------------------------

int main(int argc,char* argv[])
{
  if(argc > 1){
    std::cout << "Filename: " << argv[1] << std::endl;
    ParamBin input(argv[1]);
    std::cout << input << std::endl;
    if(input.size("SimulationName") == 1)
        std::cout << "Simulation Name:" << input.getStr("SimulationName") << std::endl;
    else if(input.size("SimulationName") == 2){
        std::vector<std::string> vals = input.getStrVec("SimulationName");
        for(int i = 0; i < vals.size(); i++)
            std::cout << "Sim Name[" << i << "] =" << vals[i] << std::endl;
    }
  }
  else 
  {
    std::cerr << "Please specify an input file " << std::endl;
    std::cerr << "ABORTING" << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}







