#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gst/gst.h>

#define BANDS 160
#define THRESHOLD -80
#define AUDIOFREQ 5000

/**
 * Initialize gstreamer.
 */
void init_audio();

/**
 * Cleanup gstreamer.
 */
void clean_audio();

#endif
