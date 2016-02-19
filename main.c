#include <stdio.h>
#include <string.h>

void showVersion() {
    printf("Version: %s.%s.%s\n", VERSION, COMMIT_NUMBER, GIT_SHA1);
}

void showUsage() {
    printf("Usage: door -v | --version     : print version\n");
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "--version") == 0)) {
            showVersion();
        } else {
            showUsage();
        }
    } else {
        showUsage();
    }
    return 0;
}