#include "SVDAG.h"
#include <functional>
#include "VoxLoader.h"
#include "Terrain.h"
#include <glm/glm.hpp>
#include <iostream>

//Contructor
SVDAG(ifstream data_file){
  std::ifstream file(data_file);
  if(file.is_open()){
    std::string line;
    while(std::getline(file, line)){
      std::istringstream iss(line);
      int value;
      if (iss >> value) {
        lines.push_back(value);
      }        
    file.close();
    }
  }
}



size_t SVDAG::hash() {
	static std::hash<long> hasher;
	if(hashValue) return hashValue;
	
	size_t result = 0;
	
	for (int i = 0; i < 8; i++) {
		if (children[i] != nullptr) {
			result ^= children[i]->hash() << i;
		}
		else {
			result ^= 1 << i;
		}
	}

	return hashValue = (hasher(result) ^ MaterialHasher()(material));
}

bool AABBInside(const glm::vec3& pt, const AABB& aabb) {
    glm::vec3 max = aabb.min + aabb.size;
    return glm::all(glm::greaterThanEqual(pt, aabb.min)) &&
           glm::all(glm::lessThanEqual(pt, max));
}

glm::vec2 intersectAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const AABB& box) {
    glm::vec3 invRayDir = 1.0f / rayDir;
    glm::vec3 tMin = (box.min - rayOrigin) * invRayDir;
    glm::vec3 tMax = (box.min + box.size - rayOrigin) * invRayDir;

    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);

    float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
    float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);

    return glm::vec2(tNear, tFar);
}

int positionToIndex(const glm::vec3& position) {
    return static_cast<int>(position.x) * 4 + static_cast<int>(position.y) * 2 + static_cast<int>(position.z);
}

void transformAABB(int childrenIndex, AABB& box) {
    box.size /= 2;
    if (childrenIndex % 2 == 1) // 1,3,5,7
        box.min.z += box.size.z;
    if (childrenIndex == 2 || childrenIndex == 3 || childrenIndex == 6 ||
        childrenIndex == 7)
        box.min.y += box.size.y;
    if (childrenIndex >= 4) // 4,5,6,7
        box.min.x += box.size.x;
}


//--------------------------------------------------------------------------//
/*
struct Material {
    // Definir la estructura Material según tus necesidades
};
*/

// extern int svdagData[];
//extern Material materials[];

const float Epsilon = 0.0005;
const float RootSize = /* tamaño de la raíz */;

bool findNodeAt(const glm::vec3& position, bool& filled, AABB& boxout, Material& mat) {
    int level = 0;
    int index = 0;
    AABB box = { glm::vec3(Epsilon), glm::vec3(RootSize) }; // inicializar a la caja raíz
    // Cambiar esto por en while despues
    for (int i = 0; i < 32; ++i) {
        int childrenIndex = positionToIndex(2.0f * (position - box.min) / (RootSize >> level));
        transformAABB(childrenIndex, box);

        int bitmask = svdag[index];

        // si no tiene hijos, todo este nodo está lleno
        if ((bitmask & 255) == 0) {
            filled = true;
            boxout = box;
            mat = materials[bitmask >> 8];
            return true;
        }

        // revisar si tiene los hijos específicos
        if (((bitmask >> childrenIndex) & 1) == 1) {
            index = svdag[index + 1 + bitCount(bitmask & ((1 << childrenIndex) - 1))];
            level += 1;
        } else {
            filled = false;
            boxout = box;
            return true;
        }
    }
    return false; // no debería llegar aquí
}


bool get_cell(int x, int y, int z){
  // Transformar el punto a vec3
  glm::vec3 position(x, y, z); 
  // Valor que se encontro 
  bool filled;
  // Caja que representa la raiz del arbol
  AABB boxout = AABB(glm::vec3(0), size); //???
  // Material guardado en la posicion encontrada 
  Material mat;
  bool aux = findNodeAt(position,filled,boxout,mat);
  return filled;
  }

/*
  ***** Cosas que creemos que vamos a necesitar ****

int componentClosest(vec3 pt, float target) {
    vec3 dist = abs(pt - target);
    // if even the closest has a very large distance, then return -1 as mismatch
    if (min(dist.x, min(dist.y, dist.z)) > Epsilon * 2) {
		return -1;
	}
    if (dist.x < dist.y && dist.x < dist.z) {
		return 0;
	} else if(dist.y < dist.z) {
		return 1;
	} else {
		return 2;
	}
}

vec3 getNormal(in vec3 pt, in AABB box) {
    vec3 localized = pt - box.min;

    int cMin = componentClosest(localized, 0);
    if (cMin != -1) {
        return vec3(cMin == 0 ? -1 : 0, cMin == 1 ? -1 : 0, cMin == 2 ? -1 : 0);
    }
    int cMax = componentClosest(localized, box.size.x);
    return vec3(cMax == 0 ? 1 : 0, cMax == 1 ? 1 : 0, cMax == 2 ? 1 : 0);
}

// return hit info
bool raytrace(
    in vec3 rayOri,
    in vec3 rayDir,
    out vec3 hitPosition,
    out vec3 normal,
    out Material mat,
    bool ignoreWater,
    out vec3 lastRayOri
) {
  vec3 ro = rayOri;
  
  lastRayOri = ro;

  // place ro to inside the box first
  vec2 t = intersectAABB(ro, rayDir, AABB(vec3(0), vec3(RootSize)));
  if (t.x > t.y || t.y < 0) {
    return false;
  }
  if (t.x > 0) ro += rayDir * (t.x + Epsilon);

  for (int i = 0; i < MAX_RAYTRACE_DEPTH; i++) {
    bool filled = false;
    AABB box;
    findNodeAt(ro, filled, box, mat);
    
    lastRayOri = ro;

    // if that point is filled, then just return color
    if (filled && (!ignoreWater || mat.water == 0)) {
      normal = getNormal(ro, box);
      hitPosition = ro - rayDir * Epsilon * 2 + normal * Epsilon;
      return true;
    }

    // otherwise, place ro to the next box. intersect inside aabb here
    vec2 t = intersectAABB(ro, rayDir, box);
    if (t.y < 0)
      break;
    ro += rayDir * (t.y + Epsilon);
    if (ro.x > RootSize || ro.y > RootSize || ro.z > RootSize || ro.x < 0 || ro.y < 0 || ro.z < 0)
      break;
  }
  return false;
}


*/