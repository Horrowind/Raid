#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raidconf.h"

void saveraidconf(RaidConf rc, const char* output) {
	FILE *fp;
	fp = fopen(output, "w+");
	fwrite(&(rc.Devices), sizeof(int), 1, fp);
	fwrite(&(rc.Size), sizeof(int), 1, fp);
	fwrite(&(rc.Chunk), sizeof(long long), 1, fp);
	fwrite(&(rc.Save), sizeof(int), 1, fp);
	for(int i = 0; i < rc.Devices; i++) {
		printf("%i: %i\n", i, rc.Entries[i].DeviceLength);
		fwrite(&(rc.Entries[i].DeviceLength), sizeof(int), 1, fp);
		fwrite(rc.Entries[i].Device, sizeof(char), rc.Entries[i].DeviceLength, fp);
		fwrite(rc.Entries[i].Bits, sizeof(int), rc.Size * (rc.Save), fp); 
	}

	fclose(fp);
}

RaidConf loadraidconf(const char* output) {
        RaidConf rc;
	FILE *fp;
        fp = fopen(output, "r");
        fread(&(rc.Devices), sizeof(int), 1, fp);
        fread(&(rc.Size), sizeof(int), 1, fp);
        fread(&(rc.Chunk), sizeof(long long), 1, fp);
        fread(&(rc.Save), sizeof(int), 1, fp);
	rc.Entries = malloc(sizeof(RaidEntry) * rc.Devices);
        for(int i = 0; i < rc.Devices; i++) {
		fread(&(rc.Entries[i].DeviceLength), sizeof(int), 1, fp);
		rc.Entries[i].Device = (char*)malloc(sizeof(char) * rc.Entries[i].DeviceLength);
		fread(rc.Entries[i].Device, 1, 12, fp);
		rc.Entries[i].Bits = malloc(sizeof(int) * rc.Size * rc.Save);
		fread(rc.Entries[i].Bits, sizeof(int), rc.Size * rc.Save, fp); 
	}

        fclose(fp);
	return rc;
}

