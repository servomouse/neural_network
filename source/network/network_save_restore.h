#pragma once

#include "network.h"

void network_save(network_t * config, char *path);
void network_restore(network_t * config, char *path, uint8_t is_micronet);
void remove_folders(char *path);
