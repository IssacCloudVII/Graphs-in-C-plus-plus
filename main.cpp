#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct data
{
    string nombre;

};
typedef struct data Data_Nodo;

struct struct_nodo
{
    Data_Nodo Datos;
    struct struct_nodo *next;
    struct struct_nodo *conexiones;
    struct struct_nodo *conexiones_final;

};
typedef struct struct_nodo Nodo;

Nodo* crear_nodo(string);
void formato(string &x);
void leer_cadena(string &x, ifstream &FILE);
int imprimir_grafo(Nodo*);
void imprimir_nodo(Nodo);
int inicializar_lista(Nodo**, Nodo**);
int insertar_lista_principio(Nodo**, Nodo*);
int insertar_lista_medio(Nodo*, Nodo*, string);
int insertar_lista_final(Nodo**, Nodo**, Nodo*);
int borrar_lista(Nodo *, Nodo**, Nodo**);
int borrar_grafo(Nodo *, Nodo**, Nodo**);
Nodo* buscar_nodo(string, Nodo*);
void buscar_total_rutas(Nodo*, vector<string>);

int main()
{
    int opcion = 0;
    Nodo* head = NULL;
    Nodo* last = NULL;


    do
    {
        cout << "Bienvenido al sistema de grafos\n";
        cout << "Seleccione una opci�n : \n";
        cout << "1. Inicializar un grafo a trav�s de un archivo.\n";
        cout << "2. Imprimir grafo.\n";
        cout << "3. Borrar grafo.\n";
        cout << "4. Buscar ruta m�s corta entre dos puntos.\n";
        cout << "0. Salir.\n";

        cin >> opcion;
        switch(opcion)
        {
            case 1:
            {
                    if(head != NULL)
                    {
                        cout << "Esto borrara el grafo actual. �Seguro que desea seguir?\n";
                        cout << "Escribir s� para continuar, no para no borrar el grafo. ";

                        string aux;
                        cin >> aux;
                        if(!(aux == "s�" || aux == "si"))
                        {
                            cout << "Se escribio no o cualquier otra frase, el grafo no ser� borrado.\n";
                            break;
                        }
                    }
                    head = NULL;
                    last = NULL;
                    int x = inicializar_lista(&head, &last);
                    if(x == 1)
                        cout << "Grafo creado exitosamente.\n";
                    else if(x == -1)
                        cout << "Archivo no encontrado";
                    else if(x == -2)
                        cout << "AVISO: El grafo no tenia conexiones\n";
            }
            break;

            case 2:
                if(imprimir_grafo(head) == -1)
                    cout << "El grafo aun no se ha introducido o no tiene nada.\n";
            break;

            case 3:
                if(borrar_grafo(head, &head, &last) == -1)
                    cout << "El grafo est� vac�o.\n";
                else
                    cout << "Grafo eliminado correctamente.\n";
            break;

            case 0:
                cout << "Gracias por usar\n";
            break;
        }
        system("pause");
        system("cls");

    }while(opcion != 0);


    return 0;
}

void imprimir_nodo(Nodo nodo)
{
    cout << "Nombre del nodo: " << nodo.Datos.nombre << "\n";
    //cout << "Next: " << nodo.next << "\n";
    //cout << "Conexiones: " << nodo.conexiones << "\n";
    cout << "Sus conexiones son: " << "\n";
    Nodo *p = nodo.conexiones;
    if(p == NULL)
        cout << "Este nodo no tiene conexiones.\n";
    else
    {
        while(p != NULL)
        {
            cout << nodo.Datos.nombre << " -> " << p -> Datos.nombre << "\n";
            p = p -> next;
        }
    }

}

void formato(string &x)
{
    for(char c : x)
        c = tolower(c);
    x[0] = toupper(x[0]);
}

Nodo* crear_nodo(string aux)
{
    Nodo* nodo = new Nodo;
    nodo -> Datos.nombre = aux;
    nodo -> next = NULL;
    nodo -> conexiones = NULL;
    return nodo;
}

