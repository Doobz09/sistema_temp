#ifndef _FUNCIONES_KNN_H_
#define _FUNCIONES_KNN_H_
#include <math.h>
#define K 3
#define NUM_POINTS 3000 
typedef struct {
    double x, y, z;  // Coordenadas del punto
    int label;    // Clase del punto (0 o 1)
} Point;





double euclidean_distance(Point a, Point b, int n);
void find_k_nearest_neighbors(Point dataset[], Point test_point, int k, int neighbors[],int n);
int classify(Point dataset[], Point test_point, int k,int n);
#endif