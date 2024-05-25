#include <vector>
#include <iostream>
#include "SVDAG.h"
#include <fstream>
#include <string.h>

using namespace std;

int main(){

    string ubicacion = "vox/menger.vox";
    SVDAG v(ubicacion);

    cout << "funciono ?" << endl;

   v.print();

   cout << "Cantidad de ciclos: " << v.cycles_number() << endl;

    return 0;
}
