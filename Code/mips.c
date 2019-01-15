#include "mips.h"

void init_all()
{
    offset = 0;
    max_frame_size = FRAME_SIZE;
    varlist = NULL;
    reg_idx = 8;
    para_ctr = 0;
    arg_ctr = 0;
    init_reg_name();
}

void increase_frame(FILE *f)//dynamically increase frame
{
    max_frame_size += FRAME_SIZE;
    fprintf(f, "subu $sp, $sp, %d\n", FRAME_SIZE);
}

void init_reg_name()
{
    for (int i = 0; i < MAX_REG; i++)
    {
        memset(reg[i].name, 0, 10);
        reg[i].var = NULL;
    }

    char *tmp = malloc(2);
    memset(tmp, 0, 2);
    strcpy(reg[0].name, "$zero");
    strcpy(reg[1].name, "$at");
    for (int i = 2; i <= 3; i++)
    {
        sprintf(tmp, "%d", i - 2);
        reg[i].name[0] = '$';
        reg[i].name[1] = 'v';
        strcat(reg[i].name, tmp);
    }
    for (int i = 4; i <= 7; i++)
    {
        sprintf(tmp, "%d", i - 4);
        reg[i].name[0] = '$';
        reg[i].name[1] = 'a';
        strcat(reg[i].name, tmp);
    }
    for (int i = 8; i <= 15; i++)
    {
        sprintf(tmp, "%d", i - 8);
        reg[i].name[0] = '$';
        reg[i].name[1] = 't';
        strcat(reg[i].name, tmp);
    }
    for (int i = 16; i <= 23; i++)
    {
        sprintf(tmp, "%d", i - 16);
        reg[i].name[0] = '$';
        reg[i].name[1] = 's';
        strcat(reg[i].name, tmp);
    }
    for (int i = 24; i <= 25; i++)
    {
        sprintf(tmp, "%d", i - 16);
        reg[i].name[0] = '$';
        reg[i].name[1] = 't';
        strcat(reg[i].name, tmp);
    }
    for (int i = 26; i <= 27; i++)
    {
        sprintf(tmp, "%d", i - 26);
        reg[i].name[0] = '$';
        reg[i].name[1] = 'k';
        strcat(reg[i].name, tmp);
    }
    strcpy(reg[28].name, "$gp");
    strcpy(reg[29].name, "$sp");
    strcpy(reg[30].name, "$fp");
    strcpy(reg[31].name, "$ra");

    // for(int i = 0; i<MAX_REG; i++)
    //     printf("%d: %s\n", i, reg[i].name);
}

