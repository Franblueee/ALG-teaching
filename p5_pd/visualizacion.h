#ifndef VISUALIZACION_H
#define VISUALIZACION_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * Muestra una representacion ASCII del mapa de la aventura por el stream
 * indicado: lista de localizaciones con sus costes G, y lista de caminos
 * directos con sus costes C.
 */
void mostrar_mapa_ascii(ostream& out,
                        const vector<vector<int>>& C,
                        const vector<int>& G);

/**
 * Genera un archivo SVG con un mapa ilustrado de la aventura:
 *   - Cada localizacion se dibuja con un icono distinto (castillo, taberna,
 *     bosque, cripta, mazmorra, templo, pueblo, montana, lago o torre),
 *     asignado segun su indice.
 *   - El guardian de cada localizacion se dibuja con un icono que depende
 *     del coste G(k): slime (verde) para G bajo, goblin (naranja) para G
 *     medio, dragon (rojo) para G alto.
 *   - Los caminos se trazan como curvas con flechas y etiquetas de coste.
 *   - Si `ruta` no esta vacia, las aristas y nodos de esa ruta se resaltan
 *     en color dorado, y se anade un cartel con el resumen de la ruta y
 *     su coste total.
 *
 * Abrible directamente en cualquier navegador, o convertible a PNG con:
 *     convert mapa.svg mapa.png         # ImageMagick
 *     inkscape mapa.svg -o mapa.png     # Inkscape
 *
 * @param fichero      Ruta del archivo .svg a crear.
 * @param C            Matriz de costes directos.
 * @param G            Vector de costes de los nodos.
 * @param ruta         Secuencia de nodos a destacar (opcional). Indices 0..n-1.
 * @param coste_total  Coste total de la ruta (solo se muestra si ruta no es vacia).
 * @return true si se pudo escribir el archivo, false en caso contrario.
 */
bool generar_svg(const string& fichero,
                 const vector<vector<int>>& C,
                 const vector<int>& G,
                 const vector<int>& ruta = {},
                 int coste_total = -1);

#endif // VISUALIZACION_H
