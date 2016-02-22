#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "module_loader.h"
#include "stdgc.h"
#include "installer.h"
#include "ramdisk.h"
#include "rsync.h"

typedef struct ModuleNode {
    struct ModuleNode *prev;
    char *moduleName;
    char *modulePath;
    char *moduleEnv;
    char *moduleEnvValue;
    char *moduleSize;
    struct ModuleNode *next;
} ModuleNode;

static ModuleNode *head = NULL;
static ModuleNode *tail = NULL;

void add_module(char *moduleName, char *modulePath, char *moduleEnv, char *moduleEnvValue, char *moduleSize) {
    ModuleNode *newNode = malloc(sizeof(ModuleNode));
    newNode->prev = NULL;
    newNode->moduleName = moduleName;
    newNode->modulePath = modulePath;
    newNode->moduleEnv = moduleEnv;
    newNode->moduleEnvValue = moduleEnvValue;
    newNode->moduleSize = moduleSize;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
        tail = newNode;
    } else {
        head->prev = newNode;
        newNode->next = head;
        head = newNode;
    }
}

void parse_config_line(char *line, char *key, char *value) {
    int i = 0, ki = 0, vi = 0;

    int part = 0;

    for (i = 0; line[i] != '\n'; i++) {
        if (line[i] == '=') {
            part = 1;
        }
        if (part == 0) {
            key[ki++] = line[i];
        } else if (line[i] != '=') {
            value[vi++] = line[i];
        }
    }
    key[ki] = '\0';
    value[vi] = '\0';
}

int load_module_config(char *moduleName) {
    char *moduleFilePath = malloc(strlen(get_config_directory()) + strlen(moduleName) + strlen(".conf") + 1);
    moduleFilePath[0] = '\0';
    strcpy(moduleFilePath, get_config_directory());
    strcat(moduleFilePath, moduleName);
    strcat(moduleFilePath, ".conf");

    char *line = NULL;
    size_t lineLength = 0;

    char *modulePath = NULL;
    char *moduleEnv = NULL;
    char *moduleEnvValue = NULL;
    char *moduleSize = NULL;

    FILE *configFile = fopen(moduleFilePath, "r");
    if (configFile != NULL) {
        while (getline(&line, &lineLength, configFile) != -1) {
            char *key = malloc(strlen(line));
            char *value = malloc(strlen(line));
            parse_config_line(line, key, value);
            if (strcmp(key, "path") == 0) {
                modulePath = value;
            } else if (strcmp(key, "env") == 0) {
                moduleEnv = value;
            } else if (strcmp(key, "value") == 0) {
                moduleEnvValue = value;
            } else if (strcmp(key, "size") == 0) {
                moduleSize = value;
            } else {
                return 1;
            }
            free(key);
        }
        fclose(configFile);
        if (modulePath != NULL && moduleEnv != NULL && moduleEnvValue != NULL && moduleSize != NULL) {
            add_module(moduleName, modulePath, moduleEnv, moduleEnvValue, moduleSize);
        } else {
            return 2;
        }
        return 0;
    } else {
        return 1;
    }
}

char *get_forked_bash_command() {
    ModuleNode *iterator = head;

    if (head == NULL) {
        return NULL;
    }

    char command[2048];
    command[0] = '\0';
    char pathString[1024];
    pathString[0] = '\0';
    strcpy(pathString, "PATH=$PATH");
    for (iterator; iterator != NULL; iterator = iterator->next) {
        strcat(command, iterator->moduleEnv);
        strcat(command, "=");
        strcat(command, iterator->moduleEnvValue);
        strcat(command, " ");
        strcat(pathString, ":");
        strcat(pathString, get_mount_point(iterator->moduleName));
        strcat(pathString, "bin");
    }


    strcat(command, pathString);

    strcat(command, " bash --norc");

    char *bashCommand = malloc(sizeof(command));
    bashCommand[0] = '\0';
    strcpy(bashCommand, command);
    return bashCommand;
}

int load_module(ModuleNode *moduleNode) {
    if (mount_ramdisk(moduleNode->moduleName, moduleNode->moduleSize) != 0) {
        return 1;
    }
    sync_module(moduleNode->moduleName, moduleNode->modulePath);

    char *envValue;

    if (strcmp(moduleNode->moduleEnvValue, "default") == 0) {
        envValue = get_mount_point(moduleNode->moduleName);
    } else {
        envValue = moduleNode->moduleEnvValue;
    }

    moduleNode->moduleEnvValue = envValue;

    return 0;
}

int unload_module(ModuleNode *moduleNode) {
    return umount_ramdisk(moduleNode->moduleName);
}

int load_modules() {
    ModuleNode *iterator = head;

    if (head == NULL) {
        return 1;
    }

    for (iterator; iterator != NULL; iterator = iterator->next) {
        if (load_module(iterator) != 0) {
            printf("Error while loading %s\n...", iterator->moduleName);
        }
    }

    return 0;
}

int unload_modules() {
    ModuleNode *iterator = head;

    if (head == NULL) {
        return 1;
    }

    for (iterator; iterator != NULL; iterator = iterator->next) {
        unload_module(iterator);
    }

    return 0;
}

