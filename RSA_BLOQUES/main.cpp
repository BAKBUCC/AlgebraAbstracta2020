#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <NTL/ZZ.h>
#include <list>

using namespace std;
using namespace NTL;

using std::ostringstream;

string alf("abcdefghijklmnopqrstuvwxyz().0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ,+-[]");

template<class T>
inline void string_to_int(string bloque, T& num)
{
    stringstream geek(bloque);
    geek >> num;
}

template <class T>
inline T valorAbsoluto(T a)
{
    if (a < 0) { return -a; }
    return a;
}

template <class T>
T generadorNumAleatorio(T fin)
{
    return RandomBnd(fin);
}

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

template <class T>
T MCD_binario(T a, T b)
{
    T g, dos;
    g = 1, dos = 2;
    while (modular<T>(a, dos) == 0 && modular<T>(b, dos) == 0)
    {
        a = a >> 1; b = b >> 1;
        g = g >> 1;
    }
    while (a != 0)
    {
        if (modular<T>(a, dos) == 0) { a = a >> 1; }
        else if (modular<T>(b, dos) == 0) { b = b >> 1; }
        else
        {
            T t = (valorAbsoluto<T>(a - b)) >> 1;
            if (a >= b) { a = t; }
            else { b = t; }
        }
    }
    return g * b;
}

template <class T>
T euclidesExtendido(T& i, T* q)
{
    T s1, s2;
    s1= 1, s2 = 0;
    T t1, t2;
    t1 = 0, t2 = 1;

    while (i != 0)
    {
        T s;
        s = s1 - (*q) * s2;
        s1 = s2; s2 = s;

        T t;
        t = t1 - (*q) * t2;
        t1 = t2; t2 = t;

        q++;
        i--;
    }

    return s1;
}

template <class T>
T inversa(T& a, T& b)
{
    T r1 = valorAbsoluto<T>(a), r2 = valorAbsoluto<T>(b);

    T* q = new T[1000000000];//aqui voy a guardar los cocientes que se generen en el while de acá abajo y lo mandaré a la función inversa si el mcd=1.
    T i;
    i = 0;

    while (r2 > 0)
    {
        *q = r1 / r2;

        T r = r1 - (*q) * r2;
        r1 = r2; r2 = r;

        q++;
        i++;
    }
    T j = i;
    while(j != 0)
    {
        q--;
        j--;
    }
    if (r1 == 1)//si el mcd=1 entonces..
    {
        T x = euclidesExtendido<T>(i, q); //..saco la inversa
        delete[]q;//borro la lista ya que no me sirve
        x = modular<T>(x, b); //saco su equivalente por si acaso este fuera del conjunto Z_n
        return x;
    }

    cout << "hiciste algo mal, el mcd no es 1" << endl;
    return T(0);
}

template <class T>
T exponenciacionModular(T a, T b, T n)
{

    T exp;
    exp = 1;
    T x;
    x = modular<T>(a, n);
    T dos;
    dos = 2;
    while (b > 0)
    {
        if (modular<T>(b, dos) == 1) { exp = modular<T>(modular<T>(exp, n) * modular<T>(x, n), n); }
        x = modular<T>(modular<T>(x, n) * modular<T>(x, n), n);
        b = b >> 1;
    }
    return exp;
}

template <class T>
struct RSA
{
    T e, N;
    RSA();//int bits); //del receptor
    RSA(T N, T e);  //del emisor

    string cifrar(string m);
    string descifrar(string m_c);
private:
    void generarClaves();
    string llenado(string m);

    T LI, LS, p, q, d, phiN;
};

template <class T>
inline T limiteInferior(int bits)
{
    T L;
    L = 1;

    while(bits--)
    {
        L = L << 1;
    }

    return L;
}

template <class T>
RSA<T>::RSA()//int bits)
{
    /*
    LI = limiteInferior<T>(bits - 1);
    cout << "bits: " << bits - 1 << "#LI: " << LI << endl;
    LS = (LI << 1) - 1;
    cout << "bits: " << bits << "#LS: LI*2 - 1 " << LS << endl;
    */
    generarClaves();
}

template <class T>
RSA<T>::RSA(T N, T e) :N(N), e(e) {}

