//
// Created by volglizolic on 10/22/18.
//

#ifndef ALAN_TREEADDRESSCODE_H
#define ALAN_TREEADDRESSCODE_H

#include "ast.h"

typedef enum {
    oINIT, //0
    oPROC, //1
    oVARDEF, oFUNCDEF, //3
    oIF, oIFELSE, oWHILE, oSET, oFUNCCALL, oRETURN, //9
    oSIGNLETYPE, oARRAYTYPE, oRTYPE, oPROCTYPE, //13
    oVAR, oVARARRAY, oCONST, oCHAR, oID, oSTRING, //19
    oPLUS, oMINUS, oMUL, oDIV, oMOD, oPREFIX, //25
    oBOOLEAN, oNOT, oEQUAL, oEQLESS, oEQMORE, oNOTEQ, oLESS, oMORE, oOR, oAND, //35
    oPOINTER, oMALLOC, oLABEL, oPARAM, oRFPARAM, //40
    oJUMP, oPUSH, oPOP, oRET //44
} operation;

typedef struct threeAddr {
    operation k;
    char *target, *operant1, *operant2;
    int number;
    char c;
    char *text;
    bool boolean;
    struct threeAddr *next; /*t for tree*/
} threeAddressNode;

typedef struct varIndexTemp{
    char *id;
    char *type;
    int size;
    struct varIndexTemp *prevEntry;
}varIndex;

typedef struct funcIndexTemp{
    char *id;
    char *type;
    char *signature;
    char *trueSignature;
    struct funcIndexTemp *prevEntry;
}funcIndex;

void initThreeAddressCode();

char* createThreeAddressCode(ast t);

#endif //ALAN_TREEADDRESSCODE_H
