#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <glm/glm.hpp>


class SVDAG {
private:
  // 
  std::vector<int32_t> svdag; 
  std::vector<Material>& materials;
  //std::unordered_map<size_t, size_t>& hashToIndex,
  // Estas tablas hash estan aqui por el momento. 
	//std::unordered_map<Material, size_t, MaterialHasher>& materialToIndex

  // Chequea si un punto está en una region del espacio
  bool AABBInside(const glm::vec3& pt, const AABB& aabb);

  // Chequea si un rayo colisiona con una region del espacio
  glm::vec2 intersectAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const AABB& box) {

  // Pasa de una posicion del espacio a un indice en el arreglo
  int positionToIndex(const glm::vec3& position) 

  //
  void transformAABB(int childrenIndex, AABB& box)

  // Encontrar un nodo en una posicion especifica
  bool findNodeAt(const glm::vec3& position, bool& filled, AABB& boxout, Material& mat) 


public:

  // Construye el SVDAG desde un archivo que ya tiene almacenado el vector de enteros
  SVDAG(ifstream data_file); 
  
  ~SVDAG() {svdag.clear(); 
  materials.clear();
  // Puede que necesitemos matar las tablas hash
  }
  
  size_t hash(); // Necesario, creo que si
	//size_t getSize() const noexcept { return size; } <- Definimos la funcion sobre el espacio que se representa?

  // True si existe la celda (x,y,z)
  // Nota: Usar findNodeAt
  bool get_cell(int x, int y, int z);

  // Estructura para crear cajas en el espacio
  struct AABB {
    glm::vec3 min;
    glm::vec3 size;
};

  // Estructura para guardar los datos de color en un punto.
  struct Material {
		glm::uvec3 color;
		unsigned int water = 0;
		bool operator==(const Material& other) const noexcept {
			return color == other.color && water == other.water;
		}
	};

};
