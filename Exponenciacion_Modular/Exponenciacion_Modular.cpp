#include <iostream>

using namespace std;

//aqui le saco el modulo de num en base a Zn, osea segun la teoria, saco su equivalente ;
int modular(const int& num, const int& Zn) //estoy optimizando la función modular haciendo que los parametros sean apodos de las variables;
{                                          //y no ocupen más espacio en la memoria; le pongo const para que por si acaso no sean cambiados las variables;
    int q = num / Zn;
    int r = num - (q * Zn);
    if (r < 0)
    {
        q--;
        r = num - (q * Zn);
    }
    return r;
}

int exponenciacionModular(int base, int exponente, int modulo)
{
    int resultado = 1, dos = 2;
    int cuadrado = modular(base, modulo);
    while (exponente)
    {
        /*aqui creo una variable "aux" para que pueda mandar a la funcion modular una direccion de memoria*/int aux = cuadrado * resultado;
        if (modular(exponente, dos)) { resultado = modular(aux, modulo); }
        exponente /= 2;
        /*aqui hago lo mismo de arriba pero con otros valores que se multiplican----------------------------->*/aux = cuadrado * cuadrado;
        cuadrado = modular(aux, modulo);
    }
    return resultado;
}

int main()
{
    int base, exponente, modulo;
    cout << "base: "; cin >> base; cout << "\nexponente: "; cin >> exponente; cout << "\nmodulo: "; cin >> modulo;
    cout << "\n\n" << base << "^" << exponente << " (modulo" << modulo << ")  =  " << exponenciacionModular(base, exponente, modulo) << endl;
    return 0;
}

