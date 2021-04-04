#ifndef VOICE_SERVER_H
#define VOICE_SERVER_H

#include <stdio.h>

int ids[1000];

int make_id();

int stream_voice(int id);

int get_all_except(int id);

#endif
