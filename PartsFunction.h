#ifndef PartsFunction_H
#define PartsFunction_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SCORE.h"
#include "MUSIC.h"
#include "UST.h"
#include "SESSION.h"
#include "masterfile.h"
#include "PASSAGE.h"
#define LINEKIND_BODY 2
//cside==(-1) でコピーは不要
#ifndef PartsFunction_C
extern PART **CreatePart(int n,int NofOP,int MovementID,int PassageID);
extern int DestroyPart(PART **x,int NofOP);
extern int IsWhite(int note,int key);
extern int NearWhite(int note,int whitefile,int pm);
extern FILE *WaveFileOpen(int n,int WaveID);
extern CHILDREN_SIDE *CreateCside(int n,int MasterNumber);
extern int balance(int MasterID,int pre);//大体の値を引数にして正確な値を返す
extern int LoadPassageInfo(FORMAT_PASSAGE *dest,int PassageID);






extern int LoadMasterInfo(MASTERFORMAT *x,int MasterID);
extern int UnloadMasterInfo(MASTERFORMAT *x);

extern int LoadMovementInfo(FORMAT_MOVEMENT *dest,int MovementID);
#endif
#endif
