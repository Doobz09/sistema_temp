// Función para calcular la distancia euclidiana entre dos puntos

#include "Funciones_knn.h"

double euclidean_distance(Point a, Point b, int n) {
    if(n==3)
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)+ pow(a.z - b.z, 2));
    
    else if(n==2)
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    
    else if(n==1)
        return sqrt(pow(a.x - b.x, 2));

    return 0;
        
}

// Función para encontrar los K vecinos más cercanos
void find_k_nearest_neighbors(Point dataset[], Point test_point, int k, int neighbors[],int n) {
    double distances[NUM_POINTS];
    int i, j;

    // Calcular las distancias entre el punto de prueba y todos los puntos en el dataset
    for (i = 0; i < NUM_POINTS; i++) {
        distances[i] = euclidean_distance(dataset[i], test_point,n);
    }

    // Ordenar los puntos por distancia usando una selección simple de k vecinos
    for (i = 0; i < k; i++) {
        double min_distance = 1e9;
        int min_index = -1;
        for (j = 0; j < NUM_POINTS; j++) {
            if (distances[j] < min_distance) {
                min_distance = distances[j];
                min_index = j;
            }
        }
        neighbors[i] = dataset[min_index].label;  // Almacenar la etiqueta del vecino
        distances[min_index] = 1e9;               // Excluir el punto ya seleccionado
    }
}

int classify(Point dataset[], Point test_point, int k,int n) {
    int neighbors[K];
    int i, label0 = 0, label1 = 0,label2 = 0;

    // Encontrar los k vecinos más cercanos
    find_k_nearest_neighbors(dataset, test_point, k, neighbors,n);

    // Contar las etiquetas de los vecinos
    for (i = 0; i < k; i++) {
        if (neighbors[i] == 1) label0++;
        else if (neighbors[i] == 2) label1++;
        else if (neighbors[i] == 3) label2++;
    }

	    // Devolver la etiqueta mayoritaria
	if (label0 > label1 && label0 > label2) {
	    return 0;
	} else if (label1 > label0 && label1 > label2) {
	    return 1;
	} else {
	    return 2;
	}
}