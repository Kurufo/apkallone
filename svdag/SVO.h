#include <memory>
#include <vector>
#include <unordered_map>
//#include <glm/glm.hpp>
#include <tuple>

using namespace std;

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

	void set(size_t x, size_t y, size_t z, utuple rgb, bool water = false);
	void toSVDAG(std::vector<int32_t>& svdag, std::vector<Material>& materials);

	size_t hash();
	size_t getSize() const noexcept { return size; }

	static SVO* sample();
	//static SVO* terrain(int size);
	//static SVO* stair(int size);
	static SVO* fromVox(const char* filename);
	//void print_svo(SVO* n, int level);
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
		std::unordered_map<Material, size_t, MaterialHasher>& materialToIndex
	);
	SVO* children[8] = { nullptr };
	size_t hashValue = 0;
	size_t size = 0;
	Material material = { {0, 0, 0} };
};
