#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>

#include "util/audio.h"
#include "util/window.h"

void print_help();
 
int main(int argc, char ** argv)  {
    int coords[] = { 0, 0, 0, 0 };

    static struct option long_options[] = {
		// Empty for now
    };

    char c = 0;
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "g:h", long_options, &option_index)) != -1) {
		char * end;
		// Keep it easy for now.
		switch (c) {
			case 'g':
			for (int idx=0, i=0; i<strlen(optarg); ++i) {
				char val = optarg[i];
				if (val == 'x' || val == '+') {
				idx++;
				continue;
				}

				coords[idx] = coords[idx] * 10 + (val - '0');
			}
			break;
			case 'h':
			print_help();
			exit(0);
		}
    }

    if (coords[0] == 0 && coords[1] == 0 && coords[2] == 0 && coords[3] == 0) {
		printf("Invalid Layout Provided!\n");
		printf("---------------------------\n");
		print_help();
		exit(0);
    }

    init_window(coords[0], coords[1], coords[2], coords[3]);

    // Make sure we show up under the bar.
    XLowerWindow(dpy, win);

    init_audio();
    clean_audio();
}

void print_help() {
    printf("Usage: barvis [options]\n");
    printf("Options:\n");
    printf("  -h --help\tDisplay this information\n");
    printf("  -g widthxheight+x+y Set the layout for the window (required).\n");
}
