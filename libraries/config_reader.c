#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linked_list.h"
#include "config_reader.h"


int isVisibleChar(char test) {
	if (test == ' ' || test == '\t' || test == '\n' || test == '\0') {
		return 0;
	}

	return 1;
}

Node *parseLine(char *line, Node *nodeContainer, Node *currentCategoryNode, int *retStatus) {
	char keyBuffer[100]; 
	char valueBuffer[100];

	int foundColon = false;

	int startingIndex = 0;

	int maxLineLen = 99;

	int parsingCategory = 0;


	for (int i=0; i<strlen(line); i++) {
		if (parsingCategory == 1) {
			//add strings to the keybuffer with an offest of - 1
			//due to account for us skipping the '[' character
			if (line[i] == ']') {
				keyBuffer[i-1] = '\0';
				break;
			}

			keyBuffer[i-1] = line[i];
		}

		if (i >= maxLineLen) {
			*retStatus = 2;	
			return currentCategoryNode;
		}

		if (line[i] == '[') {
			parsingCategory = 1;	
			continue;
		}

		if (line[i] == ':' || strlen(line) == 0) {
			foundColon = true;
			startingIndex = i+2;
			keyBuffer[i] = '\0';
			break;
		}

		keyBuffer[i] = line[i];
	}

	if (parsingCategory == 1) {
		currentCategoryNode = (Node*) newStringNode(keyBuffer);
		appendToNodeArr(nodeContainer, currentCategoryNode);
		//printf("%s\n", (char*) getNodeValue(currentCategoryNode));
	}

	if (foundColon == false) {
		keyBuffer[0] = '\0';
		*retStatus = 2;
		return currentCategoryNode;
	}


	int loopCount = strlen(line) - startingIndex;
	int indexOffset = 0;

	bool foundValueStart = false;


	
	for (int i=0; i<loopCount; i++) {
		int lineIndex = i + startingIndex;
		int index = i - indexOffset;
		char targetChar = line[lineIndex];

		if (i >= maxLineLen) {
			*retStatus = 1;	
			return currentCategoryNode;
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

	
	Node *keyNode = newStringNode(keyBuffer);
	Node *valueNode = newStringNode(valueBuffer);

	//printf("%s\n", (char*) getNodeValue(currentCategoryNode));
	assignNodeChain(currentCategoryNode, keyNode);
	assignNodeChild(keyNode, valueNode);
	
	//printf("%s\n", (char*) getChildValue(keyNode));

	//strcpy(key, keyBuffer);
	//strcpy(value, valueBuffer);

	*retStatus = 0;
	return currentCategoryNode;
}

ParsedConfig* newParsedConfig() {
	ParsedConfig* p;	

	p = (ParsedConfig*) malloc(sizeof(ParsedConfig));

	return p;
}

//Create a parse config function that can also parse categories

Node *parseConfig(char* path) {
	FILE *fptr;

	fptr = fopen(path, "r");

	if (fptr == NULL) {
		return NULL;
	}

	int maxChars = 20;
	int lineCount = 0;

	char *configArr = (char*) malloc(sizeof(char) * 10);



	char lineArr[500][100];
	char lineBuffer[100];

	int maxLineLen = 100;

				
	while (fgets(lineBuffer, maxLineLen, fptr)) {
		strcpy(lineArr[lineCount], lineBuffer);
		lineCount++;
	}


	int indexOffset = 0;

	Node *configNodeContainer = newNode('N', 10);
	Node *currentCategoryNode = newStringNode("default");
	appendToNodeArr(configNodeContainer, currentCategoryNode);
	//Node *valuesNode = newStringNode("values");
	
	for (int i=0; i<lineCount; i++) {
		// if line is invalid offset the index for the key and value arrays
		int index = i - indexOffset;

		int retStatus = 0;

		currentCategoryNode = parseLine(lineArr[i], configNodeContainer, currentCategoryNode, &retStatus);
		
		//printf("%d\n", result);
		//if we find an empty line
		
		if (retStatus == 2) {
			indexOffset++;
			continue;
		}
	}

	lineCount -= indexOffset;

	for (int i=0; i<configNodeContainer->nodeCount; i++) {
		if (getChildValue(configNodeContainer->nodeArr[i]) == NULL) {
			continue;
		}
/*
		Node *keyNode = getChildNode(configNodeContainer->nodeArr[i]);
		Node *valueNode = getChildNode(keyNode);

		printf(
			"%s: %s\n", 
			getNodeValue(keyNode),
			getNodeValue(valueNode)
			

		);
	
*/
	
	}
	fclose(fptr);

	return configNodeContainer;
}

char *getCfgValue(Node *cfgNode, char *targetCategory, char *targetKey) {
	for (int i=0; i<cfgNode->nodeCount; i++) {
		Node *categoryNode = cfgNode->nodeArr[i];
		
		if (targetCategory != NULL && strcmp(targetCategory, (char*)getNodeValue(categoryNode)) != 0) {
			continue;	
		}


		Node *keyNode = getChildNode(categoryNode);

		while (keyNode != NULL) {
			Node *valueNode = getChildNode(keyNode);

			char *key = (char*) getNodeValue(keyNode);	
			char *value = (char*) getNodeValue(valueNode);

			if (key != NULL && strcmp(key, targetKey) == 0) {
				return value;
			}

			keyNode = getChildNode(valueNode);
		}
	}

	return NULL;
}

int getCfgValueCount(Node *cfgNode, char* targetCategory) {
	if (cfgNode == NULL) {
		return 1;
	}

	Node *currentNode;
	
	currentNode = getChildNode(getArrNode(cfgNode, targetCategory));

	int count = 0;

	while (currentNode != NULL) {
		currentNode = currentNode->child;
		count++;	
	}

	return count/2;
}

/*
int main() {
   Node *cfgNode = parseConfig("test.cfg");

   char retArr[50][50];

   char *value = getCfgValue(cfgNode, NULL, "tails");

	//printf("%s\n", value);



	free(cfgNode);
}
*/
