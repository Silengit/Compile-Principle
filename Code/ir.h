#include "symtable.h"

typedef struct Operand_ *Operand;
struct Operand_
{
    enum
    {
        VARIABLE,
        TEMP,
        CONSTANT,
        ADDRESS
    } kind;
    union {
        int var_no;
        int tmp_no;
        int value;
    } u;
};

struct InterCode
{
    enum
    {
        ASSIGN,
        ADD,
        SUB,
        MUL,
        DIV_,
        RETURN_,
        LABEL_CODE,
        LABEL_TRUE,
        LABEL_GOTO,
        READ,
        WRITE,
        CALLFUNC,
        FUNCTION,
        ARG,
        PARAM,
        REFASSIGN,
        DEC
    } kind;
    union {
        struct
        {
            Operand right, left;
        } assign;
        struct
        {
            Operand result, op1, op2;
        } binop;
        int lb_idx; //label_index
        struct
        {
            Operand right, left;
            int relop, lb_idx;
        } if_goto;
        struct
        {
            Operand op;
        } ret;
        char *fun; //function_name
        struct
        {
            char *fun;
            Operand op; 
        } call_func;
        struct
        {
            Operand op; 
        } arg;
        struct
        {
            Operand op; 
        } param;
        struct
        {
            Operand op;
            int size; 
        } dec;
        struct
        {
            Operand right, left;
        } refass;
    } u;
};

struct InterCodes
{
    struct InterCode code;
    struct InterCodes *prev, *next;
};

typedef struct InterCodes InterCodes;

struct InterCodesList
{
    struct InterCodes *first;
    struct InterCodes *last;
};

struct Arg
{
    int value;
    struct Arg* next;
};

struct ArgList
{
    struct Arg* head;
};

//used for judging whether we should give a variable new var_idx
int ir_sym_idx[MAX_SYMBOL_SIZE];

static struct InterCodesList icl;
static int var_idx;
static int tmp_idx;
static int label_idx;
extern SymbolTable sym_tbl;

void init_intercodeslist();
Operand ope_var_constructor(int idx);
Operand ope_tmp_constructor(int idx);
Operand ope_int_constructor(int value);
Operand ope_varaddr_constructor(int idx);
//InterCodes* test_constructor();
InterCodes *assign_var2ival_constructor(int idx, int value);
InterCodes *assign_tmp2ival_constructor(int idx, int value);
InterCodes *assign_tmp2var_constructor(int t_idx, int v_idx);
InterCodes *assign_var2tmp_constructor(int v_idx, int t_idx);
InterCodes *assign_addr2tmp_constructor(int t1_idx, int t2_idx);
InterCodes *assign_var2tmpaddr_constructor(int v_idx, int t_idx);
InterCodes *assign_tmp2tmpaddr_constructor(int t1_idx, int t2_idx);
//InterCodes *assign_tmp2varaddr_constructor(int t_idx, int v_idx);
InterCodes *plus_constructor(int result_idx, int t1_idx, int t2_idx);
InterCodes *plus_comstructor_2ndisaddr(int result_idx, int t1_idx, int t2_idx);
InterCodes *plus_comstructor_1stisaddr(int result_idx, int t1_idx, int t2_idx);
InterCodes *plus_comstructor_allareaddr(int result_idx, int t1_idx, int t2_idx);
InterCodes *minus_constructor(int result_idx, int t1_idx, int t2_idx);
InterCodes *multi_constructor(int result_idx, int t1_idx, int t2_idx);
InterCodes *multi_constructor_with_constant(int result_idx, int t1_idx, int value);
InterCodes *true_label_constructor(int lb_idx);
InterCodes *code_label_constructor(int t1_idx, int t2_idx, int op_idx, int lb_idx);
InterCodes *goto_label_constructor(int lb_idx);
InterCodes *ret_constructor(int t_idx);
InterCodes *dec_func_constructor(char *f_name);
InterCodes *call_read_constructor(int t_idx);
InterCodes *call_func_constructor(int t_idx, char *f_name);
InterCodes *call_write_constructor(int t_idx);
InterCodes *arg_constructor(int t_idx);
InterCodes *param_constructor(int v_idx);
InterCodes *dec_var_constructor(int size, int v_idx);
InterCodes *refassign_constructor(int t_idx, int v_idx);
void add_to_icl(InterCodes *ic);
void fprint_operand(Operand o, FILE *f);
void fprint_relop(int relop, FILE *f);
void fprint_label(int label, FILE *f);
void fprint_internode(struct InterCode ic, FILE *f);
void scan_ir_tree(int idx);
void file_out(char *filename);
int compute_offset_in_struct(char *struct_name, char *field_name);
int translate_Exp(TreeNode *Exp, int idx);
int compute_array_size(Types type);
int compute_struct_size(FieldList first_field);
void translate_CompSt(TreeNode *CompSt);
void translate_Stmt(TreeNode *Stmt);
int get_relop(TreeNode *Relop);
void translate_Cond(TreeNode *Exp, int label_true, int label_false);
void translate_Args(TreeNode *Args, struct ArgList *arglist);
void translate_tree(TreeNode *Node);