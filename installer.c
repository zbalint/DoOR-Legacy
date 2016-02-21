#include "installer.h"
#include "stdgc.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

char *rootDirectory = NULL;
char *confDirectory = NULL;
char *currentDirectory = NULL;

char *logsDirectory = NULL;
char *mountDirectory = NULL;

char *get_root_directory() {
    if (rootDirectory == NULL) {
        char *home = getenv("HOME");
        rootDirectory = (char *) malloc(sizeof(home) + sizeof("/.door/"));
        rootDirectory[0] = '\0';
        strcat(rootDirectory, home);
        strcat(rootDirectory, "/.door/");
    }

    return rootDirectory;
}

char *get_config_directory() {

    if (confDirectory == NULL) {
        confDirectory = (char *) malloc(strlen(get_root_directory()) + strlen("conf/") + 1);
        confDirectory[0] = '\0';
        strcat(confDirectory, get_root_directory());
        strcat(confDirectory, "conf/");
    }
    return confDirectory;
}

char *get_current_directory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        cwd[strlen(cwd)] = '/';
        cwd[strlen(cwd)] = '\0';

        if (currentDirectory == NULL) {
            currentDirectory = malloc(strlen(cwd));
            strcpy(currentDirectory, cwd);
        }
    }

    return currentDirectory;
}

char *get_logs_directory() {
    if (logsDirectory == NULL) {
        logsDirectory = (char *) malloc(strlen(get_root_directory()) + strlen("logs/") + 1);
        logsDirectory[0] = '\0';
        strcat(logsDirectory, get_root_directory());
        strcat(logsDirectory, "logs/");
    }

    return logsDirectory;
}

char *get_mount_directory() {
    if (mountDirectory == NULL) {
        mountDirectory = (char *) malloc(strlen(get_root_directory()) + strlen("mount/") + 1);
        mountDirectory[0] = '\0';
        strcat(mountDirectory, get_root_directory());
        strcat(mountDirectory, "mount/");;
    }
    return mountDirectory;
}

int is_installed() {
    DIR *rootDir = opendir(get_root_directory());
    if (rootDir) {
        return 0;
    }
    return 1;
}

void create_directories() {
    rootDirectory = get_root_directory();

    char *binDirectory = (char *) malloc(strlen(rootDirectory) + strlen("bin/") + 1);
    logsDirectory = get_logs_directory();
    char *toolsDirectory = (char *) malloc(strlen(rootDirectory) + strlen("tools/") + 1);
    mountDirectory = get_mount_directory();
    confDirectory = get_config_directory();

    binDirectory[0] = '\0';
    strcat(binDirectory, rootDirectory);
    strcat(binDirectory, "bin/");

    toolsDirectory[0] = '\0';
    strcat(toolsDirectory, rootDirectory);
    strcat(toolsDirectory, "tools/");

    printf("Installing DoOR to %s\n", rootDirectory);

    printf("Creating \"%s\" directory... ", rootDirectory);
    if (mkdir(rootDirectory, 0755) == 0) {
        printf("done\n");

        printf("Creating \"%s\" directory... ", binDirectory);
        if (mkdir(binDirectory, 0755) == 0)
            printf("done\n");
        else
            printf("error\n");

        printf("Creating \"%s\" directory... ", logsDirectory);
        if (mkdir(logsDirectory, 0755) == 0)
            printf("done\n");
        else
            printf("error\n");

        printf("Creating \"%s\" directory... ", toolsDirectory);
        if (mkdir(toolsDirectory, 0755) == 0)
            printf("done\n");
        else
            printf("error\n");

        printf("Creating \"%s\" directory... ", mountDirectory);
        if (mkdir(mountDirectory, 0755) == 0)
            printf("done\n");
        else
            printf("error\n");

        printf("Creating \"%s\" directory... ", confDirectory);
        if (mkdir(confDirectory, 0755) == 0)
            printf("done\n");
        else
            printf("error\n");

    } else {
        printf("\nAlready installed on this system.\n");
    }
}

void create_default_config() {
    if (rootDirectory != NULL && confDirectory != NULL) {
        printf("Creating default configuration file... ");
        char *configFilePath = malloc(strlen(confDirectory) + strlen("default.conf") + 1);
        strcat(configFilePath, confDirectory);
        strcat(configFilePath, "default.conf");
        FILE *file = fopen(configFilePath, "w");
        if (file != NULL) {
            fprintf(file, "%s=%s\n", "autoLoadModules", "false");
            fprintf(file, "%s=%s\n", "autoSave", "true");
            fprintf(file, "%s=%s\n", "syncInterval", "30");
            fprintf(file, "%s=%s\n", "diskSize", "256");
            fclose(file);
            printf("done\n");
        } else {
            printf("error\n");
        }
    }
}

void create_project_config() {
    char cwd[1024];
    char projectName[64];
    printf("Creating project configuration file... ");
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        cwd[strlen(cwd)] = '\0';

        int i = 0, j = 0;
        int lastSlash = 0;
        for (i = 0; cwd[i] != '\0'; i++)
            if (cwd[i] == '/')
                lastSlash = i;

        for (i = 0; cwd[i] != '\0'; i++) {
            if (i > lastSlash) {
                projectName[j++] = cwd[i];
            }
        }
        projectName[j] = '\0';

        strcat(cwd, "/DoORConfig.conf\0");
        FILE *configFile = fopen(cwd, "w");
        if (configFile != NULL) {
            fprintf(configFile, "%s=%s\n", "projectName", projectName);
            fprintf(configFile, "%s=%s\n", "autoSave", "default");
            fprintf(configFile, "%s=%s\n", "syncInterval", "default");
            fprintf(configFile, "%s=%s\n", "diskSize", "default");
            fprintf(configFile, "%s=%s\n", "modules", "default");
            printf("done\n");
        } else {
            printf("error\n");
        }
    } else {
        printf("error\n");
    }
}

void install_door() {
    create_directories();
    create_default_config();
}