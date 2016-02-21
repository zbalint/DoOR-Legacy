//
// Created by balint on 2016.02.19..
//

#ifndef DOOR_CONFIG_LOADER_H
#define DOOR_CONFIG_LOADER_H

int load_config();

void show_loaded_config();

char *get_property(char *key);

int get_auto_save();

int is_root_user();

#endif //DOOR_CONFIG_LOADER_H
