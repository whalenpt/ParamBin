
#include "pwdataio.hpp"
#include "pwdir.h"
#include "pwexcept.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <string>
#include <filesystem>

namespace pw{

DataIO::DataIO(const fs::path& dirpath) 
{
    setDirectoryPath(dirpath);
}

void DataIO::setDirectoryPath(const fs::path& dirpath)
{
    if(fs::is_directory(dirpath)){
       m_dirpath = dirpath;
    } else{
        std::string str("DataIO::DataIO(std::filesystem::path& path) \
             specified directory path IS NOT a directory path ");
        throw pw::Exception(str);
    }
}

void DataIO::createDirectory(const fs::path& dirpath)
{
    if(fs::exists(dirpath)){
        std::string str("DataIO::createDirectory(std::filesystem::path& path) \
                        specified path ALREADY EXISTS!");
        throw pw::Exception(str);
//        throw fs::filesystem_error();
    }
    fs::create_directories(dirpath);
}


void DataIO::writeFile(const fs::path& fname,std::vector<double>& x)
{
    fs::path pathToFile = m_dirpath / fname;
    std::ofstream fout(pathToFile);
    for(int j = 0; j < x.size(); j++)
        fout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j] << std::endl;
    fout.close();
}

void DataIO::writeFile(const fs::path& fname,std::vector<double>& x,std::vector<double>& y)
{
    if(x.size() != y.size()){
        throw pw::Exception("DataIO::writeFile(fs::path fname,vector<double> x,vector<double>y)"\
                " The sizes of the x and y vectors must be the same");
    }

    fs::path pathToFile = m_dirpath / fname;
    std::ofstream fout(pathToFile);
    for(int j = 0; j < x.size(); j++){
        fout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j] 
           << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << y[j] << std::endl; 
    }
    fout.close();
}



////////////////////////////////////////////////////////////////////////////////////////




void fileOpen(const char* FILE,std::ofstream& fout){
  fout.open(FILE);
  if(!fout.is_open())
  {
    std::cerr << "Could not open " << FILE << std::endl;
    std::cerr << "ABORTING" << std::endl;
    fout.clear();
    exit(EXIT_FAILURE);
  }
}

void fileOpen(const char* FILE,std::ifstream& fin){
  fin.open(FILE);
  if(!fin.is_open())
  {
    std::cerr << "Could not open " << FILE << std::endl;
    std::cerr << "ABORTING" << std::endl;
    fin.clear();
    exit(EXIT_FAILURE);
  }
}

void readDataFile(const std::string& nm,int sz,double* x,double* y){
  std::ifstream fin;
  fileOpen(nm.c_str(),fin);
  for(int i = 0; i < sz; i++){
      fin >> x[i];
      fin >> y[i];
  }
  fin.close();
}

int getThreeColFileSize(const std::string& nm){
  std::ifstream fin;
  fileOpen(nm.c_str(),fin);
  double garb;
  int len = -1;
  while(!fin.eof())
  {
   fin >> garb;
   fin >> garb;
   fin >> garb;
   len++;
  }
  fin.close();
  return len;
}

int getTwoColFileSize(const std::string& nm) {
  std::ifstream fin;
  fileOpen(nm.c_str(),fin);
  double garb;
  int len = -1;
  while(!fin.eof())
  {
   fin >> garb;
   fin >> garb;
   len++;
  }
  fin.close();
  return len;
}

////////////////////////////////////////////////////////////////////////////////////////////

void cOut(const char* nm,int sz,const double* x)
{
    std::cout << std::endl << nm << std::endl;
    for(int j = 0; j < sz; j++){
        std::cout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j] << std::endl; 
    }
    std::cout << std::endl;
}

void cOut(const char* nm,int sz,const double* x,const double* y)
{
    std::cout << std::endl << nm << std::endl;
    for(int j = 0; j < sz; j++){
        std::cout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j] 
           << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << y[j] << std::endl; 
    }
    std::cout << std::endl;
}

void cOut(const char* nm,int sz,const dcmplx* x)
{
    std::cout << std::endl << nm << std::endl;
    for(int j = 0; j < sz; j++){
      std::cout << std::scientific << std::setprecision(PRECISION) 
           << std::setw(WIDTH) << x[j].real() 
           << std::scientific << std::setprecision(PRECISION) 
           << std::setw(WIDTH) << x[j].imag() << std::endl; 
    }
    std::cout << std::endl;
}

void cOut(const char* nm,int sz,const double* x,const dcmplx* y)
{
    std::cout << std::endl << nm << std::endl;
    for(int j = 0; j < sz; j++){
      std::cout << std::scientific << std::setprecision(PRECISION) 
           << std::setw(WIDTH) << x[j] 
           << std::scientific << std::setprecision(PRECISION) 
           << std::setw(WIDTH) << y[j].real()  
           << std::scientific << std::setprecision(PRECISION) 
           << std::setw(WIDTH) << y[j].imag() << std::endl; 
    }
    std::cout << std::endl;
}

void cOut(const std::string& nm,int sz,const double* x,const double* y){
  cOut(nm.c_str(),sz,x,y);}
void cOut(const std::string& nm,int sz,const double* x,const dcmplx* y){
  cOut(nm.c_str(),sz,x,y);}
void cOut(const std::string& nm,int sz,const double* x){
  cOut(nm.c_str(),sz,x);}
void cOut(const std::string& nm,int sz,const dcmplx* x){
  cOut(nm.c_str(),sz,x);}


