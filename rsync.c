#include <stdlib.h>
#include <string.h>
#include "rsync.h"
#include "stdgc.h"
#include "installer.h"
#include "config_loader.h"
#include "ramdisk.h"

char *get_log_file_name() {
    char *projectName = get_property("projectName");
    char *filename = malloc(strlen(projectName) + strlen(".log") + 1);
    filename[0] = '\0';
    strcpy(filename, projectName);
    strcat(filename, ".log");
    return filename;
}

char *get_sync_command(char *syncParams, char *source, char *destination, char *logsDirectory) {
    char *syncCommand = malloc(
            strlen("rsync ") +
            strlen(syncParams) +
            strlen(" ") +
            strlen(source) +
            strlen(" ") +
            strlen(destination) +
            strlen(" > ") +
            strlen(logsDirectory) +
            strlen(get_log_file_name()) +
            1
    );
    syncCommand[0] = '\0';
    strcpy(syncCommand, "rsync ");
    strcat(syncCommand, syncParams);
    strcat(syncCommand, " ");
    strcat(syncCommand, source);
    strcat(syncCommand, " ");
    strcat(syncCommand, destination);
    strcat(syncCommand, " > ");
    strcat(syncCommand, logsDirectory);
    strcat(syncCommand, get_log_file_name());

    return syncCommand;
}

int run_sync(char *projectName, char *syncParams, int invert) {
    char *logDirectory = get_logs_directory();
    char *source = get_current_directory();

    char *destination = malloc(strlen(get_mount_directory()) + strlen(projectName) + strlen("/") + 1);
    destination[0] = '\0';
    strcpy(destination, get_mount_directory());
    strcat(destination, projectName);
    strcat(destination, "/");

    char *syncCommand;
    if (invert == 0) {
        syncCommand = get_sync_command(syncParams, source, destination, logDirectory);
    } else {
        syncCommand = get_sync_command(syncParams, destination, source, logDirectory);
    }
    if (is_mounted(destination) == 0) {
        return system(syncCommand);
    } else {
        return 1;
    }
}

int sync_directory(char *projectName) {
    return run_sync(projectName, "-av --delete --recursive --force", 0);
}

int start_sync(char *projectName) {
    return run_sync(projectName, "-av", 0);
}

int stop_sync(char *projectName) {
    return run_sync(projectName, "-av --delete --recursive --force", 1);
}

