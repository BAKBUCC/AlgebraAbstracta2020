#include <iostream>

using namespace std;

//saco el valor absoluto
inline int valorAbsoluto(int &a)
{
    if (a < 0) { return -a; }
    return a;
}

//retorno el modulo/equivalente de num en Zn
inline int modular(const int &num, const int &Zn)
{
    int q = num / Zn;
    int r = num - (q * Zn);
    if (r < 0)
    {
        q++;
        r = num - (q * Zn);
    }
    return r;
}

//MCD de euler
inline int MCD(int a, int b)
{
    int r = modular(a, b);
    while (r > 0)
    {
        a = b;
        b = r;
        r = modular(a, b);
    }
    return b;
}

//MCD propuesto basado en la division susesiva de numeros a los valores a y b, los numeros que dividan a ambos se ir�n-
//multiplicando
inline int MCD_propuesto(int a, int b)
{
    int mcd = 1, dos = 2;
    while (!modular(a, dos) && !modular(b, dos))
    {
        mcd *= 2;
        a /= 2; b /= 2;
    }
    int i = 3;
    while (i < a && i < b)
    {
        if (modular(a, i) == 0 && modular(b, i) == 0)
        {
            mcd *= i;
            a /= i; b /= i;
        }
        else
        {
            i += 2;
        }
    }
    return mcd;
}


//MCD_binario de procedencia china y muy bueno para calculos rapidos y grandes
//puedes encontrarlo en el libro: Mora introduccion a la Teoria de Numeros
//parte 2 capitulo 8 , pagina 163;
int MCD_binario(int a, int b)
{
    int g = 1, dos = 2;
    while (!modular(a, dos) && !modular(b, dos))
    {
        a /= 2; b /= 2;
        g *= 2;
    }
    while (a)
    {
        if (!modular(a, dos)) { a /= 2; }
        else if (!modular(b, dos)) { b /= 2; }
        else
        {
            int c = a - b;
            int t = valorAbsoluto(c) / 2;
            if (a >= b) { a = t; }
            else { b = t; }
        }
    }
    return g * b;
}

int main()
{
    int a; cout << "a: "; cin >> a;
    int b; cout << "b: "; cin >> b; 
    cout << "\nmcd_normal(" << a << "," << b << "):" << MCD_propuesto(a, b);
    cout << "\n\nmcd_propuesto(" << a << "," << b << "):" << MCD_propuesto(a, b);
    cout << "\n\nmcd_binario(" << a << "," << b << "):" << MCD_binario(a, b);
    return 0;
}