int imprimir_grafo(Nodo* head)
{
    Nodo* p = head;
    if(p == NULL)
        return -1;
    while(p != NULL)
    {
        //cout << "Direccion actual: " << p << "\n";
        imprimir_nodo(*p);
        p = p -> next;
    }
    return 0;
}

int insertar_lista_final(Nodo** head, Nodo** last, Nodo *nodo)
{
    if(*head == NULL)
    {
        *(head) = nodo;
        **head = *nodo;
        *last = *head;
        **last = **head;
    }
    else if(*head == *last)
    {
        (*head) -> next = nodo;
        *last = nodo;
        **last = *nodo;
    }
    else
    {
        (*last) -> next = nodo;
        *last = nodo;
        **last = *nodo;
    }
}

void leer_cadena(string &x, ifstream &FILE)
{
    char c;
    FILE.get(c);
    if(c == '\n')
        FILE.get(c);
    while(c != '-' && c != '\n')
    {
        x += c;
        FILE.get(c);
        if(FILE.eof())
            return;
    }
}

int inicializar_lista(Nodo** head, Nodo** last)
{

    ifstream FILE;
    string ruta;
    cout << "Escribe la ruta del archivo que contiene al grafo: ";
    cin >> ruta;
    FILE.open(ruta, ios::in);
    if(!FILE.is_open())
        return -1;

    char c = '\0';
    string sr = "";
    while(sr != "-1")
    {
        FILE.get(c);
        if(sr == "-1")
            break;
        if(c != ',')
            sr += c;
        else
        {
            formato(sr);
            Nodo* aux_nodo = crear_nodo(sr);
            insertar_lista_final(head, last, aux_nodo);
            sr = "";
        }
    }

    string nodo_1;
    string nodo_2;

    Nodo *p = *head;
    Nodo *q = p -> next;

    leer_cadena(nodo_1, FILE);
    leer_cadena(nodo_2, FILE);

    if(FILE.eof())
        return -2;

    while(!FILE.eof())
    {
        Nodo* aux = buscar_nodo(nodo_1, *head);
        if(aux == NULL)
        {
            cout << "Error en el nombre de las conexiones\n";
            exit(0);
        }
        Nodo* aux_nodo = crear_nodo(nodo_2);
        insertar_lista_final(&(aux -> conexiones), &(aux -> conexiones_final), aux_nodo);
        nodo_1 = nodo_2 = "";
        leer_cadena(nodo_1, FILE);
        leer_cadena(nodo_2, FILE);
    }

    Nodo* aux = buscar_nodo(nodo_1, *head);
    Nodo* aux_nodo = crear_nodo(nodo_2);
    insertar_lista_final(&(aux -> conexiones), &(aux -> conexiones_final), aux_nodo);


    return 1;
}

Nodo* buscar_nodo(string nombre, Nodo* head)
{
    Nodo* p = head;
    while(p != NULL)
    {
        if(p ->Datos.nombre == nombre)
            return p;
        p = p -> next;
    }
    return NULL;
}

int borrar_lista(Nodo *p, Nodo** head, Nodo** last)
{
    if(p == NULL)
        return -1;
    if(p -> next == NULL)
    {
        *head = NULL;
        *last = NULL;
        return 0;
    }
    else
    {
        borrar_lista(p -> next, head, last);
        delete p;
        p = NULL;
    }
}

int borrar_grafo(Nodo *p, Nodo** head, Nodo** last)
{
    if(p == NULL)
        return -1;
    while(p != NULL)
    {
        borrar_lista(p ->conexiones, &(p -> conexiones), &(p -> conexiones_final));
        p = p -> next;
    }
    p = *head;
    borrar_lista(p, head, last);
    return 1;

}

void buscar_total_rutas(Nodo* head, vector<string> visitados)
{


}
