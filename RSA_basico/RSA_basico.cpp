#include <iostream>
#include <string>
#include <sstream> 
#include<stdlib.h>
#include<time.h>

using namespace std;
using std::ostringstream;

string alf("abcdefghijklmnopqrstuvwxyz().0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ,+-");

//sacar el valor absoluto del numero "a"
template <class T>
inline T valorAbsoluto(T a)
{
    if (a < 0) { return -a; }
    return a;
}

//sacar un numero aleatorio desde 1 hasta el numero "fin", quien normalmente, es es "n" de Z_n, osea, saca un numero dentro de Zn.
template <class T>
T generadorNumAleatorio(T fin)
{
    srand(time(NULL));
    return 1 + rand() % (fin - 1);
}

//saca el "num (mod Zn)"
template <class T>
T modular(T const& num, T const& Zn)
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

//saca el MCD de forma binaria, y por lo tanto, más rapido.
template <class T>
T MCD_binario(T a, T b)
{
    T g = 1;
    int dos = 2;
    while (!modular<T>(a, dos) && !modular<T>(b, dos))
    {
        a /= 2; b /= 2;
        g *= 2;
    }
    while (a)
    {
        if (!modular<T>(a, dos)) { a /= 2; }
        else if (!modular<T>(b, dos)) { b /= 2; }
        else
        {
            T t = valorAbsoluto<T>(a - b) / 2;
            if (a >= b) { a = t; }
            else { b = t; }
        }
    }
    return g * b;
}

// se saca el euclide extendido mandando la lista de cocientes (*q) que se generan  en la función "inversa", y se saca el X y Y de la función aX + bY=d
template <class T>
T euclidesExtendido(T& i, T* q)
{
    T s1 = 1, s2 = 0;
    T t1 = 0, t2 = 1;

    while (i--)
    {
        T s = s1 - (*q) * s2;
        s1 = s2; s2 = s;

        T t = t1 - (*q) * t2;
        t1 = t2; t2 = t;

        q++;
    }
    
    return s1;
}


template <class T>
T inversa(T& a, T& b)
{
    T r1 = valorAbsoluto<T>(a), r2 = valorAbsoluto<T>(b);

    T* q = new T[100];//aqui voy a guardar los cocientes que se generen en el while de acá abajo y lo mandaré a la función inversa si el mcd=1.
    T i = 0;

    while (r2 > 0)
    {
        *q = r1 / r2;

        T r = r1 - (*q) * r2;
        r1 = r2; r2 = r;

        q++;
        i++;
    }
    q -= i;
    if (r1 == 1)//si el mcd=1 entonces..
    {
        T x = euclidesExtendido<T>(i, q); //..saco la inversa
        delete[]q;//borro la lista ya que no me sirve
        x = modular<T>(x, b); //saco su equivalente por si acaso este fuera del conjunto Z_n
        return x;
    }
}

//saco la exponenciación modular siguiendo el algoritmo dado en clase.
template <class T>
T exponenciacionModular(T base, T exponente, T modulo)
{
    T resultado = 1, dos = 2;
    T cuadrado = modular<T>(base, modulo);
    while (exponente)
    {
        T aux = cuadrado * resultado;
        if (modular<T>(exponente, dos)) { resultado = modular<T>(aux, modulo); }
        exponente /= 2;
        aux = cuadrado * cuadrado;
        cuadrado = modular<T>(aux, modulo);
    }
    return resultado;
}

template <class T>
struct RSA 
{
    RSA(); //del receptor
    RSA(T N, T e);  //del emisor

    string cifrar(string m);
    string descifrar(string m_c);

    T e, N;
private:
    void generarClaves();

    T p, q, d, phiN;
};

//este es el constructor del emisor
template <class T>
RSA<T>::RSA()
{
    generarClaves();
}

//este es el constructor del receptor quien obtiene la clave publica
template <class T>
RSA<T>::RSA(T N, T e) :N(N), e(e) {}

template <class T>
void RSA<T>::generarClaves()
{
    cout << "de dos primos grandes al azar \n";
    cout << "p: "; cin >> p;
    cout << "q: "; cin >> q;

    N = p * q;
    phiN = (p - 1) * (q - 1);

    
    do
    {
        e = generadorNumAleatorio<T>(phiN);
    } while (MCD_binario<T>(e, phiN) != 1);
    
    //cout << "e: "; cin >> e;
    d = (inversa<T>(e, phiN));
}

//sigo la funcion cifrar del rsa
template <class T>
string RSA<T>::cifrar(string m)
{
    ostringstream m_c;

    for (string::iterator i = m.begin(); i < m.end(); i++)
    {
        T pos_alf = alf.find(*i);

        pos_alf = exponenciacionModular<T>(pos_alf, e, N);
        m_c << to_string(pos_alf);//guardo el resultado de la exponenciación.

        if ((i + 1) != m.end()) { m_c << "-"; }//si aún no llega al final, entonces le pongo un guion para separar del siguiente resultado del otro caracter
    }

    return m_c.str();
}

//sigo la funcion descifrar del rsa
template <class T>
string RSA<T>::descifrar(string m_c)
{
    ostringstream m_d, bloque;

    int tam_alf = alf.size();

    for (string::iterator i = m_c.begin(); i < m_c.end(); i++)
    {
        if (*i == '-' || (i + 1) == m_c.end()) //si el iterador (i) tiene el valor '-', entonces ... o  si el siguiente el iterador esta a una de llegar al final ...
        {
            if ((i + 1) == m_c.end()) { bloque << *i; }//aqui, si el iterador esta a una de llegar al final, se le agrega el valor del que estamos justo ahora, 
            //ya que no es guion, sino un numero que aún falta poner. 
            else { i++; }//si está posicionado en un guion(esto esta controlado por el primer if), no lo tomo, sino que avanzo al siguiente bloque,
            //pero aún no agrego el numero del siguiente bloque

            //conversión de string a int
            stringstream geek(bloque.str());
            T pos_alf = 0;
            geek >> pos_alf;

            //hago lo que está en la función de descifrado
            pos_alf = exponenciacionModular<T>(pos_alf, d, N);
            pos_alf = modular<T>(pos_alf, tam_alf);
            m_d << alf[pos_alf];

            //reinicio el ostringstream para poner el nuevo bloque
            bloque.str("");
            bloque.clear();
        }
        bloque << *i;//coloco el valor de i en el bloque
    }

    return m_d.str();
}

//signed long long int
int main()
{
    //Declaro dos instacias con template
    RSA<signed long long int> receptor;
    RSA<signed long long int> emisor(receptor.N, receptor.e);

    //pido un mensaje
    string mensaje;
    cin.ignore();
    cout << "mensaje:"; getline(cin, mensaje);

    //cifro el mensaje
    string mensaje_cifrado = emisor.cifrar(mensaje);
    cout << "\n\nMENSAJE CIFRADO :\n\"" << mensaje_cifrado << "\"" << endl;

    //descifro el mensaje
    string mensaje_descifrado = receptor.descifrar(mensaje_cifrado);
    cout << "\n\nMENSAJE DESCIFRADO\n\"" << mensaje_descifrado << "\"" << endl;
    return 0;
}