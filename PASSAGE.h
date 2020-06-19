#ifndef PASSAGE_H
#define PASSAGE_H
#include "music.h"
typedef struct{
	int masterfile,length,chordlength;
	CHORD start,end;//endはあまり気にされないべき
	int kind;
	int NofBar,count,size;
	int auftaktID;
	int CoreID;
	int Unit;//基本となる小節(コア)の個数(2の累乗個のみ)
}FORMAT_PASSAGE;//役立たず
typedef struct{int key;}PASSAGE_HEADER;//NEW

typedef struct{char MovenentName[200];int tempo;}FORMAT_MOVEMENT;

#ifndef PASSAGE_C
extern FORMAT_PASSAGE passage_fscanf(FILE *fp);//役立たず
extern int PassageMaker(char *movementfn,int linenumber);//NEW
//何行目(linenumber)の楽節を作って欲しいかで良くない?
#endif
#endif
