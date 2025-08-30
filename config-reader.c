#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "config-reader.h"

char* getConfigValue(char* key, struct ParsedConfig* cfgptr) {
	for (int i=0; i<cfgptr->count; i++) {
		if (strcmp(cfgptr->keyArr[i], key) == 0) {
			return cfgptr->valueArr[i];
		}
	}
	return "  ";
}
/*
char* getConfigKey(char* value, int maxCount, struct ParsedConfig* cfgptr) {
	int retIndex = 0;

	char* retArr

	for (int i=0; i<cfgptr->count; i++) {
		if (retIndex >= maxCount) {
			break;
		}

		if (strcmp(cfgptr->valueArr[i], value) == 0) {
			//printf("%s\n", cfgptr->keyArr[i]);
			strcpy(retArr[retIndex], cfgptr->keyArr[i]);
			retIndex++;
		}
	}

	return retIndex;
}
*/

int isVisibleChar(char test) {
	if (test == ' ' || test == '\t' || test == '\n' || test == '\0') {
		return 0;
	}

	return 1;
}

void parseLine(char* line, int key[50], int value[50]) {
	char keyBuffer[50]; 
	char valueBuffer[50];

	int foundColon = false;

	int startingIndex = 0;


	for (int i=0; i<strlen(line); i++) {
		if (line[i] == ':') {
			startingIndex = i+2;
			keyBuffer[i] = '\0';
			break;
		}

		keyBuffer[i] = line[i];
	}

	int loopCount = strlen(line) - startingIndex;
	int indexOffset = 0;

	bool foundValueStart = false;

	
	for (int i=0; i<loopCount; i++) {
		int lineIndex = i + startingIndex;
		int index = i - indexOffset;
		char targetChar = line[lineIndex];

		if (targetChar == '\n') {
			valueBuffer[index] = '\0';
			break;
		}
		else if (isVisibleChar(targetChar) == true) {
			foundValueStart = true;
		}
		else if (foundValueStart == false && isVisibleChar(targetChar) == false) {
			indexOffset++;
		}


		valueBuffer[index] = targetChar;

	}

	strcpy(key, keyBuffer);
	strcpy(value, valueBuffer);
}

struct ParsedConfig* newParsedConfig() {
	struct ParsedConfig* p;	

	p = (struct ParsedConfig*) malloc(sizeof(struct ParsedConfig));

	return p;
}

struct ParsedConfig* parseConfig(char* path) {
	FILE* fptr;

	fptr = fopen(path, "r");

	if (fptr == NULL) {
		printf("Error reading file");
		return NULL;
	}

	struct ParsedConfig* cfgptr = newParsedConfig();

	char lineArr[500][100];
	char lineBuffer[100];

	int maxLineLen = 100;

				
	while (fgets(lineBuffer, maxLineLen, fptr)) {
		strcpy(lineArr[cfgptr->count], lineBuffer);
		cfgptr->count++;
	}

	
	for (int i=0; i<cfgptr->count; i++) {
		parseLine(lineArr[i], cfgptr->keyArr[i], cfgptr->valueArr[i]);
	}

	fclose(fptr);

	return cfgptr;

}

/*
int main() {
	struct ParsedConfig* cfgptr = parseConfig("config.cfg");

	char retArr[50][50];
	

	printf("%s\n", cfgptr->valueArr[0]);

	free(cfgptr);
}
*/
