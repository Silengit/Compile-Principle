#include "symtable.h"

void add_symbol(Symbols s)
{
    sym_tbl.symbols[sym_idx] = s;
    sym_tbl.size = sym_idx + 1;
    sym_idx++;
}

void add_read_and_write()
{
    Symbols s0; // write's parameter
    s0.symkind = FIELDVAR;
    s0.symuni.fieldvar.name = "_parameter_of_write";
    s0.symuni.fieldvar.tail = NULL;
    s0.symuni.fieldvar.type.kind = BASIC;
    s0.symuni.fieldvar.type.u.basic = 1;
    add_symbol(s0);

    Symbols s1;
    s1.symkind = FUN;
    s1.symuni.func.name = "read";
    s1.symuni.func.args = NULL;
    s1.symuni.func.ret.kind = BASIC;
    s1.symuni.func.ret.u.basic = 1;
    add_symbol(s1);

    Symbols s2;
    s2.symkind = FUN;
    s2.symuni.func.name = "write";
    s2.symuni.func.args = &sym_tbl.symbols[0].symuni.fieldvar;
    s2.symuni.func.ret.kind = BASIC;
    s2.symuni.func.ret.u.basic = 0;
    add_symbol(s2);
}

void add_struct(Struct s)
{
    struct_tbl.structs[str_idx] = s;
    struct_tbl.size = str_idx + 1;
    str_idx++;
}

void print_symbol(Symbols s)
{
    switch (s.symkind)
    {
    case FUN:
        printf("(function)%s: ", s.symuni.func.name);
        switch (s.symuni.func.ret.kind)
        {
        case BASIC:
            switch (s.symuni.func.ret.u.basic)
            {
            case 0:
                printf("void\n");
                break;
            case 1:
                printf("int\n");
                break;
            case 2:
                printf("float\n");
                break;
            default:
                perror("print_symbol has an error!\n");
                break;
            }
            break;
        case ARRAY:
            printf("no chance should a function return array.\n");
            break;
        case STRUCTURE:
            printf("struct type\n");
            break;
        default:
            perror("print_symbol has an error!\n");
            break;
        }
        break;
    case VAR:
        printf("(var)%s: ", s.symuni.var.name);
        switch (s.symuni.var.type.kind)
        {
        case BASIC:
            switch (s.symuni.var.type.u.basic)
            {
            case 0:
                printf("void\n");
                break;
            case 1:
                printf("int\n");
                break;
            case 2:
                printf("float\n");
                break;
            default:
                perror("print_symbol has an error!\n");
                break;
            }
            break;
        case ARRAY:
            printf("array type\n");
            break;
        case STRUCTURE:
            printf("struct type\n");
            break;
        default:
            perror("print_symbol has an error!\n");
            break;
        }
        break;
    case FIELDVAR:
        printf("(fieldvar)%s: ", s.symuni.fieldvar.name);
        switch (s.symuni.fieldvar.type.kind)
        {
        case BASIC:
            switch (s.symuni.fieldvar.type.u.basic)
            {
            case 0:
                printf("void\n");
                break;
            case 1:
                printf("int\n");
                break;
            case 2:
                printf("float\n");
                break;
            default:
                perror("print_symbol has an error!\n");
                break;
            }
            break;
        case ARRAY:
            printf("array type\n");
            break;
        case STRUCTURE:
            printf("struct type\n");
            break;
        default:
            perror("print_symbol has an error!\n");
            break;
        }
        break;
    default:
        perror("print_symbol has an error!\n");
        break;
    }
}

void print_all_symbols()
{
    for (int i = 0; i < sym_tbl.size; i++)
    {
        print_symbol(sym_tbl.symbols[i]);
    }
}

Symbols init_symbol_func(TreeNode *typeNode, TreeNode *idNode, FieldList args)
{ //typeNode is TYPE or StructSpecifier, idNode is ID
    //printf("%d\n",sizeof(struct Symbol_));
    Symbols s;
    s.symkind = FUN;
    s.symuni.func.name = idNode->value.type_charstar;
    s.symuni.func.args = args;
    //handle type
    //printf("typename: %s\n", typeNode->name);
    if (strcmp(typeNode->name, "TYPE") == 0)
    {
        s.symuni.func.ret.kind = BASIC;
        switch (typeNode->type)
        {
        case (_CHARSTAR):
            if (strcmp(typeNode->value.type_charstar, "int") == 0)
                s.symuni.func.ret.u.basic = 1;
            else if (strcmp(typeNode->value.type_charstar, "float") == 0)
                s.symuni.func.ret.u.basic = 2;
            else if (strcmp(typeNode->value.type_charstar, "void") == 0)
                s.symuni.func.ret.u.basic = 0;
            else
                perror("init_symbol_func has an error in basic type!\n");
            break;
        default:
            perror("init_symbol_func has an error in basic type!\n");
            break;
        }
    }
    else if (strcmp(typeNode->name, "StructSpecifier") == 0)
    {
        s.symuni.func.ret.kind = STRUCTURE;
        s.symuni.func.ret.u.structure = NULL;
        if (strcmp(typeNode->son->sibling->name, "Tag") == 0)
        {
            TreeNode *idNode = typeNode->son->sibling->son;
            FieldList firstfield = look_up_structtbl(idNode->value.type_charstar);
            if (firstfield == NULL)
            {
                printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",
                       idNode->lineno, idNode->value.type_charstar);
                Semantic_Error = 1;
                return s;
            }
            s.symuni.func.ret.u.structure = firstfield;
        }
    }
    else
        perror("init_symbol_func has an error in typeNode->name!\n");
    return s;
}

