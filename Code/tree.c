#include "tree.h"
/*int main() {
    Tree mytree = init_tree();
    Value v0; v0.type_int = 0;
    TreeNode a = init_treeNode_ter("a", v0, _INT);
    Value v1; v1.type_charstar = "sff";
    TreeNode b = init_treeNode_ter("b", v1, _CHARSTAR);
    Value v2; v2.type_int = 2;
    TreeNode c = init_treeNode_ter("c", v2, _INT);
    Value v3; v3.type_float = 3.5;
    TreeNode d = init_treeNode_non("d", 4);
    Value v4; v4.type_int = 4;
    TreeNode e = init_treeNode_ter("e", v4, _INT);
    Value v5; v5.type_int = 5;
    TreeNode f = init_treeNode_ter("f", v5, _INT);
    Value v6; v6.type_int = 6;
    TreeNode g = init_treeNode_ter("g", v6, _INT);
    Value v7; v7.type_double = 7.77;
    TreeNode h = init_treeNode_ter("h", v7, _DOUBLE);
    mytree.root = &a;
    add_son(mytree.root, &b);
    add_sibling(&b, &c);
    add_sibling(&c, &d);
    add_son(&b, &e);
    add_son(&c, &f);
    add_son(&e, &g);
    add_sibling(&e, &h);
    print_tree(mytree);
    return 0;
}*/

Tree init_tree()
{
	Tree x;
	x.root = NULL;
	return x;
}

TreeNode init_treeNode_non(const char *name, int lineno, int column, int length)
{
	TreeNode x;
	x.symbol = NONTERMINAL;
	strcpy(x.name, name);
	x.name[19] = '\0';
	x.lineno = lineno;
	x.column = column;
	x.length = length;
	x.value.type_int = 0;
	x.type = _INT;
	x.sibling = NULL;
	x.son = NULL;
	return x;
}

TreeNode init_treeNode_ter(const char *name, Value value, Type type, int lineno, int column, int length)
{
	TreeNode x;
	x.symbol = TERMINAL;
	strcpy(x.name, name);
	x.name[19] = '\0';
	x.lineno = lineno;
	x.column = column;
	x.length = length;
	if (type != _VOID)
		x.value = value;
	else
		x.value.type_int = 0; //make no sense
	x.type = type;
	x.sibling = NULL;
	x.son = NULL;
	return x;
}

int node_idx(TreeNode dstNode)
{
	//look for the dstNode
	int idx1 = -1;
	for (int i = 0; i < idx; i++)
		if (strcmp(node[i].name, dstNode.name) == 0 &&
			node[i].lineno == dstNode.lineno &&
			node[i].column == dstNode.column &&
			node[i].length == dstNode.length)
		{
			idx1 = i;
			break;
		}
	//not exist, then struct one
	if (idx1 == -1)
	{
		idx1 = idx;
		node[idx++] = dstNode;
	}
	return idx1;
}

void add_sibling(TreeNode srcNode, TreeNode dstNode)
{
	int idx1 = node_idx(srcNode);
	int idx2 = node_idx(dstNode);
	node[idx1].sibling = &node[idx2];
}

int add_son(TreeNode srcNode, TreeNode dstNode)
{
	int idx1 = node_idx(srcNode);
	int idx2 = node_idx(dstNode);
	node[idx1].son = &node[idx2];
	return idx1;
}

void set_value(TreeNode *srcNode, Value value)
{
	if (srcNode != NULL)
		srcNode->value = value;
	else
		perror("Error! TreeNode is null!\n");
}

void print_tree(int idx)
{
	print_tNode(&node[idx], 0);
}

void print_tNode(TreeNode *Node, int depth)
{
	if (Node != NULL)
	{
		for (int i = 0; i < depth; i++)
			printf("  ");
		printf("%s", Node->name);
		switch (Node->symbol)
		{
		case TERMINAL:
		{
			switch (Node->type)
			{
			case _INT:
				printf(": %d\n", Node->value.type_int);
				break;
			case _FLOAT:
				printf(": %f\n", Node->value.type_float);
				break;
			case _DOUBLE:
				printf(": %lf\n", Node->value.type_double);
				break;
			case _CHARSTAR:
				printf(": %s\n", Node->value.type_charstar);
				break;
			case _VOID:
				printf("\n");
				break;
			default:
				perror("This type is not allowed!\n");
			}
			break;
		}
		case NONTERMINAL:
			printf(" (%d)\n", Node->lineno);
			break;
		default:
			perror("This symbol is not allowed!\n");
		}
		print_tNode(Node->son, depth + 1);
		print_tNode(Node->sibling, depth);
	}
}