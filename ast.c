#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "semantics.h"

extern int linenumber;

static ast ast_make (kind k, char *c, int n, ast t1, ast t2, ast t3, ast t4) {
  	ast p;
  	if ((p = malloc(sizeof(struct node))) == NULL)
    		exit(1);
  	p->k = k;
  	p->c = c;
 	p->n = n;
  	p->t1 = t1;
  	p->t2 = t2;
  	p->t3 = t3;
	p->t4 = t4;
	p->lineNumber = linenumber;
	return p;
}

ast ast_mk (kind k, char *c, int n, ast t1, ast t2){
	return ast_make (k, c, n, t1, t2, NULL, NULL);
}

ast ast_func_decl(char *s, ast t1, ast t2, ast t3, ast t4){
	return ast_make(PROC, s, 0, t1, t2, t3, t4);
}

ast ast_seq(kind type, ast t1, ast t2){	
	if (t2==NULL) return t1;
	return ast_mk(type, NULL, 0, t1, t2);
}

ast ast_f_par_def(kind k, char *c, ast t1){
	return ast_mk(k, c, 0, t1, NULL);
}

ast ast_dt_type(kind k){
	return ast_mk(k, NULL, 0, NULL, NULL);
}

ast ast_type(kind k, ast t1){
	return ast_mk(k, NULL, 0, t1, NULL);
}

ast ast_r_type(kind k, ast t1){
	return ast_mk(k, NULL, 0, t1, NULL);
}

ast ast_local_def(kind k, ast t1){
	return ast_mk(k, NULL, 0, t1, NULL);
}

ast ast_var_def(char *c, ast t1){
	return ast_mk(VAR, c, 1, t1, NULL);
}

ast ast_vararray_def(char *c, ast t1, int n){
	return ast_mk(VARARRAY, c, n, t1, NULL);
}

ast ast_set(ast t1, ast t2){
	return ast_mk(SET, NULL, 0, t1, t2);
}

ast ast_if(ast t1, ast t2){
	return ast_mk(IF, NULL, 0, t1, t2);
}

ast ast_if_else(ast t1, ast t2, ast t3){
	return ast_make(IFELSE, NULL, 0, t1, t2, t3, NULL);
}

ast ast_while(ast t1, ast t2){
	return ast_mk(WHILE, NULL, 0, t1, t2);
}

ast ast_return(ast t1, int k){
	return ast_mk(RETURN, NULL, k, t1, NULL);
}

ast ast_func_call(char *c, ast t1, int n){
	return ast_mk(FUNCCALL, c, n, t1, NULL);
}

ast ast_const(int n){
	return ast_mk(CONST, NULL, n, NULL, NULL);
}

ast ast_char(char c){
	char *temp = malloc(sizeof(char));
	*temp = c;
	return ast_mk(CHAR, temp, 0, NULL, NULL);
}

ast ast_prefix(ast t1, int k){
	return ast_mk(PREFIX, NULL, k, t1, NULL);
}

ast ast_op(kind k, ast t1, ast t2){
	return ast_mk(k, NULL, 0, t1, t2);
}

ast ast_id(char *c, int n, ast t1){
	return ast_mk(ID, c, n, t1, NULL);
}

ast ast_string(char *c){
	return ast_mk(STRING, c, 0, NULL, NULL);
}

ast ast_bool(int k){
	return ast_mk(BOOLEAN, NULL, k, NULL, NULL);
}

ast ast_not(ast t1){
	return ast_mk(NOT, NULL, 0, t1, NULL);
}

ast ast_comp(kind k, ast t1, ast t2){
	return ast_mk(k, NULL, 0, t1, t2);
}

#define NOTHING 0

