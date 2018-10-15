#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

#include <stdbool.h>

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

void specifyFuncSign(char *c, char *sign);

scopeEntry *findFuncScopeEntry(char *c);

bool existsInScope(char *c);

char *getVarTypeByName(char *c);

char *getLastFuncDefinedSign();

void setSize(char *c, int size);

void addToScope(char *c, scopeType scType, char *type);

#endif
