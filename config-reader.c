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

int parseLine(char* line, int key[50], int value[50]) {
	char keyBuffer[100]; 
	char valueBuffer[100];

	int foundColon = false;

	int startingIndex = 0;

	int maxLineLen = 99;


	for (int i=0; i<strlen(line); i++) {
		if (i >= maxLineLen) {
			return 1;	
		}

		if (line[i] == ':' || strlen(line) == 0) {
			foundColon = true;
			startingIndex = i+2;
			keyBuffer[i] = '\0';
			break;
		}

		keyBuffer[i] = line[i];
	}

	if (foundColon == false) {
		keyBuffer[0] = '\0';
		return 1;
	}


	int loopCount = strlen(line) - startingIndex;
	int indexOffset = 0;

	bool foundValueStart = false;


	
	for (int i=0; i<loopCount; i++) {
		int lineIndex = i + startingIndex;
		int index = i - indexOffset;
		char targetChar = line[lineIndex];

		if (i >= maxLineLen) {
			return 1;	
		}

		if (targetChar == '\n') {
			valueBuffer[index] = '\0';
			break;
		}
		else if (isVisibleChar(targetChar) == true) {
			foundValueStart = true;
		}
		else if (foundValueStart == false && isVisibleChar(targetChar) == false) {
			indexOffset++;
			continue;
		}


		valueBuffer[index] = targetChar;

	}

	strcpy(key, keyBuffer);
	strcpy(value, valueBuffer);

	return 0;
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


	int indexOffset = 0;

	
	for (int i=0; i<cfgptr->count; i++) {
		// if line is invalid offset the index for the key and value arrays
		int index = i - indexOffset;

		int result = parseLine(lineArr[i], cfgptr->keyArr[index], cfgptr->valueArr[index]);
		
		//printf("%d\n", result);
		//if we find an empty line
		
		if (result == 1) {
			indexOffset++;
			continue;
		}
	}

	cfgptr->count -= indexOffset;

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
