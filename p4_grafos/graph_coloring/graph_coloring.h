#ifndef GRAPH_COLORING_H
#define GRAPH_COLORING_H

#include <string>
#include <vector>
using namespace std;

struct Graph {
    int num_vertices;
    vector<vector<int>> adj;
    vector<int> optimal_coloring; // Optional: can be empty if not provided in input file
};

struct ColoringResult {
    bool success = false;
    vector<int> colors;
    int colors_used = 0;
    int nodes_generated = 0;
    int nodes_pruned = 0;
    int max_live_nodes = 0;
};

/**
 * @brief Analiza un archivo de grafo y construye una estructura Graph.
 * @param filepath Ruta al archivo de grafo.
 * @return Estructura Graph con vertices y lista de adyacencia.
 */
Graph parse_graph_file(const string& filepath);

/**
 * @brief Comprueba si asignar un color a un vértice es válido (ningún vértice adyacente tiene el mismo color).
 * @param vertex El vértice a colorear.
 * @param color El color a asignar.
 * @param colors Asignaciones de colores actuales.
 * @param graph La estructura del grafo.
 * @return true si la asignación de color es válida, false en caso contrario.
 */
bool is_color_valid(
    int vertex,
    int color,
    const vector<int>& colors,
    const Graph& graph
);

/**
 * @brief Cuenta el número de colores únicos utilizados en el coloreo.
 * @param colors Vector de asignaciones de colores.
 * @return Número de colores distintos utilizados.
 */
int count_used_colors(const vector<int>& colors);

/**
 * @brief Realiza un coloreo voraz del grafo.
 * @param graph El grafo a colorear.
 * @return Vector de asignaciones de colores (uno por vértice).
 */
vector<int> greedy_coloring(const Graph& graph);

/**
 * @brief Intenta colorear el grafo usando backtracking con un máximo de max_colors.
 * @param graph El grafo a colorear.
 * @param max_colors Número máximo de colores permitidos.
 * @return ColoringResult con éxito, colores y estadísticas.
 */
ColoringResult backtracking_color_with_k(const Graph& graph, int max_colors);

/**
 * @brief Encuentra el número mínimo de colores necesarios para colorear el grafo usando branch and bound.
 * @param graph El grafo a colorear.
 * @return ColoringResult con coloreo óptimo y estadísticas.
 */
ColoringResult branch_and_bound_min_colors(const Graph& graph);

/**
 * @brief Imprime la solución de coloreo en la consola.
 * @param result El ColoringResult a imprimir.
 * @param caption Título para la sección de salida.
 */
void print_coloring_solution(const ColoringResult& result, const string& caption);

/**
 * @brief Extrae el nombre de instancia de una ruta de archivo.
 * @param path La ruta del archivo.
 * @return El nombre base del archivo sin extensión.
 */
string get_instance_name(const string& path);

/**
 * @brief Escribe un archivo DOT (Graphviz) con el grafo y un coloreo.
 * @param graph El grafo a dibujar.
 * @param colors Vector de colores por vertice.
 * @param filepath Ruta del archivo DOT de salida.
 * @param title Titulo opcional para el grafo.
 * @return true si el archivo se escribe correctamente.
 */
bool write_coloring_dot(
    const Graph& graph,
    const vector<int>& colors,
    const string& filepath,
    const string& title
);

#endif // GRAPH_COLORING_H

