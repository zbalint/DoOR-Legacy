#include "ramdisk.h"
#include "stdgc.h"
#include "installer.h"
#include "config_loader.h"
#include <sys/mount.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

char *mountPoint = NULL;

int mount_disk(char *target, char *diskSize) {
    char *size = malloc(strlen("size=") + strlen(diskSize) + strlen("m") + 1);
    size[0] = '\0';
    strcpy(size, "size=");
    strcat(size, diskSize);
    strcat(size, "m");
    size[strlen(size)] = '\0';

    if (mount("none", target, "tmpfs", MS_MGC_VAL, size) == 0) {
        return 0;
    }
    return 1;
}

int umount_disk(char *target) {
    if (umount(target) == 0) {
        return 0;
    }
    return 1;
}

char *get_mount_point(char *projectName) {
    char *mountDirectory = get_mount_directory();
    mountPoint = malloc(strlen(mountDirectory) + strlen(projectName) + strlen("/") + 1);
    mountPoint[0] = '\0';
    strcpy(mountPoint, mountDirectory);
    strcat(mountPoint, projectName);
    strcat(mountPoint, "/");
    return mountPoint;
}

int mount_ramdisk(char *projectName, char *projectSize) {
    mountPoint = get_mount_point(projectName);

    printf("Creating mount point for %s... ", projectName);
    if (mkdir(mountPoint, 0755) == 0) {
        printf("done\n");
    } else {
        printf("error\n");
        return 1;
    }

    char *diskSize;
    if (projectSize == NULL) {
        diskSize = get_property("diskSize");
    } else {
        diskSize = projectSize;
    }

    printf("Mounting %s... ", mountPoint);
    if (mount_disk(mountPoint, diskSize) == 0) {
        printf("done\n");
    } else {
        printf("error\n");
        return 1;
    }
    return 0;
}

int umount_ramdisk(char *projectName) {
    mountPoint = get_mount_point(projectName);

    printf("Unmounting %s... ", mountPoint);
    if (umount_disk(mountPoint) == 0) {
        printf("done\n");
    } else {
        printf("error\n");
    }

    printf("Deleting mount point for %s... ", projectName);
    if (rmdir(mountPoint) == 0) {
        printf("done\n");
    } else {
        printf("error\n");
        return 1;
    }
    return 0;
}

int is_mounted(char *mountPoint) {
    char *mountPointCommand = malloc(strlen("mountpoint -q ") + strlen(mountPoint));
    mountPointCommand[0] = '\0';
    strcpy(mountPointCommand, "mountpoint -q ");
    strcat(mountPointCommand, mountPoint);
    return system(mountPointCommand);
}