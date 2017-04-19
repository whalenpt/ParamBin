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
#include <vector>
#include <string>
#include "../include/parambin.hpp"

//------------------------------------------------------------------------------

int main(int argc,char* argv[])
{

  using std::cerr;
  using std::endl;
  using std::cout;
  using namespace pw;

  if(argc == 2){
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







