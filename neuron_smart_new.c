#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "utils.h"
#include "neuron.h"

double act_table[] = {
    1.0000, 0.9905, 0.9628, 0.9194, 0.8638, 0.8000, 0.7317, 0.6621, 0.5937, 0.5282,
    0.4667, 0.4098, 0.3576, 0.3102, 0.2673, 0.2286, 0.1937, 0.1623, 0.1341, 0.1086,
    0.0857, 0.0650, 0.0463, 0.0293, 0.0140, -0.0000, -0.0128, -0.0244, -0.0350, -0.0448,
    -0.0537, -0.0619, -0.0695, -0.0765, -0.0830, -0.0890, -0.0945, -0.0997, -0.1045, -0.1090,
    -0.1132, -0.1171, -0.1207, -0.1242, -0.1274, -0.1304, -0.1332, -0.1359, -0.1384, -0.1408,
    -0.1431, -0.1452, -0.1472, -0.1491, -0.1510, -0.1527, -0.1543, -0.1559, -0.1574, -0.1588,
    -0.1601, -0.1614, -0.1626, -0.1638, -0.1649, -0.1660, -0.1670, -0.1680, -0.1690, -0.1699,
    -0.1708, -0.1716, -0.1724, -0.1732, -0.1740, -0.1747, -0.1754, -0.1761, -0.1767, -0.1773,
    -0.1779, -0.1785, -0.1791, -0.1797, -0.1802, -0.1807, -0.1812, -0.1817, -0.1822, -0.1826,
    -0.1831, -0.1835, -0.1840, -0.1844, -0.1848, -0.1852, -0.1856, -0.1859, -0.1863, -0.1866,
    -0.1870, -0.1873, -0.1877, -0.1880, -0.1883, -0.1886, -0.1889, -0.1892, -0.1895, -0.1898,
    -0.1901, -0.1904, -0.1907, -0.1909, -0.1912, -0.1915, -0.1917, -0.1920, -0.1922, -0.1925,
    -0.1927, -0.1930, -0.1932, -0.1934, -0.1937, -0.1939, -0.1941, -0.1943, -0.1945, -0.1948,
    -0.1950, -0.1952, -0.1954, -0.1956, -0.1958, -0.1960, -0.1962, -0.1964, -0.1966, -0.1968,
    -0.1970, -0.1971, -0.1973, -0.1975, -0.1977, -0.1979, -0.1980, -0.1982, -0.1984, -0.1986,
    -0.1987, -0.1989, -0.1991, -0.1992, -0.1994, -0.1995, -0.1997, -0.1998, -0.2000, -0.2001,
    -0.2003, -0.2004, -0.2006, -0.2007, -0.2008, -0.2010, -0.2011, -0.2012, -0.2014, -0.2015,
    -0.2016, -0.2017, -0.2018, -0.2019, -0.2020, -0.2021, -0.2022, -0.2023, -0.2024, -0.2025,
    -0.2026, -0.2027, -0.2028, -0.2029, -0.2029, -0.2030, -0.2031, -0.2031, -0.2032, -0.2032,
    -0.2033, -0.2033, -0.2034, -0.2034, -0.2034, -0.2035, -0.2035, -0.2035, -0.2035, -0.2035,
    -0.2036, -0.2036
};

double get_act_value(double offset, double amplitude, double sum) {
    sum += offset;
    if(sum < 0) {
        sum = 0 - sum;
    }
    double temp_sum = sum * 100;
    double xa = floor(temp_sum);
    double xb = ceil(temp_sum);
    uint8_t bot = (uint8_t)xa;
    uint8_t top = (uint8_t)xb;
    if(top == bot) {
        return act_table[bot];
    }
    double ya = act_table[bot];
    double yb = act_table[top];
    double res = (((yb - ya) * (temp_sum - xa)) / (xb - xa)) + ya;
    return res * amplitude;
}


double activation_func(double sum) {
    if(sum > 1.0) {
        return 1.0;
    } else if(sum < -1.0) {
        return -1.0;
    } else {
        return sum;
    }
}

double control_coeffs_func(double coeff) {
    if(coeff > 1.0) {
        return 1.0;
    } else if(coeff < -1.0) {
        return -1.0;
    } else {
        return coeff;
    }
}

void neuron_init(neuron_params_t * n_params, uint32_t num_inputs) {
    // srand(time(NULL));   // Should be called by kapellmeister
    n_params->num_inputs = num_inputs;
    n_params->num_coeffs = (1 << num_inputs);
    if(n_params->inputs) {
        free(n_params->inputs);
    }
    n_params->inputs = calloc(num_inputs, sizeof(double));
    if(n_params->indices) {
        free(n_params->indices);
    }
    n_params->indices = calloc(num_inputs, sizeof(uint32_t));
    if(n_params->coeffs) {
        free(n_params->coeffs);
    }
    n_params->coeffs = calloc(n_params->num_coeffs, 2 * sizeof(double));
    for(int i=0; i<n_params->num_coeffs; i++) {
        n_params->coeffs[i].amplitude = random_double(-0.01, 0.01);
        n_params->coeffs[i].offset = random_double(-0.01, 0.01);
    }
}

