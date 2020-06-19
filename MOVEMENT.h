#ifndef MOVEMENT_H
#define MOVEMENT_H
typedef struct{int tempo,key,NofPassage;}MOVEMENT_INFO;
#ifndef MOVEMENT_C
extern int MovementMaker(MOVEMENT_INFO *movementinfo);
#endif
#endif