////////////////////////////////////////////////////////////////////////////////////////////


  void fOut(int num,const char* FILE,int sz,const dcmplx* x)
  {
    using namespace std;
    std::ostringstream stm;
    std::ofstream fout;
    std::string str(FILE);

    stm << num;
    cout << str << endl;
    std::string fileName = str + "_" +  stm.str() + ".dat";
    fout.open(fileName.c_str());
    cout.flush();
    if(!fout.is_open())
    {
      std::cerr << "Could not open " << fileName << std::endl;
      std::cerr << "ABORTING" << std::endl;
      fout.clear();
      exit(EXIT_FAILURE);
    }

    for(int j = 0; j < sz; j++){
      fout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j].real()
           << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j].imag() << std::endl; 
    }
    fout << std::endl;
    fout.close();
  }

  void fOut(int num,const char* FILE,int sz,const double* x,const double* y)
  {
    std::ostringstream stm;
    std::ofstream fout;
    std::string str(FILE);

    stm << num;
    std::string fileName = str + "_" +  stm.str() + ".dat";
    fout.open(fileName.c_str());
    if(!fout.is_open())
    {
      std::cerr << "Could not open " << fileName << std::endl;
      std::cerr << "ABORTING" << std::endl;
      fout.clear();
      exit(EXIT_FAILURE);
    }

    for(int j = 0; j < sz; j++){
      fout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j] 
           << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << y[j] << std::endl; 
    }
    fout << std::endl;
    fout.close();
  }


  void fOut(int num,const char* FILE,int sz,const double* x,const dcmplx* y)
  {
    std::ostringstream stm;
    std::ofstream fout;
    std::string str(FILE);

    stm << num;
    std::string fileName = str + "_" +  stm.str() + ".dat";
    fout.open(fileName.c_str());
    if(!fout.is_open())
    {
      std::cerr << "Could not open " << fileName << std::endl;
      std::cerr << "ABORTING" << std::endl;
      fout.clear();
      exit(EXIT_FAILURE);
    }

    for(int j = 0; j < sz; j++){
      fout << std::scientific << std::setprecision(PRECISION) 
           << std::setw(WIDTH) << x[j] 
           << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << y[j].real() 
           << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << y[j].imag() << std::endl; 
    }
    fout << std::endl;
    fout.close();
  }

  void fOut(int num,const char* FILE,int sz,const double* x)
  {
    std::ostringstream stm;
    std::ofstream fout;
    std::string str(FILE);

    stm << num;
    std::string fileName = str + "_" +  stm.str() + ".dat";
    fout.open(fileName.c_str());
    if(!fout.is_open())
    {
      std::cerr << "Could not open " << fileName << std::endl;
      std::cerr << "ABORTING" << std::endl;
      fout.clear();
      exit(EXIT_FAILURE);
    }

    for(int j = 0; j < sz; j++){
      fout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j] << std::endl; 
    }
    fout << std::endl;
    fout.close();
  }

  void fOut(int num,const std::string& fname,int sz,const double* x,const double* y){
      fOut(num,fname.c_str(),sz,x,y);}
  void fOut(int num,const std::string& fname,int sz,const double* x,const dcmplx* y){
      fOut(num,fname.c_str(),sz,x,y);}
  void fOut(int num,const std::string& fname,int sz,const double* x){
      fOut(num,fname.c_str(),sz,x);}
  void fOut(int num,const std::string& fname,int sz,const dcmplx* x){
      fOut(num,fname.c_str(),sz,x);}

////////////////////////////////////////////////////////////////////////////////////////////

  void fOut(const char* FILE,int sz,const double* x)
  {
      std::ofstream fout;
      fileOpen(FILE,fout);
      for(int j = 0; j < sz; j++)
        fout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j] << std::endl; 
      fout << std::endl;
      fout.close();
  }

  void fOut(const char* FILE,int sz,const dcmplx* y)
  {
    std::ofstream fout;
    fileOpen(FILE,fout);
    for(int j = 0; j < sz; j++){
      fout << std::scientific << std::setprecision(PRECISION) 
           << std::setw(WIDTH) << y[j].real() 
           << std::scientific << std::setprecision(PRECISION) 
           << std::setw(WIDTH) << y[j].imag() << std::endl; 
    }
    fout.close();
  }

  void fOut(const char* FILE,int sz,const double* x,const double* y)
  {
    std::ofstream fout;
    fileOpen(FILE,fout);
    for(int j = 0; j < sz; j++){
      fout << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << x[j] 
           << std::scientific << std::setprecision(PRECISION) << std::setw(WIDTH) << y[j] << std::endl; 
    }
    fout << std::endl;
    fout.close();
  }


  void fOut(const char* FILE,int sz,const double* x,const dcmplx* y)
  {
    std::ofstream fout;
    fileOpen(FILE,fout);
    for(int j = 0; j < sz; j++){
      fout << std::scientific << std::setprecision(PRECISION) 
           << std::setw(WIDTH) << x[j] 
           << std::scientific << std::setprecision(PRECISION) 
           << std::setw(WIDTH) << y[j].real() 
           << std::scientific << std::setprecision(PRECISION) 
           << std::setw(WIDTH) << y[j].imag() << std::endl; 
    }
    fout.close();
  }
  void fOut(const std::string& fname,int sz,const double* x,const double* y) {
      fOut(fname.c_str(),sz,x,y);}
  void fOut(const std::string& fname,int sz,const double* x,const dcmplx* y) {
      fOut(fname.c_str(),sz,x,y);}
  void fOut(const std::string& fname,int sz,const double* x){
      fOut(fname.c_str(),sz,x);}
  void fOut(const std::string& fname,int sz,const dcmplx* x){
      fOut(fname.c_str(),sz,x);}

////////////////////////////////////////////////////////////////////////////////////////////


}











