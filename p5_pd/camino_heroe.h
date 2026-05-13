#ifndef CAMINO_HEROE_H
#define CAMINO_HEROE_H

#include <vector>
#include <limits>

using namespace std;

// Valor que representa "no hay camino" o "infinito".
// Se divide entre 4 para evitar overflow al sumar tres valores en la
// recurrencia (D[i][k] + G[k] + D[k][j]).
constexpr int INF = numeric_limits<int>::max() / 4;

/**
 * Calcula la matriz de costes minimos entre cada par de localizaciones,
 * teniendo en cuenta los costes G(k) de los nodos intermedios.
 *
 * @param C  Matriz n x n con los costes de los caminos directos.
 *           C[i][j] = INF si no hay camino directo de i a j.
 *           C[i][i] = 0 para todo i.
 * @param G  Vector de tamaño n con el coste de cada nodo
 *           (solo se aplica cuando el nodo actua como intermedio).
 * @param D  Matriz n x n de salida con los costes minimos.
 *           D[i][j] = INF si no se puede llegar de i a j.
 * @param P  Matriz n x n de salida con los predecesores, para reconstruir rutas.
 *           P[i][j] = -1 si el camino optimo de i a j es directo (o no existe).
 *           P[i][j] = k si el camino optimo de i a j pasa por k como intermedio.
 */
void el_camino_del_heroe(const vector<vector<int>>& C,
                         const vector<int>& G,
                         vector<vector<int>>& D,
                         vector<vector<int>>& P);

/**
 * Reconstruye la ruta optima desde el nodo origen hasta el nodo destino,
 * usando la matriz de predecesores calculada por el_camino_del_heroe.
 *
 * @param P        Matriz de predecesores.
 * @param origen   Nodo de origen (indice 0..n-1).
 * @param destino  Nodo de destino (indice 0..n-1).
 * @return         Vector con la secuencia de nodos de la ruta,
 *                 incluyendo origen y destino. Vector vacio si no hay ruta.
 */
vector<int> recuperar_ruta(const vector<vector<int>>& P,
                                int origen,
                                int destino);

#endif // CAMINO_HEROE_H
