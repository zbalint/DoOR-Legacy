#ifndef DOOR_MODULE_LOADER_H
#define DOOR_MODULE_LOADER_H

void add_module(char *moduleName, char *modulePath, char *moduleEnv, char *moduleEnvValue, char *moduleSize);

int load_module_config(char *moduleName);

int load_modules();

int unload_modules();

#endif //DOOR_MODULE_LOADER_H
