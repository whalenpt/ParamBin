
#ifndef PWCONSTANTS_H_ 
#define PWCONSTANTS_H_

#include <complex>
#include <vector>

namespace pw{

    using dcmplx = std::complex<double>;
    const dcmplx ii (0.0,1.0);

    const int PRECISION = 10;
    const int WIDTH = 20;
    const int COUT_PRECISION = 14;
    const int COUT_WIDTH = 28;

}

#endif

