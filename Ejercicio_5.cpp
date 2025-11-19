#include <iostream>
#include <ctime>  
#include <string>  
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

Matrix phi_pot(const Matrix& M, int N) {
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

Matrix gamma_pot(Matrix M, int N) {
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

//------------YYC-------------------
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

//----------BT-----------

void print_row(const Row &r) {
    for (int x : r) cout << x << " ";
    cout << endl;
}

// Convierte un testor típico en conjunto {i,j,k}
vector<int> row_to_set(const Row &r) {
    vector<int> S;
    for (int j = 0; j < (int)r.size(); j++) {
        if (r[j] == 1)
            S.push_back(j + 1);   // posición humana (1-based)
    }
    return S;
}

// Imprime el conjunto {i,j,k}
void print_set(const vector<int> &S) {
    cout << "{";
    for (int i = 0; i < (int)S.size(); i++) {
        cout << S[i];
        if (i < (int)S.size() - 1) cout << ",";
    }
    cout << "}";
}

//Funciones auxiliares
uint64_t row_to_mask(const Row &r) {
    int n = r.size();
    uint64_t m = 0;

    for (int j = 0; j < n; ++j) {
        if (r[j] == 1) {
            int bit = n - 1 - j;
            m |= (1ULL << bit);
        }
    }
    return m;
}

Row mask_to_row(uint64_t m, int n) {
    Row r(n, 0);
    for (int j = 0; j < n; ++j) {
        int bit = n - 1 - j;
        r[j] = ((m >> bit) & 1ULL) ? 1 : 0;
    }
    return r;
}

int last_one_index(const Row &r) {
    for (int j = (int)r.size() - 1; j >= 0; --j)
        if (r[j] == 1)
            return j;
    return -1;
}

//Tipicidad
bool contiene(const Row &sup, const Row &sub) {
    for (int j = 0; j < (int)sub.size(); j++) {
        if (sub[j] == 1 && sup[j] == 0)
            return false;
    }
    return true;
}

bool esTipico(const Row &alpha, const vector<Row> &T) {
    for (const Row &t : T) {
        if (contiene(alpha, t))
            return false;
    }
    return true;
}

//Proposicion 1.4
Row salto_1_4(const Row &alpha) {
    int n = alpha.size();
    int last = last_one_index(alpha);

    int k   = last + 1;
    int exp = n - k;

    uint64_t step;
    if (exp <= 0) {
        step = 1ULL;
    } else {
        step = (1ULL << exp);
    }

    uint64_t mask      = row_to_mask(alpha);
    uint64_t next_mask = mask + step;

    uint64_t limit = (1ULL << n);
    if (next_mask >= limit)
        return Row(n, 1);

    return mask_to_row(next_mask, n);
}

//Proposicion 1.5
Row salto_1_5(const Row &alpha, const Matrix &MB, const vector<int> &offenders) {
    int n = alpha.size();

    vector<int> b_positions;
    for (int idx : offenders) {
        int b = last_one_index(MB[idx]);
        if (b != -1) b_positions.push_back(b);
    }

    int k = b_positions[0];
    for (int pos : b_positions)
        if (pos < k) k = pos;

    Row new_alpha(n);
    for (int j = 0; j < n; j++) {
        if (j < k)       new_alpha[j] = alpha[j];
        else if (j == k) new_alpha[j] = 1;
        else             new_alpha[j] = 0;
    }
    return new_alpha;
}

//Proposicion 1.3
pair<bool, vector<int>> isTestor_and_offenders(const Row &alpha, const Matrix &MB) {
    int n = (int)alpha.size();
    vector<int> offenders;

    for (int i = 0; i < (int)MB.size(); ++i) {
        const Row &r = MB[i];
        bool covers  = false;
        for (int j = 0; j < n; ++j) {
            if (alpha[j] == 1 && r[j] == 1) {
                covers = true;
                break;
            }
        }
        if (!covers) {
            offenders.push_back(i);
        }
    }
    bool isTestor = offenders.empty();
    return {isTestor, offenders};
}


Matrix BT(const Matrix &MB) {
    int n = MB[0].size();

    Row alpha(n, 0);
    alpha[n - 1] = 1;

    Row ultimo(n, 1);

    Matrix tipicos;

    while (true) {
        pair<bool, vector<int>> resultado = isTestor_and_offenders(alpha, MB);
        bool esTestor      = resultado.first;
        vector<int> offenders = resultado.second;

        if (!esTestor) {
            if (!offenders.empty()) {
                alpha = salto_1_5(alpha, MB, offenders);
            } else {
                uint64_t mask = row_to_mask(alpha);
                mask += 1ULL;
                alpha = mask_to_row(mask, n);
            }
        } else {
            if (esTipico(alpha, tipicos)) {
                tipicos.push_back(alpha);
                alpha = salto_1_4(alpha);
            } else {
                uint64_t mask = row_to_mask(alpha);
                mask += 1ULL;
                alpha = mask_to_row(mask, n);
            }
        }

        if (alpha == ultimo) break;
    }

    return tipicos;
}

// Ejecutar BT
double ejecutarBT(const Matrix &A) {
    if (A.empty()) return 0.0;

    int columnas = (int)A[0].size();
    int repeticiones;

    if (columnas <= 14)      repeticiones = 30; // matrices chiquitas
    else if (columnas <= 25) repeticiones = 5;  // medianas
    else                     repeticiones = 1;  // grandes

    auto inicio = high_resolution_clock::now();
    for (int i = 0; i < repeticiones; ++i) {
        Matrix tipicos = BT(A);
        (void)tipicos;
    }
    auto fin = high_resolution_clock::now();

    duration<double> total = fin - inicio;
    return total.count() / repeticiones;
}


//Imprimir resultados

void imprimirResultados(const string& nombre, const Matrix& M) {
    int filas = (int)M.size();
    int columnas = (int)M[0].size();

    Matrix M_ordenada = ordenarPorUnos(M);

    double tYYC_normal   = ejecutarYYC(M);
    double tYYC_ordenada = ejecutarYYC(M_ordenada);

    double tBT_normal    = ejecutarBT(M);
    double tBT_ordenada  = ejecutarBT(M_ordenada);

    cout << "\n==============================\n";
    cout << "Matriz: " << nombre << "\n";
    cout << "Filas: " << filas << ", Columnas: " << columnas << "\n";
    cout << "YYC (filas originales): " << tYYC_normal   << " s\n";
    cout << "YYC (filas ordenadas) : " << tYYC_ordenada << " s\n";
    cout << "BT  (filas originales): " << tBT_normal    << " s\n";
    cout << "BT  (filas ordenadas) : " << tBT_ordenada  << " s\n";
}


int main(){
    cout << fixed << setprecision(10);

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

    cout << "\n==== RESULTADOS TABLA 5 (operador phi) ====\n";

    Matrix M1 = phi_pot(M0, 1); // ϕ(θ(A,B))
    Matrix M2 = phi_pot(M0, 2); // ϕ²(θ(A,B))
    //Matrix M3 = phi_pot(M0, 3); // ϕ³(θ(A,B))
    //Matrix M4 = phi_pot(M0, 4); // ϕ⁴(θ(A,B))
    //Matrix M5 = phi_pot(M0, 5); // ϕ⁵(θ(A,B))

    imprimirResultados("phi^1(theta(A,B))", M1);
    imprimirResultados("phi^2(theta(A,B))", M2);
    //imprimirResultados("phi^3(theta(A,B))", M3);
    //imprimirResultados("phi^4(theta(A,B))", M4);
    //imprimirResultados("phi^5(theta(A,B))", M5);

    cout << "\n==== RESULTADOS TABLA 6 (operador gamma) ====\n";

    Matrix G1 = gamma_pot(M0, 1); // γ(θ(A,B))  
    Matrix G2 = gamma_pot(M0, 2); // γ²(θ(A,B))
    //Matrix G3 = gamma_pot(M0, 3); // γ³(θ(A,B))
    //Matrix G4 = gamma_pot(M0, 4); // γ⁴(θ(A,B))

    imprimirResultados("gamma^1(theta(A,B))", G1);
    imprimirResultados("gamma^2(theta(A,B))", G2);
    //imprimirResultados("gamma^3(theta(A,B))", G3);
    //imprimirResultados("gamma^4(theta(A,B))", G4);

    return 0;
}