# Kmeans Clustering in C and Python
### This repository contains the implementation of the K-means clustering algorithm in both C and Python.

## Introduction
The K-means algorithm is a popular clustering method used for partitioning a set of N unlabeled observations into K distinct clusters. Here, every datapoint is assigned to exactly one cluster. This assignment provides an implementation of the K-means algorithm as detailed below.

## Algorithm
1. Initialize centroids as the first k datapoints: µk = xk for all k ∈ K
2. Repeat the following steps:
- Assign every xi to the closest cluster k: argmin_k d(xi, µk), for all 1 ≤ k ≤ K
- Update the centroids: µk = 1/|k| * Σ(xi∈k) xi
- Continue until convergence: either when the difference between the updated centroid and the previous one ∆µk < 0.001 or when the iteration number reaches the specified maximum.
Euclidean distance d(p, q) is used as the distance metric.

## Requirements
Input Variables:

- K: Number of clusters (1 < K < N). If invalid, the program will output "Invalid number of clusters!"
- iter: Maximum number of iterations (1 < iter < 1000). If not provided, default is 200. If invalid, the program will output "Invalid maximum iteration!"
- input_data: The data should be provided in a .txt file. It's assumed to be valid.
The program will apply K-means on the input and print the final centroids with a precision of 4 decimal places.

## Compilation and Execution
## C
Compile the program:
```
$gcc -ansi -Wall -Wextra -Werror -pedantic-errors kmeans.c -o kmeans
```
Execute:
```
$./kmeans 3 100 <input_data.txt
```
Where 3 is the number of clusters and 100 is the max iterations. The centroids will be printed to the console.

## Python
Execute:
```
$python3 kmeans.py 3 100 input_data.txt
```
Where 3 is the number of clusters and 100 is the max iterations. The centroids will be printed to the console.

## Assumptions
- Validation is done for the command line arguments.
- Outputs are formatted to 4 decimal places.
- In case of an invalid input, the program will either print the relevant error message or "An Error Has Occurred" before terminating.
- Memory management and freeing allocated memory is ensured for the C implementation.
- For successful execution, the C program will return 0, otherwise 1.
- All provided datapoints are assumed to be unique.
- Only specified includes (in C) or modules (in Python) are used.
- Vector elements use double in C and float in Python.
