#ifndef MATS_H
#define MATS_H

#include <initializer_list>
#include <iostream>
#include <assert.h>

template <typename T>
class Tensor {
    public:
        Tensor() {}
        virtual ~Tensor() {};
        virtual int* shape()  = 0;
        virtual int size() = 0;
        virtual T* point()  = 0;
        virtual void print()  = 0;
        virtual T at(int x) = 0;
};

template <typename T>
class Vector : public Tensor<T> {
    public:
        Vector(int N);
        Vector(int N, T defval);
        Vector(int N, T*);
        Vector(std::initializer_list<T> l);
        Vector(Vector<T>&);
        ~Vector() {delete Guts;}

        virtual T* point()      {return Guts;}
        virtual int* shape()    {return &Size;}
        virtual int size()      {return Size;}        
        virtual void print();
        virtual T at(int x)     {return Guts[x];}
    
    protected:
        T* Guts;
        int Size;
};

template <typename T>
class Matrix: public Vector<T> {
    public:
        Matrix(int M, int N, T defval=0);
        Matrix(int M, int N, Vector<T>&);
        
        int* shape()                {return Dims;}
        void print();
        
        T at(int x, int y)  {return Vector<T>::Guts[y*Dims[0]+x];}
        virtual bool dimsmatch(Matrix<T>&);
    private:
        int Dims[2];
};

template <typename T>
class Volume: public Matrix<T> {
    public:
        Volume(int M, int N, int O, T defval=0);
        Volume(int M, int N, int O, Vector<T>&);

        int* shape()        {return Dims;}
        void print();
        
        T at(int x,int y,int z) {return Vector<T>::Guts[z*Dims[0]+y*Dims[1]+x];}
        bool dimsmatch(Volume<T>&);
    
    private:
        int Dims[3];
};

//End of class definitions
//Start of vector member function declarations

template <typename T>
Vector<T>::Vector(int N) {
    Size = N;
    Guts = new T[Size];
}

template <typename T>
Vector<T>::Vector(int N, T* ar) {
    Size = N;
    Guts = new T[Size];
    for(int x=0;x<Size;x++) {
        Guts[x] = ar[x];
    }
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> l) {
    Size = l.size();
    Guts = new T[Size];
    int i=0;
    for(auto x=l.begin();x!=l.end();x++, i++) {
        Guts[i] = *x;
    }
}

template <typename T>
Vector<T>::Vector(Vector<T> &v) {
    Size = v.size();
    Guts = new T[Size];
    int i=0;
    for(int x=0;x<v.size();x++) {
        Guts[i] = v.at(x);
    }
}

template <typename T>
Vector<T>::Vector(int N, T defval) {
    Size = N;
    Guts = new T[Size];
    for(int x=0;x<Size;x++) {
        Guts[x] = defval;
    }
}

template <typename T>
void Vector<T>::print()  {
    for (int i=0; i<Size; i++) {
        std::cout << Guts[i] << ", " << std::endl;
    }
}

// Vector declarations end
// Matrix declarations begin

template <typename T>
Matrix<T>::Matrix(int M, int N, T defval) :
        Vector<T>::Vector(M*N, defval)
{
    Dims[0] = M;
    Dims[1] = N;
}

template <typename T>
Matrix<T>::Matrix(int M, int N, Vector<T> &vec) :
        Vector<T>::Vector(vec){
    assert(vec.size()==M*N);
    Dims[0] = M;
    Dims[1] = N;
}

template <typename T>
void Matrix<T>::print()  {
    for (int i=0; i<Dims[0]; i++) {
        for (int j=0; j<Dims[1]; j++) {
            std::cout << Vector<T>::Guts[i*Dims[0]+j] << ", ";
        }
        std::cout << std::endl;
    }
}

template <typename T>
bool Matrix<T>::dimsmatch(Matrix<T>& mat) {
    int* mshape = mat.shape();
    return (Dims[0]==mshape[0] and Dims[1]==mshape[1]);
}

//Matrix declarations end
//Volume declarations begin

template <typename T>
Volume<T>::Volume(int M, int N, int O, T defval) :
        Matrix<T>::Matrix(M*N, O, defval){
    Dims[0] = M;
    Dims[1] = N;
    Dims[2] = O;
}

template <typename T>
Volume<T>::Volume(int M, int N, int O, Vector<T> &vec) :
        Matrix<T>::Matrix(M*N, O, vec) {
    Dims[0] = M;
    Dims[1] = N;
    Dims[2] = O;
}

template <typename T>
void Volume<T>::print()  {
    for (int i=0; i<Dims[0]; i++) {
        for (int j=0; j<Dims[1]; j++) {
            for (int k = 0; k < Dims[2]; k++) {
                std::cout << Matrix<T>::Guts[i*Dims[0]+j*Dims[1]+k] << ", ";
            }
            std::cout << std::endl;
        }
        std::cout << "-------------------------" << std::endl;
    }
}

template <typename T>
bool Volume<T>::dimsmatch(Volume<T>& vol) {
    int* vshape = vol.shape();
    int* shape1 = Dims; int* shape2 = vshape;
    return (Dims[0]==vshape[0] and Dims[1]==vshape[1] and Dims[2]==vshape[2]);
}

#endif /* MATS_H */
