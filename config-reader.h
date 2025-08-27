#ifndef CONFIG_READER_H
#define CONFIG_READER_H

struct ParsedConfig {
	char keyArr[500][50];
	char valueArr[500][50];
	int maxLines;
	int count;
};

extern struct ParsedConfig* parseConfig(char* path);

#endif
