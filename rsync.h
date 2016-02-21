#ifndef DOOR_RSYNC_H
#define DOOR_RSYNC_H

int sync_directory(char *projectName);

int start_sync(char *projectName);

int stop_sync(char *projectName);

#endif //DOOR_RSYNC_H
