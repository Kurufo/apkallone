#include <memory>
#include <vector>
#include <unordered_map>
#include <tuple>

using namespace std;

// Puntos en el espacio
using utuple = tuple<unsigned int, unsigned int, unsigned int>;


class SVO {
public:
	struct Material {
		utuple color;
		unsigned int water = 0;
		bool operator==(const Material& other) const noexcept {
			return color == other.color && water == other.water;
		}
	};

	SVO(size_t size) : size(size) {}
	SVO(SVO&) = delete;
	SVO(SVO&&) = delete;
	~SVO() { for (auto& child : children) delete child; }
	SVO& operator=(SVO&) = delete;
	SVO& operator=(SVO&&) = delete;

	// Muestra el color y agua que hay en un punto especifico
	void set(size_t x, size_t y, size_t z, utuple rgb, bool water = false);
	// Creacion del SVDAG
	void toSVDAG(std::vector<int32_t>& svdag, std::vector<Material>& materials, int32_t &cycles);

	// Funcion hash especial
	size_t hash();
	// Largo del cubo del .vox
	size_t getSize() const noexcept { return size; }

	// Es un ejemplo de cuatro cubos de color rojo, azul, verde y blanco
	static SVO* sample();
	// Lectura de un archivo .vox y creacion de su SVO
	static SVO* fromVox(const char* filename);
private:
	struct MaterialHasher {
		std::size_t operator() (const Material& mat) const {
			std::hash<unsigned int> hasher;
			return hasher((get<0>(mat.color) << 16) | (get<1>(mat.color) << 8) | (get<2>(mat.color)) | (mat.water << 24));
		}
	};

	void toSVDAGImpl(
		std::vector<int32_t>& result,
		std::vector<Material>& materials,
		std::unordered_map<size_t, size_t>& hashToIndex,
		std::unordered_map<Material, size_t, MaterialHasher>& materialToIndex,
		int32_t &cycles
	);
	SVO* children[8] = { nullptr };
	size_t hashValue = 0;
	size_t size = 0;
	Material material = { {0, 0, 0} };
};
