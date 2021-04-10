
#ifndef PWDATAIO_HPP_ 
#define PWDATAIO_HPP_

#include <cctype>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "pwconstants.h"

namespace pw{
    namespace fs = std::filesystem;

    class DataIO{
        public:
            DataIO(const fs::path& dirpath);
            void setDirectoryPath(const fs::path& dirpath);
            void writeFile(const fs::path& fname,std::vector<double>& x);
            void writeFile(const fs::path& fname,std::vector<double>& x,std::vector<double>& y);

            static void createDirectory(const fs::path& dirpath);
            static fs::path currentPath() {return fs::current_path();}

        private:
            fs::path m_dirpath;
    };
            

    void fileOpen(const char* FILE,std::ofstream& fout);
    void fileOpen(const char* FILE,std::ifstream& fin);
    int getTwoColFileSize(const std::string& nm);
    int getThreeColFileSize(const std::string& nm);
    void readDataFile(const std::string& nm,int sz,double* x,double* y); 
    void cOut(const char* nm,int sz,const double* x,const double* y);
    void cOut(const char* nm,int sz,const double* x,const dcmplx* y);
    void cOut(const char* nm,int sz,const double* x);
    void cOut(const char* nm,int sz,const dcmplx* x);
    void cOut(const std::string& nm,int sz,const double* x,const double* y);
    void cOut(const std::string& nm,int sz,const double* x,const dcmplx* y);
    void cOut(const std::string& nm,int sz,const double* x);
    void cOut(const std::string& nm,int sz,const dcmplx* x);


    void fOut(const char* FILE,int sz,const double* x,const double* y);
    void fOut(const char* FILE,int sz,const double* x,const dcmplx* y);
    void fOut(const char* FILE,int sz,const double* x);
    void fOut(const char* FILE,int sz,const dcmplx* x);
    void fOut(const std::string& fname,int sz,const double* x,const double* y); 
    void fOut(const std::string& fname,int sz,const double* x,const dcmplx* y);
    void fOut(const std::string& fname,int sz,const double* x);
    void fOut(const std::string& fname,int sz,const dcmplx* x);

    void fOut(int num,const char* FILE,int sz,const double* x,const double* y);
    void fOut(int num,const char* FILE,int sz,const double* x,const dcmplx* y);
    void fOut(int num,const char* FILE,int sz,const double* x);
    void fOut(int num,const char* FILE,int sz,const dcmplx* x);
    void fOut(int num,const std::string& fname,int sz,const double* x,const double* y);
    void fOut(int num,const std::string& fname,int sz,const double* x,const dcmplx* y);
    void fOut(int num,const std::string& fname,int sz,const double* x);
    void fOut(int num,const std::string& fname,int sz,const dcmplx* x);


    template<typename T1,typename T2>
    void fOut(const char* FILE,const std::vector<T1>& x,const std::vector<T2>& y);

    template<typename T1,typename T2>
    void fOut(const char* FILE,const std::vector<T1>& x,const std::vector<T2>& y)
    {
    if(x.size() != y.size())
    {
      std::cerr << "x and y data not the same size in fOut(file,vec1,vec2)" << std::endl;
      std::cerr << "ABORTING" << std::endl;
      exit(EXIT_FAILURE);
    }

    std::ofstream fout;
    fout.open(FILE);
    if(!fout.is_open())
    {
      std::cerr << "Could not open " << FILE << std::endl;
      std::cerr << "ABORTING" << std::endl;
      fout.clear();
      exit(EXIT_FAILURE);
    }

      int sz = x.size();
    for(int j = 0; j < sz; j++){
      fout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j] 
           << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << y[j] << std::endl; 
    }
    fout << std::endl;
    fout.close();
    }

    template<typename T1,typename T2>
    void cOut(const std::vector<T1>& x,const std::vector<T2>& y);

    template<typename T1,typename T2>
    void cOut(const std::vector<T1>& x,const std::vector<T2>& y)
    {
        if(x.size() != y.size())
        {
          std::cerr << "x and y data not the same size in fOut(file,vec1,vec2)" << std::endl;
          std::cerr << "ABORTING" << std::endl;
          exit(EXIT_FAILURE);
        }
        int sz = x.size();
        for(int j = 0; j < sz; j++){
          std::cout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j] 
               << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << y[j] << std::endl; 
        }
        std::cout << std::endl;
    }

    template<typename T>
    void cOut(const std::vector<T>& y);

    template<typename T>
    void cOut(const std::vector<T>& y)
    {
        int sz = y.size();
        for(int j = 0; j < sz; j++)
          std::cout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << y[j] << std::endl;
        std::cout << std::endl;
    }

    template<typename T>
    void fOut(const char* FILE,const std::vector<T>& x);

    template<typename T>
    void fOut(const char* FILE,const std::vector<T>& x)
    {
        std::ofstream fout;
        fout.open(FILE);
        if(!fout.is_open())
        {
          std::cerr << "Could not open " << FILE << std::endl;
          std::cerr << "ABORTING" << std::endl;
          fout.clear();
          exit(EXIT_FAILURE);
        }
        int sz = x.size();
        for(int j = 0; j < sz; j++){
          fout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j] << std::endl;
        }
        fout << std::endl;
        fout.close();
    }



}

#endif


