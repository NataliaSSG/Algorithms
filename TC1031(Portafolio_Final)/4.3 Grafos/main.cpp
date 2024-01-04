#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

// CLASE DE OBJETOS IP
class IP {
public:
  long long IP;
  string IPstring;
  int ID, conexiones, logs;
};


// CLASE GRAFO
class Grafo {
public:
  vector<vector<IP>> adjList;
  map<string,int> IPdictionary;
  vector<vector<IP>> IPvec;
  vector<IP> posiblesBots;
  void readFile(string file);
  void addToDictionaryAndVector(vector<IP> vec);
  void modifyIDs();
  void createList();
  void conexiones();
  void DFS(int umbral);
  void DFSrecursivo(int id, int umbral, vector<bool>& visited);
  void detectarBots(int umbral);
  
};


// FUNCION PARA PARSEAR
vector<IP> parseLine(const string& line) {
  vector<IP> vec;
  IP ip = IP();
  istringstream iss(line);
  string stringMes, stringDia, stringHora, stringIPorigen, stringIPdestino, stringResto;
  iss >> stringMes >> stringDia >> stringHora >> stringIPorigen >> stringIPdestino;

  // PRIMER IP
  string IPorigenToInt = "";
  string stringIP2origen = "";
  for(int i = 0; i < stringIPorigen.length(); i++) {
    if(stringIPorigen[i] == '.') {
      stringIP2origen = stringIP2origen + stringIPorigen[i];
      continue;
    }
    else if (stringIPorigen[i] == ':') {
      break;
    }
    else {
      IPorigenToInt = IPorigenToInt + stringIPorigen[i];
      stringIP2origen = stringIP2origen + stringIPorigen[i];
    }
  }
  ip.IPstring = stringIP2origen;
  ip.IP = stoll(IPorigenToInt);
  ip.ID = 0;
  ip.conexiones = 0;
  ip.logs = 0;

  // SEGUNDO IP
  IP ip2 = IP();

  string IPdestinoToInt = "";
  string stringIP2destino = "";
  for(int i = 0; i < stringIPdestino.length(); i++) {
    if(stringIPdestino[i] == '.') {
      stringIP2destino = stringIP2destino + stringIPdestino[i];
      continue;
    }
    else if (stringIPdestino[i] == ':') {
      break;
    }
    else {
      IPdestinoToInt = IPdestinoToInt + stringIPdestino[i];
      stringIP2destino = stringIP2destino + stringIPdestino[i];
    }
  }
  ip2.IPstring = stringIP2destino;
  ip2.IP = stoll(IPdestinoToInt);
  ip2.ID = 0;
  ip2.conexiones = 0;
  ip2.logs = 0;

  vec.push_back(ip);
  vec.push_back(ip2);
  return vec;
}


// METODOS DE CLASE GRAFO
void Grafo::modifyIDs() {
   for (auto& innerVec : IPvec) {
        for (auto& ip : innerVec) {
            int ID = IPdictionary[ip.IPstring];
            ip.ID = ID;
        }
    }
}


// se actualiza numero de conexiones a IPs unicos 
void Grafo::conexiones() {
    for (auto& outerVec : adjList) {
        outerVec[0].conexiones = outerVec.size() - 1;
    }
}


