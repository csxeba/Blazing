#ifndef BLASING_H
#define BLASING_H

#include <assert.h>
#include <exception>

#include "mats.h"

#define CI  int*
#define CD  double*
extern "C"{
    //BLAS lvl 1
    
    //x := alpha * x
    void dscal_(CI N, CD alpha, double* x, CI incx);
    
    //y := alpha * x + y
    void daxpy_(CI N, CD alpha, CD x,
                CI incx, double* y, CI incy);
    
    //inner product of 2 vectors. Returns value!
    double ddot_(CI N, CD x, CI incx,
                 CD y, CI incy);
    
    //sum of absolute values (might be useful || returns value!)
    double dasum_(CI N, CD x, CI incx);
    
    //BLAS lvl 2
    
    //A := xy**T + A (outer product)
    void dger_(CI M, CI N, CD alpha, CD x, CI incx, CD y, CI incy,
               double* A, CI lda);

    //BLAS lvl 3
    //C := alpha*A . B + beta*C (dot product of matrices)
    // A is assumed to be MxK, B is KxN, C is MxN
    void dgemm_( char* transa,  char* transb, 
                CI M, CI N, CI K,
                CD alpha, CD A, CI lda, CD B, CI ldb,
                CD beta, double* C, CI ldc);
}
#undef CI
#undef CD

//My wrappers for the BLAS routines
void dscal(Vector<double> &a, double alpha) {
    int N = a.size();
    int incx = 1;
    dscal_(&N, &alpha, a.point(), &incx);
}

void daxpy(Vector<double> &x, Vector<double> &y, double alpha) {
    assert(x.size() == y.size());
    int N = x.size();
    int incx = 1, incy = 1;
    daxpy_(&N, &alpha, x.point(), &incx, y.point(), &incy);
}

double dasum(Vector<double> &x) {
    int N = x.size(), incx = 1;
    return dasum_(&N, x.point(), &incx);
}

double ddot(Vector<double> &a, Vector<double> &b) {
    assert(a.size()==b.size());
    int inca = 1, incb = 1;
    int N = a.size();
    return ddot_(&N, a.point(), &inca, b.point(), &incb);
}

void dger(Vector<double> x, Vector<double> y, Matrix<double> A) {
    throw 1; //Do not use! I don't understand it :D
    int M = x.size(), N = y.size();
    int incx = 1, incy = 1;
    int lda = M;
    double alpha = 1;
    
    dger_(&M, &N, &alpha, x.point(), &incx, y.point(), &incy,
               A.point(), &lda);
}

double frobenius(Matrix<double> &A, Matrix<double> &B) {
    assert(A.dimsmatch(B));
    int inca = 1, incb = 1;
    int N = A.size();
    return ddot_(&N, A.point(), &inca, B.point(), &incb);
}

double frobenius(Volume<double> &A, Volume<double> &B) {
    assert(A.dimsmatch(B));
    int inca = 1, incb = 1;
    int N = A.size();
    return ddot_(&N, A.point(), &inca, B.point(), &incb);
}

void dgemm(Matrix<double> &A, Matrix<double> &B, Matrix<double> &C) {
    int* shape1 = A.shape();
    int* shape2 = B.shape();
    assert(shape1[1] == shape2[0]);
    int M = shape1[0], N = shape2[1], K = shape1[1];
    int lda = M, ldb = K, ldc = M;
    char transa = 'N', transb = 'N';
    double alpha = 1.0, beta=1.0;
    
    dgemm_(&transa, &transb, &M, &N, &K, &alpha, A.point(), &lda,
           B.point(), &ldb, &beta, C.point(), &ldc);
}

#endif /* BLASING_H */

