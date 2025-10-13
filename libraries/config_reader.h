#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include "linked_list.h"

typedef struct {
	char keyArr[500][50];
	char valueArr[500][50];
	int maxLines;
	int count;
} ParsedConfig;

extern ParsedConfig* parseConfig(char* path);
extern char *getCfgValue(Node *cfgNode, char *targetCategory, char *targetKey);
extern int getCfgValueCount(Node *cfgNode, char* targetCategory);

#endif
