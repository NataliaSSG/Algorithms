#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// struct del nodo
struct Nodo {
  string mes;
  string dia;
  string hora;
  string IP;
  long long IPint; // para hacer ordenamiendo
  string resto;
  Nodo* anterior;
  Nodo* siguiente;
};

// struct de la lista
struct listaDoblementeEncadenada {
  Nodo* head;
  Nodo* tail;
  Nodo* IPinicio;
  Nodo* IPfin;
  listaDoblementeEncadenada() {
    head = NULL;
    tail = NULL;
    IPinicio = NULL;
    IPfin = NULL;
  }
  void insertar(Nodo*);
  void leer();
  void ordenarDesc();
  void ordenarAsc();
  void crearArchivo(const string&);
  void mostrarRegistros();
  void encontrarIPs();
};

// metodo para leer -> complejidad O(n)
void listaDoblementeEncadenada::leer() {
  Nodo* aux;
  aux = head;
  while (aux != nullptr) { 
    cout << aux->mes << " " << aux->dia << " " << aux->hora << " " << aux->IP << " " << aux -> resto << endl;
    aux = aux->siguiente;
  }
}

// metodo para agregar un nodo al final -> complejidad O(n)
void listaDoblementeEncadenada::insertar(Nodo* nodoNuevo) {
  if (head == nullptr) {
    head = nodoNuevo;
    tail = nodoNuevo;
  }
  else {
    tail->siguiente = nodoNuevo;
    nodoNuevo->anterior = tail;
    tail = nodoNuevo;
  }
}

// ordenamiento ascendente por IPs usando bubble sort -> complejidad O(n^2)
void listaDoblementeEncadenada::ordenarAsc() {
  Nodo* temp = head;
  Nodo* temp2 = head;
  Nodo* aux = new Nodo;
  while (temp != nullptr) {
    temp2 = head;
    while (temp2->siguiente != nullptr) {
      if (temp2->IPint > temp2->siguiente->IPint) {
        aux->mes = temp2->mes;
        aux->dia = temp2->dia;
        aux->hora = temp2->hora;
        aux->IP = temp2->IP;
        aux->IPint = temp2->IPint;
        aux->resto = temp2->resto;

        temp2->mes = temp2->siguiente->mes;
        temp2->dia = temp2->siguiente->dia;
        temp2->hora = temp2->siguiente->hora;
        temp2->IP = temp2->siguiente->IP;
        temp2->IPint = temp2->siguiente->IPint;
        temp2->resto = temp2->siguiente->resto;

        temp2->siguiente->mes = aux->mes;
        temp2->siguiente->dia = aux->dia;
        temp2->siguiente->hora = aux->hora;
        temp2->siguiente->IP = aux->IP;
        temp2->siguiente->IPint = aux->IPint;
        temp2->siguiente->resto = aux->resto;
      }
      temp2 = temp2->siguiente;
    }
    temp = temp->siguiente;
  }
  delete aux;
}


// ordenamiento descendente por IPs usando bubble sort -> complejidad O(n^2)
void listaDoblementeEncadenada::ordenarDesc() {
  Nodo* temp = head;
  Nodo* temp2 = head;
  Nodo* aux = new Nodo;
  while (temp != nullptr) {
    temp2 = head;
    while (temp2->siguiente != nullptr) {
      if (temp2->IPint < temp2->siguiente->IPint) {
        aux->mes = temp2->mes;
        aux->dia = temp2->dia;
        aux->hora = temp2->hora;
        aux->IP = temp2->IP;
        aux->IPint = temp2->IPint;
        aux->resto = temp2->resto;

        temp2->mes = temp2->siguiente->mes;
        temp2->dia = temp2->siguiente->dia;
        temp2->hora = temp2->siguiente->hora;
        temp2->IP = temp2->siguiente->IP;
        temp2->IPint = temp2->siguiente->IPint;
        temp2->resto = temp2->siguiente->resto;

        temp2->siguiente->mes = aux->mes;
        temp2->siguiente->dia = aux->dia;
        temp2->siguiente->hora = aux->hora;
        temp2->siguiente->IP = aux->IP;
        temp2->siguiente->IPint = aux->IPint;
        temp2->siguiente->resto = aux->resto;
      }
      temp2 = temp2->siguiente;
    }
    temp = temp->siguiente;
  }
  delete aux;
}