void neuron_set_input_idx(neuron_params_t * n_params, uint32_t input_number, uint32_t input_idx) {
    printf("Setting input %d index to %d\n", input_number, input_idx);
    if(input_number < n_params->num_inputs) {
        n_params->indices[input_number] = input_idx;
    } else {
        printf("ERROR: index out of range: input_number = %d, network size = %d\n", input_number, n_params->num_inputs);
    }
}

double neuron_get_output(neuron_params_t * n_params, double *inputs) {
    for(size_t i=0; i<n_params->num_inputs; i++) {
        n_params->inputs[i] = inputs[i];
    }

    double result = get_act_value(n_params->coeffs[0].offset, n_params->coeffs[0].amplitude, 0);    // BIAS
    for(size_t i=1; i<n_params->num_coeffs; i++) {
        double temp = 1.0;
        for(size_t j=0; j<n_params->num_inputs; j++) {
            if(((1 << j) & i) > 0) {
                temp *= inputs[j];
            }
        }
        double output = get_act_value(n_params->coeffs[i].offset, n_params->coeffs[i].amplitude, temp);
        if(isnan(output)) {
            printf("temp is NAN!!!: offset = %f, amplitude = %f, temp = %f, output = %f\n", n_params->coeffs[i].offset, n_params->coeffs[i].amplitude, temp, output);
        }
        if(isnan(result + output)) {
            printf("Result + output is NAN:: result = %f, output = %f", result, output);
        }
        result += output;
        if(isnan(result)) {
            printf("Result is NAN:: result = %f, output = %f", result, output);
        }
    }
    result = activation_func(result);
    if(isnan(activation_func(result))) {
        printf("Result for %f is NAN!!:\n", result);
        for(size_t i=0; i<n_params->num_inputs; i++) {
            printf("inputs[%lld] = %f\n", i, n_params->inputs[i]);
        }
    }
    return result;
}

void neuron_set_feedback_error(neuron_params_t * n_params, double error) {
    n_params->feedback_error += error;
    n_params->feedback_error_count += 1;
}

void neuron_set_global_error(neuron_params_t * n_params, double error) {
    n_params->global_error = error;
}

void neuron_mutate(neuron_params_t * n_params) {
    double mutation_step = 0.01;
    n_params->last_idx = random_int(0, n_params->num_coeffs);
    n_params->last_value.amplitude = n_params->coeffs[n_params->last_idx].amplitude;
    n_params->last_value.offset = n_params->coeffs[n_params->last_idx].offset;
    n_params->coeffs[n_params->last_idx].amplitude = control_coeffs_func(
        n_params->coeffs[n_params->last_idx].amplitude + random_double(-1 * mutation_step, mutation_step));
    n_params->coeffs[n_params->last_idx].offset = control_coeffs_func(
        n_params->coeffs[n_params->last_idx].offset + random_double(-1 * mutation_step, mutation_step));
}

void neuron_restore(neuron_params_t * n_params) {
    n_params->coeffs[n_params->last_idx].amplitude = n_params->last_value.amplitude;
    n_params->coeffs[n_params->last_idx].offset = n_params->last_value.offset;
}

void neuron_save_state(neuron_params_t * n_params, char *filename) {
    char *inputs_arr_fname = concat_strings("inputs_", filename);
    char *indices_arr_fname = concat_strings("indices_", filename);
    char *coeffs_arr_fname = concat_strings("coeffs_", filename);
    store_data(&n_params, sizeof(neuron_params_t), filename);
    store_data(n_params->inputs, n_params->num_inputs * sizeof(double), inputs_arr_fname);
    store_data(n_params->indices, n_params->num_inputs * sizeof(uint32_t), indices_arr_fname);
    store_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double) * 2, coeffs_arr_fname);
    free(inputs_arr_fname);
    free(indices_arr_fname);
    free(coeffs_arr_fname);
}

void neuron_restore_state(neuron_params_t * n_params, char *filename) {
    char *inputs_arr_fname = concat_strings("inputs_", filename);
    char *indices_arr_fname = concat_strings("indices_", filename);
    char *coeffs_arr_fname = concat_strings("coeffs_", filename);
    restore_data(&n_params, sizeof(neuron_params_t), filename);
    restore_data(n_params->inputs, n_params->num_inputs * sizeof(double), inputs_arr_fname);
    restore_data(n_params->indices, n_params->num_inputs * sizeof(uint32_t), indices_arr_fname);
    restore_data(n_params->coeffs, (n_params->num_coeffs) * sizeof(double) * 2, coeffs_arr_fname);
    free(inputs_arr_fname);
    free(indices_arr_fname);
    free(coeffs_arr_fname);
}

void neuron_print_coeffs(neuron_params_t * n_params) {
    for(uint32_t i=0; i<n_params->num_coeffs; i++) {
        printf("%f, %f; ", n_params->coeffs[i].amplitude, n_params->coeffs[i].offset);
    }
    printf("\n");
}
