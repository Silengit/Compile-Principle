#ifndef _MIPSH_
#define _MIPSH_
#include "ir.h"

#define MAX_REG 32
#define MAX_NAME_SIZE 32
#define FRAME_SIZE 1000

typedef struct _var* Var;
struct _var{
    char *name;
    int reg;    //-1 means unalloc, else means reg index;
    int offset; //stack position;
    Var next;
};

typedef struct _reg Reg;
struct _reg{
    char name[10]; //self name;
    char *var;  //content var name;
};

static Var varlist;

Reg reg[MAX_REG];
extern struct InterCodesList icl;
extern int tmp_idx;
static int offset;
static int max_frame_size;
static int reg_idx; //used for clear reg in order
static int para_ctr; //used for compute number of parameters of function(in callee)
                    //and if larger than 4, consider specially
static int arg_ctr; //used for compute number of arguments of function(in caller)
                    //and if larger than 4, consider specially

void init_all();
void increase_frame(FILE *f);
void init_reg_name();
void add_to_varlist(Var v);
int get_reg(Operand op, FILE *f);
int alloc_reg(FILE *f);
Var init_var(char *name, int off);
Var find_in_list(char *name);
Operand con2reg(Operand cons, FILE *f);
Operand addr2tmp(Operand addr, FILE *f);
void add_big_var(Operand op, int size, FILE *f);
void load_mem2reg(Var v, FILE *f);
void save_reg2mem(Var v, FILE *f);
void allreg2mem(FILE *f);
void fprint_reg(int idx, FILE *f);
void process(int idx);
void target_code_gen(char *filename);
void preprocess(FILE *f);
void fprint_target_code(struct InterCode ic, FILE *f);

#endif