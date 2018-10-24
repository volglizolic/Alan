//
// Created by volglizolic on 10/22/18.
//
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "threeAddressCode.h"
#include "semantics.h"
#include "ast.h"

int temporaryVarNumber;
int labelNumber;
threeAddressNode *head, *temp;
varIndex *headVarIndex, *tempVarIndex;
funcIndex *headFuncIndex, *tempFuncIndex;

char *enumToSTR(operation k){
    char *strngs[] = {
            "oINIT", //0
            "oPROC", //1
            "oVARDEF", "oFUNCDEF", //3
            "oIF", "oIFELSE", "oWHILE", "oSET", "oFUNCCALL", "oRETURN", //9
            "oSIGNLETYPE", "oARRAYTYPE", "oRTYPE", "oPROCTYPE", //13
            "oVAR", "oVARARRAY", "oCONST", "oCHAR", "oID", "oSTRING", //19
            "oPLUS", "oMINUS", "oMUL", "oDIV", "oMOD", "oPREFIX", //25
            "oBOOLEAN", "oNOT", "oEQUAL", "oEQLESS", "oEQMORE", "oNOTEQ", "oLESS", "oMORE", "oOR", "oAND", //35
            "oPOINTER", "oMALLOC", "oLABEL", "oPARAM", "oRFPARAM", //40
            "oJUMP", "oPUSH", "oPOP", "oRET" //44
    };
    return strngs[k];
}


void initThreeAddressCode(){
    temporaryVarNumber = 1;
    labelNumber = 1;
    head = malloc(sizeof(threeAddressNode));
    temp = head;
    temp->k = oINIT;
    temp->next = NULL;
    headVarIndex = malloc(sizeof(varIndex));
    tempVarIndex = headVarIndex;
    tempVarIndex->prevEntry = NULL;
    tempVarIndex->id = "EOL";
    headFuncIndex = malloc(sizeof(funcIndex));
    tempFuncIndex = headFuncIndex;
    tempFuncIndex->prevEntry = NULL;
    tempFuncIndex->id = "EOL";
}

void addFuncToIndex(ast t){
    tempFuncIndex = headFuncIndex;
    headFuncIndex = malloc(sizeof(funcIndex));
    headFuncIndex->prevEntry = tempFuncIndex;
    tempFuncIndex = headFuncIndex;
    headFuncIndex->id = t->c;
    headFuncIndex->type = t->type;
    headFuncIndex->signature = t->signature;
    headFuncIndex->trueSignature = t->trueSignature;
}

funcIndex *getFuncInfoByName(char *id){
    tempFuncIndex = headFuncIndex;
    while (tempFuncIndex != NULL && strcmp(tempFuncIndex->id, id)!=0) tempFuncIndex = tempFuncIndex->prevEntry;
    return tempFuncIndex;
}

void addVarToIndex(char* id, char* type, int size){
    tempVarIndex = headVarIndex;
    headVarIndex = malloc(sizeof(varIndex));
    headVarIndex->prevEntry = tempVarIndex;
    tempVarIndex = headVarIndex;
    headVarIndex->id = id;
    headVarIndex->type = type;
    headVarIndex->size = size;
}

varIndex* getVarInfoByName(char* id){
    tempVarIndex = headVarIndex;
    while (strcmp(tempVarIndex->id, id)!=0) tempVarIndex = tempVarIndex->prevEntry;
    return tempVarIndex;
}

char* getFixedNameOfVar(char *type, char* id, char vp){
    char *s = malloc((strlen(id)+3) * sizeof(char));
    if(strcmp(type, "int")==0)
        strcpy(s,"vi");
    else if(strcmp(type, "byte")==0)
        strcpy(s,"vb");
    else if(strcmp(type, "intarray")==0)
        strcpy(s,"va");
    else
        strcpy(s,"vs");
    s[0] = vp;
    strcat(s, id);
    return s;
}

void addLabelThreeAddressCodeLine(char *label){
    temp->next = malloc(sizeof(threeAddressNode));
    temp = temp->next;
    temp->next = NULL;
    temp->k = oLABEL;
    temp->text = malloc(strlen(label) * sizeof(char));
    strcpy(temp->text, label);
    printf("debug: label: %s\n", label);
}

void addVarThreeAddressCodeLine(operation op, char *target, char *operant1, char *operant2){
    temp->next = malloc(sizeof(threeAddressNode));
    temp = temp->next;
    temp->next = NULL;
    temp->k = op;
    temp->target = malloc((strlen(target)+1) * sizeof(char));
    strcpy(temp->target, target);
    if(operant1 == NULL)
        temp->operant1 = NULL;
    else{
        temp->operant1 = malloc((strlen(operant1)+1) * sizeof(char));
        strcpy(temp->operant1, operant1);
    }
    if(operant2 == NULL)
        temp->operant2 = NULL;
    else{
        temp->operant2 = malloc((strlen(operant2)+1) * sizeof(char));
        strcpy(temp->operant2, operant2);
    }
    printf("debug: op: %s target: %s operands:{%s, %s}\n", enumToSTR(op), target, operant1, operant2);
}

