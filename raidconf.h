//#ifndef

typedef struct RaidEntryStruct {
	int DeviceLength;
	char* Device;
	unsigned char* Bits;
} RaidEntry;

typedef struct RaidConfStruct {
	int Devices;
	int Size;
	long long Chunk;
	int Save;
	RaidEntry* Entries;
} RaidConf;

extern void saveraidconf(RaidConf rc, const char* output);
extern RaidConf loadraidconf(const char* input);