int ast_run (ast t){
	if (t == NULL) return NOTHING; 
	ast t1 = t->t1, t2 = t->t2, t3 = t->t3, t4 = t->t4;
	int n = t->n;
	char* c = t->c;
	int num1, num2, offset, cond1, cond2, expr1, expr2;
	int result, cond;
	int totalSize, typeSize;	
	
	switch (t->k) {
	case PROC:
		printf("entering creation of function: %s\n",c);
		//add id to scope with type from t2
		ast_run(t2); //return type

		//NEW SCOPE
		ast_run(t1); //param list
		//set type to scope
		ast_run(t3); //func and var def
		ast_run(t4); //body
		//DELETE SCOPE
		return NOTHING;
	case LIST:
		//ama to list einai ok na alla5w to make apo to na pairnei to kind ws parametro
		ast_run(t1);
		ast_run(t2);
		return NOTHING;
	case FPLIST:
		//ama to list einai ok na alla5w to make apo to na pairnei to kind ws parametro
		ast_run(t1);
		ast_run(t2);
		return NOTHING;		
	case PARAM:
		//hold name of param make copy
		printf("creating parameter: %s\n",c);		
		ast_run(t1);
		return NOTHING;
	case RFPARAM:
		//hold name of param link address
		printf("creating referenced parameter: %s\n",c);		
		ast_run(t1);
		return NOTHING;
	case SIGNLETYPE:
		ast_run(t1);
		return NOTHING;
	case ARRAYTYPE:
		printf("array type\n");
		ast_run(t1);
		return NOTHING;
	case RTYPE:
		//return type
		printf("function return type\n");
		return ast_run(t1);
	case PROCTYPE:
		printf("function returns void\n");
		return 0;
	case vINT:
		printf("the type is int\n");
		return 4;
	case vBYTE:
		printf("the type is byte\n");
		return 1;
	//mporei na prepei na alla5oume to local_def_list oste na dinei diaforetiko toy LIST
	case FUNCDEF:
		ast_run(t1);
		return 0;
	case VARDEF:
		ast_run(t1);	
		return 0;
	case VAR:
		printf("creating new var with name: %s ",c);
		typeSize = ast_run(t1);
		printf("and size %i\n",typeSize);
		return typeSize;
	case VARARRAY:
		printf("creating new var with name: %s ",c);
		typeSize = ast_run(t1);
		totalSize = n*typeSize;
		printf("and size %i*%i=%i\n",n,typeSize,totalSize);
		return totalSize;
	//mporei na prepei na alla5oume to stmt_list oste na dinei diaforetiko toy LIST
	case SET:
		printf("we set a val to a var\n");
		ast_run(t1);
		ast_run(t2);
		return NOTHING;
	case IF:
		printf("there is an if statement\n");
		result = ast_run(t1);
		if(result != 0) ast_run(t2);
		return NOTHING;
	case IFELSE:
		printf("there is an ifelse statement\n");
		result = ast_run(t1);
		if(result != 0) ast_run(t2);
		else ast_run(t3);
		return NOTHING;
	case WHILE:
		printf("entering while statement\n");
		result = ast_run(t1);
		//put label
		//if result == 0 jump out of loop
			ast_run(t2);	
		//jump to label
		//out of loop
		return NOTHING;
	case RETURN:
		printf("returning from a function\n");
		ast_run(t1);
		return n;
	case FUNCCALL:
		printf("evaluating parameters\n");
		ast_run(t1);
		printf("calling a function with name: %s\n", c);
		return n;
	case EXPRLIST:
		//ama to list einai ok na alla5w to make apo to na pairnei to kind ws parametro
		ast_run(t1);
		ast_run(t2);
		return NOTHING;	
	case CONST:
		printf("%i\n",n);
		return n;
	case CHAR:
		printf("%c\n",c[0]);
		return c[0];
	case PREFIX:
		printf("prefix = %i\n", n);
		return 0;
	case PLUS:
		num1 = ast_run(t1);
		num2 = ast_run(t2);
		printf("adding two expressions\n");
		return num1+num2;
	case MINUS:
		num1 = ast_run(t1);
		num2 = ast_run(t2);
		printf("subtracting two expressions\n");
		return num1-num2;
	case MOD:
		num1 = ast_run(t1);
		num2 = ast_run(t2);
		printf("moduling two expressions\n");
		return num1%num2;
	case DIV:
		num1 = ast_run(t1);
		num2 = ast_run(t2);
		printf("dividing two expressions\n");
		return num1/num2;
	case MUL:
		num1 = ast_run(t1), num2 = ast_run(t2);
		printf("multiplying two expressions\n");
		return num1*num2;
	case ID:
		//check if exists in scope
		offset = ast_run(t1); // 0 if not array due to NULL returning 0
		printf("talking about var %s with offset %i\n",c,offset);
		return 0;
	case STRING:
		printf("found a string %s\n",c);
		return 0;
	case BOOLEAN:
		printf("found a bool with val: %i", n);
		return n;
	case NOT:
		cond = ast_run(t1);
		if(cond==0) return 1;
		else return 0;
	case EQUAL:
		expr1 = ast_run(t1);
		expr2 = ast_run(t2);
		if(expr1 == expr2) return 1; else return 0;
	case EQLESS:
		expr1 = ast_run(t1);
		expr2 = ast_run(t2);
		if(expr1 <= expr2) return 1; else return 0;
	case EQMORE:
		expr1 = ast_run(t1);
		expr2 = ast_run(t2);
		if(expr1 >= expr2) return 1; else return 0;
	case NOTEQ:
		expr1 = ast_run(t1);
		expr2 = ast_run(t2);
		if(expr1 != expr2) return 1; else return 0;
	case LESS:
		expr1 = ast_run(t1);
		expr2 = ast_run(t2);
		if(expr1 < expr2) return 1; else return 0;
	case MORE:
		expr1 = ast_run(t1);
		expr2 = ast_run(t2);
		if(expr1 > expr2) return 1; else return 0;
	case OR:
		cond1 = ast_run(t1);
		cond2 = ast_run(t2);
		if(cond1 | cond2) return 1; else return 0;	
	case AND:
		cond1 = ast_run(t1);
		cond2 = ast_run(t2);
		if(cond1 & cond2) return 1; else return 0;
	}
	return 0;
}















