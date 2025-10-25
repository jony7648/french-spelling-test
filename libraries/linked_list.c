#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

Node *newNode(char type, int size) {
	Node* p;

	p = (struct childNode*) malloc(sizeof(Node));

	p->parent = NULL;
	p->child = NULL;
	p->strValue = NULL;
	p->type = type;
	p->size = size;
	p->nodeCount = 0;
	p->arrLinked = 0;


	switch (type) {
		case 'S':
			p->strValue = (char*) malloc(sizeof(char) * size);
			p->strValue[size-1] = '\0';
			break;
		case 'N':
			p->nodeArr = (Node*) malloc(sizeof(Node) * size);
	}

	return p;
}

Node *newStringNode(char *value) {
	Node* node = newNode('S', strlen(value) + 1);
	assignNodeStringValue(node, value);

	return node;
}

int isRoot(Node* node) {
	if (node->parent == NULL || node->child == NULL) {
		return 1;
	}

	return 0;
}

int isParent(Node* node) {
	if (node->child == NULL) {
		return 1;
	}

	return 0;
}

void assignNodeChild(Node* parent, Node* child) {
	if (parent == NULL) {
		return;
	}

	parent->child = child;
	child->parent = parent;
}

void assignNodeChain(Node *root, Node *child) {
	if (root == NULL) {
		return;
	}

	Node *currentNode = root;

	while (currentNode->child != NULL) {
		currentNode = currentNode->child;	
	}

	currentNode->child = child;
}

void assignChainChild(Node* parent, Node* child) {
	if (parent->child == NULL) {
		parent->child = child;
		child->parent = parent;
	}
}

int freeNode(Node *node) {
	if (node == NULL) {
		return 1;
	}

	Node *parent = node->parent;
	Node *child = node->child;

	//clear parent node as well if has one
	//
	
	//printf("%p\n", node);
	
	if (parent != NULL) {
		if (child != NULL) {
			child->parent = node->parent;
		}
		else {
			parent->child = NULL;
		}

	}
	else if (child != NULL) {
		child->parent = NULL;
	}


	if (node->strValue != NULL) {
		free(node->strValue);
		node->strValue = NULL;
	}


	free(node);
	
	node = NULL;

	return 0;
}

int freeNodeTree(Node *root) {
	//printf("%p\n", root->child);
	if (root == NULL) {
		return 1;
	}

	if (isParent(root) == 1 && root->nodeCount == 0) {
		freeNode(root);
		return 1;
	}

	Node *parent = root->child;
	Node *child = NULL;

	for (int i=0; i<root->nodeCount; i++) {
		Node *category = root->nodeArr[i];

		freeNodeTree(root->nodeArr[i]);

		if (root->arrLinked == 1) {
			break;
		}

	}

	//printf("%p\n", root);
	while (parent != NULL) {
		child = parent->child;

		//printf("%s\n", (char*)getNodeValue(child));
		
		freeNode(parent);


		parent = child;

		if (child == NULL) {
			break;
		}
	}


	//printf("Freed Pack: %s %p\n", root->strValue, root);
	root->child = NULL;
	freeNode(root);

	return 0;
}

void *getNodeValue(Node *node) {
	if (node == NULL) {
		return NULL;
	}

	switch (node->type) {
		case 'S':
			return (void*) node->strValue;
			break;
		case 'I':
			return (void*) node->intValue;
			break;
		case 'C':
			return (void*) node->charValue;
			break;
	}

	return NULL;
}

Node *getChildNode(Node *parent) {
	if (parent == NULL) {
		return NULL;
	}

	Node *child = (Node*) parent->child;

	return parent->child;
}


Node *getArrNode(Node *parent, char *nodeValue) {
	if (parent == NULL) {
		return NULL;
	}
	
	if (nodeValue == NULL) {
		nodeValue = DEFAULT_NODE;		
	}

	Node *child;

	//loop through node arr return the node that matches the
	//passed in value
	
	for (int i=0; i<parent->nodeCount; i++) {
		child = parent->nodeArr[i];

		if (strcmp(child->strValue, nodeValue) == 0) {
			return child;
		}
	}

	return NULL;
}

void *getChildValue(Node *parent) {
	if (parent == NULL) {
		return NULL;
	}

	Node *child = (Node*) parent->child;

	return getNodeValue(parent->child);
}

int assignNodeStringValue(Node *node, char *data) {
	if (node == NULL || data == NULL) {
		return 1;
	}

	//if the node is of type string and the data being copied over
	//isn't too large and the strValue isn't NULL

	if (node->type == 'S' && node->strValue != NULL && node->size-1 >= strlen(data)) {
		strcpy(node->strValue, data);
		return 0;
	}

	//If the node isn't a string type 'S'

	if (node->strValue != NULL) {
		return 2;
	}

	//allocate some memory for the new string and copy the string
	node->strValue = (char*) malloc(sizeof(char) * strlen(data) + 1);
	strcpy(node->strValue, data);
	return 0;
	
}

Node *getArrNodeFromStr(Node *parent, char *str) {
	for (int i=0; i<parent->nodeCount; i++) {
		Node *child = parent->nodeArr[i];
		char *compareStr = (char*) getNodeValue(child);

		if (strcmp(str, compareStr) == 0) {
			return child;
		}
	}

	return NULL;
}

int linkNodeArrs(Node *arrHolder) {
	if (arrHolder->type != 'N') {
		return 1;
	}

	Node *currentNode = NULL;


	for (int i=0; i<arrHolder->nodeCount; i++) {
		//printf("%d\n", i);
		if (currentNode != NULL) {
			//printf("%s\n", (char*)getNodeValue(currentNode));
		}

		if (i - 1 >= 0) {
			//currentNode = getChildNode(arrHolder->nodeArr[i]);
			//printf("%d\n", i);
			assignNodeChild(currentNode, getChildNode(arrHolder->nodeArr[i]));
		}

		currentNode = getChildNode(arrHolder->nodeArr[i]);

		while (currentNode->child != NULL) {
			currentNode = currentNode->child;
		}
	}

	arrHolder->arrLinked = 1;

	return 0;

}

int appendToNodeArr(Node *parent, Node* node) {
	if (parent->nodeCount >= node->size) {
		return 1;
	}


	node->parent = parent;
	parent->nodeArr[parent->nodeCount] = (void*)node;
	parent->nodeCount = parent->nodeCount + 1;
}