template <class T>
bool miillerTest(T d, T n)
{
    // Pick a random number in [2..n-2]
    // Corner cases make sure that n > 4
    ZZ a;
    a = generadorNumAleatorio<T>(n-4);

    // Compute a^d % n
    ZZ x;
    x = exponenciacionModular<T>(a, d, n);

    if (x == T(1) || x == n-1) { return true; }

    // Keep squaring x while one of the following doesn't
    // happen
    // (i) d does not reach n-1
    // (ii) (x^2) % n is not 1
    // (iii) (x^2) % n is not n-1
    while (d != n-1)
    {
        x = modular<T>((x * x),n);
        d = d << 1;

        if (x == T(1)){ return false; }
        if (x == n-1) { return true; }
    }

    // Return composite
    return false;
}

template <class T>
bool test_primalidad(T n, int k)
{
// Corner cases
    if (n <= T(1)) return false;
    if (n <= T(3)) return true;

    int lista[11]={2,3,5,7,11,13,17,19,23,29,31};
    for(int *i=lista; i<lista+11; i++)
    {
        if(modular<T>(n,T(*i))==0)
        {
            return false;
        }
    }


// Find r such that n = 2^d * r + 1 for some r >= 1
    ZZ d;
    d = n - 1;
    while (modular<T>(d,ZZ(2)) == 0) { d = d >> 1; }

// Iterate given number of k times
    for (int i = 0; i < k; i++)
    {
        if (!miillerTest<T>(d, n))
        {
            return false;
        }
    }
    return true;
}

template <class T>
void RSA<T>::generarClaves()
{
    T Rango = LS - LI;
    cout << "Rango: " << Rango << endl;
    //cin >> p; cin >> q; cin >> e; cin >> d;

    /*
    p = generadorNumAleatorio<T>(LS - LI);
    p += LI;
    do
    {
        p++;
    } while(test_primalidad<T>(p, 100)==false);
    */
    cout << "p: "; cin >> p; cout << p << endl;

    /*
    q = generadorNumAleatorio<T>(LS - LI);
    q += LI;
    do
    {
        q++;
    } while(test_primalidad<T>(q, 100)==false);
    */
    cout << "q: "; cin >> q; cout << q << endl;

    N = p * q;
    cout << "   N: " << N << endl;

    phiN = (p - 1) * (q - 1);
    cout << "phiN: " << phiN << endl;

    /*
    do
    {
        e = generadorNumAleatorio<T>(phiN);
    } while (MCD_binario<T>(e, phiN) != 1);
    */

    cout << "e: "; cin >> e;
    cout << "e: " << e << endl;
    //d = inversa<T>(e, phiN);
    cin >> d;
    cout << "d: " << d << endl;
}

template <class T>
string numberToString(T num)
{
    stringstream buffer;
    buffer << num;
    return buffer.str();
}

template <class T>
long lenOfNumber(T num)
{
    string number = numberToString<T>(num);
    return number.size();
}

/*
template <class T>
string numberToString(T num)
{
    if (num == 0) { return "0"; }
    long len = lenOfNumber(num);
    ostringstream str;
    for(long i = len - 1; i >= 0; i--)
    {
        str << conv<int>(num % 10);
        num /= 10;
    }

    string stri = str.str();
    int size_ = stri.size();

    for(int ini = 0; ini < size_/2; ini ++)
    {
        swap(stri.at(ini), stri.at(size_ - 1 - ini));
    }
    return stri;
}
*/

template <class T>
string RSA<T>::llenado(string m)
{
    ostringstream m_ceros;
    int pos_int;
    for (string::iterator i = m.begin(); i < m.end(); i++)
    {
        pos_int = alf.find(*i);
        T pos = conv<T>(pos_int);
        if(pos < 10 && pos >= 0) { m_ceros << "0"; }
        m_ceros << numberToString<T>(pos);
    }
    string str_m_lleno = m_ceros.str();
    pos_int = alf.find("w");

    do
    {
        T pos_w = conv<T>(pos_int);
        str_m_lleno.append(numberToString<T>(pos_w));
    }while(modular<T>(ZZ(str_m_lleno.size()), ZZ(lenOfNumber<ZZ>(N) - 1)) != 0);

    cout << "mensaje normal lleno :" << str_m_lleno << endl;

    return str_m_lleno;
}

template <class T>
T stringToNumber(string str)
{
    T number = conv<T>(str[0] - '0');
    long len = str.size();
    for(long i = 1; i < len; i++)
    {
        number *= 10;
        number += conv<T>(str[i] - '0');
    }
    return number;
}

