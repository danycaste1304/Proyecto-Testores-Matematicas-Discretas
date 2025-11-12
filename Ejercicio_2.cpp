#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm> 
using namespace std;
using namespace std::chrono;

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

    cout << "----------Testores tipicos por prefijo de filas----------" << endl;


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