#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raidconf.h"
#include "galois.h"
#include "galois.h"
#define INVALID_INPUT -1


unsigned long long parse_size(char *size) {
	char *c;
	long long s = strtoll(size, &c, 10);
	if (s > 0) {
		switch (*c) {
			case 'K':
				c++;
			default:
				break;
			case 'M':
				c++;
				s *= 1024;
				break;
			case 'G':
				c++;
				s *= 1024 * 1024;
				break;
		}
	} else
		s = INVALID_INPUT;
	if (*c)
		s = INVALID_INPUT;
	return s;
}



void usage(void) { //TODO: Useful usage
	printf("Usage:\n");
	printf(" -c<Chunk Size [K|M|G]>\n");
	printf(" -d<Device Name>\n");
	exit (8);
}



int main(int argc, char *argv[]) {
	RaidConf rc;
	int r, base;
	rc.Chunk = 1024;
	rc.Devices = 0;
	char* output;
	//TODO: check if option is already set
	//int stripeopt = 0, chunkopt = 0. redopt = 0;

	int i = 1;
        while ((i < argc) && (argv[i][0] == '-')) {
                switch (argv[i][1]) {
                        case 'c':
				rc.Chunk = parse_size(&argv[i][2]);
                                break;
                        case 'd':
                                rc.Devices++;
                                break;
                        case 'r':
				r = atoi(&argv[i][2]);
				break;
                        case 'o':
				output = malloc(sizeof(char) * strlen(&argv[i][2]));
				strcpy(output, &argv[i][2]);
                                break;
			case 'w':
				base = atoi(&argv[i][2]);
			case 's':
				break;
			default:
                                printf("Wrong Argument: %s\n", argv[i]);
                                usage();
                        }
                i++;
        }
        if(r >= rc.Devices) { 
                printf("More redundancy then devices\n");
                exit(8);
        }
	rc.Save = rc.Devices - r;
	rc.Size = base; // TODO: To be determined, set to most significant bit of rc.Devices
	rc.Entries = malloc(sizeof(RaidEntry) * rc.Devices);
	int bits = rc.Size * rc.Save;
	int vandermonde;

	for(int j = 0; j < rc.Devices; j++) {
		rc.Entries[j].Bits = malloc(bits * sizeof(int));
		for(int k = 0; k < rc.Save; k++) {
                        if(j < rc.Save) {
                                vandermonde = (j == k);
                        } else {
                                int p = 1;
                                for(int l = 1; l <= j + r - rc.Devices; l++) {
                                        p = galois_single_multiply(p, k + 1, rc.Size);
                                }
                                vandermonde = p;
                        }

			for(int l = 0; l < rc.Size; l++) {
				rc.Entries[j].Bits[k * rc.Size + l] = galois_single_multiply(vandermonde, 1 << l, rc.Size);
//				printf("%i, %i, %i: %i\n", j, k, l, rc.Entries[j].Bits[k * rc.Size + l]);
			}
		}

	}

	i = 1;
	int d = 0;
	while ((i < argc) && (argv[i][0] == '-')) {
		switch (argv[i][1]) {
			case 'c':
				printf("Chunk Size: %lldK\n",parse_size(&argv[i][2]));
				break;

			case 'd':
				rc.Entries[d].DeviceLength = strlen(&argv[i][2]) + 1;
				rc.Entries[d].Device = malloc(sizeof(char) * rc.Entries[d].DeviceLength);
				strcpy(rc.Entries[d].Device, &argv[i][2]);
				printf("Device: %s\n",rc.Entries[d].Device);
				d++;
				break;
                        case 's':
                                printf("Striped\n");
                                break;
			case 'r':
				r = atoi(&argv[i][2]);
				break;
			case 'o':
				printf("Output: %s\n", &argv[i][2]);
				break;
		}
		i++;
	}

	saveraidconf(rc, output);
	//TODO: Delete crap
	return 0;
}