Symbols init_symbol_var(TreeNode *typeNode, TreeNode *idNode, int select)
{ //typeNode is TYPE or StructSpecifier, idNode is VarDec
    //when var, select = 0; when fieldvar, select = 1; else, select = -1
    Symbols s;
    //ignore the array at present
    if (select == 0)
    {
        if (strcmp(idNode->son->name, "ID") == 0)
        { //normal variable
            s.symkind = VAR;
            s.symuni.var.name = idNode->son->value.type_charstar;
            if (strcmp(typeNode->name, "TYPE") == 0)
            {
                s.symuni.var.type.kind = BASIC;
                if (strcmp(typeNode->value.type_charstar, "int") == 0)
                    s.symuni.var.type.u.basic = 1;
                else if (strcmp(typeNode->value.type_charstar, "float") == 0)
                    s.symuni.var.type.u.basic = 2;
                else
                    perror("init_symbol_var has an error!\n");
            }
            else if (strcmp(typeNode->name, "StructSpecifier") == 0)
            {
                s.symuni.var.type.kind = STRUCTURE;
                if (strcmp(typeNode->son->sibling->name, "Tag") != 0)
                    perror("init_symbol_var has an error(grammar)!\n");
                FieldList flag = look_up_structtbl(typeNode->son->sibling->son->value.type_charstar);
                if (flag == NULL)
                {
                    printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",
                           typeNode->lineno, typeNode->son->sibling->son->value.type_charstar);
                    Semantic_Error = 1;
                }
                s.symuni.var.type.u.structure = flag;
            }
            else
                perror("init_symbol_var has an error!\n");
        }
        else
            s = init_array(typeNode, idNode, select);
    }
    else if (select == 1)
    {
        if (strcmp(idNode->son->name, "ID") == 0)
        {
            s.symkind = FIELDVAR;
            s.symuni.fieldvar.name = idNode->son->value.type_charstar;
            if (strcmp(typeNode->name, "TYPE") == 0)
            {
                s.symuni.fieldvar.type.kind = BASIC;
                if (strcmp(typeNode->value.type_charstar, "int") == 0)
                    s.symuni.fieldvar.type.u.basic = 1;
                else if (strcmp(typeNode->value.type_charstar, "float") == 0)
                    s.symuni.fieldvar.type.u.basic = 2;
                else
                    perror("init_symbol_var has an error!\n");
            }
            else if (strcmp(typeNode->name, "StructSpecifier") == 0)
            {
                s.symuni.fieldvar.type.kind = STRUCTURE;
                if (strcmp(typeNode->son->sibling->name, "Tag") != 0)
                    perror("init_symbol_var has an error(grammar)!\n");
                FieldList flag = look_up_structtbl(typeNode->son->sibling->son->value.type_charstar);
                if (flag == NULL)
                {
                    printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",
                           typeNode->lineno, typeNode->son->sibling->son->value.type_charstar);
                    Semantic_Error = 1;
                }
                s.symuni.fieldvar.type.u.structure = flag;
            }
            else
            {
                perror("init_symbol_var has an error!\n");
            }
        }
        else
            s = init_array(typeNode, idNode, select);
        s.symuni.fieldvar.tail = NULL;
    }
    else
        perror("init_symbol_var has an error!\n");
    //printf("init_symbol_var is safe.\n");
    return s;
}

Symbols init_array(TreeNode *typeNode, TreeNode *idNode, int select)
{
    Symbols s;
    if (select == 0)
        s.symkind = VAR;
    else
        s.symkind = FIELDVAR;
    int dimension = 0;
    int size[MAX_ARRAY_SIZE];
    while (strcmp(idNode->son->name, "ID") != 0)
    {
        dimension++;
        size[dimension - 1] = idNode->son->sibling->sibling->value.type_int;
        idNode = idNode->son;
    }
    int tmp_dimension = dimension;
    while (tmp_dimension >= 0)
    {
        if (tmp_dimension != 0)
        {
            Symbols new_sym;
            new_sym.symkind = VAR;
            //printf("dimension is %d\n", tmp_dimension);
            const char *suffix = "[]";
            char *name = (char *)malloc(sizeof(char) * (strlen(idNode->son->value.type_charstar) + 2 * tmp_dimension + 1));
            strcpy(name, idNode->son->value.type_charstar);
            new_sym.symuni.var.name = name;
            for (int i = 0; i < tmp_dimension; i++)
                strcat(new_sym.symuni.var.name, suffix);
            //printf("%s\n", new_sym.symuni.var.name);
            if (tmp_dimension == dimension)
            {
                if (strcmp(typeNode->name, "TYPE") == 0)
                {
                    new_sym.symuni.var.type.kind = BASIC;
                    if (strcmp(typeNode->value.type_charstar, "int") == 0)
                        new_sym.symuni.var.type.u.basic = 1;
                    else if (strcmp(typeNode->value.type_charstar, "float") == 0)
                        new_sym.symuni.var.type.u.basic = 2;
                    else
                        perror("init_symbol_var has an error!\n");
                }
                else if (strcmp(typeNode->name, "StructSpecifier") == 0)
                {
                    new_sym.symuni.var.type.kind = STRUCTURE;
                    if (strcmp(typeNode->son->sibling->name, "Tag") != 0)
                        perror("init_symbol_var has an error(grammar)!\n");
                    FieldList flag = look_up_structtbl(typeNode->son->sibling->son->value.type_charstar);
                    if (flag == NULL)
                    {
                        printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",
                               typeNode->lineno, typeNode->son->sibling->son->value.type_charstar);
                        Semantic_Error = 1;
                    }
                    new_sym.symuni.var.type.u.structure = flag;
                }
                else
                {
                    perror("init_symbol_var has an error!\n");
                }
            }
            else
            {
                new_sym.symuni.var.type.kind = ARRAY;
                new_sym.symuni.var.type.u.array.elem = &sym_tbl.symbols[sym_idx - 1].symuni.var.type;
                new_sym.symuni.var.type.u.array.size = size[dimension - tmp_dimension - 1];
            }
            add_symbol(new_sym);
        }
        else
        {
            if (select == 0)
            {
                s.symuni.var.name = idNode->son->value.type_charstar;
                s.symuni.var.type.kind = ARRAY;
                s.symuni.var.type.u.array.elem = &sym_tbl.symbols[sym_idx - 1].symuni.var.type;
                s.symuni.var.type.u.array.size = size[dimension - tmp_dimension - 1];
            }
            else
            {
                s.symuni.fieldvar.name = idNode->son->value.type_charstar;
                s.symuni.fieldvar.type.kind = ARRAY;
                s.symuni.fieldvar.type.u.array.elem = &sym_tbl.symbols[sym_idx - 1].symuni.var.type;
                s.symuni.fieldvar.type.u.array.size = size[dimension - tmp_dimension - 1];
                s.symuni.fieldvar.tail = NULL;
            }
        }
        tmp_dimension--;
    }
    return s;
}

