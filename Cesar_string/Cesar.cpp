#include <iostream>
#include <string>
#include <sstream> 

using namespace std;
using std::ostringstream; 


string alf("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789({-})_/,.");


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

struct Cesar
{
    Cesar();
    Cesar(int clave);
    Cesar(string alf);
    Cesar(string alf, int clave);
    string cifrar(string m);
    string descifrar(string m_c);
private:
    string alf;
    int clave;
};

//cuando no mandan parametros al inicializar una clase Cesar
Cesar::Cesar()
{
    alf = "abcdefghijklmnopqrstuvwxyz";
    clave = 1;
}

Cesar::Cesar(int clave)
    :clave(clave){}

Cesar::Cesar(string alf)
    :alf(alf){}

Cesar::Cesar(string alf, int clave)
    :alf(alf),clave(clave){}

//se cifra el mensaje "m" y lo retorna
string Cesar::cifrar(string m)
{
    ostringstream m_c;

    for (string::iterator i = m.begin(); i < m.end(); i++)
    {
        int pos_alf = alf.find(*i);//retornamos la posición del caracter "*I" en el "alf"
        pos_alf += clave;
        pos_alf = modulo(pos_alf, alf.size());
        m_c << alf[pos_alf];
    }

    return m_c.str();
}

//se descifra el mensaje "m_c" y lo retorna
string Cesar::descifrar(string m_c)
{
    ostringstream m_d;

    for (string::iterator i = m_c.begin(); i < m_c.end(); i++)
    {
        int pos_alf = alf.find(*i);//retornamos la posición del caracter "*I" en el "alf"
        pos_alf -= clave;
        pos_alf = modulo(pos_alf, alf.size());
        m_d << alf[pos_alf];
    }

    return m_d.str();
}

int main()
{

    Cesar emisor(alf,14);
    Cesar receptor(alf,14);

    string mensaje;
    cout << "mensaje:"; getline(cin, mensaje);

    string mensaje_cifrado = emisor.cifrar(mensaje);
    cout << mensaje_cifrado << endl;

    string mensaje_descifrado = receptor.descifrar(mensaje_cifrado);
    cout << mensaje_descifrado << endl;

    return 0;
}

