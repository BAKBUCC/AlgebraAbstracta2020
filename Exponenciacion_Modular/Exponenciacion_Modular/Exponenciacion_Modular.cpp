#include <iostream>

using namespace std;

template <class T>
T modular(T num, T Zn)
{
    T q = num / Zn;
    T r = num - (q * Zn);
    if (r < 0)
    {
        q--;
        r = num - (q * Zn);
    }
    return r;
}

template <class T>
T Algoritmo_1(T a, T p, T n)
{
    T r = 1;
    for (T i = 1; i < p; i++)
    {
        r = modular<T>(modular<T>(r, n) * modular<T>(a, n), n);
    }
    return r;
}

template <class T>
T Algoritmo_2_libre(T g, T A, T N)
{
    T a = g, b = 1;
    T dos = 2;
    while (A > 0)
    {
        if (modular<T>(A, dos)) { b = modular<T>(modular<T>(b, N) * modular<T>(a, N), N); }
        a = modular<T>(modular<T>(a, N) * modular<T>(a, N), N);
        A = A / 2;
    }
    return b;
}

template <class T>
T Algoritmo_2_bit(T g, T A, T N)   //me falta aplicar bitwise
{
    T a = g, b = 1;
    T dos = 2;
    while (A > 0)
    {
        if (modular<T>(A, dos)) { b = modular<T>(modular<T>(b, N) * modular<T>(a, N), N); }
        a = modular<T>(modular<T>(a, N) * modular<T>(a, N), N);
        A = A / 2;
    }
    return b;
}


template <class T>
T Algoritmo_3()
{
    int cant;
    cout << "# de ecuaciones: "; cin >> cant;
    //aun me falta esto
    return;
}

template <class T>
T Algoritmo_4(T a, T p, T n)
{
    if (p == 0) { return 1; }
    T dos = 2;
    if (!modular(p, dos))
    {
        T t = Algoritmo_4(a, p / 2, n);
        return modular<T>(modular<T>(t, n) * modular<T>(t, n), n);
    }
    T t = Algoritmo_4(a, (p - 1) / 2, n);
    return modular<T>(modular<T>(a, n) * modular<T>(modular<T>(t, n) * modular<T>(t, n), n), n);
}



template <class T>
T Algoritmo_5(T a, T b, T n)
{
    T c, d;
    c = 0, d = 1;
    //digamos que tenemos la lista bkbk-1...b0
    for (;;)//de i=0 a k
    {
        c *= 2;
        if ()//*iterador==1?
        {
            c += 1;
            d = modular<T>(modular<T>(d, n) * modular<T>(a, n), n);
        }
        a = modular<T>(modular<T>(a, n) * modular<T>(a, n), n);
    }
    return d;
}

template <class T>
T Algoritmo_6(T a, T b, T n)
{
    T exp = 1;
    T x = modular<T>(a, n);
    T dos = 2;
    while (b > 0)
    {
        if (modular(b, 2)) { exp = modular<T>(modular<T>(exp, n) * modular<T>(x, n), n); }
        x = modular<T>(modular<T>(x, n) * modular<T>(x, n), n);
        b /= 2;
    }
    return exp;
}

int main()
{
    std::cout << "Hello World!\n";
}