Struct init_struct(TreeNode *defList, TreeNode *idNode)
{
    Struct s;
    s.name = idNode->value.type_charstar;
    TreeNode *defNode = defList->son;
    s.first = NULL;
    FieldList last = NULL;
    while (defNode != NULL)
    {
        if (s.first == NULL)
        {
            s.first = handle_def(defNode, 1);
            last = s.first;
        }
        else
        {
            last->tail = handle_def(defNode, 1); // handle_def() return head
            last = last->tail;
        }
        if (defNode->sibling != NULL)
            defNode = defNode->sibling->son;
        else
            break;
    }
    // FieldList f = s.first;
    // while (f)
    // {
    //     printf("%s\n", f->name);
    //     f = f->tail;
    // }
    return s;
}

void handle_compstmt(TreeNode *CompSt, Symbols ret)
{
    //printf("you come here!\n");
    if(strcmp(CompSt->name, "CompSt") != 0)
    {
        printf("you should not call this function!\n");
        return;
    }
    if (strcmp(CompSt->son->sibling->name, "DefList") == 0)
    {
        TreeNode *defNode = CompSt->son->sibling->son;
        while (defNode != NULL)
        {
            handle_def(defNode, 0); //handle local variable
            if (defNode->sibling != NULL)
                defNode = defNode->sibling->son;
            else
                break;
        }
    }
    TreeNode *stmtlistNode = CompSt->son->sibling->sibling;
    if (stmtlistNode == NULL && (ret.symuni.func.ret.kind != BASIC || ret.symuni.func.ret.u.basic != 0))
        // printf("Error type 8 at Line %d: Type mismatched for return.\n",
        //        compstNode->son->sibling->lineno);
        ;
    else
    {   if (strcmp(stmtlistNode->name, "RC") == 0)
        {
            if (strcmp(CompSt->son->sibling->name, "StmtList") == 0)
                stmtlistNode = CompSt->son->sibling;
            else if (ret.symuni.func.ret.kind != BASIC || ret.symuni.func.ret.u.basic != 0)
            {
                // printf("Error type 8 at Line %d: Type mismatched for return.\n",
                //        stmtlistNode->lineno);
                return;
            }
        }
        //printf("stmtlistNode: %s\n", stmtlistNode->name);
        TreeNode *stmtNode = stmtlistNode->son;
        while (1)
        {
            //printf("stmtNode: %s\n", stmtNode->name);
            //printf("stmtNode->son: %s\n", stmtNode->son->name);
            // if (strcmp(stmtNode->son->name, "CompSt") == 0)
            // {
            //     handle_compstmt(stmtNode->son, ret);
            // }
            // else if (strcmp(stmtNode->son->name, "RETURN") == 0)
            // {
            //     Types type = handle_exp_type(stmtNode->son->sibling);
            //     if (typecmp(type, &ret.symuni.func.ret) != 0)
            //     {
            //         printf("Error type 8 at Line %d: Type mismatched for return.\n",
            //                stmtNode->lineno);
            //         Semantic_Error = 1;
            //     }
            // }
            handle_stmt(stmtNode, ret);
            if (stmtNode->sibling == NULL)
                break;
            else
                stmtNode = stmtNode->sibling->son;
        }
    }
}

void handle_stmt(TreeNode *Stmt, Symbols ret)
{
    if(strcmp(Stmt->name, "Stmt") != 0)
    {
        printf("you should not call this function!\n");
        return;
    }

    if (strcmp(Stmt->son->name, "CompSt") == 0)
    {
        handle_compstmt(Stmt->son, ret);
    }
    else if(strcmp(Stmt->son->name, "RETURN") == 0)
    {
        Types type = handle_exp_type(Stmt->son->sibling);
        if (typecmp(type, &ret.symuni.func.ret) != 0)
        { 
            printf("Error type 8 at Line %d: Type mismatched for return.\n",
                    Stmt->lineno);
            Semantic_Error = 1;
        }
    }  
    else if((strcmp(Stmt->son->name, "IF") == 0) &&
            Stmt->son->sibling->sibling->sibling->sibling->sibling == NULL)
    {
        handle_stmt(Stmt->son->sibling->sibling->sibling->sibling, ret);
    }
    else if(strcmp(Stmt->son->name, "IF") == 0)
    {
        handle_stmt(Stmt->son->sibling->sibling->sibling->sibling, ret);
        handle_stmt(Stmt->son->sibling->sibling->sibling->sibling->sibling->sibling, ret);
    }
    else if(strcmp(Stmt->son->name, "WHILE") == 0)
    {
        handle_stmt(Stmt->son->sibling->sibling->sibling->sibling, ret);
    }
}

