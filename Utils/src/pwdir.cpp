

#include <cstring>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <sstream>
#include "pwstrings.h"
#include "pwdir.h"
#include "pwexcept.h"

namespace pw{

DirAux::DirAux(std::string drnm) 
{
	namespace fs = std::filesystem;
	std::string local_path = eatWhiteSpace(drnm);
	local_path = eatWhiteSpace(local_path," \t","_");
	if(local_path.empty()){
			throw pw::Exception("Invalid string provided to DirAux constructor.");
	}
	local_dir = fs::path(local_path);
	fs::path current_dir = fs::current_path();
	full_dir = current_dir / local_path;
	if(!fs::exists(full_dir)){
			fs::create_directory(full_dir);
	}
}

}




