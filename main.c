#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "installer.h"
#include "config_loader.h"
#include "ramdisk.h"
#include "rsync.h"
#include "stdgc.h"

static int started = 0;

void show_version();

void show_usage();

void show_help();

void shutdown();

void int_handler(int sig);

void sync_loop();

void bootstrap();

int main(int argc, char *argv[]) {
    atexit(shutdown);
    signal(SIGINT, int_handler);

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
                    if (strcmp(get_property("autoSave"), "true") == 0) {
                        sync_loop();
                    }
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

void sync_loop() {
    while (1) {
        int ret = sync_directory(get_property("projectName"));
        if (WIFSIGNALED(ret) &&
            (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT))
            break;
        sleep((u_int) atol(get_property("syncInterval")));
    }
}

void bootstrap() {
    if (is_root_user() == 0) {
        load_config();
        mount_ramdisk();
        start_sync(get_property("projectName"));
        started = 1;
    } else {
        printf("You should run 'door start' as root\n");
        return;
    }
}

void shutdown() {
    if (started != 0 && is_root_user() == 0) {
        stop_sync(get_property("projectName"));
        umount_ramdisk();
        started = 0;
    }

    free_on_exit();
}

void int_handler(int sig) {
    printf("\n");
    shutdown();
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
