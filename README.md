# ALG-teaching

En este repositorio se encuentran algunos materiales para las prácticas de la asignatura Algorítmica de la Universidad de Granada. 

Se recomienda crear un entorno de Anaconda para instalar las dependencias necesarias para compilar y ejecutar el código. A continuación se muestran los comandos necesarios para crear un entorno con el compilador de C++ y gnuplot para visualizar los resultados.

```bash
# Create a new environment named 'cpp_env' and install the compiler, gnuplot, and graphviz
conda create -n cpp_env -c conda-forge cxx-compiler gnuplot graphviz

# Activate the environment
conda activate cpp_env
```

## P1 - Eficiencia

La Práctica 1 consiste en aprender a analizar la eficiencia de un algoritmo. Se incluye un fichero "benchmark_bubblesort.cpp" que implementa el algoritmo de ordenación Bubble Sort y mide su tiempo de ejecución para diferentes tamaños de entrada. También se incluye un fichero "plot_results.sh" que utiliza gnuplot para visualizar los resultados obtenidos.


