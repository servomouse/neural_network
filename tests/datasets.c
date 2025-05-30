#include "datasets.h"

dataset_entry_t linear_dataset[] = {
    // Expected coeffs: -0.02, -0.01, 0.01, 0.02, 0.03
    {.inputs = {1.0, 0.0, 0.0, 0.0}, .output = 0.01},
    {.inputs = {0.0, 1.0, 0.0, 0.0}, .output = 0.02},
    {.inputs = {0.0, 0.0, 1.0, 0.0}, .output = 0.04},
    {.inputs = {0.0, 0.0, 0.0, 1.0}, .output = 0.05},
    {.inputs = {-1.0, 0.0, 0.0, 0.0}, .output = 0.05},
    {.inputs = {0.0, -1.0, 0.0, 0.0}, .output = 0.04},
    {.inputs = {0.0, 0.0, -1.0, 0.0}, .output = 0.02},
    {.inputs = {0.0, 0.0, 0.0, -1.0}, .output = 0.01},
    {.inputs = {0.0, 0.0, 0.0, 0.0}, .output = 0.03},
    {.inputs = {1.0, 1.0, 1.0, 1.0}, .output = 0.03},
    {.inputs = {-1.0, -1.0, -1.0, -1.0}, .output = 0.03},
    {.inputs = {-1.0, -1.0, 1.0, 1.0}, .output = 0.09},
    {.inputs = {1.0, 1.0, -1.0, -1.0}, .output = -0.03},
};

dataset_entry_t xor_dataset[] = {
    {.inputs = {0, 0, 0, 0}, .output = 0.0},
    {.inputs = {0, 0, 0, 1}, .output = 1.0},
    {.inputs = {0, 0, 1, 0}, .output = 1.0},
    {.inputs = {0, 0, 1, 1}, .output = 1.0},
    {.inputs = {0, 1, 0, 0}, .output = 1.0},
    {.inputs = {0, 1, 0, 1}, .output = 1.0},
    {.inputs = {0, 1, 1, 0}, .output = 1.0},
    {.inputs = {0, 1, 1, 1}, .output = 1.0},
    {.inputs = {1, 0, 0, 0}, .output = 1.0},
    {.inputs = {1, 0, 0, 1}, .output = 1.0},
    {.inputs = {1, 0, 1, 0}, .output = 1.0},
    {.inputs = {1, 0, 1, 1}, .output = 1.0},
    {.inputs = {1, 1, 0, 0}, .output = 1.0},
    {.inputs = {1, 1, 0, 1}, .output = 1.0},
    {.inputs = {1, 1, 1, 0}, .output = 1.0},
    {.inputs = {1, 1, 1, 1}, .output = 0.0},
};

dataset_entry_t polynome_dataset[] = {
    {.inputs = {0.01, 0.05, 0.02, 0.03}, .output = 0.0},
    {.inputs = {0.07, 0.02, 0.05, 0.01}, .output = 0.0},
    {.inputs = {0.01, 0.05, 0.06, 0.04}, .output = 0.0},
    {.inputs = {0.02, 0.04, 0.07, 0.05}, .output = 0.0},
    {.inputs = {0.03, 0.03, 0.08, 0.06}, .output = 0.0},
    {.inputs = {0.04, 0.02, 0.09, 0.07}, .output = 0.0},
    {.inputs = {0.05, 0.01, 0.08, 0.98}, .output = 1.0},
    {.inputs = {0.06, 0.08, 0.97, 0.07}, .output = 1.0},
    {.inputs = {0.07, 0.07, 0.96, 0.96}, .output = 1.0},
    {.inputs = {0.08, 0.91, 0.05, 0.05}, .output = 1.0},
    {.inputs = {0.09, 0.92, 0.04, 0.95}, .output = 1.0},
    {.inputs = {0.01, 0.93, 0.93, 0.04}, .output = 1.0},
    {.inputs = {0.02, 0.94, 0.92, 0.93}, .output = 1.0},
    {.inputs = {0.91, 0.06, 0.01, 0.02}, .output = 1.0},
    {.inputs = {0.92, 0.05, 0.02, 0.91}, .output = 1.0},
    {.inputs = {0.93, 0.04, 0.93, 0.02}, .output = 1.0},
    {.inputs = {0.94, 0.03, 0.94, 0.93}, .output = 1.0},
    {.inputs = {0.95, 0.94, 0.05, 0.04}, .output = 1.0},
    {.inputs = {0.96, 0.95, 0.06, 0.95}, .output = 1.0},
    {.inputs = {0.97, 0.96, 0.97, 0.06}, .output = 1.0},
    {.inputs = {0.98, 0.97, 0.98, 0.97}, .output = 0.0},
    {.inputs = {0.97, 0.98, 0.99, 0.98}, .output = 0.0},
    {.inputs = {0.96, 0.97, 0.98, 0.97}, .output = 0.0},
    {.inputs = {0.95, 0.96, 0.97, 0.96}, .output = 0.0},
    {.inputs = {0.94, 0.95, 0.96, 0.95}, .output = 0.0},
    {.inputs = {0.93, 0.94, 0.95, 0.94}, .output = 0.0},
    {.inputs = {0.92, 0.93, 0.94, 0.93}, .output = 0.0},
    {.inputs = {0.91, 0.92, 0.93, 0.92}, .output = 0.0},
    {.inputs = {0.90, 0.91, 0.92, 0.91}, .output = 0.0},
};