void addConstThreeAddressCodeLine(operation op, char *target, char *text, char c, int k, bool boolean){
    temp->next = malloc(sizeof(threeAddressNode));
    temp = temp->next;
    temp->next = NULL;
    temp->k = op;
    if(target == NULL){
        temp->target = NULL;
    } else {
        temp->target = malloc((strlen(target)+1) * sizeof(char));
        strcpy(temp->target, target);
    }
    temp->operant1 = NULL;
    temp->operant2 = NULL;
    temp->text = text;
    temp->c = c;
    temp->number = k;
    temp->boolean = boolean;
    printf("debug: op: %s target: %s result: enum {%s, %c, %i, %d}\n", enumToSTR(op), target, text, c, k, boolean);
}

char *getNextTemporaryVariable(char c){
    char *result = malloc((int)((ceil(log10(temporaryVarNumber+1))+3)*sizeof(char)));
    char temp[10];
    sprintf(temp, "%d", temporaryVarNumber);
    result[0] = '_';
    result[1] = c;
    result[2] = '\0';
    strcat(result,temp);
    temporaryVarNumber++;
    return result;
}

char *getNextTemporaryPointer(char c){
    char *result = malloc((int)((ceil(log10(temporaryVarNumber))+3)*sizeof(char)));
    char *temp = malloc(10 * sizeof(char));
    sprintf(temp, "%d", temporaryVarNumber);
    result[0] = 'p';
    result[1] = c;
    result[2] = '\0';
    strcat(result,temp);
    temporaryVarNumber++;
    return result;
}

char *getNextLabel(){
    char *result = malloc((int)((ceil(log10(labelNumber + 1))+3)*sizeof(char)));
    char temp[10];
    sprintf(temp, "%d", labelNumber);
    result[0] = '_';
    result[1] = 'L';
    result[2] = '\0';
    strcat(result,temp);
    labelNumber++;
    return result;
}

