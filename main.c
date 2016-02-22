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
#include "module_loader.h"

static int started = 0;

void show_version();

void show_usage();

void show_help();

void tear_down();

void shutdown();

void int_handler(int sig);

void sync_loop();

int bootstrap();

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
                    if (is_root_user() != 0) {
                        create_project_config();
                    } else {
                        printf("You should run 'door init' as normal user\n");
                    }
                } else if (strcmp(argv[1], "start") == 0) {
                    if (bootstrap() == 0) {
                        printf("Starting sync loop with %ss interval... ", get_property("syncInterval"));
                        if (get_auto_save() == 0) {
                            printf("running\n");
                            sync_loop();
                        } else {
                            printf("failed. Manual sync is required\n");
                        }
                    }
                } else if (strcmp(argv[1], "stop") == 0) {
                    load_config();
                    tear_down();
                } else if (strcmp(argv[1], "sync") == 0) {
                    load_config();
                    printf("Starting manual sync... ");
                    if (sync_directory(get_property("projectName")) == 0) {
                        printf("done\n");
                    } else {
                        printf("error\n");
                    }
                } else if (strcmp(argv[1], "install") == 0) {
                    printf("Already installed on this system\n");
                } else {
                    show_usage();
                }
            } else {
                if (strcmp(argv[1], "install") == 0) {
                    if (is_root_user() != 0) {
                        install_door();
                    } else {
                        printf("You should run 'door install' as normal user\n");
                    }
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

int bootstrap() {
    if (is_root_user() == 0) {
        load_config();
        load_modules();
        mount_ramdisk(get_property("projectName"), NULL);
        start_sync(get_property("projectName"));
        started = 1;
        return 0;
    } else {
        printf("You should run 'door start' as root\n");
        return 1;
    }
}

void tear_down() {
    stop_sync(get_property("projectName"));
    umount_ramdisk(get_property("projectName"));
    unload_modules();
    started = 0;
}

void shutdown() {
    if (started != 0 && is_root_user() == 0 && get_auto_save() == 0) {
        tear_down();
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
    printf("       door sync               : manually sync files between disk and ram\n");
    printf("       door stop               : unload project / directory from ram\n");
}

void show_help() {
    printf("DoOR Version: %s.%s.%s\n", VERSION, COMMIT_NUMBER, GIT_SHA1);
    printf("Copyright (C) 2016 by Bálint Zoltán\n");
    printf("Web site: https://github.com/zbalint/DoOR/\n");

    printf("\nDoOR comes with ABSOLUTELY NO WARRANTY.  This is free software, and you\n");
    printf("are welcome to redistribute it under certain conditions.  See the GNU\n");
    printf("General Public Licence for details.\n\n");
    show_usage();
}