void handle_extdef(TreeNode *Node)
{
    if (strcmp(Node->name, "ExtDef") != 0)
    {
        printf("you should not call this function!\n");
        return;
    }

    if (strcmp(Node->son->sibling->name, "FunDec") == 0)
    {
        FieldList args = handle_arg(Node->son->sibling); //handle arg
        //printf("you find a function declaration.\n");
        TreeNode *funcNode = Node->son->sibling;
        //Specifier, or Node->son, might be a basic type or a struct type
        int flag = -1;
        Symbols newsym = init_symbol_func(Node->son->son, funcNode->son,
                                          args);
        flag = look_up_name(newsym.symuni.func.name);
        if (flag != -1)
        {
            printf("Error type 4 at Line %d: Redefined function \"%s\".\n",
                   funcNode->son->lineno,
                   funcNode->son->value.type_charstar);
            Semantic_Error = 1;
        }
        else
            add_symbol(newsym);

        TreeNode *compstNode = Node->son->sibling->sibling;
        handle_compstmt(compstNode,newsym);
        // if (strcmp(compstNode->son->sibling->name, "DefList") == 0)
        // {
        //     TreeNode *defNode = compstNode->son->sibling->son;
        //     while (defNode != NULL)
        //     {
        //         handle_def(defNode, 0); //handle local variable
        //         if (defNode->sibling != NULL)
        //             defNode = defNode->sibling->son;
        //         else
        //             break;
        //     }
        // }
        // TreeNode *stmtlistNode = compstNode->son->sibling->sibling;
        // if (stmtlistNode == NULL && (newsym.symuni.func.ret.kind != BASIC || newsym.symuni.func.ret.u.basic != 0))
        //     // printf("Error type 8 at Line %d: Type mismatched for return.\n",
        //     //        compstNode->son->sibling->lineno);
        //     ;
        // else
        // {
        //     if (strcmp(stmtlistNode->name, "RC") == 0)
        //     {
        //         if (strcmp(compstNode->son->sibling->name, "StmtList") == 0)
        //             stmtlistNode = compstNode->son->sibling;
        //         else if (newsym.symuni.func.ret.kind != BASIC || newsym.symuni.func.ret.u.basic != 0)
        //         {
        //             // printf("Error type 8 at Line %d: Type mismatched for return.\n",
        //             //        stmtlistNode->lineno);
        //             return;
        //         }
        //     }

        //     TreeNode *stmtNode = stmtlistNode->son;
        //     while (1)
        //     {
        //         if (strcmp(stmtNode->son->name, "RETURN") == 0)
        //         {
        //             Types type = handle_exp_type(stmtNode->son->sibling);
        //             if (typecmp(type, &newsym.symuni.func.ret) != 0)
        //             {
        //                 printf("Error type 8 at Line %d: Type mismatched for return.\n",
        //                        stmtNode->lineno);
        //                 Semantic_Error = 1;
        //             }
        //         }
        //         if (stmtNode->sibling == NULL)
        //             break;
        //         else
        //             stmtNode = stmtNode->sibling->son;
        //     }
        // }
    }
    else if (strcmp(Node->son->sibling->name, "ExtDecList") == 0)
    {
        TreeNode *varNode = Node->son->sibling->son; //var is VarDec
        while (1)
        {
            int flag = -1;
            TreeNode *idNode = varNode;
            if (strcmp(idNode->son->name, "ID") != 0)
                idNode = idNode->son;
            Symbols newsym = init_symbol_var(Node->son->son, idNode, 0);
            flag = look_up_name(newsym.symuni.var.name);
            if (flag != -1)
            {
                printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",
                       idNode->son->lineno,
                       newsym.symuni.var.name);
                Semantic_Error = 1;
            }
            else
                add_symbol(newsym);
            varNode = varNode->sibling;
            if (varNode != NULL)
                varNode = varNode->sibling->son; //Dec
            else
                break;
        }
    }
    else if (strcmp(Node->son->sibling->name, "SEMI") == 0)
    {
        ; //do nothing
    }
    else
    {
        perror("handle_extdef has an error3!\n");
    }
}

