#include <iostream>

using namespace std;

//aqui le saco el modulo de num en base a zn, osea segun la teoria, saco su equivalente 
int modulo(int num, int Zn)
{
    int q = num / Zn;
    int r = num - (q * Zn);
    if (r < 0)
    {
        q--;
        r = num - (q * Zn);
    }
    return r;
}

int main()
{
    int zn; cout << "Zn:"; cin >> zn;
    int num; cout << "num:"; cin >> num;
    cout << "modulo(" << num << "," << zn << ")" << "=" << modulo(num, zn) << endl;
    return 0;
}