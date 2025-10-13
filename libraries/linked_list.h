#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct {
	void *parent;
	void *child;
	char type;
	int intValue;
	char charValue;
	char *strValue;
	void **nodeArr;
	int size;
	int nodeCount;
} Node;

extern Node *newNode(char type, int size);
extern Node *newStringNode(char *value);
extern int assignNodeStringValue(Node *node, char *data);
extern void assignNodeChild(Node *parent, Node *child);
extern void assignNodeChain(Node *root, Node *child);
extern void *getNodeValue(Node *node);
extern Node *getChildNode(Node *parent);
extern Node *getArrNode(Node *parent, char *nodeValue);
extern void *getChildValue(Node *parent);
extern int freeNode(Node *node);
extern int freeNodeTree(Node *node);
extern int appendToNodeArr(Node *parent, Node *child);


#endif
