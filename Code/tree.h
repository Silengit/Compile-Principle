#ifndef _TREEH_
#define _TREEH_
#include <stdio.h>
#include <string.h>
#define MAX_NODE_SIZE 1000

enum Type
{
	_VOID,
	_INT,
	_FLOAT,
	_DOUBLE,
	_CHARSTAR,
};
typedef enum Type Type;

enum Symbol
{
	TERMINAL,
	NONTERMINAL
};
typedef enum Symbol Symbol;

typedef union {
	int type_int;
	float type_float;
	double type_double;
	char type_charstar[20];
} Value;

struct TreeNode
{
	Symbol symbol;
	char name[20];
	int lineno;
	int column;
	int length;
	Value value;
	Type type;
	struct TreeNode *sibling;
	struct TreeNode *son;
};
typedef struct TreeNode TreeNode;

TreeNode node[MAX_NODE_SIZE];
static int idx = 0;

typedef struct
{
	TreeNode *root;
} Tree;

Tree init_tree();
TreeNode init_treeNode_non(const char *name, int lineno, int column, int length);
TreeNode init_treeNode_ter(const char *name, Value value, Type type, int lineno, int column, int length);
int node_idx(TreeNode dstNode);
void add_sibling(TreeNode srcNode, TreeNode dstNode);
int add_son(TreeNode srcNode, TreeNode dstNode);
void set_value(TreeNode *srcNode, Value value);
void print_tree(int idx);
void print_tNode(TreeNode *Node, int depth);
#endif