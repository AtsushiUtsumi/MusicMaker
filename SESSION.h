#ifndef SESSION_H
#define SESSION_H
typedef struct{int head,note;}PART;
#ifndef SESSION_C
extern int NumberOfParts(char *sessionfn,int movementID);
extern int AddSession_Passage(int movementID,int passageID,char *addpartfn);
extern int LoadSession_Passage(PART **part,int NofPart,int movementID,int passageID);
#endif
#endif