template <class T>
string RSA<T>::cifrar(string m)
{
    string m_lleno = llenado(m);
    ostringstream m_c, bloque;
    int bloque_size = lenOfNumber(N) - 2;

    for (string::iterator i = m_lleno.begin(); i < m_lleno.end(); i++, --bloque_size)
    {
        bloque << *i;
        if (bloque_size == 0)
        {
            //cout << bloque.str() << "^" << e << " mod" << N << endl;
            T  bloque_T = stringToNumber <T> (bloque.str());
            bloque_T = exponenciacionModular<T>(bloque_T, e, N);

            string bloque_str = numberToString<T>(bloque_T);
            int ceros_faltantes =  lenOfNumber(N) - bloque_str.size();

            while(ceros_faltantes--){ m_c << "0"; }

            m_c << bloque_str;

            //cout << "cifrado llenandose: " << m_c.str() << endl;

            bloque.str("");
            bloque.clear();

            bloque_size = lenOfNumber(N) - 1;
        }
    }
    return m_c.str();
}

string cadaDosDigitos_unaLetra(string m)
{
    ostringstream m_d, bloque;
    int dos = 1;
    for (string::iterator i = m.begin(); i < m.end(); i++, --dos)
    {
        bloque << *i;
        if (dos == 0)
        {
            //cout << bloque.str() << " = ";
            int bloque_int = stringToNumber <int> (bloque.str());

            //cout << alf[bloque_int] << endl;
            m_d << alf[bloque_int];

            bloque.str("");
            bloque.clear();

            dos = 2;
        }
    }
    return m_d.str();
}

template <class T>
string RSA<T>::descifrar(string m_c)
{
    ostringstream m_d, bloque, newBloque;
    int bloque_size = lenOfNumber(N) - 1;
    cout << bloque_size << endl;

    for (string::iterator i = m_c.begin(); i < m_c.end(); i++, --bloque_size)
    {
        bloque << *i;
        if (bloque_size == 0)
        {
            //cout << bloque.str() << "^" << d << " mod" << N << endl;
            T  bloque_T = stringToNumber <T> (bloque.str());

            T D_p, D_q;
            D_p = exponenciacionModular(modular<T>(bloque_T, p), modular<T>(d, p - 1), p);
            D_q = exponenciacionModular(modular<T>(bloque_T, q), modular<T>(d, q - 1), q);

            T Q_1, Q_2;
            Q_1 = inversa<T>(q, p);
            Q_2 = inversa<T>(p, q);


            bloque_T = modular<T>(modular<T>(modular<T>(D_p * q, N) * Q_1, N) + modular<T>(modular<T>(D_q * p, N) * Q_2, N), N);

            string bloque_str = numberToString<T>(bloque_T);
            int ceros_faltantes =  (lenOfNumber(N) - 1) - bloque_str.size();

            while(ceros_faltantes>0){ newBloque << "0"; ceros_faltantes--;}

            newBloque << bloque_str;

            //cout << "bloque para descifrar llenandose: " << newBloque.str() << endl;

            bloque.str("");
            bloque.clear();

            bloque_size = lenOfNumber(N);
        }
    }

    //cout << "bloque para descifrar: " << newBloque.str() << endl;
    string mensaje_descifrado = cadaDosDigitos_unaLetra(newBloque.str());

    return mensaje_descifrado;
}

int main()
{
    /*
    int bits; cout<< "#bits: "; cin >> bits;

    RSA< ZZ > receptor(bits);
    cout << "N: " << receptor.N << "\ne:" << receptor.e << endl;

    RSA < ZZ > emisor(receptor.N, receptor.e);

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
    //*/

    /*
    //###########DESCIFRAR###########
    RSA<ZZ> receptor;

    cout << "N: " << receptor.N << "\ne:" << receptor.e << endl;

    int i; cin >> i;

    while (i--)
    {
        //pido un mensaje
        string mensaje_cifrado;
        cin.ignore();
        cout << "mensaje cifrado:"; getline(cin, mensaje_cifrado);

        //descifro el mensaje
        string mensaje_descifrado = receptor.descifrar(mensaje_cifrado);
        cout << "\n\nMENSAJE DESCIFRADO\n\"" << mensaje_descifrado << "\"" << endl;
    }
    */

    ///*
    //###########CIFRAR###########
    int i; cin >> i;
    while (i--)
    {
        string name;
        cin >> name;
        cout << "name" << name << endl;

        ZZ N, e;
        cout << "N: "; cin >> N;
        cout << "\ne: "; cin >> e;
        RSA<ZZ> emisor(N, e);

        //pido un mensaje
        string mensaje;
        cin.ignore();
        cout << "mensaje:"; getline(cin, mensaje);

        //cifro el mensaje
        string mensaje_cifrado = emisor.cifrar(mensaje);
        cout << "\n\nMENSAJE CIFRADO :\n\"" << mensaje_cifrado << "\"" << endl;
    }
    //*/

    return 0;
}
