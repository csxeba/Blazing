#include <iostream>

#include "mats.h"
#include "blasing.h"

using namespace std;

typedef Vector<double> dvec;
typedef Matrix<double> dmatrix2;
typedef Volume<double> dmatrix3;

int main()
{
    dvec v1({2, 2, 2, 2});
    dvec v2({2, 2, 2, 2});
    dmatrix2 mat1(4, 2, 2.0);
    dmatrix2 mat2(2, 4, 2.0);
    dmatrix2 mat3(4, 4, 0.0);
    
    dmatrix3 vol1(3, 3, 3, 3.0);
    dmatrix3 vol2(3, 3, 3, 3.0);
    
    mat3.print();
    
    dgemm(mat1, mat2, mat3);
           
    mat3.print();
    
    return 0;
}