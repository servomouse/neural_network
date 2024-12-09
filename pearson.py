# Example code for calculation of the Pearson correlation coefficient


def get_correlation(array1, array2):
    mean1 = sum(array1) / len(array1)
    mean2 = sum(array2) / len(array2)

    deviations1 = [x - mean1 for x in array1]
    deviations2 = [y - mean2 for y in array2]

    products = [deviations1[i] * deviations2[i] for i in range(len(array1))]

    numerator = sum(products)
    squared_deviations1 = [d ** 2 for d in deviations1]
    squared_deviations2 = [d ** 2 for d in deviations2]
    sum_squared_deviations1 = sum(squared_deviations1)
    sum_squared_deviations2 = sum(squared_deviations2)

    denominator = (sum_squared_deviations1 * sum_squared_deviations2) ** 0.5

    if denominator == 0:
        return 0
    else:
        return numerator / denominator


def get_max_correlation(array1, array2):
    max_range = int(0.75 * len(array1))
    max_correlation_mod = 0
    max_correlation_val = 0
    for i in range(15):
        arr1 = array1[:len(array1)-i]
        arr2 = array2[i:]
        c = get_correlation(arr1, arr2)
        mod = c * c
        if mod > max_correlation_mod:
            max_correlation_mod = mod
            max_correlation_val = c
    return max_correlation_val

if __name__ == "__main__":
    # Example arrays
    array1 = [0.1, 0.1, 0.1, 0.2, 0.1, 0.1, 0.1]
    array2 = [0.3, 0.3, 0.4, 0.5, 0.4, 0.3, 0.3]
    print(f"The Pearson correlation coefficient is {get_correlation(array1, array2)}")
