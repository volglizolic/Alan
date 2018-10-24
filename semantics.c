#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ast.h"
#include "semantics.h"
#include "parser.h"


scopeEntry *scopeHead;

extern bool correct;

int scope = 0;

scopeEntry *head;

void valid_printf (const char * fmt, ...)
{
    printf(fmt);
    correct = false;
}

char * stringcat(char *c1, char *c2){
	int size1 , size2, size;
	if(c1 == NULL) size1 = 0; else size1 = strlen(c1);
	if(c2 == NULL) size2 = 0; else size2 = strlen(c2);
	size = size1 + size2;	
	if(size == 0) return NULL;
	if(size1 == 0) return c2;
	if(size2 == 0) return c1;
	char *result = malloc(sizeof(char)*(size+1)); //+1 for the space bellow
	strcat(result,c1);
	if(strcmp(c1,"_rf"))
	    strcat(result," ");
    strcat(result,c2);
    return result;
}

void initScope(){
	head = malloc(sizeof(scopeEntry));
}

void newScope(){ scope++;}

void deleteScope(){
	scopeEntry *temp = scopeHead;
	while(scopeHead!=NULL && scopeHead->scope == scope){
		temp = scopeHead;
		scopeHead = scopeHead->prevEntry;
		free(temp);
	}
	scope--;
}

void specifyFuncSign(ast t, char *c, char *sign){
    char *tempSign = malloc(sizeof(char) * (strlen(sign) + 1));
    int i = -1;
    int index = -1;
    do{
        i++; index++;
        if(sign[i]=='_') i = i+3;
        tempSign[index] = sign[i];
    }while(sign[i]!='\0');
	//add signature to ast tree
	t->signature = tempSign;
    t->trueSignature = malloc(sizeof(char) * (strlen(sign)+1));
    strcpy(t->trueSignature,sign);

    //add signature to scope entry
	scopeEntry *temp = scopeHead;
	while (temp!=NULL && strcmp(temp->id, c)!=0) temp = temp->prevEntry;
	printf("Assigning to function %s signature %s\n", c, sign);
	temp->funcSign = tempSign;
}

scopeEntry *findFuncScopeEntry(char *c){
	scopeEntry *temp = scopeHead;
	while (temp!=NULL && strcmp(temp->id, c)!=0) temp = temp->prevEntry;
	return temp;
}

bool existsInAnyScope(char *c){
	scopeEntry *temp = scopeHead;
	while (temp!=NULL && strcmp(temp->id, c)!=0) temp = temp->prevEntry;
	return temp != NULL;
}

bool existsInThisScope(char *c, scopeType scType){
	scopeEntry *temp = scopeHead;
	while (temp!=NULL && strcmp(temp->id, c)!=0 && temp->scope!=scope && temp->scType != scType) temp = temp->prevEntry;
	if (temp == NULL) return false;
	if(strcmp(temp->id, c) == 0 && temp->scope == scope && temp->scType == scType)
		return true;
	else
		return false;
}

char *getVarTypeByName(char *c){
	scopeEntry *temp = scopeHead;
	while (temp!=NULL && strcmp(temp->id, c)!=0) temp = temp->prevEntry;
	if(temp == NULL) return NULL;	
	return temp->type;
}

char *getLastFuncDefinedReturnType(){
	scopeEntry *temp = scopeHead;
	while (temp!=NULL && temp->scType != FUNCTION) temp = temp->prevEntry;
	if(temp == NULL) return NULL;	
	return temp->funcSign;
}

void setSize(ast t, char *c, int size){
    t->size = size;
	scopeEntry *temp = scopeHead;
	while (temp!=NULL && strcmp(temp->id, c)!=0) temp = temp->prevEntry;
	if(temp == NULL) valid_printf("something went terribly wrong in setSize\n");
	temp->size = size;
}

char *findVarTypeByID(char *c){
	scopeEntry *temp = scopeHead;
	while (temp!=NULL && strcmp(temp->id, c)!=0) temp = temp->prevEntry;
	if(temp == NULL) return NULL; else return temp->type;
}

void addToScope(ast t, char *c, scopeType scType, char *type){
    //add type to ast
    t->type = malloc(sizeof(char) * (strlen(type)+1));
    strcpy(t->type, type);
    //add var/param/rfparam/etc to scope entry
	scopeEntry *temp = malloc(sizeof(scopeEntry));
	temp->prevEntry = scopeHead;
	scopeHead = temp;
	scopeHead->id = c;
	scopeHead->scType = scType;
	scopeHead->type = type;
	scopeHead->scope = scope;
}

