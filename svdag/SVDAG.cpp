#include "SVDAG.h"
#include <functional>
#include <iostream>
#include <tuple>
#include "SVO.h"
#include <cstdint>
#include <string>

const float Epsilon = 0.0005;

using namespace std;



/*SVDAG::SVDAG(vector<int32_t> u,size_t v ){
  svdag = u;
  //={161,4,5,5,256,129,8,9,512,768};
  RootSize = v;
  //=4; 

}*/

SVDAG::SVDAG(string archivo){
    // const char* texto = archivo.c_str();
    SVO* aux =  SVO::fromVox(archivo.c_str());
    RootSize = aux->getSize();
    vector<SVO::Material> basura;
    aux->toSVDAG(svdag,basura);
}


int SVDAG::positionToIndex(const std::tuple<int,int,int>& position) {
    return get<0>(position) * 4 + get<1>(position) * 2 + get<2>(position);
}

void SVDAG::transformAABB(int childrenIndex, AABB& box) {
    box.size /= 2;
    if (childrenIndex % 2 == 1) // 1,3,5,7
        get<2>(box.min) += box.size;
    if (childrenIndex == 2 || childrenIndex == 3 || childrenIndex == 6 ||
        childrenIndex == 7)
        get<1>(box.min) += box.size;
    if (childrenIndex >= 4) // 4,5,6,7
        get<0>(box.min) += box.size;
}

std::tuple<int,int,int> SVDAG::operation(std::tuple<int,int,int> position,
  std::tuple<int,int,int> boxmin, int level){
    tuple<int,int,int> u;
    get<0>(u)=2.0f *( get<0>(position)-get<0>(boxmin) ) / (RootSize >> level);
    get<1>(u)=2.0f *( get<1>(position)-get<1>(boxmin) ) / (RootSize >> level);
    get<2>(u)=2.0f *( get<2>(position)-get<2>(boxmin) ) / (RootSize >> level);
    return u;
  };

// Hay que optmizarlo
int SVDAG::countSetBits(int n){
 int count=0;
 while(n){
  count += n&1;
  n >>= 1;
 }
 return count;
}

bool SVDAG::findNodeAt(const std::tuple<int,int,int>& position, bool& filled, AABB& boxout) {
    int level = 0;
    int index = 0;
    AABB box = { make_tuple(Epsilon, Epsilon, Epsilon), RootSize}; // inicializar a la caja raíz
    // Cambiar esto por en while despues
    for (int i = 0; i < 32; ++i) {
        int childrenIndex = positionToIndex(operation(position,box.min,level));
        transformAABB(childrenIndex, box);

        int bitmask = svdag[index];

        // si no tiene hijos, todo este nodo está lleno
        if ((bitmask & 255) == 0) {
            filled = true;
            boxout = box;
            return true;
        }

        // revisar si tiene los hijos específicos
        if (((bitmask >> childrenIndex) & 1) == 1) {
            index = svdag[index + 1 + countSetBits(bitmask & ((1 << childrenIndex) - 1))];
            level += 1;
        } else {
            filled = false;
            boxout = box;
            return true;
        }
    }
    return false; // no debería llegar aquí
}


bool SVDAG::get_cell(int x, int y, int z){
  // Transformar el punto a vec3
  std::tuple<int,int,int> position(x, y, z); 
  // Valor que se encontro 
  bool filled;
  // Caja que representa la raiz del arbol
  AABB boxout = {make_tuple(0,0,0), RootSize }; //???
  // Material guardado en la posicion encontrada 
  bool aux = findNodeAt(position,filled,boxout);
  return filled;
  }
