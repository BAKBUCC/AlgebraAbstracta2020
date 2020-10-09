#include <iostream>
#include <string>
#include <sstream> 
#include<stdlib.h>
#include<time.h>

using namespace std;
using std::ostringstream;


string alf("abcdefghijklmnopqrstuvwxyz().0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ,+-");//("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789({-})_/,.");



//retornamos el modulo de num y Zn
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

//saco el valor absoluto
inline int valorAbsoluto(int a)
{
    if (a < 0) { return -a; }
    return a;
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

int* alg_ext_eucl_2(int a, int b)
{
    int r1 = valorAbsoluto(a), r2 = valorAbsoluto(b);
    int s1 = 1, s2 = 0;
    int t1 = 0, t2 = 1;

    // este array de q guardara los cocientes de r1/r2
    int* q = new int[50];
    int i = 0;//este indice contara las veces que itera hasta que r2 sea 0

    while (r2 > 0)
    {
        *q = r1 / r2;

        int r = r1 - (*q) * r2;
        r1 = r2; r2 = r;

        q++;
        i++;
    }
    q -= i;  // volvemos al array "q" a sus inicios
    if (r1 == 1)  // d==1
    {
        while (i--) // i iterara hasta que acabe el array
        {
            int s = s1 - (*q) * s2;
            s1 = s2; s2 = s;

            int t = t1 - (*q) * t2;
            t1 = t2; t2 = t;

            q++;
        }
        int* gcd = new int[3];
        gcd[0] = r1; gcd[1] = s1; gcd[2] = t1;// gcd[0] = a*gcd[1] + b*gcd[2] // d= a*x + b*y;
        return gcd;
    }
    delete[]q;
    return &r1;//sino , solo devolverá el mcd que está en r1
}

int generadorNumAleatorio(int fin)
{
    srand(time(NULL));
    return 1 + rand() % (fin - 1);
}



struct Afin
{
    Afin();
    Afin(string alf);
    Afin(int a, int b, string alf);
    string cifrar(string m);
    string descifrar(string m_c);
    int a, b;
private:
    void generarClaves(int& a, int& b);
    string alf;
};

//contructor del emisor
Afin::Afin()
{
    alf = "abcdefghijklmnopqrstuvwxyz";
    generarClaves(a, b);
}

//constructor del emisor
Afin::Afin(string alf)
    : alf(alf) 
{
    cout << "alfabetos.size(): " << alf.size() << endl;
    generarClaves(a, b);
}

//constructor del receptor
Afin::Afin(int a, int b, string alf)
    :a(a), b(b), alf(alf) {}

void Afin::generarClaves(int& a, int& b)
{
    do 
    {
        a = generadorNumAleatorio(alf.size());
    } while (MCD_binario(a, alf.size()) != 1); //n y a son coprimos
    cout << "a: " << a << endl;
    do
    {
        b = generadorNumAleatorio(alf.size());//dentro de Zn
    } while (b == a);
    cout << "b: " << b << endl;
}

//se cifra el mensaje "m" y lo retorna
string Afin::cifrar(string m)
{
    ostringstream m_c;

    for (string::iterator i = m.begin(); i < m.end(); i++)
    {
        int pos_alf = alf.find(*i);//retornamos la posición del caracter "*I" en el "alf"
        pos_alf *= a;
        pos_alf += b;
        pos_alf = modulo(pos_alf, alf.size());
        m_c << alf[pos_alf];
    }

    return m_c.str();
}

//se descifra el mensaje "m_c" y lo retorna
string Afin::descifrar(string m_c)
{
    ostringstream m_d;
    
    int* x = alg_ext_eucl_2(a, alf.size());// tenemos un array que contiene contine arr[0]=mcd, arr[1]=x arr[2]=y y cada uno es referido a mcd = a(x) + tam_alf(y)
    x++; // avanzamos a arr[1]=x quien es el inverso de a.
    cout << "x: " << *x << endl;

    for (string::iterator i = m_c.begin(); i < m_c.end(); i++)
    {
        int pos_alf = alf.find(*i);//retornamos la posición del caracter "*i" en el "alf"
        pos_alf -= b;
        pos_alf *= (*x);
        pos_alf = modulo(pos_alf, alf.size());
        m_d << alf[pos_alf];
    }

    return m_d.str();
}

int main()
{
    /*
    Afin emisor(alf);
    Afin receptor(emisor.a, emisor.b, alf);

    string mensaje;
    cout << "mensaje:"; getline(cin, mensaje);

    string mensaje_cifrado = emisor.cifrar(mensaje);
    cout << "\n\nMENSAJE CIFRADO :\n\"" << mensaje_cifrado << "\"" << endl;

    string mensaje_descifrado = receptor.descifrar(mensaje_cifrado);
    cout << "\n\nMENSAJE DESCIFRADO\n\"" << mensaje_descifrado << "\"" << endl;


    //NUEVO MENSAJE

    cout << "otro mensaje:"; getline(cin, mensaje);

    mensaje_cifrado = emisor.cifrar(mensaje);
    cout << "\n\nMENSAJE CIFRADO :\n\"" << mensaje_cifrado << "\"" << endl;

    mensaje_descifrado = receptor.descifrar(mensaje_cifrado);
    cout << "\n\nMENSAJE DESCIFRADO\n\"" << mensaje_descifrado << "\"" << endl;
    */
    Afin receptor(41, 33, alf);

    string mensaje;
    cout << "mensaje:"; getline(cin, mensaje);

    string mensaje_descifrado = receptor.descifrar(mensaje);
    cout << "\n\n1_MENSAJE DESCIFRADO\n\"" << mensaje_descifrado << "\"" << endl;

    cout << "otro mensaje:"; getline(cin, mensaje);

    mensaje_descifrado = receptor.descifrar(mensaje);
    cout << "\n\n!2_MENSAJE DESCIFRADO\n\"" << mensaje_descifrado << "\"" << endl;


    return 0;
}