// se crea la lista de adyacencia con IPs unicos
void Grafo::createList() {
 vector<vector<IP>> list;
  list.resize(IPdictionary.size());
  for (const auto& innerVec : IPvec) {
    IP sourceIP = innerVec[0];
    IP destinationIP = innerVec[1];

    // que se agregue cada IP al inicio de su propio vector
    if (list[sourceIP.ID].empty() || list[sourceIP.ID][0].IPstring != sourceIP.IPstring) {
        list[sourceIP.ID].insert(list[sourceIP.ID].begin(), sourceIP);
    }
    if (list[destinationIP.ID].empty() || list[destinationIP.ID][0].IPstring != destinationIP.IPstring) {
        list[destinationIP.ID].insert(list[destinationIP.ID].begin(), destinationIP);
    }

    // para que no se repitan IPs en los vectores de cada IP
    bool existsInSource = false;
    bool existsInDestination = false;
    for (auto& ip : list[sourceIP.ID]) {
      if (ip.IPstring == destinationIP.IPstring) {
        existsInSource = true;
        break;
      }
    }
    for (auto& ip : list[destinationIP.ID]) {
      if (ip.IPstring == sourceIP.IPstring) {
        existsInDestination = true;
        break;
      }
    }
    if (!existsInSource && sourceIP.IP != destinationIP.IP) {
      list[sourceIP.ID].push_back(destinationIP);
    }
    if (!existsInDestination && sourceIP.IP != destinationIP.IP) {
      list[destinationIP.ID].push_back(sourceIP);
    }
    
    list[sourceIP.ID][0].logs++;
    list[destinationIP.ID][0].logs++;
    
  }
  adjList = list;
  conexiones(); // actualizacion de conexiones
}


void Grafo::readFile(string file) {
  int id = 0;
  int i = 0;
  ifstream archivo(file);
  string line;
  while (getline(archivo, line)) {
      vector<IP> vec = parseLine(line);
      IPvec.push_back(vec);

      // se crea diccionario para asignar IDs
      if (IPdictionary.find(vec[0].IPstring) == IPdictionary.end()) {
          IPdictionary[vec[0].IPstring] = id;
          id++;
      }
     if (IPdictionary.find(vec[1].IPstring) == IPdictionary.end()) {
          IPdictionary[vec[1].IPstring] = id;
          id++;
      }
  }
  archivo.close();
  modifyIDs();
  createList();
}


// funcion DFS recursiva para recorrer cada nodo
void Grafo::DFS(int umbral) {
    vector<bool> visitados(IPdictionary.size(), false); 
    for (int i = 0; i < adjList.size(); i++) {
        if (!visitados[i]) {
            DFSrecursivo(i, umbral, visitados);
        }
    }
}


void Grafo::DFSrecursivo(int id, int umbral, vector<bool>& visitados) {
    visitados[id] = true; 
    if (adjList[id][0].logs > umbral) {
        // se agrega nodo a vector de bots
        posiblesBots.push_back(adjList[id][0]);
    }
    for (const auto& ip : adjList[id]) {
        if (!visitados[ip.ID]) {
            DFSrecursivo(ip.ID, umbral, visitados);  
        }
    }
}


// funcion para detectar bots a partir de cantidad de logs
void Grafo::detectarBots(int umbral) {
    DFS(umbral);
    if (posiblesBots.empty()) {
      cout << "No hay actividad sospechosa." << endl;
      return;
    }

    // bubble sort para ordenar vector de bots
    for (int i = 0; i < posiblesBots.size(); i++) {
      bool flag = false;
      for (int j = 0; j < posiblesBots.size() - i - 1; j++) {
          if (posiblesBots[j].logs < posiblesBots[j+1].logs) {
              IP temp = posiblesBots[j];
              posiblesBots[j] = posiblesBots[j+1];
              posiblesBots[j+1] = temp;
              flag = true;
          }
      }
      if (!flag) {
          break;
      }
    }

    // se despliegan posibles bots
    cout << "Los posibles bots con umbral de " << umbral << " son: " << endl;
    int it = 1;
    for (auto& i : posiblesBots) {
      cout << it << ". ";
      cout << "IP: " << i.IPstring << ", Logs: " << i.logs << endl;
      it++;
    }
    cout << endl;
    cout << "El bot master es: " << endl;
    cout << "IP: " << posiblesBots[0].IPstring << " con " << posiblesBots[0].logs << " logs." << endl << endl;
}


int main() {
  Grafo grafo;
  grafo.readFile("bitacora_grafo.txt");
  int umbral = 160;
  grafo.detectarBots(umbral);
  
}



