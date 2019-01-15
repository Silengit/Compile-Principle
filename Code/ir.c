#include "ir.h"

void init_intercodeslist()
{
    icl.first = NULL;
    icl.last = NULL;
    var_idx = 1;
    tmp_idx = 1;
    label_idx = 1;
    memset(ir_sym_idx, 0, MAX_SYMBOL_SIZE * sizeof(int));
}

// void test_constructor()
// {
//     struct InterCodes *tmp = (struct InterCodes *)malloc(sizeof(struct InterCodes));
//     printf("sizeof is %d\n",sizeof(*tmp));
//     tmp->code.kind = ASSIGN;
//     Operand r = (Operand)malloc(sizeof(struct Operand_));
//     r->kind = CONSTANT;
//     r->u.value = 1;
//     Operand l = (Operand)malloc(sizeof(struct Operand_));
//     l->kind = VARIABLE;
//     l->u.var_no = 0;
//     tmp->code.u.assign.left = l;
//     tmp->code.u.assign.right = r;
//     tmp->next = NULL;
//     tmp->prev = NULL;
//     return tmp;
// }

Operand ope_var_constructor(int idx)
{
    Operand o = (Operand)malloc(sizeof(struct Operand_));
    o->kind = VARIABLE;
    o->u.var_no = idx;
    return o;
}

Operand ope_tmp_constructor(int idx)
{
    Operand o = (Operand)malloc(sizeof(struct Operand_));
    o->kind = TEMP;
    o->u.tmp_no = idx;
    return o;
}

Operand ope_int_constructor(int value)
{
    Operand o = (Operand)malloc(sizeof(struct Operand_));
    o->kind = CONSTANT;
    o->u.value = value;
    return o;
}

Operand ope_tmpaddr_constructor(int idx)
{
    Operand o = (Operand)malloc(sizeof(struct Operand_));
    o->kind = ADDRESS;
    o->u.tmp_no = idx;
    return o;
}

