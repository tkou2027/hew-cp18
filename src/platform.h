#ifndef PLATFORM_H
#define PLATFORM_H

// sound
int platform_opensound(char* path);
void platform_closesound(int hsound);
void platform_playsound(int hsound, int repeat);
void platform_stopsound(int hsound);

// input
int platform_getkey(int port);
void platform_resetkey(void);

#endif // PLATFORM_H