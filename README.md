# ALG-teaching

En este repositorio se encuentran algunos materiales para las prácticas de la asignatura Algorítmica de la Universidad de Granada. 

Se recomienda crear un entorno de Anaconda para instalar las dependencias necesarias para compilar y ejecutar el código. A continuación se muestran los comandos necesarios para crear un entorno con el compilador de C++ y gnuplot para visualizar los resultados.

```bash
# Create a new environment named 'cpp_env' and install the compiler, gnuplot, and graphviz
conda create -n cpp_env -c conda-forge cxx-compiler gnuplot graphviz

# Activate the environment
conda activate cpp_env
```

## P1 - Eficiencia (`p1_eficiencia`)

La Práctica 1 consiste en aprender a analizar la eficiencia de un algoritmo. Se incluye un fichero "benchmark_bubblesort.cpp" que implementa el algoritmo de ordenación Bubble Sort y mide su tiempo de ejecución para diferentes tamaños de entrada. También se incluye un fichero "plot_results.sh" que utiliza gnuplot para visualizar los resultados obtenidos.

Ejemplo:

```bash
g++ benchmark_bubblesort.cpp -o benchmark_bubblesort
./benchmark_bubblesort > results.txt
./plot_results.sh
```

## P3 - Greedy (`p3_greedy`)

Implementaciones greedy para el problema del viajante de comercio (TSP):

- Herísticas `nearest_neighbor` y `nearest_insertion`
- En `data/` se encuentran ficheros `.tsp` con instancias TSPLIB.

Ejemplo:

```bash
g++ main.cpp nearest_neighbor.cpp nearest_insertion.cpp data_loader.cpp plot_tour.cpp -o tsp_greedy
./tsp_greedy data/berlin52.tsp
```

## P4 - Algoritmos para la Exploración de Grafos (`p4_grafos`)

Se ofrecen dos variantes de la Práctica 4. Una está basada en el problema de Coloreado de Grafos, material realizado por María Isabel Abellán Benítez (miab@ugr.es). 
La otra variante está basada en el problema del Viajante de Comercio (material realizado por el autor de este repositorio).

Estructura:

- `graph_coloring/`: backtracking y branch & bound para coloreado de grafos (con instancias en `data/` y salidas en `plots/`).
- `tsp/`: backtracking y branch & bound para TSP (con instancias en `data/` y salidas en `plots/`).

Ejemplo:

```bash
# Coloreado
g++ graph_coloring/main.cpp graph_coloring/graph_coloring.cpp -o graph_coloring_demo
./graph_coloring_demo graph_coloring/data/graph1.txt

# TSP
g++ tsp/main.cpp tsp/nearest_neighbor.cpp tsp/branch_and_bound_simple.cpp tsp/branch_and_bound_mst.cpp tsp/data_loader.cpp tsp/plot_tour.cpp -o tsp_bb
./tsp_bb tsp/data/example10.tsp
```

## P5 - Programación Dinámica (`p5_pd`)

En la Práctica 5 se propone resolver el problema "El Viaje del Héroe", material realizado por Salvador López Joya (slopezjoya@ugr.es).

Ejemplo:

```bash
make
./camino_heroe ejemplo1.txt
```
