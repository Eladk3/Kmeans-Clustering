import sys
import math


def euclidean_distance(point1, point2):
    """
    Calculates the Euclidean distance between two points represented as tuples of floats.
    """
    assert len(point1) == len(point2), "Points must have the same dimensionality"
    sum_squares = sum([(point1[i] - point2[i]) ** 2 for i in range(len(point1))])
    return math.sqrt(sum_squares)


def mean_point(points):
    """
    Calculates the mean point of a set of points represented as tuples of floats.
    """
    num_points = len(points)
    points = list(points)
    if num_points == 0:
        return None
    num_dimensions = len(points[0])
    sum_components = [0.0] * num_dimensions
    for point in points:
        for i in range(num_dimensions):
            sum_components[i] += point[i]
    mean_components = [sum_components[i] / num_points for i in range(num_dimensions)]
    return tuple(mean_components)


def all_less_than_epsilon(lst, epsilon):
    return all(num < epsilon for num in lst)


def main():
    try:
        # Check if the correct number of arguments were given
        if len(sys.argv) < 3 or len(sys.argv) > 4:
            print("An Error Has Occurred")
            sys.exit(1)

        input_data = sys.argv[-1]

        # Reading the text file and creating an array of arrays of the points
        with open(input_data, "r") as file:
            lines = file.readlines()
        arrays_of_points = []
        for line in lines:
            line = line.strip()
            values = [float(value) for value in line.split(",")]
            arrays_of_points.append(values)

        error = False

        # Check if the first argument is a natural number and valid
        num_clusters = sys.argv[1]
        if not num_clusters.isdigit() or not (1 < int(num_clusters) < len(arrays_of_points)):
            print("Invalid number of clusters!")
            error = True

        # Check if the second argument (if provided) is a natural number and valid
        max_iterations = sys.argv[2] if len(sys.argv) == 4 else "200"
        if not max_iterations.isdigit() or not (1 < int(max_iterations) < 1000):
            print("Invalid maximum iteration!")
            error = True

        if error:
            sys.exit(1)

        # The Algorithm

        centroids = {tuple(arrays_of_points[i]): set() for i in range(int(num_clusters))}

        iteration_count = 0
        epsilon = 0.001

        while True:
            # Clear the clusters before assigning points to them
            for center_key in centroids.keys():
                centroids[center_key].clear()

            temp_new_centers = []
            for point in arrays_of_points:
                min_d = float('inf')
                closest_center = ()
                for center_key in centroids.keys():
                    d = euclidean_distance(point, center_key)
                    if d < min_d:
                        min_d = d
                        closest_center = center_key
                centroids[closest_center].add(tuple(point))

            delta = []
            for center_key in centroids.keys():
                new_center = mean_point(centroids[center_key])
                delta.append(euclidean_distance(center_key, new_center))
                temp_new_centers.append(new_center)

            new_dict = {}
            i = 0
            for key in centroids.keys():
                new_dict[temp_new_centers[i]] = centroids[key]
                i += 1
            centroids = new_dict

            if (all_less_than_epsilon(delta, epsilon)) or iteration_count == int(max_iterations):
                break

            iteration_count += 1

        for key in centroids.keys():
            key_str = ""
            for value in key:
                key_str += "{:.4f},".format(value)
            key_str = key_str[:-1] + ""
            print(key_str)

    except ValueError:
        # Handle the case where user input is not a number
        print("An Error Has Occurred")

    except ZeroDivisionError:
        # Handle the case where user input is zero
        print("An Error Has Occurred")

    except Exception as e:
        # Handle any other unexpected errors
        print("An Error Has Occurred", e)
        sys.exit(1)  # Terminate the program with a non-zero exit code


if __name__ == "__main__":
    main()