FieldList handle_def(TreeNode *Node, int select)
{ //if select = 0, means var; else if select = 1, means fieldlist
    if (strcmp(Node->name, "Def") != 0 || (select != 0 && select != 1))
    {
        printf("you should not call this function!\n");
        return NULL;
    }

    if (strcmp(Node->son->sibling->name, "DecList") == 0)
    {
        //printf("you find a var declaration.\n");
        TreeNode *varNode = Node->son->sibling->son; //Dec
        FieldList start = NULL, end = NULL;
        Symbols newsym;
        while (1) //does it terminate?
        {
            int flag = -1;
            if (select == 0)
            {
                newsym = init_symbol_var(Node->son->son, varNode->son, 0);
                flag = look_up_name(newsym.symuni.var.name);
            }
            else if (select == 1)
            {
                newsym = init_symbol_var(Node->son->son, varNode->son, 1);
                flag = look_up_name(newsym.symuni.fieldvar.name);
            }
            else
                perror("handle_def has an error!\n");

            if (flag != -1)
            {
                if (select == 0)
                    printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",
                           varNode->son->son->lineno,
                           newsym.symuni.var.name);
                else
                    printf("Error type 15 at Line %d: Redefined field \"%s\".\n",
                           varNode->son->son->lineno,
                           newsym.symuni.fieldvar.name);
                Semantic_Error = 1;
            }
            else
            {
                add_symbol(newsym);
                if (select == 0)
                {
                    if (varNode->son->sibling != NULL)
                    {
                        Types type = handle_exp_type(varNode->son->sibling->sibling);
                        if (type != NULL &&
                            typecmp(type, &sym_tbl.symbols[sym_idx - 1].symuni.var.type) != 0)
                        {
                            printf("Error type 5 at Line %d: Type mismatched for assignment.\n",
                                   Node->lineno);
                            Semantic_Error = 1;
                        }
                    }
                }
                else if (select == 1)
                {
                    if (varNode->son->sibling != NULL)
                    {
                        printf("Error type 15 at Line %d: Field \"%s\" initialed in struct or args.\n",
                               varNode->son->son->lineno,
                               varNode->son->son->value.type_charstar);
                        Semantic_Error = 1;
                    }
                    if (start == NULL)
                    {
                        start = &sym_tbl.symbols[sym_idx - 1].symuni.fieldvar;
                        end = start;
                    }
                    else
                    {
                        end->tail = &sym_tbl.symbols[sym_idx - 1].symuni.fieldvar;
                        end = end->tail;
                    }
                }
            }

            varNode = varNode->sibling;
            if (varNode != NULL)
                varNode = varNode->sibling->son; //Dec
            else
                break;
        }
        //printf("handle_def is safe.\n");
        return start;
    }
    else
    {
        perror("handle_def has an error!\n");
        return NULL;
    }
}

FieldList handle_arg(TreeNode *Node)
{
    if (strcmp(Node->name, "FunDec") != 0)
    {
        printf("you should not call this function!\n");
        return NULL;
    }

    if (strcmp(Node->son->sibling->sibling->name, "VarList") == 0)
    {
        //printf("you find a var declaration.\n");
        TreeNode *varNode = Node->son->sibling->sibling->son; //ParamDec
        FieldList start = NULL, end = NULL;
        while (1)
        {
            int flag = -1;
            Symbols newsym = init_symbol_var(varNode->son->son, varNode->son->sibling, 1);
            flag = look_up_name(newsym.symuni.fieldvar.name);
            if (flag != -1)
            {
                printf("Error type 3 at Line %d: Redefined variable \"%s\".\n",
                       varNode->son->sibling->son->lineno,
                       newsym.symuni.fieldvar.name);
                Semantic_Error = 1;
            }
            else
            {
                add_symbol(newsym);
                if (start == NULL)
                {
                    start = &sym_tbl.symbols[sym_idx - 1].symuni.fieldvar;
                    end = start;
                }
                else
                {
                    end->tail = &sym_tbl.symbols[sym_idx - 1].symuni.fieldvar;
                    end = end->tail;
                }
            }
            varNode = varNode->sibling;
            if (varNode != NULL)
                varNode = varNode->sibling->son; //Dec
            else
                break;
        }
        return start;
    }
    else if (strcmp(Node->son->sibling->sibling->name, "RP") == 0)
    { // no parameter to handle
        return NULL;
    }
    else
    {
        perror("handle_arg has an error!\n");
        return NULL;
    }
}

void handle_struct(TreeNode *Node)
{
    if (strcmp(Node->name, "StructSpecifier") != 0)
    {
        printf("you should not call this function!\n");
        return;
    }
    if (strcmp(Node->son->sibling->name, "OptTag") == 0) //define
    {
        FieldList flag = 0;
        Struct s = init_struct(Node->son->sibling->sibling->sibling, Node->son->sibling->son);
        flag = look_up_structtbl(Node->son->sibling->son->value.type_charstar);
        if (flag != NULL)
        {
            printf("Error type 16 at Line %d: Duplicated name \"%s\".\n",
                   Node->son->lineno, Node->son->sibling->son->value.type_charstar);
            Semantic_Error = 1;
        }
        else
            add_struct(s);
    }
    else if (strcmp(Node->son->sibling->name, "Tag") == 0)
    {
        //donothing
        ;
    }
    else
        perror("handle_struct has an error!\n");
}

void scan_node(TreeNode *Node)
{
    if (Node != NULL)
    {
        if (strcmp(Node->name, "ExtDef") == 0)
            handle_extdef(Node); //handle function or extern variable
        else if (strcmp(Node->name, "StructSpecifier") == 0)
            handle_struct(Node);
        scan_node(Node->son);
        scan_node(Node->sibling);
    }
}

void scan_tree(int idx)
{
    //print_tree(idx);
    add_read_and_write();
    Semantic_Error = 0;
    scan_node(&node[idx]);
    //print_all_symbols();
    semantic_check_stmt(&node[idx]);
}

// int symeql(Symbols x, Symbols y)
// {
//     int equal = 0;
//     if (x.symkind == y.symkind)
//     {
//         switch (x.symkind)
//         {
//         case FUN:
//             if (strcmp(x.symuni.func.name, y.symuni.func.name) == 0)
//                 equal = 1; //is it enough?
//             break;
//         case VAR:
//             if (strcmp(x.symuni.var.name, y.symuni.var.name) == 0)
//                 equal = 1;
//             break;
//         case FIELDVAR:
//             if (strcmp(x.symuni.fieldvar.name, y.symuni.fieldvar.name) == 0)
//                 equal = 1;
//             break;
//         default:
//             perror("function symeql has an error!\n");
//             break;
//         }
//     }
//     return equal;
// }

