#include <iostream>

using namespace std;

//saco el valor absoluto
inline int valorAbsoluto(int a)
{
    if (a < 0) { return -a; }
    return a;
}

//retorno el modulo/equivalente de num en Zn
inline int modulo(int num, int Zn)
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
    int r = modulo(a, b);
    while (r > 0)
    {
        a = b;
        b = r;
        r = modulo(a, b);
    }
    return b;
}

//MCD propuesto basado en la division susesiva de numeros a los valores a y b, los numeros que dividan a ambos se irán-
//multiplicando
inline int MCD_propuesto(int a, int b)
{
    int mcd = 1;
    while (!modulo(a, 2) && !modulo(b, 2))
    {
        mcd *= 2;
        a /= 2; b /= 2;
    }
    int i = 3;
    while (i < a && i < b)
    {
        if (modulo(a, i) == 0 && modulo(b, i) == 0)
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
    int g = 1;
    while (!modulo(a, 2) && !modulo(b, 2))
    {
        a /= 2; b /= 2;
        g *= 2;
    }
    while (a)
    {
        if (!modulo(a, 2)) { a /= 2; }
        else if (!modulo(b, 2)) { b /= 2; }
        else
        {
            int t = valorAbsoluto(a - b) / 2;
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
