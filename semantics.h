#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

#include <stdbool.h>
#include "ast.h"

bool correct;

typedef enum {
	FUNCTION, PARAMETER, RFPARAMETER, VARIABLE
} scopeType;

typedef struct tempScope{
	char *id;
	scopeType scType;
	char *type;
	char *funcSign;
	int size;
	int scope;
	struct tempScope *prevEntry;
}scopeEntry;

void initScope();

char* run_type(ast t);

void newScope();

void deleteScope();

void specifyFuncSign(ast t, char *c, char *sign);

scopeEntry *findFuncScopeEntry(char *c);

bool existsInScope(char *c);

char *getVarTypeByName(char *c);

char *getLastFuncDefinedSign();

void setSize(ast t, char *c, int size);

void addToScope(ast t, char *c, scopeType scType, char *type);

char * stringcat(char *c1, char *c2);

#endif
