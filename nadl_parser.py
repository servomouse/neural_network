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


def prepare_data(data):
    config = []
    counter = 0
    for l in data.split("\n"):
        temp = l.strip()
        if temp.startswith("#") or len(temp) == 0:
            counter += 1
            continue
        elif '#' in temp:
            temp = temp.split("#")[0].strip()
        config.append({"line_number": counter, "data": temp})
        counter += 1
    return config


def parse_module_name(data):
    m_name = data[0]["data"].split("module")[1].strip()
    m_name = m_name.split(' ')
    syntax_error_line = f"Error: invalid syntax at line {data[0]['line_number']}: \"{data[0]['data']}\"\n" +\
                        "\tCorrect syntax looks like this: \"module: modulename\""
    if len(m_name) > 2:
        raise Exception(syntax_error_line)
        sys.exit(1)
    elif len(m_name) == 2:
        if m_name[0] == ':':
            if m_name[1][0] in symbols:
                return m_name[1], data[1:]
            else:
                raise Exception(f"Error: Invalid module name: {m_name[1]};\n"
                                f"\tModule name should start with a symbol, but it starts with {m_name[1][0]}")
    elif len(m_name) == 1:
        if m_name[0][0] == ':':
            if len(m_name[0]) == 1:
                raise Exception(syntax_error_line)
            if m_name[0][1] in symbols:
                return m_name[0][1:], data[1:]
            else:
                raise Exception(f"Error: Invalid module name: {m_name[0][1:]};\n"
                                f"\tModule name should start with a symbol, but it starts with {m_name[0][1]}")
        else:
            raise Exception(syntax_error_line)
    else:
        raise Exception(syntax_error_line)


def parse_inputs(data):
    if "inputs" not in data[0]['data']:
        raise Exception(f"Invalid syntax at line {data[0]['line_number']}: {data[0]['data']}")
    temp = data[0]['data'].split('inputs')
    if len(temp[0]) > 0:
        raise Exception(f"Invalid syntax at line {data[0]['line_number']}: {data[0]['data']}")
    temp = temp[1].strip()
    temp = temp.split(':')
    if len(temp[0]) > 0:
        raise Exception(f"Invalid syntax at line {data[0]['line_number']}: {data[0]['data']}")
    temp = temp[1].strip()
    print(temp)
    sys.exit(0)


def parse_module(data):
    module_name = None
    while len(data) > 0:
        if "module" in data[0]['data']:
            module_name, data = parse_module_name(data)
            print(f"{module_name = }")
            inputs, data = parse_inputs(data)
        elif "inputs" in data[0]['data']:
            inputs, data = parse_inputs(data)
        else:
            raise Exception(f"Invalid syntax: at line {data[0]['line_number']}: {data[0]['data']}\n"
                            "Start config with \"module: modulename\" or \"inputs: [\"")


def parse_modules(data):
    modules = {}
    mod_name, mod_config, data = parse_module(data)


def parse(filename):
    with open(filename) as f:
        data = f.read()
    # config = parse_item(data)
    data = prepare_data(data)
    modules = parse_modules(data)
    # for l in data:
    #     print(l)


if __name__ == "__main__":
    filename = sys.argv[1] if len(sys.argv) > 1 else "nadl_example.nad"
    parse(filename)