u_dataset_entry_t micronet_dataset[] = {
    {.inputs = {-1.0, 1.0, -1.0, 1.0},   .output = {1.0}},
    {.inputs = {-1.0, 1.0, 0.9, 0.8},    .output = {-0.3732}},
    {.inputs = {-0.9, -0.6, 0.9, 1.0},   .output = {0.1148}},
    {.inputs = {0.9, 0.8, -1.0, 0.7},    .output = {-0.128}},
    {.inputs = {0.9, -0.8, 1.0, -0.9},   .output = {0.2721}},
    {.inputs = {-1.0, -0.8, -0.7, 1.0},  .output = {-0.1756}},
    {.inputs = {0.7, -0.8, 1.0, 1.0},    .output = {-0.1756}},
    {.inputs = {0.9, -0.8, 1.0, 0.9},    .output = {-0.2721}},
    {.inputs = {-1.0, -0.8, 1.0, -0.7},  .output = {-0.1756}},
    {.inputs = {-1.0, -1.0, 0.9, 1.0},   .output = {0.729}},
    {.inputs = {-0.8, -0.9, -1.0, -0.9}, .output = {0.2721}},
    {.inputs = {-0.9, 0.8, 1.0, 0.8},    .output = {-0.1911}},
    {.inputs = {-0.9, 0.9, -0.8, -1},    .output = {-0.2721}},
    {.inputs = {1.0, 1.0, 0.9, -0.9},    .output = {-0.5314}},
    {.inputs = {1.0, 1.0, -0.6, 0.8},    .output = {-0.1106}},
    {.inputs = {0.8, 0.9, -1.0, 0.7},    .output = {-0.128}},
    {.inputs = {1.0, 0.8, -0.8, -0.9},   .output = {0.1911}},
    {.inputs = {-0.8, 0.9, 1.0, -0.9},   .output = {0.2721}},
    {.inputs = {-1.0, -1.0, -1.0, -0.6}, .output = {0.216}},
    {.inputs = {-0.6, -1.0, -1.0, 1.0},  .output = {-0.216}},
    {.inputs = {1.0, 0.6, -0.9, -0.9},   .output = {0.1148}},
    {.inputs = {-1.0, 0.8, -0.9, 0.7},   .output = {0.128}},
    {.inputs = {0.8, 0.8, 1.0, 0.9},     .output = {0.1911}},
    {.inputs = {1.0, 1.0, 1.0, 0.5},     .output = {0.125}},
    {.inputs = {0.8, -0.7, -1.0, 0.9},   .output = {0.128}},
    {.inputs = {0.9, -1.0, -0.8, 0.9},   .output = {0.2721}},
    {.inputs = {-0.7, 0.9, 1.0, 0.8},    .output = {-0.128}},
    {.inputs = {-0.7, -0.8, 1.0, 1.0},   .output = {0.1756}},
    {.inputs = {-1.0, -1.0, -0.8, -0.9}, .output = {0.3732}},
    {.inputs = {-1.0, -1.0, -0.9, 0.8},  .output = {-0.3732}},
    {.inputs = {1.0, -1.0, -0.8, 0.7},   .output = {0.1756}},
    {.inputs = {-0.9, -0.6, -1.0, 0.9},  .output = {-0.1148}},
    {.inputs = {-1.0, 1.0, 0.9, -0.7},   .output = {0.25}},
    {.inputs = {-1.0, -1.0, 0.6, -0.9},  .output = {-0.1575}},
    {.inputs = {1.0, -1.0, 0.9, -1},     .output = {0.729}},
    {.inputs = {-0.9, 0.7, -1.0, -0.9},  .output = {-0.1823}},
    {.inputs = {-0.8, -1.0, 0.8, 0.8},   .output = {0.1342}},
    {.inputs = {0.7, -1.0, 1.0, 1.0},    .output = {-0.343}},
    {.inputs = {0.9, -1.0, 1.0, 1.0},    .output = {-0.729}},
    {.inputs = {-0.9, 0.6, -1.0, 1.0},   .output = {0.1575}},
    {.inputs = {-0.9, 0.8, -0.9, -1},    .output = {-0.2721}},
    {.inputs = {-0.6, 0.8, -1.0, -1},    .output = {-0.1106}},
    {.inputs = {-0.9, 0.8, 0.9, 0.8},    .output = {-0.1393}},
    {.inputs = {-0.9, -0.8, 1.0, 0.9},   .output = {0.2721}},
    {.inputs = {1.0, 0.7, 0.7, 1.0},     .output = {0.1176}},
    {.inputs = {0.9, 0.7, -0.9, 0.9},    .output = {-0.1329}},
    {.inputs = {-0.9, 1.0, 0.9, 1.0},    .output = {-0.5314}},
    {.inputs = {0.9, -0.9, -0.6, 1.0},   .output = {0.1148}},
    {.inputs = {0.6, 0.9, 1.0, -1},      .output = {-0.1575}},
    {.inputs = {0.7, 1.0, -1.0, 0.8},    .output = {-0.1756}},
    {.inputs = {0.7, 0.7, 1.0, -1},      .output = {-0.1176}},
    {.inputs = {-1.0, 0.9, -1.0, -0.7},  .output = {-0.25}},
    {.inputs = {0.8, 1.0, 0.9, -1},      .output = {-0.3732}},
    {.inputs = {0.7, 1.0, 0.8, -0.9},    .output = {-0.128}},
    {.inputs = {0.8, -1.0, 0.9, -1},     .output = {0.3732}},
    {.inputs = {1.0, -1.0, 0.7, 0.8},    .output = {-0.1756}},
};