char* createThreeAddressCode(ast t){
    if (t == NULL) return NULL;
    ast t1 = t->t1, t2 = t->t2, t3 = t->t3, t4 = t->t4;
    int n = t->n, size = t->size;
    char* c = t->c;
    char *target, *op1, *op2, *tempstr1, *tempstr2;
    char *type = t->type;
    char *label1, *label2;
    varIndex *varData;
    funcIndex *funcData;

    switch (t->k) {
        case PROC:
            addFuncToIndex(t);
            //printf("entering creation of function: %s\n",c);
            addConstThreeAddressCodeLine(oFUNCDEF, c, NULL, '\0', 0, true);
            //add id to scope with type from t2
            //createThreeAddressCode(t2); //return type is not needed, we have that info from semantics

            //NEW SCOPE
            createThreeAddressCode(t1); //param list
            //set type to scope
            createThreeAddressCode(t3); //func and var def
            createThreeAddressCode(t4); //body
            addConstThreeAddressCodeLine(oRET, NULL, NULL, '\0', 0, true);
            //DELETE SCOPE
            return NULL;
        case LIST:
            //ama to list einai ok na alla5w to make apo to na pairnei to kind ws parametro
            createThreeAddressCode(t1);
            createThreeAddressCode(t2);
            return NULL;
        case FPLIST:
            //ama to list einai ok na alla5w to make apo to na pairnei to kind ws parametro
            createThreeAddressCode(t1);
            createThreeAddressCode(t2);
            return NULL;
        case PARAM:
            //hold name of param make copy
            if(strcmp(type,"int")==0) size = 4 ; else size = 1;
            addVarToIndex(c, type, size);
            c = getFixedNameOfVar(type, c, 'v');
            addConstThreeAddressCodeLine(oPARAM, c, type, '\0', size, true);
            return NULL;
        case RFPARAM:
            //hold name of param link address
            //hold name of param make copy
            type = run_type(t1);
            if(strcmp(type,"int")==0) size = 4 ; else size = 1;
            addVarToIndex(c, type, size);
            c = getFixedNameOfVar(type, c, 'p');
            addConstThreeAddressCodeLine(oRFPARAM, c, type, '\0', size, true);
            return NULL;
        case SIGNLETYPE:
            createThreeAddressCode(t1);
            return NULL;
        case ARRAYTYPE:
            printf("array type\n");
            createThreeAddressCode(t1);
            return NULL;
        case RTYPE:
            //return type
            printf("function return type\n");
            createThreeAddressCode(t1);
            return NULL;
        case PROCTYPE:
            printf("function returns void\n");
            return NULL;
        case vINT:
            printf("the type is int\n");
            return NULL;
        case vBYTE:
            printf("the type is byte\n");
            return NULL;
            //mporei na prepei na alla5oume to local_def_list oste na dinei diaforetiko toy LIST
        case FUNCDEF:
            createThreeAddressCode(t1);
            return NULL;
        case VARDEF:
            target = createThreeAddressCode(t1);
            return target;
        case VAR:
            addVarToIndex(c, type, size);
            c = getFixedNameOfVar(type, c, 'v');
            addConstThreeAddressCodeLine(oMALLOC, c, type, '\0', size, true);
            return NULL;
        case VARARRAY:
            addVarToIndex(c, type, size);
            c = getFixedNameOfVar(type, c, 'v');
            addConstThreeAddressCodeLine(oMALLOC, c, type, '\0', size, true);
            return NULL;
        case SET:
            target = createThreeAddressCode(t1);
            op1 = createThreeAddressCode(t2);
            addVarThreeAddressCodeLine(oSET, target, op1, NULL);
            return target;
        case IF:
            label1 = getNextLabel();
            tempstr1 = createThreeAddressCode(t1);
            addVarThreeAddressCodeLine(oIF, tempstr1, label1, NULL);
            createThreeAddressCode(t2);
            addLabelThreeAddressCodeLine(label1);
            return NULL;
        case IFELSE:
            label1 = getNextLabel();
            label2 = getNextLabel();
            tempstr1 = createThreeAddressCode(t1);
            addVarThreeAddressCodeLine(oIF, tempstr1, label1, NULL);
            createThreeAddressCode(t2);
            addConstThreeAddressCodeLine(oJUMP, label2, NULL, '\0', 0, true);
            addLabelThreeAddressCodeLine(label1);
            createThreeAddressCode(t3);
            addLabelThreeAddressCodeLine(label2);
            return NULL;
        case WHILE:
            label1 = getNextLabel();
            label2 = getNextLabel();
            addLabelThreeAddressCodeLine(label1);
            tempstr1 = createThreeAddressCode(t1);
            addVarThreeAddressCodeLine(oIF, tempstr1, label2, NULL);
            createThreeAddressCode(t2);
            addConstThreeAddressCodeLine(oJUMP, label1, NULL, '\0', 0, true);
            addLabelThreeAddressCodeLine(label2);
            return NULL;
        case RETURN:
            if(strcmp(type, "void")==0){
                createThreeAddressCode(t1);
                return NULL;
            }
            target = createThreeAddressCode(t1);
            addConstThreeAddressCodeLine(oPUSH, target, NULL, '\0', size, true);
            addConstThreeAddressCodeLine(oRET, NULL, NULL, '\0', 0, true);
            printf("returning from a function\n");
            return NULL;
        case FUNCCALL:
            //printf("Func %s call start\n", c);
            funcData = getFuncInfoByName(c);
            if(funcData == NULL){
                printf("Unknown function: %s. Skipping...\n", c);
                return "voidOrNotImplementedFunction";
            }
            if (t1 != NULL){
                //parameters exist and we must push them
                char *trueSignature = funcData->trueSignature;
                bool rfBit[20]; int i = 0, index = 0; rfBit[0] = false;
                while (trueSignature[i] != '\0'){
                    if(trueSignature[i] == ' ') {
                        index++;
                        rfBit[index] = false;
                    }
                    if(trueSignature[i] == '_') rfBit[index] = true;
                    i++;
                }
                char *varList = createThreeAddressCode(t1);
                i = -1; int prev = 0; index = 0;
                char *temp = malloc(64 * sizeof(char));
                do{
                    i++;
                    if(varList[i]==' ' || varList[i]=='\0'){
                        temp[i-prev] = '\0';
                        prev = i + 1;
                        if(rfBit[index]){
                            tempstr1 = getNextTemporaryPointer(funcData->type[0]);
                            addVarThreeAddressCodeLine(oPOINTER, tempstr1, temp, NULL);
                            addConstThreeAddressCodeLine(oPUSH, tempstr1, NULL, '\0', 0, true);
                        } else {
                            addConstThreeAddressCodeLine(oPUSH, temp, NULL, '\0', 0, true);
                        }
                        index ++;
                    } else {
                        temp[i-prev] = varList[i];
                    }
                }while (varList[i] != '\0');
            }
            addConstThreeAddressCodeLine(oFUNCCALL, funcData->id, NULL, '\0', 0, true);
            if(strcmp(t->type,"int")==0){
                target = getNextTemporaryVariable('i');
                addConstThreeAddressCodeLine(oPOP, target, NULL, '\0', 4, true);
            } else if (strcmp(t->type,"byte")==0){
                target = getNextTemporaryVariable('b');
                addConstThreeAddressCodeLine(oPOP, target, NULL, '\0', 1, true);
            } else {
                target = "void";
            }
            //printf("Func %s call end\n", c);
            return target;
        case EXPRLIST:
            //ama to list einai ok na alla5w to make apo to na pairnei to kind ws parametro
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            return stringcat(op1,op2);
        case CONST:
            target = getNextTemporaryVariable('i');
            addConstThreeAddressCodeLine(oCONST, target, NULL, '\0', n, true);
            return target;
        case CHAR:
            target = getNextTemporaryVariable('b');
            addConstThreeAddressCodeLine(oCHAR, target, NULL, c[0], 0, true);
            return target;
        case PREFIX:
            if(t1->n == 1)
                return createThreeAddressCode(t1);
            op1 = createThreeAddressCode(t1);
            target = getNextTemporaryVariable(op1[1]);
            addVarThreeAddressCodeLine(oPREFIX, target, op1, NULL);
            return target;
        case PLUS:
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            target = getNextTemporaryVariable(op1[1]);
            addVarThreeAddressCodeLine(oPLUS, target, op1, op2);
            return target;
        case MINUS:
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            target = getNextTemporaryVariable(op1[1]);
            addVarThreeAddressCodeLine(oMINUS, target, op1, op2);
            return target;
        case MOD:
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            target = getNextTemporaryVariable(op1[1]);
            addVarThreeAddressCodeLine(oMOD, target, op1, op2);
            return target;
        case DIV:
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            target = getNextTemporaryVariable(op1[1]);
            addVarThreeAddressCodeLine(oDIV, target, op1, op2);
            return target;
        case MUL:
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            target = getNextTemporaryVariable(op1[1]);
            addVarThreeAddressCodeLine(oMUL, target, op1, op2);
            return target;
        case ID:
            if(n == 0) return getFixedNameOfVar(getVarInfoByName(c)->type, c, 'v');
            varData = getVarInfoByName(c);
            op1 = getFixedNameOfVar(varData->type, c, 'v');
            op2 = getNextTemporaryVariable('i');
            if((varData->type)[1] == 's' || (varData->type)[1] == 'b') {
                op2 = createThreeAddressCode(t1);
                target = getNextTemporaryPointer('b');
            } else {
                tempstr1 = getNextTemporaryVariable('i');
                addConstThreeAddressCodeLine(oCONST, tempstr1, NULL, '\0', 4, true);
                tempstr2 = createThreeAddressCode(t1);
                addVarThreeAddressCodeLine(oMUL,op2, tempstr1, tempstr2);
                target = getNextTemporaryPointer('i');
            }
            addVarThreeAddressCodeLine(oPOINTER, target, op1, op2); //pointer with base op1 and offset op2
            return target;
        case STRING:
            target = getNextTemporaryVariable('s');
            addConstThreeAddressCodeLine(oSTRING, target, c, '\0', 0, true);
            return target;
        case BOOLEAN:
            target = getNextTemporaryVariable('t');
            if(n==0)
                addConstThreeAddressCodeLine(oBOOLEAN, target, NULL, '\0', 0, false);
            else
                addConstThreeAddressCodeLine(oBOOLEAN, target, NULL, '\0', 0, true);
            return target;
        case NOT:
            target = getNextTemporaryVariable('t');
            op1 = createThreeAddressCode(t1);
            addVarThreeAddressCodeLine(oNOT, target, op1, NULL);
            return target;
        case EQUAL:
            target = getNextTemporaryVariable('t');
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            addVarThreeAddressCodeLine(oEQUAL, target, op1, op2);
            return target;
        case EQLESS:
            target = getNextTemporaryVariable('t');
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            addVarThreeAddressCodeLine(oEQLESS, target, op1, op2);
            return target;
        case EQMORE:
            target = getNextTemporaryVariable('t');
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            addVarThreeAddressCodeLine(oEQMORE, target, op1, op2);
            return target;
        case NOTEQ:
            target = getNextTemporaryVariable('t');
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            addVarThreeAddressCodeLine(oNOTEQ, target, op1, op2);
            return target;
        case LESS:
            target = getNextTemporaryVariable('t');
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            addVarThreeAddressCodeLine(oLESS, target, op1, op2);
            return target;
        case MORE:
            target = getNextTemporaryVariable('t');
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            addVarThreeAddressCodeLine(oMORE, target, op1, op2);
            return target;
        case OR:
            target = getNextTemporaryVariable('t');
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            addVarThreeAddressCodeLine(oOR, target, op1, op2);
            return target;
        case AND:
            target = getNextTemporaryVariable('t');
            op1 = createThreeAddressCode(t1);
            op2 = createThreeAddressCode(t2);
            addVarThreeAddressCodeLine(oAND, target, op1, op2);
            return target;
        default:
            return NULL;
    }
}

