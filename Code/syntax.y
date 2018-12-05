%locations

%{
    #define YYSTYPE struct TreeNode
    #include "lex.yy.c"
    void yyerror(char* msg);
    extern int yylineno;
    extern int Lex_Error;
    int Bison_Error = 0;
%}

/* declared types */
//%union
//%define api.value.type {TreeNode}

/* declared tokens */
%token INT
%token FLOAT
%token ID
%token TYPE
%token LC RC
%token STRUCT
%token RETURN
%token IF WHILE
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%token SEMI COMMA
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT /*%NEG*/
%left LB RB LP RP DOT

/* declared non-terminals */
/* %type <type_double> Exp  */

%%
/* High-level Definitions */
Program : 
    ExtDefList {  
        $$ = init_treeNode_non("Program", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        int root_idx = add_son($$, $1);
        if(Lex_Error == 0 && Bison_Error == 0)
            scan_ir_tree();
        //if(Lex_Error == 0 && Bison_Error == 0)
        //    scan_tree(root_idx);
        //if(Lex_Error == 0 && Bison_Error == 0)
        //    print_tree(root_idx);
    };
ExtDefList : /* empty */  {
        $$ = init_treeNode_non("NULL", 0, 0, 0);
    }
    | ExtDef ExtDefList {  
        $$ = init_treeNode_non("ExtDefList", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        if($2.lineno != 0)
            add_sibling($1, $2);
    }; 
ExtDef :
    error SEMI {
        Bison_Error = 1;
    }
    | Specifier ExtDecList SEMI {  
        $$ = init_treeNode_non("ExtDef", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | Specifier SEMI {   
        $$ = init_treeNode_non("ExtDef", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
    }
    | Specifier FunDec CompSt {  
        $$ = init_treeNode_non("ExtDef", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    };
ExtDecList : 
    VarDec {   
        $$ = init_treeNode_non("ExtDecList", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
    }
    | VarDec COMMA ExtDecList {  
        $$ = init_treeNode_non("ExtDecList", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    };

/* Specifiers */
Specifier : 
    TYPE {  
        $$ = init_treeNode_non("Specifier", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
    }
    | StructSpecifier {   
        $$ = init_treeNode_non("Specifier", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
    };
StructSpecifier : 
    STRUCT OptTag LC DefList RC {
        $$ = init_treeNode_non("StructSpecifier", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        if($2.lineno != 0){
            add_sibling($1, $2);
            add_sibling($2, $3);
        }
        else
            add_sibling($1, $3);
        if($4.lineno != 0){
            add_sibling($3, $4);
            add_sibling($4, $5);
        }
        else
            add_sibling($3, $5);
    }
    | STRUCT Tag {
        $$ = init_treeNode_non("StructSpecifier", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
    };
OptTag : /* empty */{
            $$ = init_treeNode_non("NULL", 0, 0, 0);
       }
       | ID {
            $$ = init_treeNode_non("OptTag", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
            add_son($$, $1);
       };
Tag : 
    ID {
        $$ = init_treeNode_non("Tag", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
    };

/* Declarators */
VarDec : 
    ID {
        $$ = init_treeNode_non("VarDec", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        //printf("id = %s\n", $1.value.type_charstar);
        add_son($$, $1);
    }
    | VarDec LB INT RB {
        $$ = init_treeNode_non("VarDec", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
        add_sibling($3, $4);
    };
FunDec : 
    error RP {
        Bison_Error = 1;
    }
    | ID LP VarList RP {
        $$ = init_treeNode_non("FunDec", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
        add_sibling($3, $4);    
    }
    | ID LP RP {
        $$ = init_treeNode_non("FunDec", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);      
    };
VarList : 
    ParamDec COMMA VarList {
        $$ = init_treeNode_non("VarList", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);  
    }
    | ParamDec {
        $$ = init_treeNode_non("VarList", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
    };
ParamDec : 
    error RB {
        Bison_Error = 1;
    }
    error COMMA {
        Bison_Error = 1;
    }
    | Specifier VarDec{
        $$ = init_treeNode_non("ParamDec", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
    };

/* Statements */
CompSt :
    error RC {
        Bison_Error = 1;
    }
    | LC DefList StmtList RC {
        $$ = init_treeNode_non("CompSt", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        //printf("{: %d\n", @1.first_line);
        //printf("}: %d\n", @4.first_line);
        add_son($$, $1);
        if($2.lineno != 0&$3.lineno != 0){
            add_sibling($1, $2);
            add_sibling($2, $3);
            add_sibling($3, $4);  
        }
        else if($3.lineno != 0){
            add_sibling($1, $3);
            add_sibling($3, $4);
        }
        else if($2.lineno != 0){
            add_sibling($1, $2);
            add_sibling($2, $4);
        }
        else
            add_sibling($1, $4);
    };
StmtList : /* empty */ {
        $$ = init_treeNode_non("NULL", 0, 0, 0);
    }
    | Stmt StmtList {
        $$ = init_treeNode_non("StmtList", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        if($2.lineno != 0)
            add_sibling($1, $2);
    };
Stmt : 
    error SEMI {
        Bison_Error = 1;
        //printf("haha\n");
    }
    | Exp SEMI{
        $$ = init_treeNode_non("Stmt", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);    
    }
    | CompSt {
        $$ = init_treeNode_non("Stmt", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
    }
    | RETURN Exp SEMI {
        $$ = init_treeNode_non("Stmt", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);     
        add_sibling($2, $3);  
    }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
        $$ = init_treeNode_non("Stmt", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);     
        add_sibling($2, $3); 
        add_sibling($3, $4);     
        add_sibling($4, $5);
    }
    | IF LP Exp RP Stmt ELSE Stmt {
        $$ = init_treeNode_non("Stmt", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);     
        add_sibling($2, $3); 
        add_sibling($3, $4);     
        add_sibling($4, $5);
        add_sibling($5, $6);     
        add_sibling($6, $7);
    }
    | WHILE LP Exp RP Stmt {
        $$ = init_treeNode_non("Stmt", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);     
        add_sibling($2, $3); 
        add_sibling($3, $4);     
        add_sibling($4, $5);
    };

/* Local Definitions */
DefList : /* empty */ {
        $$ = init_treeNode_non("NULL", 0, 0, 0);
    }
    | Def DefList {
        $$ = init_treeNode_non("DefList", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        if($2.lineno != 0)
            add_sibling($1, $2);
    };
Def : 
    error SEMI {
        Bison_Error = 1;
    }
    | Specifier DecList SEMI {
        $$ = init_treeNode_non("Def", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    };
DecList : 
    Dec {
        $$ = init_treeNode_non("DecList", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
    }
    | Dec COMMA DecList {
        $$ = init_treeNode_non("DecList", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    };
Dec : 
    VarDec {
        $$ = init_treeNode_non("Dec", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
    }
    | VarDec ASSIGNOP Exp {
        $$ = init_treeNode_non("Dec", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    };

/* Expressions */
Exp : 
    Exp ASSIGNOP Exp {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | Exp AND Exp {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | Exp OR Exp {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | Exp RELOP Exp {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | Exp PLUS Exp {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | Exp MINUS Exp {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | Exp STAR Exp {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | Exp DIV Exp {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | LP Exp RP {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | MINUS Exp {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
    }
    | NOT Exp {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
    }
    | ID LP Args RP {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
        add_sibling($3, $4);
    }
    | ID LP RP {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | Exp LB Exp RB {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
        add_sibling($3, $4);
    }
    | Exp DOT ID {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | ID {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
    }
    | INT {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
    }
    | FLOAT {
        $$ = init_treeNode_non("Exp", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
    };
Args : Exp COMMA Args {
        $$ = init_treeNode_non("Args", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
        add_sibling($1, $2);
        add_sibling($2, $3);
    }
    | Exp {
        $$ = init_treeNode_non("Args", @$.first_line, @$.first_column, @$.last_column - @$.first_column);
        add_son($$, $1);
    };

%%
void yyerror(char* msg) {
    fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}