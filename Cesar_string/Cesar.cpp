#include <iostream>
#include <string>
#include <sstream> 

using namespace std;
using std::ostringstream; 


string alf("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789{-}_/,.");

int modulo(int Zn, int num)
{
    if (num >= 0 && num < Zn)
    {
        return num;
    }
    else if (num >= Zn)
    {
        return num - Zn * (num / Zn);
    }
    else if(numero < 0)
    {
        return num - Zn * ((num / Zn) - 1);
    }
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
    string alf="abcdefghijklmnopqrstuvwxyz";
    int clave=1;
};

Cesar::Cesar()
    {}

Cesar::Cesar(int clave)
    :clave(clave){}

Cesar::Cesar(string alf)
    :alf(alf){}

Cesar::Cesar(string alf, int clave)
    :alf(alf),clave(clave){}

string Cesar::cifrar(string m)
{
    ostringstream m_c;

    for (string::iterator i = m.begin(); i < m.end(); i++)
    {
        int pos_alf = alf.find(*i);
        pos_alf += clave;
        pos_alf = modulo(alf.size(), pos_alf);
        m_c << alf[pos_alf];
    }

    return m_c.str();
}

string Cesar::descifrar(string m_c)
{
    ostringstream m_d;

    for (string::iterator i = m_c.begin(); i < m_c.end(); i++)
    {
        int pos_alf = alf.find(*i);
        pos_alf -= clave;
        pos_alf = modulo(alf.size(), pos_alf);
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
