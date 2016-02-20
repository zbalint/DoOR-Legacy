//
// Created by balint on 2016.02.19..
//

#include <stdio.h>
#include <string.h>
#include "stdgc.h"
#include "config_loader.h"
#include "installer.h"

typedef struct ConfigLine {
    char *key;
    char *value;
} ConfigLine;

typedef struct ConfigNode {
    struct ConfigNode *prev;
    ConfigLine *configLine;
    struct ConfigNode *next;
} ConfigNode;

static ConfigNode *head = NULL;
static ConfigNode *tail = NULL;

void add_config_node(ConfigLine *line) {
    ConfigNode *newNode = malloc(sizeof(ConfigNode));
    newNode->prev = NULL;
    newNode->configLine = line;
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

ConfigLine *new_config_line(char *line) {
    ConfigLine *newLine = (ConfigLine *) malloc(sizeof(ConfigLine));
    int i = 0, ki = 0, vi = 0;

    char key[20];
    char value[20];
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
    newLine->key = malloc(strlen(key));
    newLine->value = malloc(strlen(value));
    strcpy(newLine->key, key);
    strcpy(newLine->value, value);

    return newLine;
}

ConfigLine *get_config_line_by_key(char *key) {
    ConfigNode *iterator = head;
    for (iterator; iterator != NULL; iterator = iterator->next) {
        if (strcmp(iterator->configLine->key, key) == 0) {
            return iterator->configLine;
        }
    }
    return NULL;
}

int load_config() {
    char *configDirectory = get_config_directory();
    if (configDirectory == NULL) {
        printf("Error\n");
        return 0;
    }
    char *configFilePath = malloc(strlen(configDirectory) + strlen("default.conf") + 1);
    char *line = NULL;
    size_t lineLength = 0;
    ssize_t read = 0;

    if (configFilePath == NULL) {
        return 1;
    }

    configFilePath[0] = '\0';
    strcat(configFilePath, configDirectory);
    strcat(configFilePath, "default.conf");

    printf("Loading default configuration... ");
    FILE *configFile = fopen(configFilePath, "r");
    if (configFile != NULL) {
        while ((read = getline(&line, &lineLength, configFile)) != -1) {
            ConfigLine *configLine = new_config_line(line);
            add_config_node(configLine);
        }
        fclose(configFile);
        printf("done\n");
    } else {
        printf("error\n");
    }

    printf("Loading project configuration... ");
    char *projectConfigFile = malloc(strlen(get_current_directory()) + strlen("/DoORConfig.conf") + 1);
    strcpy(projectConfigFile, get_current_directory());
    strcat(projectConfigFile, "/DoORConfig.conf");
    projectConfigFile[strlen(projectConfigFile)] = '\0';

    configFile = fopen(projectConfigFile, "r");
    if (configFile != NULL) {
        while ((read = getline(&line, &lineLength, configFile)) != -1) {
            ConfigLine *tempLine = new_config_line(line);
            ConfigLine *currentLine = get_config_line_by_key(tempLine->key);

            if (currentLine != NULL) {
                if (strcmp(tempLine->value, "default") != 0)
                    currentLine->value = tempLine->value;
            } else {
                ConfigLine *configLine = new_config_line(line);
                add_config_node(configLine);
            }
        }
        fclose(configFile);
        printf("done\n");
    } else {
        printf("error\n");
    }
}

void showLoadedConfig() {
    ConfigNode *iterator = head;
    printf("\n********************LOADED CONFIG********************\n");
    for (iterator; iterator != NULL; iterator = iterator->next) {
        printf("%s = %s\n", iterator->configLine->key, iterator->configLine->value);
    }
    printf("********************LOADED CONFIG********************\n");
}