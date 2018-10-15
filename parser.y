%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "semantics.h"
#include "parser.h"
#include "debugger.h"


void yyerror (const char *msg);
extern bool correct;
extern int linenumber;
ast t;
%}

%union{
	char c;
	int n;
	char* s;
	ast a;	
}

%token T_int "int"
%token T_byte "byte"
%token T_if "if"
%token T_else "else"
%token T_while "while"
%token T_true "true"
%token T_false "false"
%token T_return "return"
%token T_proc "proc"
%token T_reference "reference"
%token<s> T_id
%token<n> T_const
%token<c> T_char 
%token<s> T_string 
%token T_equal "=="
%token T_lessThan "<="
%token T_moreThan ">="
%token T_notEqual "!="

%left '+' '-'
%left '*' '/' '%'
%left UPLUS UMINUS UNOT
%left '&' '|'

%type<a> program
%type<a> func_def
%type<a> f_par_list
%type<a> f_par_def
%type<a> data_type
%type<a> type
%type<a> r_type
%type<a> local_def_list
%type<a> local_def
%type<a> var_def
%type<a> stmt_list
%type<a> stmt
%type<a> compound_stmt
%type<a> func_call
%type<a> expr_list
%type<a> expr
%type<a> l_value
%type<a> cond


   
%%

program: func_def { t = $$ = $1; };

func_def: /*isws na metrame poso megali einai i lista par kai na tin pername sto int tou ast*/
	T_id '(' f_par_list ')' ':' r_type local_def_list compound_stmt {printf("%i\n",linenumber); $$ = ast_func_decl($1, $3, $6, $7, $8); }
	|T_id '(' ')' ':' r_type local_def_list compound_stmt {printf("%i\n",linenumber); $$ = ast_func_decl($1, NULL, $5, $6, $7); };

f_par_list: 
	f_par_def { $$ = ast_seq(FPLIST, $1, NULL); }
	|f_par_def ',' f_par_list { $$ = ast_seq(FPLIST, $1, $3); };

f_par_def:
	T_id ':' "reference" type { $$ = ast_f_par_def(RFPARAM, $1, $4); }
	|T_id ':' type { $$ = ast_f_par_def(PARAM, $1, $3); };

type:
	data_type { $$ = ast_type(SIGNLETYPE, $1); }
	|data_type '[' ']' { $$ = ast_type(ARRAYTYPE, $1); };	

r_type:
	data_type { $$ = ast_r_type(RTYPE, $1); }
	|"proc" { $$ = ast_r_type(PROCTYPE, NULL); };

data_type:
	"int" { $$ = ast_dt_type(vINT); }
	|"byte" { $$ = ast_dt_type(vBYTE); };

local_def_list: 
	/*nothing*/ { $$ = NULL; }
	|local_def local_def_list { $$ = ast_seq(LIST, $1, $2); };	

local_def:
	func_def { $$ = ast_local_def(FUNCDEF, $1); }
	|var_def { $$ = ast_local_def(VARDEF, $1); };

var_def:
	T_id ':' data_type ';' { $$ = ast_var_def($1, $3); }
	|T_id ':' data_type '[' T_const ']' ';' { $$ = ast_vararray_def($1, $3, $5); };

stmt_list:
	/*nothing*/ { $$ = NULL; }
	|stmt stmt_list { $$ = ast_seq(LIST, $1, $2); };

stmt:
	';' { $$ = NULL; }
	|l_value '=' expr ';' { $$ = ast_set($1, $3); }
	|compound_stmt { $$ = $1; }
	|func_call ';' { $$ = $1; }
	|"if" '(' cond ')' stmt  { $$ = ast_if($3, $5); }
	|"if" '(' cond ')' stmt "else" stmt { $$ = ast_if_else($3, $5, $7); }
	|"while" '(' cond ')' stmt { $$ = ast_while($3, $5); }
	|"return" expr ';' { $$ = ast_return($2,1); }
	|"return" ';' { $$ = ast_return(NULL,0); };

compound_stmt:
	'{' stmt_list '}' { $$ = $2; };

func_call:
	T_id '(' ')' { $$ = ast_func_call($1, NULL, 0); }
	|T_id '(' expr_list ')' { $$ = ast_func_call($1, $3, 1); };

expr_list:
	expr { $$ = ast_seq(EXPRLIST, $1, NULL); }
	|expr ',' expr_list { $$ = ast_seq(EXPRLIST, $1, $3); };

expr:
	T_const { $$ = ast_const($1); }
	|T_char { $$ = ast_char($1); }
	|l_value { $$ = $1; }
	|'(' expr ')' { $$ = $2; }
	|func_call { $$ = $1; }
	|'+' expr  %prec UPLUS { $$ = ast_prefix($2, 1); }
	|'-' expr  %prec UMINUS { $$ = ast_prefix($2, -1); }
	|expr '+' expr { $$ = ast_op(PLUS, $1, $3); }
	|expr '-' expr { $$ = ast_op(MINUS, $1, $3); }
	|expr '%' expr { $$ = ast_op(MOD, $1, $3); }
	|expr '/' expr { $$ = ast_op(DIV, $1, $3); }
	|expr '*' expr { $$ = ast_op(MUL, $1, $3); };

l_value:
	T_id { $$ = ast_id($1, 0, NULL); }
	|T_id '[' expr ']' { $$ = ast_id($1, 1, $3); }
	|T_string { $$ = ast_string($1); };

cond:
	"True" { $$ = ast_bool(1); }
	|"False" { $$ = ast_bool(0); }
	|'(' cond ')' { $$ = $2; }
	|'!' cond %prec UNOT { $$ = ast_not($2); }
	|expr "==" expr { $$ = ast_comp(EQUAL, $1, $3); }
	|expr "<=" expr { $$ = ast_comp(EQLESS, $1, $3); }
	|expr ">=" expr { $$ = ast_comp(EQMORE, $1, $3); }
	|expr "!=" expr { $$ = ast_comp(NOTEQ, $1, $3); }
	|expr '<' expr { $$ = ast_comp(LESS, $1, $3); }
	|expr '>' expr { $$ = ast_comp(MORE, $1, $3); }
	|cond '|' cond { $$ = ast_comp(OR, $1, $3); }
	|cond '&' cond { $$ = ast_comp(AND, $1, $3); };

%%

void yyerror (const char *msg) {
  fprintf(stderr, "Alan error: %s\n", msg);
  fprintf(stderr, "Aborting, I've had enough with line %d...\n",linenumber);
  exit(1);
}

int main() {

	linenumber = 1;
	correct = true;
  	if (yyparse()) return 1;
	set_lineNumber(t);
	//ast_run(t);
	initScope();
	run_type(t);
	if (correct)
 		printf("Compilation was successful.\n");
	else
		printf("Compilation was unsuccessful.\n");
  	return 0;
}