void add_to_varlist(Var v)
{
    if (varlist == NULL)
        varlist = v;
    else
    {
        Var ptr = varlist;
        while (ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = v;
    }
}

int get_reg(Operand op, FILE *f)
{
    //printf("you have come here1\n");
    char *name = malloc(MAX_NAME_SIZE);
    memset(name, 0, MAX_NAME_SIZE);
    switch (op->kind)
    {
    case VARIABLE:
        name[0] = 'v';
        sprintf(name + 1, "%d", op->u.var_no);
        break;
    case TEMP:
    case ADDRESS:
        name[0] = 't';
        sprintf(name + 1, "%d", op->u.tmp_no);
        break;
    default:
        perror("you should not get an register for an constant!\n");
        return 0;
        break;
    }
    //printf("%s\n", name);

    Var v = find_in_list(name);
    //printf("you have come here2\n");

    int flag = 0;
    if (!v)
    {
        offset += 4;
        v = init_var(name, offset);
        while(offset > max_frame_size)
            increase_frame(f);
        //printf("%s\n", name);
        add_to_varlist(v);
        flag = 1;
    }
    //printf("you have come here3\n");
    if (v->reg == -1)
    {
        int idx = alloc_reg(f);
        v->reg = idx;
        reg[idx].var = v->name;
        if(!flag)// case: reg has been clear but use this value again:
        {        // should read value from stack(stack must store this value)
            load_mem2reg(v, f);
        }
    }
    return v->reg;
}

int alloc_reg(FILE *f)
{
    for (int i = 8; i <= 25; i++)
    {
        if (reg[i].var == NULL)
            return i;
    }

    int v_reg = reg_idx;
    //printf("%d\n", v_reg);
    //printf("%s\n", reg[v_reg].var);
    Var v = find_in_list(reg[v_reg].var);
    reg[v_reg].var = NULL;
    //printf("you come here1!\n");
    if (v != NULL)
    {
        //printf("you come here2!\n");
        save_reg2mem(v, f);
        v->reg = -1;
    }

    if (reg_idx < 25)
        reg_idx++;
    else
        reg_idx = 8;

    return v_reg;
}

Var init_var(char *name, int off)
{
    Var v = malloc(sizeof(struct _var));
    v->name = name;
    v->reg = -1;
    v->offset = off;
    v->next = NULL;
}

Var find_in_list(char *name)
{
    Var ptr = varlist;
    
    while (ptr != NULL)
    {
        if (strcmp(ptr->name, name) == 0)
            return ptr;
        ptr = ptr->next;
    }
    return ptr;
}

Operand con2reg(Operand cons, FILE *f)
{
    if(cons->kind != CONSTANT){
        perror("you should not call con2reg!\n");
        return NULL;
    }
    tmp_idx++;
    Operand l = ope_tmp_constructor(tmp_idx);
    struct InterCode c;
    c.kind = ASSIGN;
    c.u.assign.left = l;
    c.u.assign.right = cons;
    fprint_target_code(c, f);
    return l;
}

Operand addr2tmp(Operand addr, FILE *f)
{
    if(addr->kind != ADDRESS){
        perror("you should not call addr2tmp!\n");
        return NULL;
    }
    tmp_idx++;
    Operand l = ope_tmp_constructor(tmp_idx);
    struct InterCode c;
    c.kind = ASSIGN;
    c.u.assign.left = l;
    c.u.assign.right = addr;
    fprint_target_code(c, f);
    return l;
}

void add_big_var(Operand op, int size, FILE *f)
{
    if(op->kind != VARIABLE)
    {
        perror("you should not call add_big_var!\n");
        return;
    }

    char *name = malloc(MAX_NAME_SIZE);
    memset(name, 0, MAX_NAME_SIZE);
    name[0] = 'v';
    sprintf(name + 1, "%d", op->u.var_no);
    offset += size;
    Var v = init_var(name, offset);
    while(offset > max_frame_size)
        increase_frame(f);
    add_to_varlist(v);
}

void load_mem2reg(Var v, FILE *f)
{
    //addi $v1, $fp, -v->offset
    fprintf(f, "addi $v1, $fp, -%d\n", v->offset);
    
    //lw reg(v), 0($v1)
    fprintf(f, "lw ");
    fprint_reg(v->reg, f);
    fprintf(f, ", 0($v1)\n");
}

void save_reg2mem(Var v, FILE *f)
{
    //addi $v1, $fp, -v->offset
    fprintf(f, "addi $v1, $fp, -%d\n", v->offset);
    
    //sw reg(v), 0($v1)
    fprintf(f, "sw ");
    fprint_reg(v->reg, f);
    fprintf(f, ", 0($v1)\n");
}

void allreg2mem(FILE *f)
{
    Var ptr = varlist;
    while(ptr != NULL)
    {
        if(ptr->reg != -1)
        {
            save_reg2mem(ptr, f);
            reg[ptr->reg].var = NULL;
            ptr->reg = -1;
        }
        ptr = ptr->next;
    }
}

void fprint_reg(int idx, FILE *f)
{
    fprintf(f, "%s", reg[idx].name);
}

void process(int idx)
{
    scan_ir_tree(idx);
    init_all();
    target_code_gen("test.asm");
}

void target_code_gen(char *filename)
{
    FILE *fout = fopen(filename, "wb");
    preprocess(fout);
    struct InterCodes *i = icl.first;
    while (i != NULL)
    {
        fprint_target_code(i->code, fout);
        i = i->next;
    }
    fclose(fout);
}

void preprocess(FILE *f)
{
    fprintf(f, ".data\n");
    fprintf(f, "_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(f, "_ret: .asciiz \"\\n\"\n");
    fprintf(f, ".globl main\n");
    fprintf(f, ".text\n");
    fprintf(f, "read:\n");
    fprintf(f, "li $v0, 4\n");
    fprintf(f, "la $a0, _prompt\n");
    fprintf(f, "syscall\n");
    fprintf(f, "li $v0, 5\n");
    fprintf(f, "syscall\n");
    fprintf(f, "jr $ra\n");
    fprintf(f, "\nwrite:\n");
    fprintf(f, "li $v0, 1\n");
    fprintf(f, "syscall\n");
    fprintf(f, "li $v0, 4\n");
    fprintf(f, "la $a0, _ret\n");
    fprintf(f, "syscall\n");
    fprintf(f, "move $v0, $0\n");
    fprintf(f, "jr $ra\n");
}

void fprint_target_code(struct InterCode ic, FILE *f)
{
    int l_reg, r_reg, res_reg;
    Operand op1, op2;
    Var v;
    char *name = malloc(MAX_NAME_SIZE);
    switch (ic.kind)
    {
    case ASSIGN:
        switch (ic.u.assign.left->kind)
        {
        case VARIABLE:
        case TEMP:
            switch (ic.u.assign.right->kind)
            {
            case CONSTANT:
                l_reg = get_reg(ic.u.assign.left, f);
                fprintf(f, "li ");
                fprint_reg(l_reg, f);
                fprintf(f, ", %d\n",
                        ic.u.assign.right->u.value);
                break;
            case VARIABLE:
            case TEMP:
                l_reg = get_reg(ic.u.assign.left, f);
                r_reg = get_reg(ic.u.assign.right, f);
                fprintf(f, "move ");
                fprint_reg(l_reg, f);
                fprintf(f, ", ");
                fprint_reg(r_reg, f);
                fprintf(f, "\n");
                break;
            case ADDRESS:
                l_reg = get_reg(ic.u.assign.left, f);
                r_reg = get_reg(ic.u.assign.right, f);
                fprintf(f, "lw ");
                fprint_reg(l_reg, f);
                fprintf(f, ", 0(");
                fprint_reg(r_reg, f);
                fprintf(f, ")\n");
                break;
            default:
                break;
            }
            break;
        case ADDRESS:
            switch (ic.u.assign.right->kind)
            {
            case VARIABLE:
            case TEMP:
                l_reg = get_reg(ic.u.assign.left, f);
                r_reg = get_reg(ic.u.assign.right, f);
                fprintf(f, "sw ");
                fprint_reg(r_reg, f);
                fprintf(f, ", 0(");
                fprint_reg(l_reg, f);
                fprintf(f, ")\n");
                break;
            case ADDRESS:
                perror("ADDR TO ADDR MAKES NO SENSE!\n");
                break;
            case CONSTANT:
                op1 = con2reg(ic.u.assign.right, f);
                l_reg = get_reg(ic.u.assign.left, f);
                r_reg = get_reg(op1, f);
                fprintf(f, "sw ");
                fprint_reg(r_reg, f);
                fprintf(f, ", 0(");
                fprint_reg(l_reg, f);
                fprintf(f, ")\n");
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;
    case ADD:
        switch (ic.u.binop.result->kind)
        {
        case VARIABLE:
        case TEMP:
            switch (ic.u.binop.op1->kind)
            {
            case VARIABLE:
            case TEMP:
                switch (ic.u.binop.op2->kind)
                {
                case CONSTANT:
                    res_reg = get_reg(ic.u.binop.result, f);
                    l_reg = get_reg(ic.u.binop.op1, f);
                    fprintf(f, "addi ");
                    fprint_reg(res_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(l_reg, f);
                    fprintf(f, ", %d\n", ic.u.binop.op2->u.value);
                    break;
                case VARIABLE:
                case TEMP:
                    res_reg = get_reg(ic.u.binop.result, f);
                    l_reg = get_reg(ic.u.binop.op1, f);
                    r_reg = get_reg(ic.u.binop.op2, f);
                    fprintf(f, "add ");
                    fprint_reg(res_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(l_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(r_reg, f);
                    fprintf(f, "\n");
                    break;
                case ADDRESS:
                    op1 = addr2tmp(ic.u.binop.op2, f);
                    res_reg = get_reg(ic.u.binop.result, f);
                    l_reg = get_reg(ic.u.binop.op1, f);
                    r_reg = get_reg(op1, f);
                    fprintf(f, "add ");
                    fprint_reg(res_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(l_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(r_reg, f);
                    fprintf(f, "\n");
                    break;
                default:
                    break;
                }
                break;
            case ADDRESS:
                op1 = addr2tmp(ic.u.binop.op1, f);
                {
                    switch (ic.u.binop.op2->kind)
                    {
                    case CONSTANT:
                        res_reg = get_reg(ic.u.binop.result, f);
                        l_reg = get_reg(op1, f);
                        fprintf(f, "addi ");
                        fprint_reg(res_reg, f);
                        fprintf(f, ", ");
                        fprint_reg(l_reg, f);
                        fprintf(f, ", %d\n", ic.u.binop.op2->u.value);
                        break;
                    case VARIABLE:
                    case TEMP:
                        res_reg = get_reg(ic.u.binop.result, f);
                        l_reg = get_reg(op1, f);
                        r_reg = get_reg(ic.u.binop.op2, f);
                        fprintf(f, "add ");
                        fprint_reg(res_reg, f);
                        fprintf(f, ", ");
                        fprint_reg(l_reg, f);
                        fprintf(f, ", ");
                        fprint_reg(r_reg, f);
                        fprintf(f, "\n");
                        break;
                    case ADDRESS:
                        op2 = addr2tmp(ic.u.binop.op2, f);
                        res_reg = get_reg(ic.u.binop.result, f);
                        l_reg = get_reg(op1, f);
                        r_reg = get_reg(op2, f);
                        fprintf(f, "add ");
                        fprint_reg(res_reg, f);
                        fprintf(f, ", ");
                        fprint_reg(l_reg, f);
                        fprintf(f, ", ");
                        fprint_reg(r_reg, f);
                        fprintf(f, "\n");
                        break;
                    default:
                        break;
                    }
                }
                break;
            case CONSTANT:
                printf("TODO: consider if add's op1 is constant!\n");
                break;
            default:
                break;
            }
            break;
        case ADDRESS:
            printf("TODO: consider if add's result is addr!\n");
            break;
        default:
            break;
        }
        break;
    case SUB:
        switch (ic.u.binop.result->kind)
        {
        case VARIABLE:
        case TEMP:
            switch (ic.u.binop.op1->kind)
            {
            case VARIABLE:
            case TEMP:
                switch (ic.u.binop.op2->kind)
                {
                case CONSTANT:
                    res_reg = get_reg(ic.u.binop.result, f);
                    l_reg = get_reg(ic.u.binop.op1, f);
                    fprintf(f, "addi ");
                    fprint_reg(res_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(l_reg, f);
                    fprintf(f, ", -%d\n", ic.u.binop.op2->u.value);
                    break;
                case VARIABLE:
                case TEMP:
                    res_reg = get_reg(ic.u.binop.result, f);
                    l_reg = get_reg(ic.u.binop.op1, f);
                    r_reg = get_reg(ic.u.binop.op2, f);
                    fprintf(f, "sub ");
                    fprint_reg(res_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(l_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(r_reg, f);
                    fprintf(f, "\n");
                    break;
                case ADDRESS:
                    op1 = addr2tmp(ic.u.binop.op2, f);
                    res_reg = get_reg(ic.u.binop.result, f);
                    l_reg = get_reg(ic.u.binop.op1, f);
                    r_reg = get_reg(op1, f);
                    fprintf(f, "sub ");
                    fprint_reg(res_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(l_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(r_reg, f);
                    fprintf(f, "\n");
                    break;
                default:
                    break;
                }
                break;
            case ADDRESS:
                op1 = addr2tmp(ic.u.binop.op1, f);
                {
                    switch (ic.u.binop.op2->kind)
                    {
                    case CONSTANT:
                        res_reg = get_reg(ic.u.binop.result, f);
                        l_reg = get_reg(op1, f);
                        fprintf(f, "addi ");
                        fprint_reg(res_reg, f);
                        fprintf(f, ", ");
                        fprint_reg(l_reg, f);
                        fprintf(f, ", -%d\n", ic.u.binop.op2->u.value);
                        break;
                    case VARIABLE:
                    case TEMP:
                        res_reg = get_reg(ic.u.binop.result, f);
                        l_reg = get_reg(op1, f);
                        r_reg = get_reg(ic.u.binop.op2, f);
                        fprintf(f, "sub ");
                        fprint_reg(res_reg, f);
                        fprintf(f, ", ");
                        fprint_reg(l_reg, f);
                        fprintf(f, ", ");
                        fprint_reg(r_reg, f);
                        fprintf(f, "\n");
                        break;
                    case ADDRESS:
                        op2 = addr2tmp(ic.u.binop.op2, f);
                        res_reg = get_reg(ic.u.binop.result, f);
                        l_reg = get_reg(op1, f);
                        r_reg = get_reg(op2, f);
                        fprintf(f, "sub ");
                        fprint_reg(res_reg, f);
                        fprintf(f, ", ");
                        fprint_reg(l_reg, f);
                        fprintf(f, ", ");
                        fprint_reg(r_reg, f);
                        fprintf(f, "\n");
                        break;
                    default:
                        break;
                    }
                }
                break;
            case CONSTANT:
                printf("TODO: consider if sub's op1 is constant!\n");
                break;
            default:
                break;
            }
            break;
        case ADDRESS:
            printf("TODO: consider if sub's result is addr!\n");
            break;
        default:
            break;
        }
        break;
    case MUL:
        switch (ic.u.binop.result->kind)
        {
        case VARIABLE:
        case TEMP:
            switch (ic.u.binop.op1->kind)
            {
            case VARIABLE:
            case TEMP:
                switch (ic.u.binop.op2->kind)
                {
                case CONSTANT:
                    op2 = con2reg(ic.u.binop.op2, f);
                    res_reg = get_reg(ic.u.binop.result, f);
                    l_reg = get_reg(ic.u.binop.op1, f);
                    r_reg = get_reg(op2, f);
                    fprintf(f, "mul ");
                    fprint_reg(res_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(l_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(r_reg, f);
                    fprintf(f, "\n");
                    break;
                case VARIABLE:
                case TEMP:
                    res_reg = get_reg(ic.u.binop.result, f);
                    l_reg = get_reg(ic.u.binop.op1, f);
                    r_reg = get_reg(ic.u.binop.op2, f);
                    fprintf(f, "mul ");
                    fprint_reg(res_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(l_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(r_reg, f);
                    fprintf(f, "\n");
                    break;
                case ADDRESS:
                    printf("you should not mul an address\n");
                    break;
                default:
                    break;
                }
                break;
            case ADDRESS:
                printf("you should not mul an address\n");
                break;
            case CONSTANT:
                printf("TODO: consider if mul's op1 is constant!\n");
                break;
            default:
                break;
            }
            break;
        case ADDRESS:
            printf("TODO: consider if sub's result is addr!\n");
            break;
        default:
            break;
        }
        break;
    case DIV_:
        switch (ic.u.binop.result->kind)
        {
        case VARIABLE:
        case TEMP:
            switch (ic.u.binop.op1->kind)
            {
            case VARIABLE:
            case TEMP:
                switch (ic.u.binop.op2->kind)
                {
                case CONSTANT:
                    op2 = con2reg(ic.u.binop.op2, f);
                    res_reg = get_reg(ic.u.binop.result, f);
                    l_reg = get_reg(ic.u.binop.op1, f);
                    r_reg = get_reg(op2, f);
                    fprintf(f, "div ");
                    fprint_reg(l_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(r_reg, f);
                    fprintf(f, "\nmflo ");
                    fprint_reg(res_reg, f);
                    fprintf(f, "\n");
                    break;
                case VARIABLE:
                case TEMP:
                    res_reg = get_reg(ic.u.binop.result, f);
                    l_reg = get_reg(ic.u.binop.op1, f);
                    r_reg = get_reg(ic.u.binop.op2, f);
                    fprintf(f, "div ");
                    fprint_reg(l_reg, f);
                    fprintf(f, ", ");
                    fprint_reg(r_reg, f);
                    fprintf(f, "\nmflo ");
                    fprint_reg(res_reg, f);
                    fprintf(f, "\n");
                    break;
                case ADDRESS:
                    printf("you should not div an address\n");
                    break;
                default:
                    break;
                }
                break;
            case ADDRESS:
                printf("you should not div an address\n");
                break;
            case CONSTANT:
                printf("TODO: consider if div's op1 is constant!\n");
                break;
            default:
                break;
            }
            break;
        case ADDRESS:
            printf("TODO: consider if div's result is addr!\n");
            break;
        default:
            break;
        }
        break;
    case RETURN_:

        // fprintf(f, "subu $sp, $sp, 4\n");
        // fprintf(f, "sw $ra, 0($sp)\n");
        // fprintf(f, "subu $sp, $sp, 4\n");
        // fprintf(f, "sw $fp, 0($sp)\n");
        // fprintf(f, "subu $sp, $sp, %d\n", framesize);
        // fprintf(f, "addi $fp, $sp, %d\n", framesize + 8);


        fprintf(f, "subu $sp, $fp, 8\n");
        fprintf(f, "lw $fp, 0($sp)\n");
        fprintf(f, "addi $sp, $sp, 4\n");
        fprintf(f, "lw $ra, 0($sp)\n");
        fprintf(f, "addi $sp, $sp, 4\n");


        switch(ic.u.ret.op->kind)
        {
            case CONSTANT:
                op1 = con2reg(ic.u.ret.op, f);
                res_reg = get_reg(op1, f);
                fprintf(f, "move $v0, ");
                fprint_reg(res_reg, f);
                fprintf(f, "\njr $ra\n");
                break;
            case VARIABLE:
            case TEMP:
                res_reg = get_reg(ic.u.ret.op, f);
                fprintf(f, "move $v0, ");
                fprint_reg(res_reg, f);
                fprintf(f, "\njr $ra\n");
                break;
            case ADDRESS:
                perror("you can't return an address in this lab!\n");
                break;
            default:
                break;
        }
        break;
    case LABEL_CODE:
        op1 = ic.u.if_goto.left;
        op2 = ic.u.if_goto.right;
        switch(ic.u.if_goto.left->kind)
        {
            case CONSTANT:
                op1 = con2reg(ic.u.if_goto.left, f);
                break;
            case ADDRESS:
                perror("LABEL_CODE don't accept an address!\n");
                break;
            default:
                break;
        }
        switch(ic.u.if_goto.right->kind)
        {
            case CONSTANT:
                op2 = con2reg(ic.u.if_goto.right, f);
                break;
            case ADDRESS:
                perror("LABEL_CODE don't accept an address!\n");
                break;
            default:
                break;
        }
        l_reg = get_reg(op1, f);
        r_reg = get_reg(op2, f);

        allreg2mem(f);//should do the same thing as Label

        switch(ic.u.if_goto.relop)
        {
        case 1:
            fprintf(f, "bgt ");
            break;
        case 2:
            fprintf(f, "blt ");
            break;
        case 3:
            fprintf(f, "bge");
            break;
        case 4:
            fprintf(f, "ble ");
            break;
        case 5:
            fprintf(f, "beq ");
            break;
        case 6:
            fprintf(f, "bne ");
            break;
        default:
            perror("something must be wrong in relop!\n");
            break;
        }
        fprint_reg(l_reg, f);
        fprintf(f, ", ");
        fprint_reg(r_reg, f);
        fprintf(f, ", ");
        fprint_label(ic.u.if_goto.lb_idx, f);
        fprintf(f, "\n");
        break;
    case LABEL_TRUE: //you must save every register into memory and clear registers
        allreg2mem(f);
        fprint_label(ic.u.lb_idx, f);
        fprintf(f, ":\n");
        break;
    case LABEL_GOTO:
        allreg2mem(f);//should do the same thing as Label
        fprintf(f, "j ");
        fprint_label(ic.u.lb_idx, f);
        fprintf(f, "\n");
        break;
    case READ:
        fprintf(f, "jal read\nmove ");
        res_reg = get_reg(ic.u.call_func.op, f);
        fprint_reg(res_reg, f);
        fprintf(f, ", $v0\n");
        break;
    case WRITE:
        res_reg = get_reg(ic.u.call_func.op, f);
        fprintf(f, "move $a0, ");
		fprint_reg(res_reg, f);
        fprintf(f, "\njal write\n");
        break;
    case CALLFUNC:

        allreg2mem(f);
        arg_ctr = 0;

        if(ic.u.call_func.op->kind == ADDRESS){
            perror("CALLFUNC don't accept an address as an arg in this lab!\n");
            return;
        }

        fprintf(f, "jal %s\nmove ", ic.u.call_func.fun);
        res_reg = get_reg(ic.u.call_func.op, f);
        fprint_reg(res_reg, f);
        fprintf(f, ", $v0\n");
        break;
    case FUNCTION:
        fprintf(f, "\n%s:\n", ic.u.fun);
        offset = 12;
        // subu $sp, $sp, framesizeg
        // sw $ra, (framesizeg - 4)($sp)
        // sw $fp, (framesizeg - 8)($sp)
        // addi $fp, $sp, framesizeg
        fprintf(f, "subu $sp, $sp, 4\n");
        fprintf(f, "sw $ra, 0($sp)\n");
        fprintf(f, "subu $sp, $sp, 4\n");
        fprintf(f, "sw $fp, 0($sp)\n");
        fprintf(f, "subu $sp, $sp, %d\n", FRAME_SIZE);
        fprintf(f, "addi $fp, $sp, %d\n", FRAME_SIZE + 8);

        para_ctr = 0;//compute para_num;

        break;
    case ARG:
        if(arg_ctr <= 3)
        {
            l_reg = get_reg(ic.u.arg.op, f);
            fprintf(f, "move $a%d, ", arg_ctr);
            fprint_reg(l_reg, f);
            fprintf(f, "\n");
            arg_ctr++;
        }
        else
        {
            l_reg = get_reg(ic.u.arg.op, f);
            fprintf(f, "sw ");
            fprint_reg(l_reg, f);
            fprintf(f, ", %d($sp)\n", (arg_ctr-3) * 4);
            arg_ctr++;
            // printf("TODO: arguments are too many!\n");
            // return;
        }
        break;
    case PARAM:
        if(para_ctr <= 3)
        {
            l_reg = get_reg(ic.u.param.op, f);
            fprintf(f, "move ");
            fprint_reg(l_reg, f);
            fprintf(f, ", $a%d\n", para_ctr);
            para_ctr++;
        }
        else
        {
            l_reg = get_reg(ic.u.param.op, f);
            fprintf(f, "lw ");
            fprint_reg(l_reg, f);
            fprintf(f, ", %d($fp)\n", (para_ctr-3) * 4);
            para_ctr++;
            // printf("TODO: parameters are too many!\n");
            // return;
        }
        break;
    case REFASSIGN:
        if(ic.u.refass.left->kind != TEMP)
        {
            perror("Refassign's left is not a temp: this can't happen!\n");
            return;
        }
        if(ic.u.refass.right->kind != VARIABLE)
        {
            perror("Refassign's right is not a var: this can't happen!\n");
            return;
        }

        memset(name, 0, MAX_NAME_SIZE);
        name[0] = 'v';
        sprintf(name + 1, "%d", ic.u.refass.right->u.var_no);
        v = find_in_list(name);
        if(v == NULL)
        {
            perror("Refassign's right must have declared!\n");
            return;
        }

        res_reg = get_reg(ic.u.refass.left, f);
        fprintf(f, "addi ");
        fprint_reg(res_reg, f);
        fprintf(f, ", $fp, -%d\n", v->offset);
        break;
    case DEC:
        add_big_var(ic.u.dec.op, ic.u.dec.size, f);
        break;
    default:
        break;
    }
}