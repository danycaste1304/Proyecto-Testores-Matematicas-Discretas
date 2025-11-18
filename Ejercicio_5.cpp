#include <iostream>
#include <ctime>    
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <cstdint> 
#include <chrono> 
#include <algorithm> 
#include <utility>
#include <iomanip>  
#include <cmath> 
using namespace std;
using namespace std::chrono;

using Row = vector<int>;
using Matrix = vector<Row>;

Matrix theta(const Matrix& A, const Matrix& B) {
    Matrix R;

    for (int i = 0; i < (int)A.size(); i++) {
        for (int j = 0; j < (int)B.size(); j++) {
            Row fila;

            for (int k = 0; k < (int)A[i].size(); k++) {    //Agrega los elementos de A
                fila.push_back(A[i][k]);
            }

            for (int k = 0; k < (int)B[j].size(); k++) {   //Agrega los elementos de B
                fila.push_back(B[j][k]);
            }

            R.push_back(fila);
        }
    }

    return R;
}

Matrix gammaAB(const Matrix& A, const Matrix& B) {
    Matrix R;

    int colA = A[0].size();
    int colB = B[0].size();

    //Copiar las filas de A seguidas de ceros
    for (int i = 0; i < (int)A.size(); i++) {
        Row fila;

        for (int k = 0; k < colA; k++) {
            fila.push_back(A[i][k]);
        }

        for (int k = 0; k < colB; k++) {
            fila.push_back(0);
        }

        R.push_back(fila);
    }

    //Copiar las filas de B precedidas de ceros
    for (int i = 0; i < (int)B.size(); i++) {
        Row fila;

        for (int k = 0; k < colA; k++) {
            fila.push_back(0);
        }

        for (int k = 0; k < colB; k++) {
            fila.push_back(B[i][k]);
        }

        R.push_back(fila);
    }

    return R;
}

Matrix phi(const Matrix& A, const Matrix& B) {
    if (A.size() != B.size()) {
        throw runtime_error("A y B deben tener el mismo numero de filas");
    }

    Matrix R;

    for (int i = 0; i < (int)A.size(); i++) {
        Row fila;

        // Copiamos A[i]
        for (int k = 0; k < (int)A[i].size(); k++) {
            fila.push_back(A[i][k]);
        }

        // Copiamos B[i]
        for (int k = 0; k < (int)B[i].size(); k++) {
            fila.push_back(B[i][k]);
        }

        R.push_back(fila);
    }

    return R;
}

Matrix phi_power(const Matrix& M, int N) {
    Matrix R;
    for (int i = 0; i < (int)M.size(); i++) {
        Row fila;
        for (int rep = 0; rep < N; rep++) {
            for (int k = 0; k < (int)M[i].size(); k++) {     //concatenar M con sigo mismo
                fila.push_back(M[i][k]);
            }
        }
        R.push_back(fila);
    }
    return R;
}

Matrix gamma_power(Matrix M, int N) {
    Matrix R = M;     // γ¹(M) = M
    for (int i = 2; i <= N; i++) {
        R = gammaAB(R, M);   // vamos agregando bloques
    }
    return R;
}

int contarUnos(const Row& fila) {
    int c = 0;
    for (int x : fila) {
        if (x == 1) c++;
    }
    return c;
}

Matrix ordenarPorUnos(const Matrix& M) {
    Matrix R = M; // copia

    int n = R.size();

    for (int i = 0; i < n - 1; i++) {
        int indiceMenor = i;

        // buscar el índice de la fila con menos unos desde i hasta el final
        for (int j = i + 1; j < n; j++) {
            if (contarUnos(R[j]) < contarUnos(R[indiceMenor])) {
                indiceMenor = j;
            }
        }

        // intercambiar filas R[i] y R[indiceMenor]
        Row temp = R[i];
        R[i] = R[indiceMenor];
        R[indiceMenor] = temp;
    }
    return R;
}

//YYC
bool esTestor(const Matrix& matriz, const vector<int>& columnas, int filas) {
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

void generarCombinaciones(const Matrix& matriz, int filas, int columnasTotales, int r, vector<vector<int>>& testores) {
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


double ejecutarYYC(const Matrix& A) {
    if (A.empty()) {
        return 0.0;  // nada que hacer
    }

    int filas    = (int)A.size();
    int columnas = (int)A[0].size();

    auto inicio = high_resolution_clock::now();

    // Bucle original sobre k (prefijo de filas)
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
        duration<double> tiempoTotal   = t1 - inicio;

        // Ahora solo estamos midiendo el tiempo total (sin cout).
        (void)tiempoParcial;
        (void)tiempoTotal;
        (void)encontrados;
    }

    auto fin = high_resolution_clock::now();
    duration<double> total = fin - inicio;

    return total.count();  // tiempo total en segundos
}

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

    Matrix B = {
        {1,1,0,0,0,0}, 
        {1,0,1,1,0,0}, 
        {1,0,1,0,1,0}, 
        {1,0,0,1,1,0}, 
        {1,0,0,1,0,1}, 
        {0,1,1,1,0,0}, 
        {0,1,1,0,0,1}, 
        {0,1,0,0,1,1}, 
        {0,0,1,1,1,0}, 
        {0,0,1,0,1,1}, 
        {0,0,0,1,1,1}  
    };

    Matrix T = theta(A, B);
    Matrix M0 = theta(A, B);

    //Tabla 5
    
    Matrix M1 = phi_power(M0, 1); // ϕ(θ(A,B))
    Matrix M2 = phi_power(M0, 2); // ϕ²(θ(A,B))
    Matrix M3 = phi_power(M0, 3); // ϕ³(θ(A,B))
    Matrix M4 = phi_power(M0, 4); // ϕ⁴(θ(A,B))
    Matrix M5 = phi_power(M0, 5); // ϕ⁵(θ(A,B))

    //Tabla 6
    Matrix G1 = gamma_power(M0, 1); // γ(θ(A,B))  
    Matrix G2 = gamma_power(M0, 2); // γ²(θ(A,B))
    Matrix G3 = gamma_power(M0, 3); // γ³(θ(A,B))
    Matrix G4 = gamma_power(M0, 4); // γ⁴(θ(A,B))

    return 0;
}
