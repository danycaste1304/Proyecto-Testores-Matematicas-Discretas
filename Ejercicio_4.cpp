#include <iostream>
#include <vector>
#include <cstdint>
using namespace std;

using Row = vector<int>;
using Matrix = vector<Row>;

void print_row(const Row &r);

uint64_t row_to_mask(const Row &r);
Row mask_to_row(uint64_t m, int n);

int last_one_index(const Row &alpha);             // último 1 de alpha
int find_rightmost_one(const Row &r);             // 1 más a la derecha en fila r

bool contiene(const Row &sup, const Row &sub);
bool esTipico(const Row &alpha, const vector<Row> &T);

pair<bool, vector<int>> isTestor_and_offenders(const Row &alpha, const Matrix &MB);

Row salto_1_4(const Row &alpha);
Row salto_1_5(const Row &alpha, const Matrix &MB, const vector<int> &offenders);

vector<Row> BT(const Matrix &MB);

vector<int> row_to_set(const Row &r);
void print_set(const std::vector<int> &S);


int main(){
    Matrix B = {
        {1,1,0,0,0,0}, //{1,2}
        {1,0,1,1,0,0}, //{1,3,4}
        {1,0,1,0,1,0}, //{1,3,5}
        {1,0,0,1,1,0}, //{1,4,5}
        {1,0,0,1,0,1}, //{1,4,6}
        {0,1,1,1,0,0}, //{2,3,4}
        {0,1,1,0,0,1}, //{2,3,6}
        {0,1,0,0,1,1}, //{2,5,6}
        {0,0,1,1,1,0}, //{3,4,5}
        {0,0,1,0,1,1}, //{3,5,6}
        {0,0,0,1,1,1}  //{4,5,6}
    };

    cout << "Matriz B: " << endl;

    for (auto &row : B) {
        print_row(row);

    }

    int filas = 11;
    int columnas = 6;

    cout << "\nComprobar que B es MB" << endl;
    vector<bool> basicas(filas, true);

    // Recorrer cada fila i
    for (int i = 0; i < filas; i++) {

        //si ya fue eliminada, no la comparamos
        if (!basicas[i]) continue;

        // la comparamos con las filas que vienen después
        for (int k = 0; k < filas; k++) {
            if (!basicas[k]||k==i) continue;

            bool hayDeterminante = false;

    // Revisamos columnas de 2 en 2 (c y c+1)
            for (int c = 0; c < columnas - 1; c++) {
                int a1 = B[i][c];
                int b1 = B[k][c];
                int a2 = B[i][c+1];
                int b2 = B[k][c+1];

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
                if (B[i][c] < B[k][c]) {
                    iContieneK = false;
                }
                if (B[k][c] < B[i][c]) {
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
    cout << "\nMatriz basica:\n";
    for (int i = 0; i < filas; i++) {
        if (!basicas[i]) continue;
        for (int j = 0; j < columnas; j++) {
            if(B[i][j]==1){
            }
            cout << B[i][j] << " ";
        }
        cout << endl;
    }


    // Ejecutar BT
    Matrix tipicos = BT(B);

    cout << "\nTestores tipicos de B" << endl;

    for (const Row &t : tipicos) {
        print_row(t);                    
    }
    cout<<endl;

    
}



void print_row(const Row &r) {
    for (int x : r) cout << x << " ";
    cout << endl;
}

// Convierte un testor típico en conjunto {i,j,k}
vector<int> row_to_set(const Row &r) {
    vector<int> S;
    for (int j = 0; j < r.size(); j++) {
        if (r[j] == 1)
            S.push_back(j + 1);   // posición humana (1-based)
    }
    return S;
}

// Imprime el conjunto {i,j,k}
void print_set(const vector<int> &S) {
    cout << "{";
    for (int i = 0; i < S.size(); i++) {
        cout << S[i];
        if (i < S.size() - 1) cout << ",";
    }
    cout << "}";
}


uint64_t row_to_mask(const Row &r) {
    int n = r.size();
    uint64_t m = 0;

    for (int j = 0; j < n; ++j) {
        if (r[j] == 1) {
            int bit = n - 1 - j;       // col j  -> bit (n-1-j)
            m |= (1ULL << bit);
        }
    }
    return m;
}

Row mask_to_row(uint64_t m, int n) {
    Row r(n, 0);
    for (int j = 0; j < n; ++j) {
        int bit = n - 1 - j;           // bit (n-1-j) -> col j
        r[j] = ((m >> bit) & 1ULL) ? 1 : 0;
    }
    return r;
}

int last_one_index(const Row &r) {
    // Devuelve la posición del 1 más a la derecha en la fila r
    // Convención: columnas como j=0..n-1, col1 es j=0
    for (int j = (int)r.size() - 1; j >= 0; --j)
        if (r[j] == 1)
            return j;
    return -1; // No debería ocurrir si alpha es testor
}

bool contiene(const Row &sup, const Row &sub) {
    for (int j = 0; j < sub.size(); j++) {
        // si sub requiere un 1 en una posición donde sup tiene 0,
        // entonces sub NO está contenido en sup
        if (sub[j] == 1 && sup[j] == 0)
            return false;
    }
    return true;
}

bool esTipico(const Row &alpha, const vector<Row> &T) {
    for (const Row &t : T) {
        // si alpha contiene a un testor típico previo,
        // entonces alpha NO es típico
        if (contiene(alpha, t))
            return false;
    }
    return true;  // ningún t está contenido → sí es típico
}

Row salto_1_4(const Row &alpha) {
    int n = alpha.size();
    int last = last_one_index(alpha);   // índice 0-based

    int k = last + 1;                   // índice 1-based
    int exp = n - k;                    // exponente en la fórmula

    uint64_t step;
    if (exp <= 0) {
        // entonces 2^0 = 1 → salto mínimo de una posición
        step = 1ULL;
    } else {
        step = (1ULL << exp);          // 2^(n-k)
    }

    // Conversión α → mask
    uint64_t mask = row_to_mask(alpha);

    // Sumar el salto
    uint64_t next_mask = mask + step;

    // Si se sale del rango n bits → devolver vector nulo
    uint64_t limit = (1ULL << n);
    if (next_mask >= limit)
        return Row(n, 1);   // fin de búsqueda

    // Convertimos de vuelta a vector
    return mask_to_row(next_mask, n);
}

Row salto_1_5(const Row &alpha, const Matrix &MB, const vector<int> &offenders) {
    int n = alpha.size();
    
    // Paso 1: obtener b_i para cada fila fallada
    vector<int> b_positions;  
    for (int idx : offenders) {
        int b = last_one_index(MB[idx]);
        if (b != -1) b_positions.push_back(b);
    }

    // Paso 2: K = el más a la izquierda de esos rightmost (mínimo índice)
    int k = b_positions[0];
    for (int pos : b_positions)
        if (pos < k) k = pos;

    // Paso 3: construir α' de acuerdo a la regla de 1.5
    Row new_alpha(n);
    for (int j = 0; j < n; j++) {
        if (j < k)       new_alpha[j] = alpha[j];
        else if (j == k) new_alpha[j] = 1;
        else             new_alpha[j] = 0;
    }
    return new_alpha;
}


pair<bool, vector<int>> isTestor_and_offenders(const Row &alpha, const Matrix &MB) {
    int n = (int)alpha.size();
    vector<int> offenders;
    // recorrer cada fila r de MB y comprobar si alpha la cubre
    for (int i = 0; i < (int)MB.size(); ++i) {
        const Row &r = MB[i];
        bool covers = false;
        for (int j = 0; j < n; ++j) {
            if (alpha[j] == 1 && r[j] == 1) { // existe columna j que ambos tienen 1
                covers = true;
                break;
            }
        }
        if (!covers) {
            offenders.push_back(i); // guardamos índice (0-based) de fila no cubierta
        }
    }
    bool isTestor = offenders.empty();
    return {isTestor, offenders};
}

Matrix BT(const Matrix &MB) {
    int n = MB[0].size();          // número de columnas

    // α = 00...0 (primer n-úplo)
    Row alpha(n, 0);
    alpha[n-1]=1;

    // Último n-úplo = 11...1
    Row ultimo(n, 1);

    // Conjunto de testores típicos
    Matrix tipicos;

    // Mientras no lleguemos a 11...1
    while (true) {

        // *** Cambio necesario para evitar error de C++17 ***
        pair<bool, vector<int>> temp = isTestor_and_offenders(alpha, MB);
        bool esTestor = temp.first;
        vector<int> offenders = temp.second;
        // *****************************************************

        if (!esTestor) {
            alpha = salto_1_5(alpha, MB, offenders);

        } else {
            if (esTipico(alpha, tipicos)) {
                // α ES TÍPICO → guardarlo
                tipicos.push_back(alpha);

                alpha = salto_1_4(alpha);

            } else {
                // α es testor PERO NO es típico
                // avanzar α al siguiente n-úplo
                uint64_t mask = row_to_mask(alpha);
                mask += 1ULL;

                // convertir a row
                alpha = mask_to_row(mask, n);
            }
        }

        if (alpha == ultimo) break;
    }

    return tipicos;
}
