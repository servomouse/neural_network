#pragma once

#include "network.h"

void network_save(micro_network_t * config, char *path);
void network_restore(micro_network_t * config, char *path);
void remove_folders(char *path);