char *run_type(ast t){
	if (t == NULL) return "void"; 
	ast t1 = t->t1, t2 = t->t2, t3 = t->t3, t4 = t->t4;
	char *c = t->c;
	int n = t->n;
	int size;
	char *type1, *type2, *funcSign;
	scopeEntry *funcScopeEntry;

	//printf("%i\n",t->k);

	switch (t->k) {
	case PROC:
		//add id to scope with type from t2
		type2 = run_type(t2); //return type
		if(existsInThisScope(c, FUNCTION)) {valid_printf("line:%i\t function %s is already defined.\n", t->lineNumber, c); return "void";}
		addToScope(t, c, FUNCTION, type2);
		newScope();
		funcSign = run_type(t1); //param list
		specifyFuncSign(t, c, funcSign);
		if(strcmp(type2,"int")==0) size = 4 ; else size = 1;
        setSize(t, c, size);
		run_type(t3); //func and var def
		run_type(t4); //body
		deleteScope();
		return type2;//proc type
	case LIST:
		run_type(t1);
		run_type(t2);
		return "void";
	case FPLIST:
		type1 = run_type(t1);
		type2 = run_type(t2);
		return stringcat(type1,type2);
	case PARAM:
		type1 = run_type(t1);
		addToScope(t, c, PARAMETER, type1);
		return type1;
	case RFPARAM:
		type1 = run_type(t1);
		addToScope(t, c, RFPARAMETER, type1);
		return stringcat("_rf", type1);
	case SIGNLETYPE:
		return run_type(t1);
	case ARRAYTYPE:
		type1 = run_type(t1);
		if (strcmp(type1,"int")==0){
			return "intarray";
		}else{
		 	return "bytearray";
		}
	case RTYPE:
		return run_type(t1);
	case PROCTYPE:
		return "void";
	case vINT:
		return "int";
	case vBYTE:
		return "byte";
	case FUNCDEF:
		return run_type(t1);
	case VARDEF:
		return run_type(t1);
	case VAR:
		if(existsInThisScope(c, VARIABLE)) {valid_printf("line:%i\t variable %s is already defined.\n", t->lineNumber, c); return "void";}
		type1 = run_type(t1);
		if(strcmp(type1,"int")==0) size = 4 ; else size = 1;
		addToScope(t, c, VARIABLE, type1);
		setSize(t, c, size);
		return type1;
	case VARARRAY:
		if(existsInThisScope(c, VARIABLE)) {valid_printf("line:%i\t variable %s is already defined.\n", t->lineNumber, c); return "void";}
		type1 = run_type(t1);
		if(n<=0) {valid_printf("line:%i\t %s is not defined correctly. (maybe try setting a possitive size in array)\n", t->lineNumber, c); n = 1;}
		if(strcmp(type1,"int")==0) size = 4 * n; else size = n;
		if(strcmp(type1,"int")==0)
			type1 = "intarray";
		else
			type1 = "bytearray";
		addToScope(t, c, VARIABLE, type1);
		setSize(t, c, size);
		return type1;
	case SET:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(type1 == NULL){ valid_printf("line:%i\t Variable undefined.\n", t->lineNumber); return "void";}
		if(strcmp(type1,type2)!=0 && !(strcmp(type1,"int")==0 && (strcmp("byte",type2)==0))) valid_printf("line:%i\texpected value %s but got %s instead.\n", t->lineNumber, type1, type2);
		return type1;
	case IF:
		type1 = run_type(t1);
		if(strcmp(type1,"boolean")!=0) valid_printf("line:%i\texpected boolean as while condition but got %s instead.\n", t->lineNumber, type1);
		run_type(t2);
		return "void";
	case IFELSE:
		type1 = run_type(t1);
		if(strcmp(type1,"boolean")!=0) valid_printf("line:%i\texpected boolean as while condition but got %s instead.\n", t->lineNumber, type1);
		run_type(t2);
		run_type(t3);	
		return "void";
	case WHILE:
		type1 = run_type(t1);
		if(strcmp(type1,"boolean")!=0) valid_printf("line:%i\texpected boolean as while condition but got %s instead.\n", t->lineNumber, type1);
		run_type(t2);	
		return "void";
	case RETURN:
		type1 = run_type(t1);
		funcSign = getLastFuncDefinedReturnType();
		if(funcSign == NULL){
			valid_printf("line:%i\treturn found without a function declaration.\n", t->lineNumber, funcSign, type1);
			t->type = "void";
			t->size = 0;
			return "void";
		}
		if(strcmp(funcSign,type1)!=0) valid_printf("line:%i\texpected to return type %s but got %s instead.\n", t->lineNumber, funcSign, type1);
		t->type = type1;
		if(strcmp(type1,"int")==0) t->size = 4; else if(strcmp(type1,"byte")==0) t->size = 1; else t->size = 0;
		return type1;
	case FUNCCALL:
		type1 = run_type(t1);
		t->type = malloc(sizeof(char) * (strlen(type1)+1));
		t->type = type1;
		if(!existsInAnyScope(c)){ valid_printf("line:%i\tfunction %s is called but not declared\n",t->lineNumber, c); return "void";}
		funcScopeEntry = findFuncScopeEntry(c);
		if(strcmp(type1, funcScopeEntry->funcSign)!=0) valid_printf("line:%i\tparameter type missmatch in function %s call (Func sign: %s but is given: %s)\n", t->lineNumber, c, funcScopeEntry->funcSign, type1);
		return funcScopeEntry->type;
	case EXPRLIST:
		type1 = run_type(t1);
		type2 = run_type(t2);
		return stringcat(type1,type2);
	case CONST:
		return "int";
	case CHAR:
		return "byte";
	case PREFIX:
		return run_type(t1);
	case PLUS:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,type2)!=0 && (strcmp(type1,"byte")!=0 || strcmp(type1,"int")!=0)) valid_printf("line:%i\texpected same type enum int and byte but got %s and %s instead.\n", t->lineNumber, type1, type2);
		return type1;
	case MINUS:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,type2)!=0 && (strcmp(type1,"byte")!=0 || strcmp(type1,"int")!=0)) valid_printf("line:%i\texpected same type enum int and byte but got %s and %s instead.\n", t->lineNumber, type1, type2);
		return type1;
	case MOD:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,type2)!=0 && (strcmp(type1,"byte")!=0 || strcmp(type1,"int")!=0)) valid_printf("line:%i\texpected same type enum int and byte but got %s and %s instead.\n", t->lineNumber, type1, type2);
		return type1;
	case DIV:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,type2)!=0 && (strcmp(type1,"byte")!=0 || strcmp(type1,"int")!=0)) valid_printf("line:%i\texpected same type enum int or byte but got %s and %s instead.\n", t->lineNumber, type1, type2);
		return type1;
	case MUL:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,type2)!=0 && (strcmp(type1,"byte")!=0 || strcmp(type1,"int")!=0)) valid_printf("line:%i\texpected same type enum int or byte but got %s and %s instead.\n", t->lineNumber, type1, type2);
		return type1;
	case ID:
		type1 = run_type(t1);
		char *temp = findVarTypeByID(c);
		if(temp==NULL){
			valid_printf("line:%i\t variable %s is undefined.\n", t->lineNumber, c);
			return "void";
		}
		if((strcmp(temp,"byte")==0 || strcmp(temp,"int")==0) && (strcmp(type1,"void")!=0)) valid_printf("line:%i\t %s is not an array.\n", t->lineNumber, c);
		if(strcmp(temp,"byte")==0) return "byte";
		if(strcmp(temp,"int")==0) return "int";
		if((strcmp(temp,"bytearray")==0)&&(strcmp(type1,"void")==0)) return "bytearray";
		if((strcmp(temp,"bytearray")==0)&&(strcmp(type1,"int")==0)) return "byte";
		if((strcmp(temp,"intarray")==0)&&(strcmp(type1,"void")==0)) return "intarray";
		if((strcmp(temp,"intarray")==0)&&(strcmp(type1,"int")==0)) return "int";
		valid_printf("line:%i\t error in %s: maybe you intended to write <var> or <var>[i] instead of <var>[].\n", t->lineNumber, c);
		return "void";
	case STRING:
		return "bytearray";
	case BOOLEAN:
		return "boolean";
	case NOT:
		type1 = run_type(t1);
		if(strcmp(type1,"boolean")!=0) valid_printf("line:%i\texpected boolean type but got %s instead.\n", t->lineNumber, type1);
		return "boolean";
	case EQUAL:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,type2)!=0) valid_printf("line:%i\texpected same type but got %s and %s instead.\n", t->lineNumber, type1, type2);
		return "boolean";
	case EQLESS:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,type2)!=0) valid_printf("line:%i\texpected same type but got %s and %s instead.\n", t->lineNumber, type1, type2);
		return "boolean";
	case EQMORE:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,type2)!=0) valid_printf("line:%i\texpected same type but got %s and %s instead.\n", t->lineNumber, type1, type2);
		return "boolean";
	case NOTEQ:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,type2)!=0) valid_printf("line:%i\texpected same type but got %s and %s instead.\n", t->lineNumber, type1, type2);
		return "boolean";
	case LESS:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,type2)!=0) valid_printf("line:%i\texpected same type but got %s and %s instead.\n", t->lineNumber, type1, type2);
		return "boolean";
	case MORE:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,type2)!=0) valid_printf("line:%i\texpected same type but got %s and %s instead.\n", t->lineNumber, type1, type2);
		return "boolean";
	case OR:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,"boolean")!=0) valid_printf("line:%i\texpected boolean but got %s instead.\n", t->lineNumber, type1);
		if(strcmp(type2,"boolean")!=0) valid_printf("line:%i\texpected boolean but got %s instead.\n", t->lineNumber, type2);
		return "boolean";	
	case AND:
		type1 = run_type(t1);
		type2 = run_type(t2);
		if(strcmp(type1,"boolean")!=0) valid_printf("line:%i\texpected boolean but got %s instead.\n", t->lineNumber, type1);
		if(strcmp(type2,"boolean")!=0) valid_printf("line:%i\texpected boolean but got %s instead.\n", t->lineNumber, type2);
		return "boolean";
	};
	return NULL;
}
