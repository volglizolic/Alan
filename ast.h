#ifndef __AST_H__
#define __AST_H__

typedef enum {
	PROC,
	PARAM, RFPARAM,
	//PARAMLIST, DEFLIST, STMTLIST, 
	LIST, EXPRLIST, FPLIST, //mporei na prepei na alla5oume to LIST me e5eidikeumenous tupous opws stin apo panw grammi
	VARDEF, FUNCDEF,
	IF, IFELSE, WHILE, SET, FUNCCALL, RETURN,
	vINT, vBYTE,
	SIGNLETYPE, ARRAYTYPE, RTYPE, PROCTYPE,
	VAR, VARARRAY, CONST, CHAR, ID, STRING,
	PLUS, MINUS, MUL, DIV, MOD, PREFIX,
	BOOLEAN, NOT, EQUAL, EQLESS, EQMORE, NOTEQ, LESS, MORE, OR, AND
} kind;

typedef struct node {
	kind k;
	char *c;
	int n;
	int lineNumber;
  	struct node *t1, *t2, *t3, *t4; /*t for tree*/
  	char *type;
  	char *signature;
  	int size;
  	char *trueSignature;
} *ast;


ast ast_func_decl(char *s, ast t1, ast t2, ast t3, ast t4);
ast ast_seq(kind type, ast t1, ast t2);
ast ast_f_par_def(kind k, char *c, ast t1);
ast ast_dt_type(kind k);
ast ast_type(kind k, ast t1);
ast ast_r_type(kind k, ast t1);
ast ast_local_def(kind k, ast t1);
ast ast_var_def(char *c, ast t1);
ast ast_vararray_def(char *c, ast t1, int n);
ast ast_set(ast t1, ast t2);
ast ast_if(ast t1, ast t2);
ast ast_if_else(ast t1, ast t2, ast t3);
ast ast_while(ast t1, ast t2);
ast ast_return(ast t1, int k);
ast ast_func_call(char *c, ast t1, int n);
ast ast_const(int n);
ast ast_char(char c);
ast ast_prefix(ast t1, int k);
ast ast_op(kind k, ast t1, ast t2);
ast ast_id(char *c, int n, ast t1);
ast ast_string(char *c);
ast ast_bool(int k);
ast ast_not(ast t1);
ast ast_comp(kind k, ast t1, ast t2);

int ast_run (ast t);

#endif