InterCodes *assign_var2ival_constructor(int idx, int value)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ASSIGN;
    Operand l = ope_var_constructor(idx);
    Operand r = ope_int_constructor(value);
    c->code.u.assign.left = l;
    c->code.u.assign.right = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *assign_tmp2ival_constructor(int idx, int value)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ASSIGN;
    Operand l = ope_tmp_constructor(idx);
    Operand r = ope_int_constructor(value);
    c->code.u.assign.left = l;
    c->code.u.assign.right = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *assign_var2tmp_constructor(int v_idx, int t_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ASSIGN;
    Operand l = ope_var_constructor(v_idx);
    Operand r = ope_tmp_constructor(t_idx);
    c->code.u.assign.left = l;
    c->code.u.assign.right = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *assign_tmp2var_constructor(int t_idx, int v_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ASSIGN;
    Operand l = ope_tmp_constructor(t_idx);
    Operand r = ope_var_constructor(v_idx);
    c->code.u.assign.left = l;
    c->code.u.assign.right = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *assign_addr2tmp_constructor(int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ASSIGN;
    Operand l = ope_tmpaddr_constructor(t1_idx);
    Operand r = ope_tmp_constructor(t2_idx);
    c->code.u.assign.left = l;
    c->code.u.assign.right = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *assign_addr2tmpaddr_constructor(int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ASSIGN;
    Operand l = ope_tmpaddr_constructor(t1_idx);
    Operand r = ope_tmpaddr_constructor(t2_idx);
    c->code.u.assign.left = l;
    c->code.u.assign.right = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *assign_var2tmpaddr_constructor(int v_idx, int t_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ASSIGN;
    Operand l = ope_var_constructor(v_idx);
    Operand r = ope_tmpaddr_constructor(t_idx);
    c->code.u.assign.left = l;
    c->code.u.assign.right = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *assign_tmp2tmpaddr_constructor(int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ASSIGN;
    Operand l = ope_tmp_constructor(t1_idx);
    Operand r = ope_tmpaddr_constructor(t2_idx);
    c->code.u.assign.left = l;
    c->code.u.assign.right = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *plus_constructor(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ADD;
    Operand l = ope_tmp_constructor(t1_idx);
    Operand r = ope_tmp_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *plus_comstructor_2ndisaddr(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ADD;
    Operand l = ope_tmp_constructor(t1_idx);
    Operand r = ope_tmpaddr_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *plus_comstructor_1stisaddr(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ADD;
    Operand l = ope_tmpaddr_constructor(t1_idx);
    Operand r = ope_tmp_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *plus_comstructor_allareaddr(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ADD;
    Operand l = ope_tmpaddr_constructor(t1_idx);
    Operand r = ope_tmpaddr_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *minus_constructor(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = SUB;
    Operand l = ope_tmp_constructor(t1_idx);
    Operand r = ope_tmp_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *minus_comstructor_2ndisaddr(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = SUB;
    Operand l = ope_tmp_constructor(t1_idx);
    Operand r = ope_tmpaddr_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *minus_comstructor_1stisaddr(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = SUB;
    Operand l = ope_tmpaddr_constructor(t1_idx);
    Operand r = ope_tmp_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *minus_comstructor_allareaddr(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = SUB;
    Operand l = ope_tmpaddr_constructor(t1_idx);
    Operand r = ope_tmpaddr_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *multi_constructor(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = MUL;
    Operand l = ope_tmp_constructor(t1_idx);
    Operand r = ope_tmp_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *multi_comstructor_2ndisaddr(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = MUL;
    Operand l = ope_tmp_constructor(t1_idx);
    Operand r = ope_tmpaddr_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *multi_comstructor_1stisaddr(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = MUL;
    Operand l = ope_tmpaddr_constructor(t1_idx);
    Operand r = ope_tmp_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *multi_comstructor_allareaddr(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = MUL;
    Operand l = ope_tmpaddr_constructor(t1_idx);
    Operand r = ope_tmpaddr_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *multi_constructor_with_constant(int result_idx, int t1_idx, int value)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = MUL;
    Operand l = ope_tmp_constructor(t1_idx);
    Operand r = ope_int_constructor(value);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *div_constructor(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = DIV_;
    Operand l = ope_tmp_constructor(t1_idx);
    Operand r = ope_tmp_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *div_comstructor_2ndisaddr(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = DIV_;
    Operand l = ope_tmp_constructor(t1_idx);
    Operand r = ope_tmpaddr_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *div_comstructor_1stisaddr(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = DIV_;
    Operand l = ope_tmpaddr_constructor(t1_idx);
    Operand r = ope_tmp_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *div_comstructor_allareaddr(int result_idx, int t1_idx, int t2_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = DIV_;
    Operand l = ope_tmpaddr_constructor(t1_idx);
    Operand r = ope_tmpaddr_constructor(t2_idx);
    Operand result = ope_tmp_constructor(result_idx);
    c->code.u.binop.result = result;
    c->code.u.binop.op1 = l;
    c->code.u.binop.op2 = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *true_label_constructor(int lb_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = LABEL_TRUE;
    c->code.u.lb_idx = lb_idx;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *code_label_constructor(int t1_idx, int t2_idx, int op_idx, int lb_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = LABEL_CODE;
    Operand l = ope_tmp_constructor(t1_idx);
    Operand r = ope_tmp_constructor(t2_idx);
    c->code.u.if_goto.left = l;
    c->code.u.if_goto.right = r;
    c->code.u.if_goto.relop = op_idx;
    c->code.u.if_goto.lb_idx = lb_idx;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *code_label_constructor_2ndisaddr(int t1_idx, int t2_idx, int op_idx, int lb_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = LABEL_CODE;
    Operand l = ope_tmp_constructor(t1_idx);
    Operand r = ope_tmpaddr_constructor(t2_idx);
    c->code.u.if_goto.left = l;
    c->code.u.if_goto.right = r;
    c->code.u.if_goto.relop = op_idx;
    c->code.u.if_goto.lb_idx = lb_idx;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *code_label_constructor_1stisaddr(int t1_idx, int t2_idx, int op_idx, int lb_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = LABEL_CODE;
    Operand l = ope_tmpaddr_constructor(t1_idx);
    Operand r = ope_tmp_constructor(t2_idx);
    c->code.u.if_goto.left = l;
    c->code.u.if_goto.right = r;
    c->code.u.if_goto.relop = op_idx;
    c->code.u.if_goto.lb_idx = lb_idx;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *code_label_constructor_allareaddr(int t1_idx, int t2_idx, int op_idx, int lb_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = LABEL_CODE;
    Operand l = ope_tmpaddr_constructor(t1_idx);
    Operand r = ope_tmpaddr_constructor(t2_idx);
    c->code.u.if_goto.left = l;
    c->code.u.if_goto.right = r;
    c->code.u.if_goto.relop = op_idx;
    c->code.u.if_goto.lb_idx = lb_idx;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *goto_label_constructor(int lb_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = LABEL_GOTO;
    c->code.u.lb_idx = lb_idx;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *ret_constructor(int t_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = RETURN_;
    Operand o = ope_tmp_constructor(t_idx);
    c->code.u.ret.op = o;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *ret_constructor_addr(int t_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = RETURN_;
    Operand o = ope_tmpaddr_constructor(t_idx);
    c->code.u.ret.op = o;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *dec_func_constructor(char *f_name)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = FUNCTION;
    c->code.u.fun = f_name;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *call_read_constructor(int t_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = READ;
    c->code.u.call_func.fun = "read";
    Operand o = ope_tmp_constructor(t_idx);
    c->code.u.call_func.op = o;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *call_func_constructor(int t_idx, char *f_name)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = CALLFUNC;
    c->code.u.call_func.fun = f_name;
    Operand o = ope_tmp_constructor(t_idx);
    c->code.u.call_func.op = o;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *call_write_constructor(int t_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = WRITE;
    c->code.u.call_func.fun = "write";
    Operand o = ope_tmp_constructor(t_idx);
    c->code.u.call_func.op = o;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *arg_constructor(int t_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = ARG;
    Operand o = ope_tmp_constructor(t_idx);
    c->code.u.arg.op = o;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *param_constructor(int v_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = PARAM;
    Operand o = ope_var_constructor(v_idx);
    c->code.u.param.op = o;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *dec_var_constructor(int size, int v_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = DEC;
    Operand o = ope_var_constructor(v_idx);
    c->code.u.dec.op = o;
    c->code.u.dec.size = size;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

InterCodes *refassign_constructor(int t_idx, int v_idx)
{
    struct InterCodes *c = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    c->code.kind = REFASSIGN;
    Operand l = ope_tmp_constructor(t_idx);
    Operand r = ope_var_constructor(v_idx);
    c->code.u.refass.left = l;
    c->code.u.refass.right = r;
    c->next = NULL;
    c->prev = NULL;
    return c;
}

void add_to_icl(InterCodes *ic)
{
    if (icl.first == NULL)
        icl.first = icl.last = ic;
    else
    {
        icl.last->next = ic;
        ic->prev = icl.last;
        icl.last = ic;
    }
}

void fprint_operand(Operand o, FILE *f)
{
    switch (o->kind)
    {
    case VARIABLE:
        fprintf(f, "v%d", o->u.var_no);
        break;
    case TEMP:
        fprintf(f, "t%d", o->u.tmp_no);
        break;
    case ADDRESS:
        fprintf(f, "*t%d", o->u.tmp_no);
        break;
    case CONSTANT:
        fprintf(f, "#%d", o->u.value);
        break;
    default:
        break;
    }
}

void fprint_relop(int relop, FILE *f)
{
    switch (relop)
    {
    case 1:
        fprintf(f, " > ");
        break;
    case 2:
        fprintf(f, " < ");
        break;
    case 3:
        fprintf(f, " >= ");
        break;
    case 4:
        fprintf(f, " <= ");
        break;
    case 5:
        fprintf(f, " == ");
        break;
    case 6:
        fprintf(f, " != ");
        break;
    default:
        break;
    }
}

void fprint_label(int label, FILE *f)
{
    fprintf(f, "label%d", label);
}

void fprint_internode(struct InterCode ic, FILE *f)
{
    switch (ic.kind)
    {
    case ASSIGN:
        fprint_operand(ic.u.assign.left, f);
        fprintf(f, " := ");
        fprint_operand(ic.u.assign.right, f);
        fprintf(f, "\n");
        break;
    case ADD:
        fprint_operand(ic.u.binop.result, f);
        fprintf(f, " := ");
        fprint_operand(ic.u.binop.op1, f);
        fprintf(f, " + ");
        fprint_operand(ic.u.binop.op2, f);
        fprintf(f, "\n");
        break;
    case SUB:
        fprint_operand(ic.u.binop.result, f);
        fprintf(f, " := ");
        fprint_operand(ic.u.binop.op1, f);
        fprintf(f, " - ");
        fprint_operand(ic.u.binop.op2, f);
        fprintf(f, "\n");
        break;
    case MUL:
        fprint_operand(ic.u.binop.result, f);
        fprintf(f, " := ");
        fprint_operand(ic.u.binop.op1, f);
        fprintf(f, " * ");
        fprint_operand(ic.u.binop.op2, f);
        fprintf(f, "\n");
        break;
    case DIV_:
        fprint_operand(ic.u.binop.result, f);
        fprintf(f, " := ");
        fprint_operand(ic.u.binop.op1, f);
        fprintf(f, " / ");
        fprint_operand(ic.u.binop.op2, f);
        fprintf(f, "\n");
        break;
    case RETURN_:
        fprintf(f, "RETURN ");
        fprint_operand(ic.u.ret.op, f);
        fprintf(f, "\n");
        break;
    case LABEL_CODE:
        fprintf(f, "IF ");
        fprint_operand(ic.u.if_goto.left, f);
        fprint_relop(ic.u.if_goto.relop, f);
        fprint_operand(ic.u.if_goto.right, f);
        fprintf(f, " GOTO ");
        fprint_label(ic.u.if_goto.lb_idx, f);
        fprintf(f, "\n");
        break;
    case LABEL_TRUE:
        fprintf(f, "LABEL ");
        fprint_label(ic.u.lb_idx, f);
        fprintf(f, " :\n");
        break;
    case LABEL_GOTO:
        fprintf(f, "GOTO ");
        fprint_label(ic.u.lb_idx, f);
        fprintf(f, "\n");
        break;
    case READ:
        fprintf(f, "READ ");
        fprint_operand(ic.u.call_func.op, f);
        fprintf(f, "\n");
        break;
    case WRITE:
        fprintf(f, "WRITE ");
        fprint_operand(ic.u.call_func.op, f);
        fprintf(f, "\n");
        break;
    case CALLFUNC:
        fprint_operand(ic.u.call_func.op, f);
        fprintf(f, " := CALL %s\n", ic.u.call_func.fun);
        break;
    case FUNCTION:
        fprintf(f, "FUNCTION %s :\n", ic.u.fun);
        break;
    case ARG:
        fprintf(f, "ARG ");
        fprint_operand(ic.u.arg.op, f);
        fprintf(f, "\n");
        break;
    case PARAM:
        fprintf(f, "PARAM ");
        fprint_operand(ic.u.param.op, f);
        fprintf(f, "\n");
        break;
    case REFASSIGN:
        fprint_operand(ic.u.refass.left, f);
        fprintf(f, " := &");
        fprint_operand(ic.u.refass.right, f);
        fprintf(f, "\n");
        break;
    case DEC:
        fprintf(f, "DEC ");
        fprint_operand(ic.u.dec.op, f);
        fprintf(f, " %d\n", ic.u.dec.size);
        break;
    default:
        break;
    }
}

void scan_ir_tree(int idx)
{
    init_intercodeslist();
    translate_tree(&node[idx]);
    file_out("test.ir");
}

void file_out(char *filename)
{
    FILE *fout = fopen(filename, "wb");
    struct InterCodes *i = icl.first;
    while (i != NULL)
    {
        fprint_internode(i->code, fout);
        i = i->next;
    }
    fclose(fout);
}

int compute_offset_in_struct(char *struct_name, char *field_name)
{
    //printf("struct name is %s\n", struct_name);
    Types t = look_up_type(struct_name);
    FieldList first_field = t->u.structure;
    //printf("first_field name is %s\n", first_field->name);
    FieldList field = first_field;
    int offset = 0;
    while (field != NULL && strcmp(field->name, field_name) != 0)
    {
        if (field->type.kind == BASIC)
            offset += 4;
        else if (field->type.kind == ARRAY)
            offset += compute_array_size(&field->type);
        else
            offset += compute_struct_size(field->type.u.structure);
        field = field->tail;
    }
    //printf("offset = %d\n", offset);
    return offset;
}

int translate_Exp(TreeNode *Exp, int idx) //idx means place in manual, 0 means NULL.
{                                         //s_idx means symbol
    if (strcmp(Exp->name, "Exp") != 0)
    {
        printf("you should not call translate_Exp!\n");
        return 0;
    }

    if (Exp->son->sibling != NULL &&
        strcmp(Exp->son->sibling->name, "ASSIGNOP") == 0)
    {
        if (strcmp(Exp->son->son->name, "ID") == 0)
        {
            int s_idx = look_up_name(Exp->son->son->value.type_charstar);
            int v_idx;
            if (ir_sym_idx[s_idx] == 0)
            {
                v_idx = var_idx++;
                ir_sym_idx[s_idx] = v_idx;
            }
            else
            {
                v_idx = ir_sym_idx[s_idx];
            }
            int t1_idx = tmp_idx++;
            //printf("t1_idx = %d\n",t1_idx);
            int if_addr = translate_Exp(Exp->son->sibling->sibling, t1_idx);
            InterCodes *c1;
            if (if_addr == 0)
                c1 = assign_var2tmp_constructor(v_idx, t1_idx);
            else
                c1 = assign_var2tmpaddr_constructor(v_idx, t1_idx);
            add_to_icl(c1);
        }
        else if (Exp->son->son->sibling != NULL &&
                 strcmp(Exp->son->son->sibling->name, "DOT") == 0)
        {
            int t1_idx = tmp_idx++;
            translate_Exp(Exp->son, t1_idx);
            int t2_idx = tmp_idx++;
            int if_addr = translate_Exp(Exp->son->sibling->sibling, t2_idx);
            InterCodes *c;
            if(if_addr == 0)
                c = assign_addr2tmp_constructor(t1_idx, t2_idx);
            else
                c = assign_addr2tmpaddr_constructor(t1_idx, t2_idx);
            add_to_icl(c);
        }
        else if (Exp->son->son->sibling != NULL &&
                 strcmp(Exp->son->son->sibling->name, "LB") == 0)
        {
            int t1_idx = tmp_idx++;
            translate_Exp(Exp->son, t1_idx);
            int t2_idx = tmp_idx++;
            int if_addr =  translate_Exp(Exp->son->sibling->sibling, t2_idx);
            InterCodes *c;
            if(if_addr == 0)
                c= assign_addr2tmp_constructor(t1_idx, t2_idx);
            else
                c = assign_addr2tmpaddr_constructor(t1_idx, t2_idx);
            add_to_icl(c);
        }
        return 0;
    }
    else if (Exp->son->sibling != NULL &&
             strcmp(Exp->son->sibling->name, "PLUS") == 0)
    {
        int t1_idx = tmp_idx++;
        int t2_idx = tmp_idx++;
        int if_addr1 = translate_Exp(Exp->son, t1_idx);
        int if_addr2 = translate_Exp(Exp->son->sibling->sibling, t2_idx);
        InterCodes *c;
        if (if_addr1 == 0 && if_addr2 == 0)
            c = plus_constructor(idx, t1_idx, t2_idx);
        else if (if_addr1 == 0 && if_addr2 == 1)
            c = plus_comstructor_2ndisaddr(idx, t1_idx, t2_idx);
        else if (if_addr1 == 1 && if_addr2 == 0)
            c = plus_comstructor_1stisaddr(idx, t1_idx, t2_idx);
        else
            c = plus_comstructor_allareaddr(idx, t1_idx, t2_idx);
        add_to_icl(c);
        return 0;
    }
    else if (Exp->son->sibling != NULL &&
             strcmp(Exp->son->sibling->name, "MINUS") == 0)
    {
        int t1_idx = tmp_idx++;
        int t2_idx = tmp_idx++;
        int if_addr1 = translate_Exp(Exp->son, t1_idx);
        int if_addr2 = translate_Exp(Exp->son->sibling->sibling, t2_idx);
        InterCodes *c;
        if (if_addr1 == 0 && if_addr2 == 0)
            c = minus_constructor(idx, t1_idx, t2_idx);
        else if (if_addr1 == 0 && if_addr2 == 1)
            c = minus_comstructor_2ndisaddr(idx, t1_idx, t2_idx);
        else if (if_addr1 == 1 && if_addr2 == 0)
            c = minus_comstructor_1stisaddr(idx, t1_idx, t2_idx);
        else
            c = minus_comstructor_allareaddr(idx, t1_idx, t2_idx);
        add_to_icl(c);
        return 0;
    }
    else if (Exp->son->sibling != NULL &&
             strcmp(Exp->son->sibling->name, "STAR") == 0)
    {
        int t1_idx = tmp_idx++;
        int t2_idx = tmp_idx++;
        int if_addr1 = translate_Exp(Exp->son, t1_idx);
        int if_addr2 = translate_Exp(Exp->son->sibling->sibling, t2_idx);
        InterCodes *c;
        if (if_addr1 == 0 && if_addr2 == 0)
            c = multi_constructor(idx, t1_idx, t2_idx);
        else if (if_addr1 == 0 && if_addr2 == 1)
            c = multi_comstructor_2ndisaddr(idx, t1_idx, t2_idx);
        else if (if_addr1 == 1 && if_addr2 == 0)
            c = multi_comstructor_1stisaddr(idx, t1_idx, t2_idx);
        else
            c = multi_comstructor_allareaddr(idx, t1_idx, t2_idx);
        add_to_icl(c);
        return 0;
    }
    else if (Exp->son->sibling != NULL &&
             strcmp(Exp->son->sibling->name, "DIV") == 0)
    {
        //printf("you have come here!\n");
        int t1_idx = tmp_idx++;
        int t2_idx = tmp_idx++;
        int if_addr1 = translate_Exp(Exp->son, t1_idx);
        int if_addr2 = translate_Exp(Exp->son->sibling->sibling, t2_idx);
        InterCodes *c;
        if (if_addr1 == 0 && if_addr2 == 0)
            c = div_constructor(idx, t1_idx, t2_idx);
        else if (if_addr1 == 0 && if_addr2 == 1)
            c = div_comstructor_2ndisaddr(idx, t1_idx, t2_idx);
        else if (if_addr1 == 1 && if_addr2 == 0)
            c = div_comstructor_1stisaddr(idx, t1_idx, t2_idx);
        else
            c = div_comstructor_allareaddr(idx, t1_idx, t2_idx);
        add_to_icl(c);
        return 0;
    }
    else if ((Exp->son->sibling != NULL && strcmp(Exp->son->sibling->name, "RELOP") == 0) ||
             strcmp(Exp->son->name, "NOT") == 0 ||
             (Exp->son->sibling != NULL && strcmp(Exp->son->sibling->name, "AND") == 0) ||
             (Exp->son->sibling != NULL && strcmp(Exp->son->sibling->name, "OR") == 0))
    {
        int lb1_idx = label_idx++;
        int lb2_idx = label_idx++;
        InterCodes *c0 = assign_tmp2ival_constructor(idx, 0);
        add_to_icl(c0);
        translate_Cond(Exp, lb1_idx, lb2_idx);
        InterCodes *c1 = true_label_constructor(lb1_idx);
        add_to_icl(c1);
        InterCodes *c2 = assign_tmp2ival_constructor(idx, 1);
        add_to_icl(c2);
        InterCodes *c3 = true_label_constructor(lb2_idx);
        add_to_icl(c3);
        return 0;
    }
    else if (strcmp(Exp->son->name, "LP") == 0)
    {
        translate_Exp(Exp->son->sibling, idx);
        return 0;
    }
    else if (strcmp(Exp->son->name, "MINUS") == 0)
    {
        int t1_idx = tmp_idx++;
        int t2_idx = tmp_idx++;
        InterCodes *c1 = assign_tmp2ival_constructor(t1_idx, 0);
        add_to_icl(c1);
        int is_addr = translate_Exp(Exp->son->sibling, t2_idx);
        InterCodes *c2;
        if(is_addr == 0)
            c2 = minus_constructor(idx, t1_idx, t2_idx);
        else    
            c2 = minus_comstructor_2ndisaddr(idx, t1_idx, t2_idx);
        add_to_icl(c2);
        return 0;
    }
    else if (Exp->son->sibling != NULL &&
             strcmp(Exp->son->sibling->name, "LP") == 0 &&
             strcmp(Exp->son->sibling->sibling->name, "RP") == 0)
    {
        char *f_name = Exp->son->value.type_charstar;
        if (strcmp(f_name, "read") == 0)
        {
            InterCodes *c = call_read_constructor(idx);
            add_to_icl(c);
        }
        else
        {
            InterCodes *c = call_func_constructor(idx, f_name);
            add_to_icl(c);
        }
        return 0;
    }
    else if (Exp->son->sibling != NULL &&
             strcmp(Exp->son->sibling->name, "LP") == 0 &&
             strcmp(Exp->son->sibling->sibling->name, "Args") == 0)
    {
        char *f_name = Exp->son->value.type_charstar;
        struct ArgList *arg_list = (struct ArgList *)malloc(sizeof(struct ArgList));
        arg_list->head = NULL;
        translate_Args(Exp->son->sibling->sibling, arg_list);
        if (strcmp(f_name, "write") == 0)
        {
            InterCodes *c1 = call_write_constructor(arg_list->head->value);
            add_to_icl(c1);
        }
        else
        {
            struct Arg *arg = arg_list->head;
            while (arg != NULL)
            {
                InterCodes *c1 = arg_constructor(arg->value);
                add_to_icl(c1);
                arg = arg->next;
            }
            InterCodes *c2 = call_func_constructor(idx, f_name);
            add_to_icl(c2);
        }
        return 0;
    }
    else if (Exp->son->sibling != NULL &&
             strcmp(Exp->son->sibling->name, "LB") == 0)
    {//only consider int array
        int size[MAX_ARRAY_SIZE];
        TreeNode *ID = Exp->son->son;
        int dimension = 1;
        while(strcmp(ID->name, "ID") != 0)
        {
            dimension++;
            ID = ID->son;  
        }
        //printf("%s's dimension = %d\n", ID->value.type_charstar,dimension);
        for(int i = 0;i < dimension; i++)
        {
            const char *suffix = "[]";
            char *name = (char *)malloc(sizeof(char) * (strlen(ID->value.type_charstar) + 2 * i + 1));
            strcpy(name, ID->value.type_charstar);
            for (int j = 0; j < i; j++)
                strcat(name, suffix);
            //printf("%s ",name);
            int tmps_idx = look_up_name(name);
            //printf("%d\n", sym_tbl.symbols[tmps_idx].symuni.var.type.u.array.size);
            size[i] = sym_tbl.symbols[tmps_idx].symuni.var.type.u.array.size;
            //printf("%d\n", size[i]);
        }
        int ctr = dimension;
        int t1_idx = tmp_idx++;
        int t0_idx = tmp_idx++;
        InterCodes *c_ = assign_tmp2ival_constructor(t1_idx, 0);
        add_to_icl(c_);
        while(strcmp(Exp->son->son->name, "ID") != 0)
        {
            if(ctr == dimension){
                translate_Exp(Exp->son->sibling->sibling, t0_idx);
                InterCodes *c1 = multi_constructor_with_constant(t1_idx, t0_idx, 4);
                add_to_icl(c1);
            }
            else
            {
                int base = 4;
                for(int i = ctr; i< dimension; i++)
                {
                    base *= size[i];
                    //printf("base = %d\n", base);
                }
                translate_Exp(Exp->son->sibling->sibling, t0_idx);
                InterCodes *c0 = multi_constructor_with_constant(t0_idx, t0_idx, base);
                add_to_icl(c0);
                InterCodes *c1 = plus_constructor(t1_idx, t1_idx, t0_idx);
                add_to_icl(c1);
            }
            Exp = Exp->son;
            ctr--;
        }     
        int base = 4;
        for(int i = 1; i< dimension; i++)
        {
            base *= size[i];
            //printf("base = %d\n", base);
        }
        translate_Exp(Exp->son->sibling->sibling, t0_idx);
        InterCodes *c0 = multi_constructor_with_constant(t0_idx, t0_idx, base);
        add_to_icl(c0);
        InterCodes *c1 = plus_constructor(t1_idx, t1_idx, t0_idx);
        add_to_icl(c1); 
        int t2_idx = tmp_idx++;
        int s_idx = look_up_name(Exp->son->son->value.type_charstar);
        int v_idx = ir_sym_idx[s_idx];
        if (sym_tbl.symbols[s_idx].symkind == VAR)
        {
            InterCodes *c2 = refassign_constructor(t2_idx, v_idx);
            add_to_icl(c2);
        }
        else if (sym_tbl.symbols[s_idx].symkind == FIELDVAR)
        {
            InterCodes *c2 = assign_tmp2var_constructor(t2_idx, v_idx);
            add_to_icl(c2);
        }
        /*InterCodes *c2 = refassign_constructor(t2_idx, v_idx);
        add_to_icl(c2);*/
        InterCodes *c3 = plus_constructor(idx, t2_idx, t1_idx);
        add_to_icl(c3);
        return 1;
    }
    else if (Exp->son->sibling != NULL &&
             strcmp(Exp->son->sibling->name, "DOT") == 0)
    {
        TreeNode *ID = Exp->son->sibling->sibling;
        if (strcmp(Exp->son->son->name, "ID") != 0)
        {
            printf("TODO\n");
            return 0;
        }
        int offset = compute_offset_in_struct(Exp->son->son->value.type_charstar,
                                              ID->value.type_charstar);
        int s_idx = look_up_name(Exp->son->son->value.type_charstar);
        int v_idx = ir_sym_idx[s_idx];
        //compute offset;
        int t1_idx = tmp_idx++;
        InterCodes *c1 = assign_tmp2ival_constructor(t1_idx, offset);
        add_to_icl(c1);
        //compute first address;
        //destruct as parameter and declaration;
        int t2_idx = tmp_idx++;
        if (sym_tbl.symbols[s_idx].symkind == VAR)
        {
            InterCodes *c2 = refassign_constructor(t2_idx, v_idx);
            add_to_icl(c2);
        }
        else if (sym_tbl.symbols[s_idx].symkind == FIELDVAR)
        {
            InterCodes *c2 = assign_tmp2var_constructor(t2_idx, v_idx);
            add_to_icl(c2);
        }
        else
        {
            printf("Cases not handled!\n");
            return 0;
        }
        //plus offset and first address;
        InterCodes *c3 = plus_constructor(idx, t2_idx, t1_idx);
        add_to_icl(c3);
        return 1;
    }
    else if (strcmp(Exp->son->name, "ID") == 0)
    {
        int s_idx = look_up_name(Exp->son->value.type_charstar);
        int v_idx;
        if (ir_sym_idx[s_idx] == 0)
        {
            v_idx = var_idx++;
            ir_sym_idx[s_idx] = v_idx;
        }
        else
        {
            v_idx = ir_sym_idx[s_idx];
        }
        int t_idx = (idx == 0 ? tmp_idx++ : idx);
        InterCodes *c;
        Types type = look_up_type(Exp->son->value.type_charstar);
        if (type->kind == BASIC)
            c = assign_tmp2var_constructor(t_idx, v_idx);
        else
            c = refassign_constructor(t_idx, v_idx);
        add_to_icl(c);
        return 0;
    }
    else if (strcmp(Exp->son->name, "INT") == 0)
    {
        //printf("you have come to int!\n");
        int value = Exp->son->value.type_int;
        int t_idx = (idx == 0 ? tmp_idx++ : idx);
        InterCodes *c = assign_tmp2ival_constructor(t_idx, value);
        add_to_icl(c);
        return 0;
    }
    else
    {
        perror("translate_Exp has an unexpected float type!\n");
        return 0;
    }
}

int compute_array_size(Types type)
{
    if (type->kind != ARRAY)
    {
        printf("you shouldn't call compute_array_size!\n");
        return 0;
    }

    int size = 0;
    switch (type->u.array.elem->kind)
    {
    case BASIC:
        size = type->u.array.size * 4;
        break;
    case ARRAY:
        size = type->u.array.size * compute_array_size(type->u.array.elem);
        break;
    case STRUCTURE:
        size = type->u.array.size * compute_struct_size(type->u.structure);
        break;
    default:
        break;
    }
    return size;
}

int compute_struct_size(FieldList first_field)
{
    int size = 0;
    FieldList field = first_field;
    while (field != NULL)
    {
        if (field->type.kind == BASIC)
            size += 4;
        else if (field->type.kind == ARRAY)
            size += compute_array_size(&field->type);
        else
            size += compute_struct_size(field->type.u.structure);
        field = field->tail;
    }
    return size;
}

void translate_CompSt(TreeNode *CompSt)
{
    if (strcmp(CompSt->name, "CompSt") != 0)
    {
        printf("you should not call translate_CompSt!\n");
        return;
    }

    //DefList
    if (strcmp(CompSt->son->sibling->name, "DefList") == 0)
    {
        TreeNode *Def = CompSt->son->sibling->son;
        while (1)
        {
            TreeNode *Specifier = Def->son;
            int base_size;
            if (strcmp(Specifier->son->name, "StructSpecifier") == 0)
            {
                TreeNode *Tag = Specifier->son->son->sibling;
                TreeNode *ID = Tag->son;
                FieldList first_field = look_up_structtbl(ID->value.type_charstar);
                base_size = compute_struct_size(first_field);
                //printf("struct size is %d\n",base_size);
                TreeNode *Dec = Specifier->sibling->son;
                while (1)
                {
                    TreeNode *VarDec = Dec->son;
                    TreeNode *ID = VarDec->son;
                    if (strcmp(ID->name, "ID") != 0)
                    {
                        printf("Cannot translate: Code contains variables of multi-dimensional array type or parameters of array type.\n");
                        return;
                    }
                    int s_idx = look_up_name(ID->value.type_charstar);
                    int v_idx;
                    if (ir_sym_idx[s_idx] == 0)
                    {
                        v_idx = var_idx++;
                        ir_sym_idx[s_idx] = v_idx;
                    }
                    else
                    {
                        v_idx = ir_sym_idx[s_idx];
                    }
                    InterCodes *c = dec_var_constructor(base_size, v_idx);
                    add_to_icl(c);
                    if (Dec->sibling == NULL)
                    {
                        break;
                    }
                    else
                    {
                        Dec = Dec->sibling->sibling->son;
                    }
                }
            }
            else
            {
                TreeNode *Dec = Specifier->sibling->son;
                TreeNode *VarDec = Dec->son;
                TreeNode *ID = VarDec->son;
                while (1)
                {
                    if (strcmp(ID->name, "ID") != 0)
                    {
                        ID = ID->son;
                        int base_size = 4;
                        int dimension = 1;
                        while(strcmp(ID->name, "ID") != 0)
                        {
                            dimension++;
                            ID = ID->son;  
                        }
                        //printf("%s's dimension = %d\n", ID->value.type_charstar,dimension);
                        for(int i = 0;i < dimension; i++)
                        {
                            const char *suffix = "[]";
                            char *name = (char *)malloc(sizeof(char) * (strlen(ID->value.type_charstar) + 2 * i + 1));
                            strcpy(name, ID->value.type_charstar);
                            for (int j = 0; j < i; j++)
                                strcat(name, suffix);
                            //printf("%s ",name);
                            int tmps_idx = look_up_name(name);
                            //printf("%d\n", sym_tbl.symbols[tmps_idx].symuni.var.type.u.array.size);
                            base_size *= sym_tbl.symbols[tmps_idx].symuni.var.type.u.array.size;
                        }
                        //printf("base_size = %d\n", base_size);
                        int s_idx = look_up_name(ID->value.type_charstar);
                        int v_idx;
                        if (ir_sym_idx[s_idx] == 0)
                        {
                            v_idx = var_idx++;
                            ir_sym_idx[s_idx] = v_idx;
                        }
                        else
                        {
                            v_idx = ir_sym_idx[s_idx];
                        }
                        InterCodes *c = dec_var_constructor(base_size, v_idx);
                        add_to_icl(c);
                    }
                    else
                    {
                        if(VarDec->sibling!=NULL)
                        {
                            int s_idx = look_up_name(ID->value.type_charstar);
                            int v_idx;
                            if (ir_sym_idx[s_idx] == 0)
                            {
                                v_idx = var_idx++;
                                ir_sym_idx[s_idx] = v_idx;
                            }
                            else
                            {
                                v_idx = ir_sym_idx[s_idx];
                            }
                            int t1_idx = tmp_idx++;
                            int if_addr = translate_Exp(VarDec->sibling->sibling, t1_idx);
                            InterCodes *c1;
                            if (if_addr == 0)
                                c1 = assign_var2tmp_constructor(v_idx, t1_idx);
                            else
                                c1 = assign_var2tmpaddr_constructor(v_idx, t1_idx);
                            add_to_icl(c1);
                        }
                    }
                    if (Dec->sibling == NULL)
                    {
                        break;
                    }
                    else
                    {
                        Dec = Dec->sibling->sibling->son;
                        VarDec = Dec->son;
                        ID = VarDec->son;
                    }
                }
            }
            if (Def->sibling == NULL)
            {
                break;
            }
            else
            {
                Def = Def->sibling->son;
            }
        }
    }

    //StmtList
    if (strcmp(CompSt->son->sibling->name, "StmtList") == 0)
    {
        TreeNode *Stmt = CompSt->son->sibling->son;
        while (1)
        {
            translate_Stmt(Stmt);
            if (Stmt->sibling != NULL)
                Stmt = Stmt->sibling->son;
            else
                break;
        }
    }
    else if (CompSt->son->sibling->sibling != NULL &&
             strcmp(CompSt->son->sibling->sibling->name, "StmtList") == 0)
    {
        TreeNode *Stmt = CompSt->son->sibling->sibling->son;
        while (1)
        {
            translate_Stmt(Stmt);
            if (Stmt->sibling != NULL)
                Stmt = Stmt->sibling->son;
            else
                break;
        }
    }
}

void translate_Stmt(TreeNode *Stmt)
{
    if (strcmp(Stmt->name, "Stmt") != 0)
    {
        printf("you should not call translate_Stmt!\n");
        return;
    }

    if (strcmp(Stmt->son->name, "Exp") == 0)
    {
        translate_Exp(Stmt->son, 0);
    }
    else if (strcmp(Stmt->son->name, "CompSt") == 0)
    {
        translate_CompSt(Stmt->son);
    }
    else if (strcmp(Stmt->son->name, "RETURN") == 0)
    {
        int t_idx = tmp_idx++;
        int if_addr = translate_Exp(Stmt->son->sibling, t_idx);
        InterCodes *c;
        if(if_addr == 0)
            c = ret_constructor(t_idx);
        else
            c = ret_constructor_addr(t_idx);
        add_to_icl(c);
    }
    else if (strcmp(Stmt->son->name, "IF") == 0 &&
             Stmt->son->sibling->sibling->sibling->sibling->sibling == NULL)
    {
        int lb1_idx = label_idx++;
        int lb2_idx = label_idx++;
        translate_Cond(Stmt->son->sibling->sibling, lb1_idx, lb2_idx);
        InterCodes *c1 = true_label_constructor(lb1_idx);
        add_to_icl(c1);
        translate_Stmt(Stmt->son->sibling->sibling->sibling->sibling);
        InterCodes *c2 = true_label_constructor(lb2_idx);
        add_to_icl(c2);
    }
    else if (strcmp(Stmt->son->name, "IF") == 0 &&
             Stmt->son->sibling->sibling->sibling->sibling->sibling != NULL)
    {
        int lb1_idx = label_idx++;
        int lb2_idx = label_idx++;
        int lb3_idx = label_idx++;
        translate_Cond(Stmt->son->sibling->sibling, lb1_idx, lb2_idx);
        InterCodes *c1 = true_label_constructor(lb1_idx);
        add_to_icl(c1);
        translate_Stmt(Stmt->son->sibling->sibling->sibling->sibling);
        InterCodes *c2 = goto_label_constructor(lb3_idx);
        add_to_icl(c2);
        InterCodes *c3 = true_label_constructor(lb2_idx);
        add_to_icl(c3);
        translate_Stmt(Stmt->son->sibling->sibling->sibling->sibling->sibling->sibling);
        InterCodes *c4 = true_label_constructor(lb3_idx);
        add_to_icl(c4);
    }
    else if (strcmp(Stmt->son->name, "WHILE") == 0)
    {
        int lb1_idx = label_idx++;
        int lb2_idx = label_idx++;
        int lb3_idx = label_idx++;
        InterCodes *c1 = true_label_constructor(lb1_idx);
        add_to_icl(c1);
        translate_Cond(Stmt->son->sibling->sibling, lb2_idx, lb3_idx);
        InterCodes *c2 = true_label_constructor(lb2_idx);
        add_to_icl(c2);
        translate_Stmt(Stmt->son->sibling->sibling->sibling->sibling);
        InterCodes *c3 = goto_label_constructor(lb1_idx);
        add_to_icl(c3);
        InterCodes *c4 = true_label_constructor(lb3_idx);
        add_to_icl(c4);
    }
    else
    {
        perror("translate_Stmt has an unexpected case!\n");
    }
}

int get_relop(TreeNode *Relop)
{
    return Relop->value.type_int;
}

void translate_Cond(TreeNode *Exp, int label_true, int label_false)
{
    if (Exp->son->sibling != NULL &&
        strcmp(Exp->son->sibling->name, "RELOP") == 0)
    {
        int t1_idx = tmp_idx++;
        int t2_idx = tmp_idx++;
        int if_addr1 = translate_Exp(Exp->son, t1_idx);
        int if_addr2 = translate_Exp(Exp->son->sibling->sibling, t2_idx);
        int op_idx = get_relop(Exp->son->sibling);
        InterCodes *c1;
        if (if_addr1 == 0 && if_addr2 == 0)
            c1 = code_label_constructor(t1_idx, t2_idx, op_idx, label_true);
        else if (if_addr1 == 0 && if_addr2 == 1)
            c1 = code_label_constructor_2ndisaddr(t1_idx, t2_idx, op_idx, label_true);
        else if (if_addr1 == 1 && if_addr2 == 0)
            c1 = code_label_constructor_1stisaddr(t1_idx, t2_idx, op_idx, label_true);
        else
            c1  = code_label_constructor_allareaddr(t1_idx, t2_idx, op_idx, label_true);
        add_to_icl(c1);
        InterCodes *c2 = goto_label_constructor(label_false);
        add_to_icl(c2);
    }
    else if (strcmp(Exp->son->name, "NOT") == 0)
    {
        translate_Cond(Exp->son->sibling, label_false, label_true);
    }
    else if (Exp->son->sibling != NULL &&
             strcmp(Exp->son->sibling->name, "AND") == 0)
    {
        int lb_idx = label_idx++;
        translate_Cond(Exp->son, lb_idx, label_false);
        InterCodes *c = true_label_constructor(lb_idx);
        add_to_icl(c);
        translate_Cond(Exp->son->sibling->sibling, label_true, label_false);
    }
    else if (Exp->son->sibling != NULL &&
             strcmp(Exp->son->sibling->name, "OR") == 0)
    {
        int lb_idx = label_idx++;
        translate_Cond(Exp->son, label_true, lb_idx);
        InterCodes *c = true_label_constructor(lb_idx);
        add_to_icl(c);
        translate_Cond(Exp->son->sibling->sibling, label_true, label_false);
    }
    else
    {
        int t1_idx = tmp_idx++;
        int is_addr = translate_Exp(Exp, t1_idx);
        int t2_idx = tmp_idx++;
        InterCodes *c1 = assign_tmp2ival_constructor(t2_idx, 0);
        add_to_icl(c1);
        InterCodes *c2;
        if(is_addr == 0)
            c2 = code_label_constructor(t1_idx, t2_idx, 6, label_true);
        else
            c2 = code_label_constructor_1stisaddr(t1_idx, t2_idx, 6, label_true);
        add_to_icl(c2);
        InterCodes *c3 = goto_label_constructor(label_false);
        add_to_icl(c3);
    }
}

void translate_Args(TreeNode *Args, struct ArgList *arglist)
{
    if (strcmp(Args->name, "Args") != 0)
    {
        printf("you should not call translate_Args!\n");
        return;
    }

    if (Args->son->sibling == NULL)
    {
        int t_idx = tmp_idx++;
        int if_addr = translate_Exp(Args->son, t_idx);
        if (if_addr == 1)
        {
            InterCodes *c = assign_tmp2tmpaddr_constructor(t_idx, t_idx);
            add_to_icl(c);
        }
        struct Arg *arg = (struct Arg *)malloc(sizeof(struct Arg));
        arg->value = t_idx;
        arg->next = arglist->head;
        arglist->head = arg;
    }
    else
    {
        int t_idx = tmp_idx++;
        int if_addr = translate_Exp(Args->son, t_idx);
        if (if_addr == 1)
        {
            InterCodes *c = assign_tmp2tmpaddr_constructor(t_idx, t_idx);
            add_to_icl(c);
        }
        struct Arg *arg = (struct Arg *)malloc(sizeof(struct Arg));
        arg->value = t_idx;
        arg->next = arglist->head;
        arglist->head = arg;
        translate_Args(Args->son->sibling->sibling, arglist);
    }
}

void translate_tree(TreeNode *Node)
{
    if (Node != NULL)
    { // evety stmt, including stmt in compst
        if (strcmp(Node->name, "FunDec") == 0)
        {
            char *f_name = Node->son->value.type_charstar;
            InterCodes *c = dec_func_constructor(f_name);
            add_to_icl(c);
            TreeNode *VarList = Node->son->sibling->sibling;
            if (strcmp(VarList->name, "VarList") == 0)
            {
                TreeNode *ParamDec = VarList->son;
                while (1)
                {
                    TreeNode *VarDec = ParamDec->son->sibling;
                    if (strcmp(VarDec->son->name, "ID") == 0)
                    {
                        int s_idx = look_up_name(VarDec->son->value.type_charstar);
                        int v_idx;
                        if (ir_sym_idx[s_idx] == 0)
                        {
                            v_idx = var_idx++;
                            ir_sym_idx[s_idx] = v_idx;
                        }
                        else
                        {
                            v_idx = ir_sym_idx[s_idx];
                        }
                        InterCodes *c = param_constructor(v_idx);
                        add_to_icl(c);
                    }
                    else
                    {//array
                        VarDec = VarDec->son;
                        if(strcmp(VarDec->son->name, "ID") != 0)
                            printf("TODO\n");
                        else
                        {
                            int s_idx = look_up_name(VarDec->son->value.type_charstar);
                            int v_idx;
                            if (ir_sym_idx[s_idx] == 0)
                            {
                                v_idx = var_idx++;
                                ir_sym_idx[s_idx] = v_idx;
                            }
                            else
                            {
                                v_idx = ir_sym_idx[s_idx];
                            }
                            InterCodes *c = param_constructor(v_idx);
                            add_to_icl(c);
                        }
                    }
                    if (ParamDec->sibling == NULL)
                        break;
                    else
                    {
                        ParamDec = ParamDec->sibling->sibling->son;
                    }
                }
            }
        }
        else if (strcmp(Node->name, "CompSt") == 0)
        {
            translate_CompSt(Node);//should not translate son
            translate_tree(Node->sibling);
            return;
        }
        translate_tree(Node->son);
        translate_tree(Node->sibling);
    }
}