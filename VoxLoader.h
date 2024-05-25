#pragma once

#include <functional>
#include <tuple>

using namespace std;

using utuple = tuple<unsigned int, unsigned int, unsigned int>;


extern void loadVox(const char* filename, std::function<void(int x, int y, int z, utuple color)> emit);
