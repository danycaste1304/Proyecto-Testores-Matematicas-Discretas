#include <iostream>
#include <ctime>     // time
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <cstdint> //Permite agregar uint64_t
#include <chrono> //Ayuda a calcular el tiempo de ejecución del algoritmo
#include <algorithm> 
#include <utility>
#include <stdexcept>
#include <iomanip>  
using namespace std;

using Row = vector<int>;
using Matrix = vector<Row>;

int main(){
    Matrix A = {
        {1,0,1,0,1,0},
        {1,1,0,0,1,0},
        {0,1,1,0,1,1},
        {1,0,0,1,0,1},
        {0,1,0,1,1,0},
        {1,0,1,0,0,1},
        {0,1,1,1,0,0}
    };

    return 0;
}