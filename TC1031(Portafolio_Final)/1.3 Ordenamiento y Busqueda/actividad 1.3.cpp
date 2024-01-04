#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// estructura para parsear cada linea del archivo
struct bitacoraFormato {
    int mes;
    string mesStr;
    int dia;
    int hora;
    int minuto;
    int segundo;
    string horaStr;
    string resto;
};


// parsear con estructura 
bitacoraFormato parseLine(const string& line) {
    bitacoraFormato obj; // obj = cada linea
    istringstream iss(line);
    string stringMes, stringDia, stringHora, stringResto;

    iss >> stringMes >> stringDia >> stringHora;
    obj.mesStr = stringMes;

    // cambiar mes a entero
    if (stringMes == "Jan") {
        obj.mes = 1;
    } else if (stringMes == "Feb") {
        obj.mes = 2;
    } else if (stringMes == "Mar") {
        obj.mes = 3;
    } else if (stringMes == "Apr") {
        obj.mes = 4;
    } else if (stringMes == "May") {
        obj.mes = 5;
    } else if (stringMes == "Jun") {
        obj.mes = 6;
    } else if (stringMes == "Jul") {
        obj.mes = 7;
    } else if (stringMes == "Aug") {
        obj.mes = 8;
    } else if (stringMes == "Sep") {
        obj.mes = 9;
    } else if (stringMes == "Oct") {
        obj.mes = 10;
    } else if (stringMes == "Nov") {
        obj.mes = 11;
    } else if (stringMes == "Dec") {
        obj.mes = 12;
    }

    obj.horaStr = stringHora;
    obj.dia = stoi(stringDia); // convertir a entero
    obj.hora = stoi(stringHora.substr(0, 2));
    obj.minuto = stoi(stringHora.substr(3, 2));
    obj.segundo = stoi(stringHora.substr(6, 2));
   
    getline(iss >> ws, obj.resto, '\n');
    return obj;
}

void merge(vector<bitacoraFormato>& vecNuevo, vector<bitacoraFormato>& vecIzq, vector<bitacoraFormato>& vecDer) {
    int sizeIzq = vecIzq.size();
    int sizeDer = vecDer.size();
    int i = 0; // indice del vector izq
    int j = 0; // indice del vector derecho
    int k = 0; // indice del vector nuevo
    while (i < sizeIzq && j < sizeDer) {
        // buscando valor mas grande para poner en vector nuevo (descendiente)
        // considerando revisar el siguiente valor si el anterior es igual (por ejemplo, que se revise el dia si el mes es igual)
        if (vecIzq[i].mes > vecDer[j].mes ||
        vecIzq[i].mes == vecDer[j].mes && vecIzq[i].dia > vecDer[j].dia ||
        vecIzq[i].mes == vecDer[j].mes && vecIzq[i].dia == vecDer[j].dia && vecIzq[i].hora > vecDer[j].hora ||
        vecIzq[i].mes == vecDer[j].mes && vecIzq[i].dia == vecDer[j].dia && vecIzq[i].hora == vecDer[j].hora && vecIzq[i].minuto > vecDer[j].minuto ||
        vecIzq[i].mes == vecDer[j].mes && vecIzq[i].dia == vecDer[j].dia && vecIzq[i].hora == vecDer[j].hora && vecIzq[i].minuto == vecDer[j].minuto && vecIzq[i].segundo > vecDer[j].segundo) {
            vecNuevo[k] = vecIzq[i];
            i++;
            k++;
        }
        else {
            vecNuevo[k] = vecDer[j];
            j++;
            k++;
        }
    }
    // rellenando vector nuevo con valores restantes del uno de los dos vectores
    while (i < sizeIzq) { // si el vector con restantes es el izquierdo
        vecNuevo[k] = vecIzq[i];
        i++;
        k++;
    }
    while (j < sizeDer) { // si el vector con restantes es el derecho
        vecNuevo[k] = vecDer[j];
        j++;
        k++;
    }
}

