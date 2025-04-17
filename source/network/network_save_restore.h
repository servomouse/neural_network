#pragma once

#include "network.h"

void network_save(network_t * config, char *path);
void network_restore(network_t * config, char *path);
void remove_folders(char *path);
