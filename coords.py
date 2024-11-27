

def append_equals(arr, i):
    print(f"append_equals({i})")
    arr.append([i, i, i])
    arr.append([i, i, -i])
    arr.append([i, -i, i])
    arr.append([i, -i, -i])
    arr.append([-i, i, i])
    arr.append([-i, i, -i])
    arr.append([-i, -i, i])
    arr.append([-i, -i, -i])


def append_two_different(arr, i, j):
    """ [i, i, j] """
    print(f"append_two_different({i}, {j})")
    arr.append([i, i, j])
    arr.append([i, j, i])
    arr.append([j, i, i])
    if i > 0:
        arr.append([i, -i, j])
        arr.append([-i, i, j])
        arr.append([-i, -i, j])

        arr.append([i, j, -i])
        arr.append([-i, j, i])
        arr.append([-i, j, -i])

        arr.append([j, i, -i])
        arr.append([j, -i, i])
        arr.append([j, -i, -i])
    if j > 0:
        arr.append([i, i, -j])
        arr.append([i, -j, i])
        arr.append([-j, i, i])
        if i > 0:
            arr.append([i, -i, -j])
            arr.append([-i, i, -j])
            arr.append([-i, -i, -j])

            arr.append([i, -j, -i])
            arr.append([-i, -j, i])
            arr.append([-i, -j, -i])

            arr.append([-j, i, -i])
            arr.append([-j, -i, i])
            arr.append([-j, -i, -i])


def append_all_different(arr, i, j, k):
    print(f"append_all_different({i}, {j}, {k})")
    arr.append([i, j, k])
    if k != 0:
        arr.append([i, j, -k])
    if j != 0:
        arr.append([i, -j, k])
        if k != 0:
            arr.append([i, -j, -k])
    if i != 0:
        arr.append([-i, j, k])
        if k != 0:
            arr.append([-i, j, -k])
        if j != 0:
            arr.append([-i, -j, k])
            if k != 0:
                arr.append([-i, -j, -k])


def get_coords(radius):
    cells = []
    # for i in range(radius+1):
    i = radius
    for j in range(i+1):
        for k in range(j, i+1):
            if i == j == k:
                append_equals(cells, i)
            elif i == j:
                append_two_different(cells, i, k)
            elif i == k:
                append_two_different(cells, i, j)
            elif j == k:
                append_two_different(cells, j, i)
            else:
                print(i, j, k)
                append_all_different(cells, i, j, k)
                append_all_different(cells, i, k, j)
                append_all_different(cells, k, j, i)
                append_all_different(cells, k, i, j)
                append_all_different(cells, j, i, k)
                append_all_different(cells, j, k, i)

    return cells


if __name__ == "__main__":
    c = get_coords(3)
    counter = 0
    for cell in c:
        print(counter, cell)
        counter += 1
