#include "installer.h"
#include "stdgc.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <string.h>

char *rootDirectory = NULL;
char *confDirectory = NULL;

void create_directories() {
    struct stat st = {0};
    struct passwd *pw = getpwuid(getuid());

    rootDirectory = (char *) malloc(sizeof(pw->pw_dir) + sizeof("/.door/"));
    strcat(rootDirectory, pw->pw_dir);
    strcat(rootDirectory, "/.door/");

    char *binDirectory = (char *) malloc(strlen(rootDirectory) + strlen("bin/") + 1);
    char *logsDirectory = (char *) malloc(strlen(rootDirectory) + strlen("logs/") + 1);
    char *toolsDirectory = (char *) malloc(strlen(rootDirectory) + strlen("tools/") + 1);
    char *mountDirectory = (char *) malloc(strlen(rootDirectory) + strlen("mount/") + 1);
    confDirectory = (char *) malloc(strlen(rootDirectory) + strlen("conf/") + 1);

    strcat(binDirectory, rootDirectory);
    strcat(binDirectory, "bin/");

    strcat(logsDirectory, rootDirectory);
    strcat(logsDirectory, "logs/");

    strcat(toolsDirectory, rootDirectory);
    strcat(toolsDirectory, "tools/");

    strcat(mountDirectory, rootDirectory);
    strcat(mountDirectory, "mount/");

    strcat(confDirectory, rootDirectory);
    strcat(confDirectory, "conf/");

    printf("Installing DoOR to %s\n", rootDirectory);

    if (stat(rootDirectory, &st) == -1) {
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
            printf("\nSomething went wrong.\n");
        }
    } else {
        printf("Directory already exist.\n");
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

int install_door() {
    create_directories();
    create_default_config();
}