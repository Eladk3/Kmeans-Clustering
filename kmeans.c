#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct node {
    double* coordinates;
    int dimensions;
    struct node* next;
} Node;

typedef struct centroid {
    double* coordinates;
    int dimensions;
} Centroid;


long K;


void free_all_memory(Node* head, Centroid* centroids, int* assignments, double* old_centroids) {
    Node* temp;
    int i;

    /* Free memory for the linked list */
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->coordinates);
        free(temp);
    }

    /* Free memory for the centroids */
    if(centroids != NULL) {
        for(i = 0; i < K; i++) {
            free(centroids[i].coordinates);
        }
        free(centroids);
    }

    /* Free memory for assignments and old_centroids */
    if(assignments != NULL) {
        free(assignments);
    }

    if(old_centroids != NULL) {
        free(old_centroids);
    }
}

Node* add_node(Node* head, Node** tail, const double* coordinates, int dimensions) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    int i;

    if(new_node == NULL) {
        printf("An Error Has Occurred\n");
        free_all_memory(head, NULL, NULL, NULL);
        exit(1);
    }
    new_node->coordinates = (double*)malloc(dimensions * sizeof(double));
    if(new_node->coordinates == NULL) {
        printf("An Error Has Occurred\n");
        free(new_node);
        free_all_memory(head, NULL, NULL, NULL);
        exit(1);
    }

    for(i = 0; i < dimensions; i++) {
        new_node->coordinates[i] = coordinates[i];
    }
    new_node->dimensions = dimensions;
    new_node->next = NULL;

    /* If the Linked List is empty, then make the new node as head and tail */
    if (head == NULL) {
        head = new_node;
        *tail = new_node;
    } else {
        /* Else add new node at the end and update tail */
        (*tail)->next = new_node;
    }
    *tail = new_node; /* Update the tail to new node */

    return head;
}


Node* read_data(int* num_points, int* dimensions, Node** tail) {
    Node* data = NULL;
    double value;
    double coordinates[100]; /* Assuming a maximum of 100 dimensions */
    int dim = 0;
    char c;

    while(scanf("%lf", &value) != EOF) {
        coordinates[dim++] = value;
        scanf("%c", &c); /* Read the next character */
        if(c == '\n' || c == EOF) { /* End of line or end of file */
            data = add_node(data, tail, coordinates, dim);
            if(*dimensions == 0) {
                *dimensions = dim;
            }
            (*num_points)++;
            dim = 0;
        }
    }
    return data;
}


