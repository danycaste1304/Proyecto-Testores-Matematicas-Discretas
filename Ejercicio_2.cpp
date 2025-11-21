#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm> 
#include <stdexcept>
#include <cstdlib>  
#include <ctime>   
#include <iomanip>    
using namespace std;
using namespace std::chrono;

using Matrix = vector<vector<int>>;

vector<int> sumarFilas(const Matrix &A, const vector<int> &cols) {
    int filas = A.size();
    vector<int> sumas(filas, 0);  // aquí guardamos la suma de cada fila

    for (int i = 0; i < filas; i++) {
        int suma = 0;
        // recorro las columnas seleccionadas
        for (int j = 0; j < (int)cols.size(); j++) {
            int c = cols[j];           // índice de columna (0,1,2,...)
            suma += A[i][c];           // sumo el valor A[i][c]
        }
        sumas[i] = suma;               // guardo la suma de la fila i
    }

    return sumas;
}

bool condicionYYC(const Matrix &A,
                  const vector<int> &sumas,
                  const vector<int> &cols) 
{
    int numColumnas = cols.size();

    // 1) Contamos filas donde la suma es 1
    vector<int> filasConUno;
    for (int i = 0; i < (int)sumas.size(); i++) {
        if (sumas[i] == 1) {
            filasConUno.push_back(i);
        }
    }

    // Condición 1: |cols| >= número de filas con suma 1
    if (numColumnas < (int)filasConUno.size()) {
        return false;
    }

    // 2) Revisamos que cada columna tenga al menos un 1 en esas filas
    for (int c = 0; c < numColumnas; c++) {
        int columna = cols[c];  // índice real de la columna
        bool tieneUno = false;

        // Revisamos en las filas con suma 1
        for (int f = 0; f < (int)filasConUno.size(); f++) {
            int fila = filasConUno[f];
            if (A[fila][columna] == 1) {
                tieneUno = true;
                break;
            }
        }

        if (!tieneUno) {
            return false;  // esta columna NO tiene ningún 1 → falla condición
        }
    }

    return true;  // ambas condiciones se cumplieron
}

int main() {
    Matrix A = {
        {1,0,1,0},
        {0,1,1,0},
        {1,1,0,1}
    };

    vector<int> columnas = {0, 2};  // usando columnas 0 y 2
    vector<int> sumas = sumarFilas(A, columnas);

    bool ok = condicionYYC(A, sumas, columnas);

    cout << "¿Cumple condiciones? " << (ok ? "SI" : "NO") << endl;

    return 0;
}
