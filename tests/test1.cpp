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

  using std::cerr;
  using std::endl;
  using std::cout;

  if(argc == 2){
    std::cout << "FLAG 1" << std::endl;
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
  else if(argc > 2)
  {
    cerr << "To many arguments to " << argv[0] << endl;
    cerr << "ABORTING" << endl;
    exit(EXIT_FAILURE);
  }
  else 
  {
    cerr << "Please specify an input file " << endl;
    cerr << "ABORTING" << endl;
    exit(EXIT_FAILURE);
  }
}