// int look_up_symtbl(Symbols s)
// {
//     //have not called add function
//     if (sym_idx == 0)
//         return 0;
//     else
//     {
//         for (int i = 0; i < sym_tbl.size; i++)
//         {
//             if (symeql(sym_tbl.symbols[i], s) == 1)
//                 return 1;
//         }
//         return 0;
//     }
// }

FieldList look_up_structtbl(char *name)
{
    for (int i = 0; i < str_idx; i++)
        if (strcmp(name, struct_tbl.structs[i].name) == 0)
            return struct_tbl.structs[i].first;
    return NULL;
}

int look_up_name(char *name)
{
    if (sym_idx == 0)
        return -1;
    else
    {
        for (int i = 0; i < sym_tbl.size; i++)
        {
            if (sym_tbl.symbols[i].symkind == FUN)
            {
                if (strcmp(sym_tbl.symbols[i].symuni.func.name, name) == 0)
                    return i;
            }
            else if (sym_tbl.symbols[i].symkind == VAR)
            {
                if (strcmp(sym_tbl.symbols[i].symuni.var.name, name) == 0)
                    return i;
            }
            else if (sym_tbl.symbols[i].symkind == FIELDVAR)
            {
                if (strcmp(sym_tbl.symbols[i].symuni.fieldvar.name, name) == 0)
                    return i;
            }
            else
                perror("look_up_name has an error!\n");
        }
        return -1;
    }
}

Types look_up_type(char *name)
{
    if (sym_idx == 0)
    {
        return NULL;
    }

    for (int i = 0; i < sym_tbl.size; i++)
        if (sym_tbl.symbols[i].symkind == FUN)
        {
            if (strcmp(name, sym_tbl.symbols[i].symuni.func.name) == 0)
                return &sym_tbl.symbols[i].symuni.func.ret;
        }
        else if (sym_tbl.symbols[i].symkind == VAR)
        {
            if (strcmp(name, sym_tbl.symbols[i].symuni.var.name) == 0)
                return &sym_tbl.symbols[i].symuni.var.type;
        }
        else if (sym_tbl.symbols[i].symkind == FIELDVAR)
        {
            if (strcmp(name, sym_tbl.symbols[i].symuni.fieldvar.name) == 0)
                return &sym_tbl.symbols[i].symuni.fieldvar.type;
        }
        else
        {
            perror("look_up_name has an error!\n");
            return NULL;
        }

    return NULL;
}

int is_op(char *opname)
{
    static const char *list[] = {
        "AND",
        "OR",
        "RELOP",
        "PLUS",
        "MINUS",
        "STAR",
        "DIV"};
    for (int i = 0; i < sizeof(list) / sizeof(list[0]); i++)
        if (strcmp(list[i], opname) == 0)
            return 1;
    return 0;
}

void semantic_check_stmt(TreeNode *Node)
{
    if (Node != NULL)
    {
        if (strcmp(Node->name, "Stmt") == 0)
        {
            if (strcmp(Node->son->name, "Exp") == 0)
            {
                handle_exp_type(Node->son);
            }
            else if (strcmp(Node->son->name, "IF") == 0 || strcmp(Node->son->name, "WHILE") == 0)
            {
                TreeNode *expNode = Node->son->sibling->sibling;
                Types type = handle_exp_type(expNode);
                if ((type->kind != BASIC || type->u.basic != 1) &&
                    type != NULL)
                {
                    printf("Error type 0 at Line %d: Condition must be an integer.\n",
                           expNode->lineno);
                    Semantic_Error = 1;
                }
            }
        }
        semantic_check_stmt(Node->son);
        semantic_check_stmt(Node->sibling);
    }
}

