#ifndef DOOR_RAMDISK_H
#define DOOR_RAMDISK_H

char *get_mount_point(char *projectName);

int mount_disk(char *target, char *diskSize);

int umount_disk(char *target);

int mount_ramdisk(char *projectName, char *projectSize);

int umount_ramdisk(char *projectName);

int is_mounted(char *mountPoint);

#endif //DOOR_RAMDISK_H
