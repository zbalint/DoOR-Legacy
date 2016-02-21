#ifndef DOOR_RAMDISK_H
#define DOOR_RAMDISK_H

int mount_disk(char *target, char *diskSize);

int umount_disk(char *target);

int mount_ramdisk();

int umount_ramdisk();

#endif //DOOR_RAMDISK_H
