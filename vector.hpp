#include <iostream>
using namespace std;

template <typename T>
class Vector{
    private:
    T* a = (T*)malloc(1 * sizeof(T));
    unsigned long n = 1;
    unsigned long end = 0;

    public:
    T operator[](unsigned long x){
        return a[x];
    }

    T top(){
        return a[end - 1];
    }

    void resize(unsigned long sz){
        if(sz > n){
            while(sz > n) n <<= 1;

            T* tmp = (T*)malloc(n * sizeof(T));
            for(int i = 0; i < end; i++){
                tmp[i] = a[i];
            }
            free(a);
            a = tmp;
        }
        end = sz;
    }

    void push(T x){
        if(end + 1 >= n){
            T* tmp = (T*)malloc((n << 1) * sizeof(T));
            for(int i = 0; i < end; i++){
                tmp[i] = a[i];
            }
            free(a);
            a = tmp;
            n <<= 1;
        }
        a[end++] = x;
    }

    unsigned long size(){
        return end;
    }
};