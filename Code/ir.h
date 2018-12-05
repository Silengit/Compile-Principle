#include "symboltable.h"

typedef struct Operand_* Operand;
struct Operand_ {
    enum { VARIABLE, CONSTANT, ADDRESS} kind;
    union {
        int var_no;
        int value;
    } u;
};

struct InterCode
{
    enum { ASSIGN, ADD, SUB, MUL} kind;
    union {
        struct { Operand right, left; } assign;
        struct { Operand result, op1, op2; } binop;
    } u;
};

struct InterCodes { 
    struct InterCode code; 
    struct InterCodes *prev, *next; 
};

struct InterCodesList{
    struct InterCodes *first;
    struct InterCodes *last;
};

static struct InterCodesList icl;

void init_intercodeslist();
void test_constructor();
void print_intercodeslist();
void print_internode(struct InterCode ic);
void scan_ir_tree();