void mergeSort(vector<bitacoraFormato>& vec) {
    // complejidad de O(n log n)
    int sizeVec = vec.size();
    if (sizeVec > 1) {
        int mitad = sizeVec/2;
        // creando vectores
        vector<bitacoraFormato> vecIzq(mitad);
        vector<bitacoraFormato> vecDer(sizeVec-mitad);
        
        for (int i = 0; i < mitad; i++) {
            vecIzq[i] = vec[i];
        }
        for (int i = mitad; i < sizeVec; i++) {
            vecDer[i-mitad] = vec[i];
        }
        mergeSort(vecIzq);
        mergeSort(vecDer);
        merge(vec,vecIzq,vecDer);
    }
}
 

// para encontrar el índice de la fecha fin
int busquedaSecuencialFechaFin(vector<bitacoraFormato>data, int mes, int dia) { // complejidad de O(n) ya que tiene solamente un for loop que itera n veces, con n = tamaño del vector
    for(int i = 0; i <= data.size()-1; i++) {
        if (data[i].mes == mes && data[i].dia == dia)
            return i;
    }
    return -1;
}


// para encontrar el índice de la fecha inicio
int busquedaSecuencialFechaInicio(vector<bitacoraFormato>data, int mes, int dia) { // complejidad de O(n) ya que tiene solamente un for loop que itera n veces, con n = tamaño del vector
    for(int i = data.size(); i >= 0; i--) {
        if (data[i].mes == mes && data[i].dia == dia)
            return i;
    }
    return -1;
}


void crearArchivo(const vector<bitacoraFormato>& data, const string& filename) {
    ofstream outfile(filename);
    for (const auto& obj : data) {
        outfile << obj.mesStr << " " << obj.dia << " " << obj.horaStr << " " << obj.resto << endl;
    }
    outfile.close();
}


int main() {
    ifstream file("bitacora.txt");
    vector<bitacoraFormato> data; // vector de estructura 1
    string line;
    while (getline(file, line)) {
        bitacoraFormato obj = parseLine(line);
        data.push_back(obj);
    }
    
    file.close();
    mergeSort(data); // ordenando por fecha y hora

    // preguntar a usuario fecha inicio y fecha fin
    bool seguir = true;
    while (seguir == true) {
      int mesInicio;
      cout << "Escriba el mes de la fecha de inicio en uno o dos digitos (un solo digito si es uno de los 9 primeros meses): ";
      cin >> mesInicio;
      cout << "Escriba el dia de la fecha inicio: ";
      int diaInicio;
      cin >> diaInicio;
      cout << endl;
      int mesFin;
      cout << "Escriba el mes de la fecha fin en uno o dos digitos (un solo digito si es uno de los 9 primeros meses): ";
      cin >> mesFin;
      cout << "Escriba el dia de la fecha fin: ";
      int diaFin;
      cin >> diaFin;
      int indiceInicio = busquedaSecuencialFechaInicio(data,mesInicio,diaInicio);
      int indiceFin = busquedaSecuencialFechaFin(data,mesFin,diaFin);
      
      if (indiceInicio == -1 || indiceFin == -1) {
        cout << endl;
        cout << "Fecha(s) invalidas, intentelo nuevamente." << endl << endl;
      }
      else {
        cout << endl;
        cout << "Los registros dentro de la fecha inicio y fin que ingreso son: " << endl;
        while (indiceFin < indiceInicio) {
          cout << data[indiceFin].mesStr << " " << data[indiceFin].dia << " " << data[indiceFin].horaStr << " " << data[indiceFin].resto << endl;
          indiceFin++;
        }
        seguir = false;
      }
   }
    
    // creando archivo con todas las fechas
    cout << endl;
    crearArchivo(data, "bitacoras_sorted.txt");
    cout << "¡Su archivo ordenado se ha creado exitosamente!" << endl;
    return 0;
}