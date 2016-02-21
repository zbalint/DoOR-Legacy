#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "installer.h"
#include "config_loader.h"
#include "ramdisk.h"

void show_version();

void show_usage();

void show_help();

void shutdown();

void int_handler(int sig);

void bootstrap();

int main(int argc, char *argv[]) {
    atexit(shutdown);
    signal(SIGINT, int_handler);

//    mount_disk("/home/balint/MountTest", "32");
//    umount_disk("/home/balint/MountTest");

    if (argc > 1) {
        if ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "--version") == 0)) {
            show_version();
        } else if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0)) {
            show_help();
        } else if ((strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "--config") == 0)) {
            load_config();
            show_loaded_config();
        } else {
            if (is_installed() == 0) {
                if (strcmp(argv[1], "init") == 0) {
                    create_project_config();
                } else if (strcmp(argv[1], "start") == 0) {
                    bootstrap();
                } else if (strcmp(argv[1], "stop") == 0) {

                } else {
                    show_usage();
                }
            } else {
                if (strcmp(argv[1], "install") == 0) {
                    install_door();
                } else {
                    printf("DoOR does not installed on this system.\n");
                }
            }
        }
    } else {
        show_usage();
    }
    return 0;
}

void bootstrap() {
    load_config();
    printf("Bootstrapping project %s....\n", get_property("projectName"));
    mount_ramdisk();
    umount_ramdisk();
}

void shutdown() {
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
    printf("       door -c | --config      : print loaded config\n");
    printf("       door install            : install door\n");
    printf("       door init               : create project config in current directory\n");
    printf("       door start              : load project / current directory to the ram\n");
}

void show_help() {
    printf("SHOW HELP\n");
}
