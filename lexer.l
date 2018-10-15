%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "parser.h"


#define T_eof 0

int linenumber = 1;

int commentLevel = 0;

//for the string and/or chars to work properly we must translate hex codes and escape caracters
//o logos pou den diabazw mprosta einai gia na min skaei to programma ama o allos moy dwsei "sad\x"
char* stripstring (char *string){
	string++;
	string[strlen(string)-1] = 0;
	int state = 16; // state 16: no / before | state 17: / before | state 18 x before | state 0-15 /x<state> before
	char *res = malloc(sizeof(char)*strlen(string));
	int counter = 0;
	for(int i=0; i < strlen(string); i++){
		if(string[i] != '\\' && state == 16){
			res[counter] = string[i];
			counter++;
		} else if (string[i] == '\\' && state == 16){
			state = 17;
		} else if (state == 17 && string[i]=='r'){
			res[counter] = 0x0D;
			counter++;	
			state = 16;
		} else if (state == 17 && string[i]=='t'){
			res[counter] = 0x09;
			counter++;	
			state = 16;
		} else if (state == 17 && string[i]=='n'){
			res[counter] = 0x0A;
			counter++;	
			state = 16;
		} else if (state == 17 && string[i]=='0'){
			res[counter] = 0;
			counter++;	
			state = 16;
		} else if (state == 17 && string[i]!='x'){
			res[counter] = string[i];
			counter++;	
			state = 16;
		} else if (state == 17 && string[i]=='x'){
			state = 18;
		} else if (state == 18){
			if(string[i]<='9' && string[i]>='0') state = string[i]-'0';
			else if (string[i]<='f' && string[i]>='a') state = string[i] - 'a' + 10;
			else if (string[i]<='F' && string[i]>='A') state = string[i] - 'A' + 10;
			else {
				state = 16;
				printf ("Unrecognized character in: %s\n", string);
			}
		} else if (state<=15){
			if(string[i]<='9' && string[i]>='0') res[counter] = string[i]- '0' + (state * 16);
			else if (string[i]<='f' && string[i]>='a') res[counter] = string[i] - 'a' + 10 + (state * 16);
			else if (string[i]<='F' && string[i]>='A') res[counter] = string[i] - 'A' + 10 + (state * 16);
			else {
				counter--;
				printf ("Unrecognized character in: %s\n", string);
			}
			counter++;
			state = 16;
		} else {
			state = 16;
			printf ("Unrecognized character in: %s\n", string);	
		}
	}
	return res;
}

%}

L[a-zA-Z_]
D[0-9]
W[ \t\r]
HEX[0-9A-Fa-f]
Anotation[(){}=!+\-*/%<>|&\[\]:;,]

%x comment
%x oneLineComment

%%

"int" 		{return T_int;}
"byte" 		{return T_byte;}
"if" 		{return T_if;}
"else" 		{return T_else;}
"while" 	{return T_while;}
"true" 		{return T_true;}
"false"		{return T_false;}
"return" 	{return T_return;}
"proc" 		{return T_proc;}
"reference" 	{return T_reference;}

"=="		{return T_equal;}
"<="		{return T_lessThan;}
">="		{return T_moreThan;}
"!="		{return T_notEqual;}
{Anotation}	{return yytext[0];}

{D}+ 		{ yylval.n = atoi(yytext); return T_const; }
{L}({L}|{D})*	{ yylval.s = strdup(yytext); return T_id; }

 
"\'"((\\([ntr0\\\'\"]))|("\\x"({HEX}{HEX}))|([^"'\\]))"\'" { yylval.c = stripstring(yytext)[0]; return T_char; }
"\""((\\([ntr0\\\'\"]))|("\\x"({HEX}{HEX}))|([^"'\\]))*"\"" { yylval.s = stripstring(yytext); return T_string; }

"--"		{ BEGIN(oneLineComment); }
<oneLineComment>\n { linenumber++; BEGIN(INITIAL); }
<oneLineComment>. {/*nothing*/}

"(*"        { commentLevel++; BEGIN(comment); }
<comment>"(*" { commentLevel++; }

<comment>"*)" { commentLevel--; if(commentLevel==0) BEGIN(INITIAL); }
<INITIAL,comment>\n { linenumber++; }
<comment>. {/*nothing*/}

{W}+		{/*nothing*/}
.		{ printf( "Unrecognized character: %s\n", yytext); }

%%



