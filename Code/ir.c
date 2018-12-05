#include "ir.h"

void init_intercodeslist()
{
    icl.first = NULL;
    icl.last = NULL;
}

void test_constructor()
{
    struct InterCodes *tmp = (struct InterCodes *)malloc(sizeof(struct InterCodes));
    //printf("sizeof is %d\n",sizeof(*tmp));
    tmp->code.kind = ASSIGN;
    Operand r = (Operand)malloc(sizeof(struct Operand_));
    r->kind = CONSTANT;
    r->u.value = 1;
    Operand l = (Operand)malloc(sizeof(struct Operand_));
    l->kind = VARIABLE;
    l->u.var_no = 0;
    tmp->code.u.assign.left = l;
    tmp->code.u.assign.right = r;
    tmp->next = NULL;
    tmp->prev = NULL;
    if (icl.first == NULL)
        icl.first = icl.last = tmp;
    else
    {
        icl.last->next = tmp;
        tmp->prev = icl.last;
        icl.last = tmp;
    }
}

void print_intercodeslist()
{
    struct InterCodes *i = icl.first;
    while (i != NULL)
    {
        print_internode(i->code);
        i = i->next;
    }
}

void print_internode(struct InterCode ic)
{
    switch (ic.kind)
    {
    case ASSIGN:
        switch (ic.u.assign.left->kind)
        {
        case VARIABLE:
            printf("v%d", ic.u.assign.left->u.var_no);
            break;
        case ADDRESS:
            printf("*v%d", ic.u.assign.left->u.var_no);
            break;
        default:
            break;
        }

        printf(" := ");

        switch (ic.u.assign.right->kind)
        {
        case VARIABLE:
            printf("v%d\n", ic.u.assign.right->u.var_no);
            break;
        case ADDRESS:
            printf("*v%d\n", ic.u.assign.right->u.var_no);
            break;
        case CONSTANT:
            printf("#%d\n", ic.u.assign.right->u.value);
            break;
        default:
            break;
        }
        break;
    case ADD: //TODO;
        break;
    case SUB:
        break;
    case MUL:
        break;
    default:
        break;
    }
}

void scan_ir_tree()
{
    init_intercodeslist();
    test_constructor();
    print_intercodeslist();
    file_out("test.ir");
}

void file_out(char *filename)
{
    FILE *fout = fopen(filename, "wb");
    struct InterCodes *i = icl.first;
    fprintf(fout,"FUNCTION main :\n");
    fprintf(fout,"DEC v0 8\n");
    while (i != NULL)
    {
        struct InterCode ic = icl.first->code;
        switch (ic.kind)
        {
        case ASSIGN:
            switch (ic.u.assign.left->kind)
            {
            case VARIABLE:
                fprintf(fout,"v%d", ic.u.assign.left->u.var_no);
                break;
            case ADDRESS:
                fprintf(fout,"*v%d", ic.u.assign.left->u.var_no);
                break;
            default:
                break;
            }

            fprintf(fout, " := ");

            switch (ic.u.assign.right->kind)
            {
            case VARIABLE:
                fprintf(fout, "v%d\n", ic.u.assign.right->u.var_no);
                break;
            case ADDRESS:
                fprintf(fout, "*v%d\n", ic.u.assign.right->u.var_no);
                break;
            case CONSTANT:
                fprintf(fout, "#%d\n", ic.u.assign.right->u.value);
                break;
            default:
                break;
            }
            break;
        case ADD: //TODO;
            break;
        case SUB:
            break;
        case MUL:
            break;
        default:
            break;
        }
        i = i->next;
    }
    fclose(fout);
}