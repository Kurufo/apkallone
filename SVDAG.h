#ifndef SVDAG_H
#define SVDAG_H
#include <vector>
#include <iostream>
#include <tuple>
#include <string>

using namespace std;

class SVDAG {
private:
  // Tamaño de la imagen (Largo de un Cubo)
  size_t RootSize;
  // Estructura base del SVDAG
  std::vector<int32_t> svdag;  
  // Numero de ciclos
  int32_t cycles;

  // Estructura para crear cajas en el espacio
  struct AABB {
    std::tuple<int,int,int> min;
    size_t size;
  };
  

  // Pasa de una posicion del espacio a un indice en el arreglo
  int positionToIndex(const std::tuple<int,int,int>& position);

  std::tuple<int,int,int> operation(std::tuple<int,int,int> position,
  std::tuple<int,int,int> boxmin, int level);

  //
  void transformAABB(int childrenIndex, AABB& box);

  // Encontrar un nodo en una posicion especifica
  bool findNodeAt(const std::tuple<int,int,int>& position, bool& filled, AABB& boxout); 

  int countSetBits(int n);



public:

  // Construye el SVDAG desde un archivo que ya tiene almacenado el vector de enteros
  //SVDAG(ifstream data_file); 
  //SVDAG(vector<int32_t> u,size_t v);
  SVDAG(string archivo);

  ~SVDAG() {};
  
  // True si existe la celda (x,y,z)
  // Nota: Usar findNodeAt
  bool get_cell(int x, int y, int z);

  int32_t cycles_number();

  void print();

};

#endif