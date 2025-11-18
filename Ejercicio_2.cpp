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

vector<vector<int>> ingresoMatrizBasica() {

    int filas, columnas;
    
    // Pedir dimensiones al usuario
    cout << "-------- Generacion de Matriz Booleana --------" << endl;
    cout << "Ingrese el numero de filas (maximo 100): ";
    cin >> filas;
    cout << "Ingrese el numero de columnas (maximo 10): ";
    cin >> columnas;

    // Validar límites
    if (filas <= 0 || filas > 100 || columnas <= 0 || columnas > 10) {
        throw out_of_range("Error: dimensiones fuera de rango.");
    }

    // Semilla aleatoria para que los resultados cambien cada vez
    srand(time(nullptr));

    // Crear y llenar la matriz booleana
    vector<vector<int>> matriz(filas, vector<int>(columnas));

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = rand() % 2; // Genera 0 o 1
        }
    }

    // Mostrar la matriz booleana generada
    cout << "\nMatriz booleana generada aleatoriamente:\n";
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }

    // Convertir en matriz básica
    vector<bool> basicas(filas, true);

    // Recorrer cada fila i
    for (int i = 0; i < filas; i++) {

        // Si ya fue eliminada, no la comparamos
        if (!basicas[i]) continue;

        // La comparamos con todas las demás filas
        for (int k = 0; k < filas; k++) {
            if (!basicas[k] || k == i) continue;

            bool hayDeterminante = false;

            // Revisamos columnas de 2 en 2 (c y c+1)
            for (int c = 0; c < columnas - 1; c++) {
                int a1 = matriz[i][c];
                int b1 = matriz[k][c];
                int a2 = matriz[i][c+1];
                int b2 = matriz[k][c+1];

                // patrón 0,1 seguido de 1,0
                bool pat1 = (a1 == 0 && b1 == 1) && (a2 == 1 && b2 == 0);
                // patrón 1,0 seguido de 0,1
                bool pat2 = (a1 == 1 && b1 == 0) && (a2 == 0 && b2 == 1);

                if (pat1 || pat2) {
                    hayDeterminante = true;
                    break;
                }
            }

            if (hayDeterminante) {
                // no se elimina ninguna de las dos, pero seguimos comparando i con la que sigue
                continue;
            }

            // si NO hay determinante, entonces una contiene a la otra.
            bool iContieneK = true;
            bool kContieneI = true;

            for (int c = 0; c < columnas; c++) {
                if (matriz[i][c] < matriz[k][c]) {
                    iContieneK = false;
                }
                if (matriz[k][c] < matriz[i][c]) {
                    kContieneI = false;
                }
            }

            if (iContieneK && !kContieneI) {
                basicas[i] = false;
                break;
            } else if (kContieneI && !iContieneK) {
                basicas[k] = false;
            }
        }
    }

    // Mostrar la matriz básica
    float filas_den = 0;
    float unos = 0;
    vector<vector<int>> matrizBasica;

    cout << "\nMatriz basica:\n";
    for (int i = 0; i < filas; i++) {
        if (!basicas[i]) continue;
        filas_den++;
        matrizBasica.push_back(matriz[i]);
        for (int j = 0; j < columnas; j++) {
            if (matriz[i][j] == 1) {
                unos++;
            }
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }

    return matrizBasica;
}


bool esTestor(const vector<vector<int>>& matriz, const vector<int>& columnas, int filas) {
    for (int i = 0; i < filas; i++) {
        for (int j = i + 1; j < filas; j++) {
            bool diferentes = false;
            for (int c : columnas) {
                if (matriz[i][c] != matriz[j][c]) {
                    diferentes = true;
                    break;
                }
            }
            if (!diferentes) return false; 
        }
    }
    return true;
}

void generarCombinaciones(const vector<vector<int>>& matriz, int filas, int columnasTotales,
                          int r, vector<vector<int>>& testores) {
    vector<int> comb(r);
    for (int i = 0; i < r; i++) comb[i] = i;

    while (true) {
        if (esTestor(matriz, comb, filas)) {
            bool esMinimo = true;
            for (const auto& t : testores) {
                bool subset = true;
                for (int x : t) {
                    if (find(comb.begin(), comb.end(), x) == comb.end()) { 
                        subset = false;
                        break;
                    }
                }
                if (subset) {
                    esMinimo = false;
                    break;
                }
            }
            if (esMinimo) {
                testores.push_back(comb);
            }
        }

        int i;
        for (i = r - 1; i >= 0; --i) {
            if (comb[i] != i + columnasTotales - r) break;
        }
        if (i < 0) break;
        comb[i]++;
        for (int j = i + 1; j < r; j++)
            comb[j] = comb[j - 1] + 1;
    }
}

int main() {
    // MATRIZ A
    /*
    vector<vector<int>> A = {
        {1, 0, 1, 0, 1, 0},
        {1, 1, 0, 0, 1, 0},
        {0, 1, 1, 0, 1, 1},
        {1, 0, 0, 1, 0, 1},
        {0, 1, 0, 1, 1, 0},
        {1, 0, 1, 0, 0, 1},
        {0, 1, 1, 1, 0, 0}
    };
    

    int filas = A.size();
    int columnas = A[0].size();
    */

    vector<vector<int>> A = ingresoMatrizBasica();
    if (A.empty()) {
        cout << "\nLa matriz basica resultante esta vacia. No se puede ejecutar YYC.\n";
        return 0;
    }

    int filas = A.size();
    int columnas = A[0].size();
    
    cout << "\n ----------Testores tipicos por prefijo de filas----------" << endl;

    cout << fixed << setprecision(20); 

    auto inicio = high_resolution_clock::now();

    for (int k = 2; k <= filas; k++) {
        auto t0 = high_resolution_clock::now();

        vector<vector<int>> testores;
        bool encontrados = false;

        for (int r = 1; r <= columnas; r++) {
            generarCombinaciones(A, k, columnas, r, testores);
            if (!testores.empty()) {
                encontrados = true;
                break;
            }
        }

        auto t1 = high_resolution_clock::now();
        duration<double> tiempoParcial = t1 - t0;
        duration<double> tiempoTotal = t1 - inicio;

        cout << "\nFilas consideradas: " << k << endl;
        cout << "Testores tipicos: ";
        if (testores.empty()) cout << "(ninguno)";
        else {
            for (auto& t : testores) {
                cout << "{";
                for (size_t i = 0; i < t.size(); i++) {
                    cout << t[i];
                    if (i < t.size() - 1) cout << ",";
                }
                cout << "} ";
            }
        }
        cout << "\nTiempo parcial: " << tiempoParcial.count() << " s";
        cout << "\nTiempo acumulado: " << tiempoTotal.count() << " s\n";
    }

    // Densidad final
    int unos = 0;
    for (auto& fila : A)
        for (int v : fila) if (v == 1) unos++;
    double densidad = (double)unos / (filas * columnas);

    cout << endl;
    cout << "Densidad de la matriz: " << densidad << endl;

    return 0;
}