Types handle_exp_type(TreeNode *Node) //TODO
{                                     //-1 means not matched, -2 means not found
    if (strcmp(Node->name, "Exp") != 0)
    {
        printf("you should not call this function!\n");
        return NULL;
    }

    if (Node->son->sibling != NULL && strcmp(Node->son->sibling->name, "ASSIGNOP") == 0)
    {
        if (strcmp(Node->son->son->name, "INT") == 0 || strcmp(Node->son->son->name, "FLOAT") == 0)
        {
            printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n",
                   Node->son->lineno);
            Semantic_Error = 1;
        }
        Types ltype = handle_exp_type(Node->son);
        // if(ltype != NULL)
        //     printf("assign ltype = %d\n", ltype->kind);
        Types rtype = handle_exp_type(Node->son->sibling->sibling);
        // if(rtype != NULL)
        //     printf("assign rtype = %d\n", rtype->kind);
        if (typecmp(ltype, rtype) != 0 && ltype != NULL && rtype != NULL)
        {
            printf("Error type 5 at Line %d: Type mismatched for assignment.\n",
                   Node->lineno);
            Semantic_Error = 1;
        }
        return NULL;
    }
    else if (Node->son->sibling != NULL && is_op(Node->son->sibling->name))
    {
        //printf("you come here.\n");
        Types ltype = handle_exp_type(Node->son);
        // if (ltype != NULL && ltype->kind == BASIC)
        //     printf("op ltype = %d\n", ltype->u.basic);
        Types rtype = handle_exp_type(Node->son->sibling->sibling);
        // if (rtype != NULL && rtype->kind == BASIC)
        //     printf("op rtype = %d\n", rtype->u.basic);
        // printf("equal = %d\n", typecmp(ltype, rtype));
        if (ltype != NULL &&
            rtype != NULL &&
            (typecmp(ltype, rtype) != 0 || ltype->kind != BASIC))
        {
            printf("Error type 7 at Line %d: Type mismatched for operands.\n",
                   Node->lineno);
            Semantic_Error = 1;
            return NULL;
        }
        else if (strcmp(Node->son->sibling->name, "RELOP") == 0)
        {
            Types tmp = (Types)malloc(sizeof(Types));
            tmp->kind = BASIC;
            tmp->u.basic = 1;
            return tmp;
        }
        else
            return ltype;
    }
    else if (strcmp(Node->son->name, "LP") == 0)
        return handle_exp_type(Node->son->sibling);
    else if (strcmp(Node->son->name, "MINUS") == 0)
    {
        Types type = handle_exp_type(Node->son->sibling);
        if (type->kind != BASIC)
        {
            printf("Error type 0 at Line %d: Minus must act on INT or FLOAT.\n",
                   Node->lineno);
            Semantic_Error = 1;
            return NULL;
        }
        return type;
    }
    else if (strcmp(Node->son->name, "NOT") == 0)
    {
        Types type = handle_exp_type(Node->son->sibling);
        if (type->kind != BASIC || type->u.basic != 1)
        {
            printf("Error type 0 at Line %d: Condition must be an integer.\n",
                   Node->lineno);
            Semantic_Error = 1;
            return NULL;
        }
        return type;
    }
    else if (Node->son->sibling != NULL && strcmp(Node->son->sibling->name, "LP") == 0)
        return handle_func_in_exp(Node->son);
    else if (Node->son->sibling != NULL && strcmp(Node->son->sibling->name, "LB") == 0)
        return handle_array_in_exp(Node);
    else if (Node->son->sibling != NULL && strcmp(Node->son->sibling->name, "DOT") == 0)
        return handle_struct_in_exp(Node);
    else if (strcmp(Node->son->name, "ID") == 0)
    {
        int idx = look_up_name(Node->son->value.type_charstar);
        if (idx == -1)
        {
            printf("Error type 1 at Line %d: Undefined variable \"%s\".\n",
                   Node->lineno, Node->son->value.type_charstar);
            Semantic_Error = 1;
            return NULL;
        }
        else
        {
            // Types tmp = look_up_type(Node->son->value.type_charstar);
            // if (tmp != NULL && tmp->kind == BASIC)
            //     printf("id type = %d\n", tmp->u.basic);
            return look_up_type(Node->son->value.type_charstar);
        }
    }
    else if (strcmp(Node->son->name, "INT") == 0)
    {
        Types tmp = (Types)malloc(sizeof(Types));
        tmp->kind = BASIC;
        tmp->u.basic = 1;
        return tmp;
    }
    else if (strcmp(Node->son->name, "FLOAT") == 0)
    {
        Types tmp = (Types)malloc(sizeof(Types));
        tmp->kind = BASIC;
        tmp->u.basic = 2;
        return tmp;
    }
    else
    {
        perror("handle_exp_type has an error!\n");
        return NULL;
    }
}

Types handle_func_in_exp(TreeNode *idNode)
{
    int idx = look_up_name(idNode->value.type_charstar);
    if (idx == -1)
    {
        printf("Error type 2 at Line %d: Undefined function \"%s\".\n",
               idNode->lineno, idNode->value.type_charstar);
        Semantic_Error = 1;
        return NULL;
    }
    else if (sym_tbl.symbols[idx].symkind != FUN)
    {
        printf("Error type 11 at Line %d: \"%s\" is not a function.\n",
               idNode->lineno, idNode->value.type_charstar);
        Semantic_Error = 1;
        return NULL;
    }
    else if (strcmp(idNode->sibling->sibling->name, "RP") == 0)
    {
        if (sym_tbl.symbols[idx].symuni.func.args != NULL){
            printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments.\n",
                   idNode->lineno, idNode->value.type_charstar);
            Semantic_Error = 1;
        }
        return &sym_tbl.symbols[idx].symuni.func.ret;
    }
    else
    {
        TreeNode *argNode = idNode->sibling->sibling->son;
        FieldList arg = sym_tbl.symbols[idx].symuni.func.args;
        //printf("%s\n", sym_tbl.symbols[idx].symuni.func.name);
        if (arg == NULL)
        {
            printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments.\n",
                   idNode->lineno, idNode->value.type_charstar);
            Semantic_Error = 1;
            return &sym_tbl.symbols[idx].symuni.func.ret;
        }
        while (1)
        {
            //printf("%s\n", arg->name);
            Types type = handle_exp_type(argNode);
            if (typecmp(&arg->type, type) != 0)
            {
                printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments.\n",
                       idNode->lineno, idNode->value.type_charstar);
                Semantic_Error = 1;
                break;
            }
            if (arg->tail != NULL && argNode->sibling != NULL)
            {
                arg = arg->tail;
                argNode = argNode->sibling->sibling->son;
            }
            else if (arg->tail != NULL)
            {
                printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments.\n",
                       idNode->lineno, idNode->value.type_charstar);
                Semantic_Error = 1;
                break;
            }
            else if (argNode->sibling != NULL)
            {
                printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments.\n",
                       idNode->lineno, idNode->value.type_charstar);
                Semantic_Error = 1;
                break;
            }
            else
                break;
        }
        return &sym_tbl.symbols[idx].symuni.func.ret;
    }
}

