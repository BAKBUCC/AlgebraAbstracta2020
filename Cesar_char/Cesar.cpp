#include <iostream>

using namespace std;

char alf[200] = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789({-})_/,.";

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

//retornamos la posición del caracter "c" en el "alf"
int find(char* alf, char* c)
{
	char* c_alf = alf;
	for (int pos = 0; *c_alf; c_alf++, pos++)
	{
		if (*c_alf == *c) { return pos; }
	}
	return -1;
}

//imprimo la cadena de caracteres que conformen "c"
void print_char(char* c)
{
	for (; *c; c++)
	{
		cout << *c;
	}
	cout << endl;
}

//hallo el tamanio de la cadena "alf" y lo retorna
int size(char* alf)
{
	int size = 0;
	for (; *alf; alf++)
	{
		size++;
	}
	return size;
}

struct Cesar
{
	Cesar();
	Cesar(int clave);
	Cesar(char* alf);
	Cesar(char* alf, int clave);
	char* cifrar(char* m);
	char* descifrar(char* m_c);
private:
	char *ptr_alf;
	int clave;
};

//cuando no mandan parametros al inicializar una clase Cesar
Cesar::Cesar()
{
	char alf[200] = "abcdefghijklmnopqrstuvwxyz";
	ptr_alf = alf;
	clave = 1;
}

Cesar::Cesar(int clave)
	:clave(clave)
{
	char alf[200] = "abcdefghijklmnopqrstuvwxyz";
	ptr_alf = alf;
}

Cesar::Cesar(char* alf)
	:ptr_alf(alf)
{
	clave = 1;
}

Cesar::Cesar(char* alf, int clave)
	:clave(clave),ptr_alf(alf){}

//se cifra el mensaje "m" y lo retorna
char* Cesar::cifrar(char* m)
{
	char* m_c = m;

	//guardo el tamanio del alfabeto "alf" y de el mensaje "m_c"
	int size_alf = size(ptr_alf);
	int size_m_c = size(m_c);

	for (; *m; m_c++ ,m++)
	{
		int pos_alf = find(ptr_alf, m);
		pos_alf += clave;
		pos_alf = modulo(pos_alf, size_alf);
		*m_c = *(ptr_alf + pos_alf);
	}
	m_c -= size_m_c;  //vuelvo el puntero m_c a su comienzo
	return m_c;
}

//se descifra el mensaje "m_c" y lo retorna
char* Cesar::descifrar(char* m_c)
{
	char* m_d=m_c;

	//guardo el tamanio del alfabeto "alf" y de el mensaje "m_d"
	int size_alf = size(ptr_alf);
	int size_m_d = size(m_d);

	for (; *m_c; m_c++, m_d++)
	{
		int pos_alf = find(ptr_alf, m_c);
		pos_alf -= clave;
		pos_alf = modulo(pos_alf, size_alf);
		*m_d = *(ptr_alf + pos_alf);//: *m_d = alf[ pos_alf ]
	}
	m_d -= size_m_d;  //vuelvo el puntero m_d a su comienzo
	return m_d;
}

int main()
{
	Cesar emisor(alf, 103), receptor(alf, 103);
	
	char *mensaje = new char[200];
	cout << "mensaje:"; cin.getline(mensaje, 200, '\n');


	char* mensaje_cifrado = emisor.cifrar(mensaje);
	print_char(mensaje_cifrado);

	char* mensaje_descifrado = receptor.descifrar(mensaje_cifrado);
	print_char(mensaje_descifrado);

	delete[]mensaje;
	return 0;
}