double distance(Node* point, Centroid* centroid) {
    double sum = 0.0;
    int i;
    for(i = 0; i < point->dimensions; i++) {
        double diff = point->coordinates[i] - centroid->coordinates[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

int nearest_centroid(Node* point, Centroid* centroids) {
    int nearest = 0;
    double min_distance = distance(point, &centroids[0]);
    int i;
    for(i = 1; i < K; i++) {
        double dist = distance(point, &centroids[i]);
        if(dist < min_distance) {
            min_distance = dist;
            nearest = i;
        }
    }
    return nearest;
}

void calculate_centroids(Node* data, Centroid* centroids, int* assignments, int num_points, double* old_centroids) {
    int* counts = (int*)calloc(K, sizeof(int));
    int i, j;
    Node* point = data;

    if(counts == NULL) {
        printf("An Error Has Occurred\n");
        free_all_memory(data, centroids, assignments, old_centroids);
        exit(1);
    }

    /** Reset centroid coordinates */
    for (i = 0; i < K; i++) {
        for (j = 0; j < centroids[i].dimensions; j++) {
            centroids[i].coordinates[j] = 0.0;
        }
    }

    /** Update centroid coordinates */
    for (i = 0; i < num_points; i++) {
        int index = assignments[i];
        counts[index]++;
        for (j = 0; j < point->dimensions; j++) {
            centroids[index].coordinates[j] += point->coordinates[j];
        }
        point = point->next;
    }

    /** Calculate centroid means */
    for (i = 0; i < K; i++) {
        if (counts[i] > 0) {
            for (j = 0; j < centroids[i].dimensions; j++) {
                centroids[i].coordinates[j] /= counts[i];
            }
        }
    }

    free(counts);
}

void k_means(Node* data, int max_iterations, int num_points, double epsilon) {
    Centroid* centroids = (Centroid*)malloc(K * sizeof(Centroid));
    Node* point = data;
    int dims = point->dimensions; /** Save the dimensions into a variable */
    double* old_centroids;
    double max_shift;
    int* assignments;
    int iteration, i, j;

    if(centroids == NULL) {
        printf("An Error Has Occurred\n");
        free_all_memory(data, NULL, NULL, NULL);
        exit(1);
    }

    /** Initialize centroids using the first K nodes from the linked list */
    for(i = 0; i < K; i++) {
        centroids[i].coordinates = (double*)malloc(dims * sizeof(double));
        if(centroids[i].coordinates == NULL) {
            printf("An Error Has Occurred\n");
            free_all_memory(data, centroids, NULL, NULL);
            exit(1);
        }
        for(j = 0; j < dims; j++) {
            centroids[i].coordinates[j] = point->coordinates[j];
        }
        centroids[i].dimensions = dims;
        if(point->next != NULL) { /* move to the next point if it is not the end of the list */
            point = point->next;
        }
    }

    old_centroids = (double*)malloc(K * dims * sizeof(double));
    assignments = (int*)malloc(num_points * sizeof(int));

    if(old_centroids == NULL || assignments == NULL) {
        printf("An Error Has Occurred\n");
        free_all_memory(data, centroids, assignments, old_centroids);
        exit(1);
    }

    for(iteration = 0; iteration < max_iterations; iteration++) {
        /** Keep track of the old centroids */
        for(i = 0; i < K; i++) {
            for(j = 0; j < dims; j++) {
                old_centroids[i * dims + j] = centroids[i].coordinates[j];
            }
        }

        point = data;
        for(i = 0; i < num_points; i++) {
            assignments[i] = nearest_centroid(point, centroids);
            point = point->next;
        }

        calculate_centroids(data, centroids, assignments, num_points, old_centroids);

        /** Calculate the maximum centroid shift */
        max_shift = 0.0;
        for(i = 0; i < K; i++) {
            double shift = 0.0;
            for(j = 0; j < dims; j++) {
                double diff = old_centroids[i * dims + j] - centroids[i].coordinates[j];
                shift += diff * diff;
            }
            shift = sqrt(shift);  /** Take square root to get Euclidean distance */
            if(shift > max_shift) {
                max_shift = shift;
            }
        }

        /** Check convergence */
        if(max_shift < epsilon) {
            break;
        }
    }

    for(i = 0; i < K; i++) {
        for(j = 0; j < centroids[i].dimensions; j++) {
            if (j > 0) printf(",");
            printf("%.4f", centroids[i].coordinates[j]);
        }
        printf("\n");
    }

    for(i = 0; i < K; i++) {
        free(centroids[i].coordinates);
    }
    free(centroids);
    free(assignments);
    free(old_centroids);
}


int main(int argc, char* argv[]) {
    char* end; /** Pointer for error checking */
    int error = 0;
    int num_points = 0;
    int dimensions = 0;
    long max_iterations = 200; /** Default value for max_iterations */
    Node* data;
    double epsilon = 0.001;
    Node* temp;
    Node* tail = NULL;

    if(argc < 2) {
        printf("An Error Has Occurred\n");
        return 1;
    }

    if(argc > 3) {
        printf("An Error Has Occurred\n");
        return 1;
    }

    if(argc == 3) {
        end = NULL;  /** Reset end pointer */
        max_iterations = strtol(argv[2], &end, 10);
        if(*end != '\0' || max_iterations <= 1 || max_iterations >= 1000) {
            printf("Invalid maximum iteration!\n");
            error = 1;
        }
    }

    end = NULL;  /** Reset end pointer */
    K = strtol(argv[1], &end, 10);

    if (K > 2147483647 || max_iterations > 2147483647) {
        printf("An Error Has Occurred\n");
        error = 1;
    }

    data = read_data(&num_points, &dimensions, &tail);

    if(*end != '\0' || K <= 1 || K >= num_points) {
        printf("Invalid number of clusters!\n");
        free_all_memory(data, NULL, NULL, NULL);
        return 1;
    }

    if (error) {
        free_all_memory(data, NULL, NULL, NULL);
        return 1;
    }

    k_means(data, (int)max_iterations, num_points, epsilon);

    /** Free the memory allocated for the linked list */
    while(data != NULL) {
        temp = data;
        data = data->next;
        free(temp->coordinates);
        free(temp);
    }

    return 0;
}

