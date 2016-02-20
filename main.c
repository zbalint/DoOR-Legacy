#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "installer.h"

void show_version();

void show_usage();

void show_help();

void shutdown();

void int_handler(int sig);

int main(int argc, char *argv[]) {
    atexit(shutdown);

    signal(SIGINT, int_handler);

    if (argc > 1) {
        if ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "--version") == 0)) {
            show_version();
        } else if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0)) {
            show_help();
        } else if (strcmp(argv[1], "install") == 0) {
            install_door();
        } else {
            show_usage();
        }
    } else {
        show_usage();
    }

    return 0;
}

void shutdown() {
    printf("Hello\n");
}

void int_handler(int sig) {
    exit(0);
}

void show_version() {
    printf("Version: %s.%s.%s\n", VERSION, COMMIT_NUMBER, GIT_SHA1);
}

void show_usage() {
    printf("Usage: door -v | --version     : print version\n");
    printf("       door -h | --help        : print help\n");
}

void show_help() {
    printf("SHOW HELP\n");
}
