#pragma once

#include "micronet.h"

void micronet_save(micro_network_t * config, char *path);
void micronet_restore(micro_network_t * config, char *path);
void remove_folders(char *path);
