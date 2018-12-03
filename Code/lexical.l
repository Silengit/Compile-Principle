%{
#include "syntax.tab.h"
#include "symboltable.h"
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

float power(float base, float idx);
Value value_default = {0};
int yycolumn = 1;
int Lex_Error = 0;
%}
digit [0-9]
letter [_a-zA-Z]
fdigit ({digit}*\.{digit}+|{digit}+\.)
%%
"//" {  
    char c = input();
    while(c != '\n') c = input();
    yylineno ++;
}
"/*" {
    char buf[2];
    buf[0] = input();
    buf[1] = input();
    while(buf[0] != '*' || buf[1] != '/'){
        if(buf[0] == '\n')
            yylineno ++;
        buf[0] = buf[1]; buf[1] = input();
    }
}
";" { 
    yylval = init_treeNode_ter("SEMI", value_default, _VOID, yylineno, yycolumn, yyleng);
    return SEMI;
}
"," { 
    yylval = init_treeNode_ter("COMMA", value_default, _VOID, yylineno, yycolumn, yyleng);
    return COMMA;
}
"=" { 
    yylval = init_treeNode_ter("ASSIGNOP", value_default, _VOID, yylineno, yycolumn, yyleng);
    return ASSIGNOP;
}
">"|"<"|">="|"<="|"=="|"!=" { 
    yylval = init_treeNode_ter("RELOP", value_default, _VOID, yylineno, yycolumn, yyleng);
    return RELOP; 
}
"+" { 
    yylval = init_treeNode_ter("PLUS", value_default, _VOID, yylineno, yycolumn, yyleng);
    return PLUS; 
}
"-" { 
    yylval = init_treeNode_ter("MINUS", value_default, _VOID, yylineno, yycolumn, yyleng);
    return MINUS; 
}
"*" { 
    yylval = init_treeNode_ter("STAR", value_default, _VOID, yylineno, yycolumn, yyleng);
    return STAR; 
}
"/" { 
    yylval = init_treeNode_ter("DIV", value_default, _VOID, yylineno, yycolumn, yyleng);
    return DIV; 
}
"&&" {
    yylval = init_treeNode_ter("AND", value_default, _VOID, yylineno, yycolumn, yyleng);
    return AND; 
}
"||" { 
    yylval = init_treeNode_ter("OR", value_default, _VOID, yylineno, yycolumn, yyleng);
    return OR; 
}
"." { 
    yylval = init_treeNode_ter("DOT", value_default, _VOID, yylineno, yycolumn, yyleng);
    return DOT; 
}
"!" { 
    yylval = init_treeNode_ter("NOT", value_default, _VOID, yylineno, yycolumn, yyleng);
    return NOT; 
}
"int"|"float"|"void" { 
    Value temp;
    strcpy(temp.type_charstar, yytext);
    yylval = init_treeNode_ter("TYPE", temp, _CHARSTAR, yylineno, yycolumn, yyleng);
    return TYPE; 
}
"(" { 
    yylval = init_treeNode_ter("LP", value_default, _VOID, yylineno, yycolumn, yyleng);
    return LP; 
}
")" { 
    yylval = init_treeNode_ter("RP", value_default, _VOID, yylineno, yycolumn, yyleng);
    return RP; 
} 
"[" { 
    yylval = init_treeNode_ter("LB", value_default, _VOID, yylineno, yycolumn, yyleng);
    return LB; 
}
"]" { 
    yylval = init_treeNode_ter("RB", value_default, _VOID, yylineno, yycolumn, yyleng);
    return RB; 
}
"{" { 
    yylval = init_treeNode_ter("LC", value_default, _VOID, yylineno, yycolumn, yyleng);
    return LC; 
}
"}" { 
    yylval = init_treeNode_ter("RC", value_default, _VOID, yylineno, yycolumn, yyleng);
    return RC; 
} 
"struct" { 
    yylval = init_treeNode_ter("STRUCT", value_default, _VOID, yylineno, yycolumn, yyleng);
    return STRUCT; 
} 
"return" { 
    yylval = init_treeNode_ter("RETURN", value_default, _VOID, yylineno, yycolumn, yyleng);
    return RETURN; 
} 
"if" { 
    yylval = init_treeNode_ter("IF", value_default, _VOID, yylineno, yycolumn, yyleng);
    return IF; 
} 
"else" { 
    yylval = init_treeNode_ter("ELSE", value_default, _VOID, yylineno, yycolumn, yyleng);
    return ELSE; 
} 
"while" {
    yylval = init_treeNode_ter("WHILE", value_default, _VOID, yylineno, yycolumn, yyleng);
    return WHILE; 
}
"0"|([1-9]{digit}*) { 
    Value temp; temp.type_int = atoi(yytext); 
    yylval = init_treeNode_ter("INT", temp, _INT, yylineno, yycolumn, yyleng);
    return INT; 
}
"0"[0-7]+ { 
    int num = 0;
    int i = 1;
    while(i < yyleng){
        num += (yytext[i]-'0')*power(8,(yyleng - i - 1));
        i++;
    }
    Value temp; temp.type_int = num; 
    yylval = init_treeNode_ter("INT", temp, _INT, yylineno, yycolumn, yyleng);
    return INT;
}
0[Xx]({digit}|[A-F]|[a-f])+ { 
    int num = 0;
    int i = 2;
    while(i < yyleng){
        if(yytext[i]>='0'&&yytext[i]<='9')
            num += (yytext[i]-'0')*power(16,(yyleng - i - 1));
        else if(yytext[i]>='A'&&yytext[i]<='F')
            num += (yytext[i]-'A'+10)*power(16,(yyleng - i - 1));
        else
            num += (yytext[i]-'a'+10)*power(16,(yyleng - i - 1));
        i++;
    }
    Value temp; temp.type_int = num; 
    yylval = init_treeNode_ter("INT", temp, _INT, yylineno, yycolumn, yyleng);
    return INT;
}
{fdigit}[Ee][+-]?{digit}+ {
    char base_buf[100];
    char index_buf[100];
    float base = 0;
    int index = 0;
    float num = 0;
    int i = 0;
    int j = 0;
    while(yytext[i] != 'e' && yytext[i] != 'E'){
        base_buf[i] = yytext[i];
        i++;
    }
    base_buf[i] = '\0';
    base = atof(base_buf);
    i++;
    if(yytext[i] == '+'){
        i++;
        while(yytext[i]!='\0')
            index_buf[j++]=yytext[i++];
        index_buf[j] = '\0';
        index = atoi(index_buf);
    }
    else if(yytext[i] == '-'){
        i++;
        while(yytext[i]!='\0')
            index_buf[j++]=yytext[i++];
        index_buf[j] = '\0';
        index = -atoi(index_buf);
    }
    else{
        while(yytext[i]!='\0')
            index_buf[j++]=yytext[i++];
        index_buf[j] = '\0';
        index = atoi(index_buf);
    }
    num = base * power(10, index);
    Value temp; temp.type_float = num; 
    yylval = init_treeNode_ter("FLOAT", temp, _FLOAT, yylineno, yycolumn, yyleng);
    return FLOAT;
}
[+-]?{fdigit} {
    Value temp; temp.type_float = atof(yytext); 
    yylval = init_treeNode_ter("FLOAT", temp, _FLOAT, yylineno, yycolumn, yyleng);
    return FLOAT; 
}
{letter}({letter}|{digit})* { 
    Value temp; 
    //temp.type_charstar = strdup(yytext);
    strcpy(temp.type_charstar, yytext);
    temp.type_charstar[19] = '\0';
    yylval = init_treeNode_ter("ID", temp, _CHARSTAR, yylineno, yycolumn, yyleng);
    return ID; 
}
" " { 
    ; 
}
"\n" { 
    yycolumn = 1;
    yylineno ++; 
}
. { 
    Lex_Error = 1;
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext); 
}
%%

float power(float base, float idx){
    float res = 1;
    if(idx < 0){
        while(idx < 0){
            res /= base;
            idx ++;
        }
    }
    else{
        while(idx > 0){
            res *= base;
            idx --;
        }
    }
    return res;
}