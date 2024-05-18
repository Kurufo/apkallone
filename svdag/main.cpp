#include <vector>
#include <iostream>
#include "SVDAG.h"
#include <fstream>

using namespace std;

int main(){

    //vector<int32_t> u ={161,4,5,5,256,129,8,9,512,768};
    vector<int32_t> u ={161,4,5,8,256,1,7,512,129,11,12,768,1024};
    size_t rootsize = 4;
    SVDAG v(u,rootsize);
    //
    cout << "Me vengo" << endl;
    cout << "Dame los punto" << endl;
    int x,y,z;
    cout << "valor x: " << endl;
    cin >> x;
    cout << "valor y: " << endl;
    cin >> y;
    cout << "valor z: " << endl;
    cin >> z;
    cout << "Estas ahi?\n"; 
    cout << v.get_cell(x,y,z) << endl;

    return 0;
}
