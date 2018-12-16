#ifndef _SYMBOLTABLEH_
#define _SYMBOLTABLEH_

#define MAX_SYMBOL_SIZE 100
#define MAX_STRUCT_SIZE 100
#define MAX_ARRAY_SIZE 10

#include "tree.h"
#include <stdlib.h>
typedef struct Type_ *Types;
typedef struct FieldList_ *FieldList;

struct Type_
{
    enum
    {
        BASIC,
        ARRAY,
        STRUCTURE
    } kind;
    union {
        //basic type
        int basic;
        struct
        {
            Types elem;
            int size;
        } array;
        FieldList structure;
    } u;
};

struct FieldList_
{
    char *name;        //name of domain
    struct Type_ type; //type of domain
    FieldList tail;    //next domain
};

struct FuncSymbol
{
    char *name;
    struct Type_ ret;
    FieldList args;
};

struct VarSymbol
{
    char *name;
    struct Type_ type;
};

typedef struct Symbol_ Symbols;

struct Symbol_
{
    enum
    {
        FUN,
        VAR,
        FIELDVAR
    } symkind;
    union {
        struct FuncSymbol func;
        struct VarSymbol var;
        struct FieldList_ fieldvar;
    } symuni;
};

typedef struct SymbolTable_ SymbolTable;

struct SymbolTable_
{
    int size;
    Symbols symbols[MAX_SYMBOL_SIZE];
};

typedef struct Struct_ Struct;

struct Struct_
{
    char *name;
    FieldList first;
};

typedef struct TypeTable_ TypeTable;

struct TypeTable_
{
    int size;
    Struct structs[MAX_STRUCT_SIZE];
};

SymbolTable sym_tbl;
static int sym_idx = 0;
static TypeTable struct_tbl;
static int str_idx = 0;
extern TreeNode node[MAX_NODE_SIZE];

int Semantic_Error;

void add_symbol(Symbols s);
void add_read_and_write();
void add_struct(Struct s);
void print_symbol(Symbols s);
void print_all_symbols();
Symbols init_symbol_func(TreeNode *typeNode, TreeNode *idNode, FieldList args);
Symbols init_symbol_var(TreeNode *typeNode, TreeNode *idNode, int select);
Symbols init_array(TreeNode *typeNode, TreeNode *idNode, int select);
Struct init_struct(TreeNode *defList, TreeNode *idNode);
void handle_extdef(TreeNode *Node);
FieldList handle_def(TreeNode *Node, int select);
FieldList handle_arg(TreeNode *Node);
void handle_struct(TreeNode *Node);
void scan_node(TreeNode *Node);
void scan_tree(int idx);
// int symeql(Symbols x, Symbols y);
// int look_up_symtbl(Symbols s);
FieldList look_up_structtbl(char *name);
int look_up_name(char *name);
Types look_up_type(char *name);
int is_op(char *opname);
void semantic_check_stmt(TreeNode *Node);
Types handle_exp_type(TreeNode *Node);
Types handle_func_in_exp(TreeNode *idNode);
Types handle_struct_in_exp(TreeNode *expNode);
Types handle_array_in_exp(TreeNode *expNode);
Types compute_array_type(TreeNode *expNode);
int typecmp(Types t1, Types t2);
#endif