Types handle_struct_in_exp(TreeNode *expNode)
{ //TODO: considering array, function
    //int type = handle_exp_type(expNode->son);
    if (strcmp(expNode->son->son->name, "ID") == 0)
    {
        if (expNode->son->son->sibling == NULL)
        {
            Types type = look_up_type(expNode->son->son->value.type_charstar);
            if (type->kind != STRUCTURE)
            {
                printf("Error type 13 at Line %d: Illegal use of \".\".\n",
                       expNode->lineno);
                Semantic_Error = 1;
                return NULL;
            }
            else
            {
                int idx = look_up_name(expNode->son->son->value.type_charstar);
                FieldList field = sym_tbl.symbols[idx].symuni.var.type.u.structure;
                Types flag = NULL;
                while (field != NULL)
                { // TODO
                    if (strcmp(field->name, expNode->son->sibling->sibling->value.type_charstar) == 0)
                        flag = &field->type;
                    field = field->tail;
                }
                if (flag == NULL)
                {
                    printf("Error type 14 at Line %d: Non-existent field \"%s\".\n",
                           expNode->lineno, expNode->son->sibling->sibling->value.type_charstar);
                    Semantic_Error = 1;
                    return NULL;
                }
                return flag;
            }
        }
    }
    return NULL;
}

Types handle_array_in_exp(TreeNode *expNode)
{
    Types type = handle_exp_type(expNode->son->sibling->sibling);
    if (type->kind != BASIC || type->u.basic != 1)
    {
        if (type->kind == BASIC && type->u.basic == 2 &&
            strcmp(expNode->son->sibling->sibling->son->name, "FLOAT") == 0) //compute exp
            printf("Error type 12 at Line %d: \"%f\" is not an integer.\n",
                   expNode->lineno, expNode->son->sibling->sibling->son->value.type_float);
        else
            printf("Error type 12 at Line %d: Exp is not an integer.\n",
                   expNode->lineno);
        Semantic_Error = 1;
    }
    TreeNode *idNode = expNode->son->son;
    while (idNode->sibling != NULL)
        idNode = idNode->son;
    int idx = look_up_name(idNode->value.type_charstar);
    if (sym_tbl.symbols[idx].symuni.var.type.kind != ARRAY)
    {
        printf("Error type 10 at Line %d: \"%s\" is not an array.\n",
               idNode->lineno, idNode->value.type_charstar);
        Semantic_Error = 1;
        return NULL;
    }
    return compute_array_type(expNode);
}

Types compute_array_type(TreeNode *expNode)
{
    Types type = NULL;
    int dimension = 1;
    while (expNode->son->son->sibling != NULL)
    {
        dimension++;
        expNode = expNode->son;
    }
    //printf("dimension = %d\n",dimension);
    int idx = look_up_name(expNode->son->son->value.type_charstar);
    if (sym_tbl.symbols[idx].symuni.var.type.kind != ARRAY)
        return NULL;
    //int d = 0;
    int kind = sym_tbl.symbols[idx].symuni.var.type.kind;
    Types arraytype = sym_tbl.symbols[idx].symuni.var.type.u.array.elem;
    while (dimension > 0)
    {
        type = arraytype;
        dimension--;
        kind = arraytype->kind;
        if (kind == ARRAY)
            arraytype = arraytype->u.array.elem;
        else
            break;
    }

    //printf("kind = %d\n", kind);
    // while (1)
    // {
    //     d++;
    //     kind = arraytype->kind;
    //     if (kind == ARRAY)
    //         arraytype = arraytype->u.array.elem;
    //     else
    //         break;
    // }
    // if (d == dimension)
    //     type = arraytype;
    return type;
}

int typecmp(Types t1, Types t2)
{ // -1 means no match and 0 means match
    if (t1 == NULL || t2 == NULL)
        return -1;
    else if (t1->kind != t2->kind)
        return -1;
    else
    {
        switch (t1->kind)
        {
        case BASIC:
            if (t1->u.basic != t2->u.basic)
                return -1;
            else
                return 0;
            break;
        case ARRAY:
        {
            Types a1 = t1->u.array.elem;
            Types a2 = t2->u.array.elem;
            while (a1->kind == ARRAY && a2->kind == ARRAY)
            {
                a1 = a1->u.array.elem;
                a2 = a2->u.array.elem;
            }
            if ((a1->kind == ARRAY && a2->kind != ARRAY) ||
                (a1->kind != ARRAY && a2->kind == ARRAY))
                return -1;
            else if (typecmp(a1, a2) != 0)
                return -1;
            else
                return 0;
            break;
        }
        case STRUCTURE:
        {
            FieldList l1 = t1->u.structure;
            FieldList l2 = t2->u.structure;
            int flag = 0;
            while (l1 != NULL && l2 != NULL)
            {
                if (typecmp(&l1->type, &l2->type) != 0)
                {
                    flag = -1;
                    break;
                }
                else
                {
                    l1 = l1->tail;
                    l2 = l2->tail;
                }
            }
            if (flag == -1)
                return -1;
            else if ((l1 == NULL && l2 != NULL) ||
                     (l1 != NULL && l2 == NULL))
                return -1;
            else
                return 0;
            break;
        }
        default:
            perror("typecmp has an error!\n");
            return -1;
            break;
        }
    }
}

int compute_para_num(char *funcname)
{
    int idx = look_up_name(funcname);
    if(idx == -1)
    {
        perror("function doesn't exist!\n");
        return -1;
    }
    
    int n = 0;
    FieldList ptr = sym_tbl.symbols[idx].symuni.func.args;
    while(ptr != NULL)
    {
        n++;
        ptr = ptr->tail;
    }
    return n;
}