// metodo para parsear -> complejidad O(n)
Nodo* parseLine(const string& line) {
  Nodo *nodo = new Nodo();
  istringstream iss(line);
  string stringMes, stringDia, stringHora, stringIP, stringResto;
  iss >> stringMes >> stringDia >> stringHora >> stringIP;
  nodo->mes = stringMes;
  nodo->dia = stringDia;
  nodo->hora = stringHora;
  nodo->IP = stringIP;

  string stringIP2 = "";
  for(int i = 0; i < stringIP.length(); i++) {
    if(stringIP[i] == '.' || stringIP[i] == ':') {
      continue;
    }
    else {
      stringIP2 = stringIP2 + stringIP[i];
    }
  }

  nodo->IPint = stoll(stringIP2);
  getline(iss >> ws, nodo->resto, '\n');
  
  nodo->anterior = NULL;
  nodo->siguiente = NULL;
  return nodo;
}

// metodo para crear archivo despues de ordenar la lista -> complejidad O(n)
void listaDoblementeEncadenada::crearArchivo(const string& filename) {
    ofstream outfile(filename);
    Nodo* aux;
    aux = head;
    while (aux != nullptr) {
        outfile << aux->mes << " " << aux->dia << " " << aux->hora << " " << aux->IP << " " << aux -> resto << endl;
        aux = aux->siguiente;
  }
    outfile.close();
}

// metodo para buscar los IPs ingresados por el usuario -> complejidad O(n^2)
void listaDoblementeEncadenada::encontrarIPs() {
  Nodo* aux = head;
  bool seguir = true;
  bool seguir2 = true;
  string IPinicioStr;
  string IPfinStr;
  while (seguir == true || seguir2 == true) {
    cout << "Ingrese el IP inicio: ";
    cin >> IPinicioStr;
    cout << "Ingrese el IP fin: "; 
    cin >> IPfinStr;
    cout << endl;
    aux = head;

    while (aux != nullptr) {
      if (aux->IP == IPinicioStr) {
        IPinicio = aux;
        seguir = false;
        break;
      }
      aux = aux->siguiente;
    }

    while (aux != nullptr) {
      if (aux->IP == IPfinStr) {
        IPfin = aux;
        seguir2 = false;
        break;
      }
      aux = aux->siguiente;
    }
    if (seguir == true || seguir2 == true) {
      cout << "IPs invalidos. Ingreselos nuevamente." << endl;
    }
  }
}

// metodo para mostrar los registros a partir de las IPs ingresados -> complejidad O(n)
void listaDoblementeEncadenada::mostrarRegistros() {
  Nodo* aux = IPinicio;
  ofstream outfile("bitacoras_sorted_busqueda.txt");
    while (aux != IPfin->siguiente) {
      outfile << aux->mes << " " << aux->dia << " " << aux->hora << " " << aux->IP << " " << aux -> resto << endl;
      cout << aux->mes << " " << aux->dia << " " << aux->hora << " " << aux->IP << " " << aux->resto << endl;
      aux = aux->siguiente;
    } 
    outfile.close();
}

int main() {
  ifstream file("bitacora.txt");
  listaDoblementeEncadenada lista;
  string line;
  while (getline(file, line)) {
      Nodo *nodo = parseLine(line);
      lista.insertar(nodo);
  }
  file.close();
  lista.ordenarAsc();  
  lista.crearArchivo("bitacoras_listas_sorted_completo.txt");
  lista.encontrarIPs();
  lista.mostrarRegistros();
}
