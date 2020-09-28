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

//MCD_binario de procedencia china y muy bueno para calculos rapidos y grandes
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

//################PRIMERA FORMA################
//se devuelve un array con los valores de el [0]=mcd,[1]=x,[2]=y; 
int* alg_ext_eucl_F(int a, int b)       // primera forma la fase final
{
    int r1 = valorAbsoluto(a), r2 = valorAbsoluto(b);
    int s1 = 1, s2 = 0;
    int t1 = 0, t2 = 1;

    while (r2 > 0) 
    {
        int q = r1 / r2;

        int r = r1 - q * r2;
        r1 = r2; r2 = r;

        int s = s1 - q * s2;
        s1 = s2; s2 = s;

        int t = t1 - q * t2;
        t1 = t2; t2 = t;
    }
    int* gcd = new int[3];
    gcd[0] = r1; gcd[1] = s1; gcd[2] = t1;// gcd[0] = a*gcd[1] + b*gcd[2] // d= a*x + b*y;
    return gcd;
}

int* alg_ext_eucl_I(int a, int b)       //primera forma, fase inicial, no es tan buena ya que si se cumple que gcd es 1 hará el eu extendido 
{
    int mcd;
    mcd = MCD_binario(a, b);//saca el mcd de forma binaria y si sale 1, entonces hará el euclides extendido
    if (mcd == 1) 
    {
        return alg_ext_eucl_F(a, b);
    }
    return &mcd;//sino, solo devolverá el mcd;
}

//################SEGUNDA FORMA################
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

int main()
{
    int a; cout << "a: "; cin >> a;
    int b; cout << "b: "; cin >> b; 
   
    cout << "\n\nmcd_binario(" << a << "," << b << "):" << MCD_binario(a, b) << "\n\n";
    
    int* formula = alg_ext_eucl_2(a, b);

    //utilizando el algoritmo extendido de euclides
    if (*formula == 1) 
    {
        cout << "d = a(x) + b(y)\n";
        cout << *(formula) << " = " << a << "(";
        formula++;
        cout << *(formula) << ") + " << b << "(";
        formula++;
        cout << *(formula) << ")" << endl;
    }
    else
    {
        formula = alg_ext_eucl_F(a, b);// esta es la primera forma, pero la fase final
        cout << "como el mcd(a,b)!=1, no es necesario mencionar el inverso ya que no importa, pero igual lo demostraremos con la ###primera forma###\n";
        
        cout << "d = a(x) + b(y)\n";
        cout << *(formula) << " = " << a << "(";
        formula++;
        cout << *(formula) << ") + " << b << "(";
        formula++;
        cout << *(formula) << ")\n\n";
        
        cout << "ya que si llamamos a la funcion de la ##SEGUNDA FORMA## entonces quedaria solo esto\n";
        
        formula = alg_ext_eucl_2(a, b);//notese que estoy llamando a la segunda forma

        cout << "mcd(" << a << "," << b << "):" << *(formula) << "\n\n";
    }
    return 0;
}