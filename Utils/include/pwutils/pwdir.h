
#ifndef PWDIR_H_
#define PWDIR_H_ 

#include <string>
#include <filesystem>

namespace pw{

class DirAux{
    public:
        DirAux(std::string dirnm);
        ~DirAux() {}
        std::string getLocalDir() {return local_dir.string();}
        std::string getFullDir() {return full_dir.string();}
    private:
				std::filesystem::path local_dir;
				std::filesystem::path full_dir;
};

}

#endif



