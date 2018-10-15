#include <limits.h>
#include <stddef.h>
#include "ast.h"
#include "debugger.h"


int min2(int i, int j){
    if (i<j) return i;
    else return j;
}

int min3(int i, int j, int k){
    return min2(i,min2(j,k));
}

int min4(int i, int j, int k, int l){
    return min2(i,min3(j,k,l));
}

int min5(int i, int j, int k, int l, int m){
    return min2(i,min4(j,k,l,m));
}

int set_lineNumber (ast t){
    if (t == NULL) return INT_MAX;
    ast t1 = t->t1, t2 = t->t2, t3 = t->t3, t4 = t->t4;
    int num1, num2, num3, num4;

    switch (t->k) {
        case PROC:
            num1 = set_lineNumber(t1); //param list
            num2 = set_lineNumber(t2); //return type
            num3 = set_lineNumber(t3); //func and var def
            num4 = set_lineNumber(t4); //body
            t->lineNumber = min5(num1,num2,num3,num4,t->lineNumber);
            return t->lineNumber;;
        case LIST:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case FPLIST:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case PARAM:
            num1 = set_lineNumber(t1);
            t->lineNumber = min2(num1, t->lineNumber);
            return t->lineNumber;
        case RFPARAM:
            num1 = set_lineNumber(t1);
            t->lineNumber = min2(num1, t->lineNumber);
            return t->lineNumber;
        case SIGNLETYPE:
            num1 = set_lineNumber(t1);
            t->lineNumber = min2(num1, t->lineNumber);
            return t->lineNumber;
        case ARRAYTYPE:
            num1 = set_lineNumber(t1);
            t->lineNumber = min2(num1, t->lineNumber);
            return t->lineNumber;
        case RTYPE:
            t->lineNumber = set_lineNumber(t1);
            return num1 = t->lineNumber;
        case PROCTYPE:
            t->lineNumber = set_lineNumber(t1);
            return num1 = t->lineNumber;
        case vINT:
            t->lineNumber = set_lineNumber(t1);
            return num1 = t->lineNumber;
        case vBYTE:
            t->lineNumber = set_lineNumber(t1);
            return num1 = t->lineNumber;
        case FUNCDEF:
            num1 = set_lineNumber(t1);
            t->lineNumber = min2(num1, t->lineNumber);
            return t->lineNumber;
        case VARDEF:
            num1 = set_lineNumber(t1);
            t->lineNumber = min2(num1, t->lineNumber);
            return t->lineNumber;
        case VAR:
            num1 = set_lineNumber(t1);
            t->lineNumber = min2(num1, t->lineNumber);
            return t->lineNumber;
        case VARARRAY:
            num1 = set_lineNumber(t1);
            t->lineNumber = min2(num1, t->lineNumber);
            return t->lineNumber;
        case SET:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case IF:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case IFELSE:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            num3 = set_lineNumber(t3);
            t->lineNumber = min4(num1,num2,num3,t->lineNumber);
            return t->lineNumber;
        case WHILE:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case RETURN:
            num1 = set_lineNumber(t1);
            t->lineNumber = min2(num1, t->lineNumber);
            return t->lineNumber;
        case FUNCCALL:
            num1 = set_lineNumber(t1);
            t->lineNumber = min2(num1, t->lineNumber);
            return t->lineNumber;
        case EXPRLIST:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case CONST:
            t->lineNumber = set_lineNumber(t1);
            return num1 = t->lineNumber;
        case CHAR:
            t->lineNumber = set_lineNumber(t1);
            return num1 = t->lineNumber;
        case PREFIX:
            t->lineNumber = set_lineNumber(t1);
            return num1 = t->lineNumber;
        case PLUS:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case MINUS:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case MOD:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case DIV:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case MUL:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case ID:
            num1 = set_lineNumber(t1);
            t->lineNumber = min2(num1, t->lineNumber);
            return t->lineNumber;
        case STRING:
            t->lineNumber = set_lineNumber(t1);
            return num1 = t->lineNumber;
        case BOOLEAN:
            t->lineNumber = set_lineNumber(t1);
            return num1 = t->lineNumber;
        case NOT:
            num1 = set_lineNumber(t1);
            t->lineNumber = min2(num1, t->lineNumber);
            return t->lineNumber;
        case EQUAL:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case EQLESS:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case EQMORE:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case NOTEQ:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case LESS:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case MORE:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case OR:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
        case AND:
            num1 = set_lineNumber(t1);
            num2 = set_lineNumber(t2);
            t->lineNumber = min3(num1,num2,t->lineNumber);
            return t->lineNumber;
    }
    return 0;
}
