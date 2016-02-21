#ifndef DOOR_INSTALLER_H
#define DOOR_INSTALLER_H

char *get_root_directory();

char *get_config_directory();

char *get_current_directory();

char *get_logs_directory();

char *get_mount_directory();

void create_project_config();

int is_installed();

void install_door();

void uninstall_door();

#endif //DOOR_INSTALLER_H
