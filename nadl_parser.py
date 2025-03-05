import sys


symbols = [
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'g', 'k',
    'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
    'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_'
]

digits = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']

line_number = 0


def parse_comment(string):
    for i in range(len(string)):
        if string[i] == '\n':
            return string[i:]


def parse_name(string):
    global line_number
    item_name = ''
    for i in range(len(string)):
        if (string[i] in symbols) or (string[i] in digits):
            item_name += string[i]
        elif string[i] == ':':
            return item_name, string[i:]
        else:
            raise Exception(f"Error: Invalid symbol: {string[i]} at line {line_number}!")
            sys.exit(1)


def parse_integer(string):
    value = ''
    for i in range(len(string)):
        if string[i] in digits:
            value += string[i]
        else:
            return int(value), string[i:]


def parse_array(string):
    global line_number
    items = []
    last_integer = False
    while len(string) > 0:
        if string[0] in digits:
            val, string = parse_integer(string)
            items.append(val)
            last_integer = True
        elif string[0] == ':':
            string = string[1:]
            pass    # process range
        elif string[0] == 'x':
            if string[1] != ' ':
                raise Exception(f"Error: variable cannot start with 'x' (line {line_number})!")
            string = string[1:]
        elif string[0] in symbols:
            pass    # process var name
        elif string[0] == '[':
            val, string = parse_array(string[1:])
            items.append(val)
        elif string[0] in [' ', ',']:
            string = string[1:]
        elif string[0] == '\n':
            line_number += 1
            string = string[1:]
        elif string[0] == '#':
            string = parse_comment(string[1:])
            line_number += 1
        else:
            raise Exception(f"Error: invalid symbol: {string[0]} at line {line_number}")


def parse_item(string):
    global line_number
    items = {}
    while len(string) > 0:
        if string[0] in [' ', '\n']:
            string = string[1:]
        elif string[0] == '#':
            string = parse_comment(string[1:])
            line_number += 1
        else:
            item_name, string = parse_name(string)
            items[item_name] = parse_array(string)
            return items


def parse(filename):
    with open(filename) as f:
        data = f.read()
    config = parse_item(data)
    print(config)


if __name__ == "__main__":
    filename = sys.argv[1] if len(sys.argv) > 1 else "nadl_example.nad"
    parse(filename)
