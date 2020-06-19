#include <stdio.h>
#include <stdlib.h>
#include "PartsFunction.h"

typedef struct{char TrackName[200];char BeltFn[260];//これが NULL でなければ出力する
}TRACK;
typedef struct{int NofTrack;TRACK *track;}TRACKS;

static int CreateTracks(TRACKS *dest,int argc,char *argv[])
{
	int i;dest->NofTrack = argc;dest->track = (TRACK*)calloc(argc,sizeof(TRACK));if(dest->track==NULL){printf("メモリ確保失敗");exit(1);}
	for(i=0;i<argc;i++)
	{
		//printf("%s\n",argv[i]);
		sprintf(dest->track[i].TrackName,"%s",argv[i]);
	}
	return 0;
}

static int OneTrackOut(const TRACK *source)
{
	USTFILEINFO ust={120};
	//if(source->BeltFn!=NULL)
	{
		printf("[%s]をファイル出力\n",source->TrackName);
		ust_file_out(source->TrackName,&ust);
	}
	return 0;
}

static int AllTracksOut(const TRACKS *source){int i;for(i=0;i<source->NofTrack;i++){OneTrackOut(&source->track[i]);}return 0;}

static int DestroyTracks(TRACKS *x)
{//int i;for(i=0;i<x->NofTrack;i++){printf("トラック名[%s]を破棄します\n",x->track[i].TrackName);}
	free(x->track);x->track = NULL;
	return 0;
}

/*
static int MakeTodo_TEST()//曲全体の流れを見て各楽節で何をするか決める
{
	FILE *fp;int i;
	char *fn = "ギターが各楽節ですべきこと.txt";
	fp = fopen(fn,"w");
	for(i=1;i<=9;i++)
	{
		fprintf(fp,"2,");
		switch(i)
		{
		case 1:case 9:
			fprintf(fp,"リフ");//前奏後奏
			break;
		case 2:case 6://Aメロ
			fprintf(fp,"リズム");
			break;
		case 3:case 7://サビ前
			fprintf(fp,"アルペジオ");
			break;
		case 4:case 8://サビ
			fprintf(fp,"コード");
			break;
		case 5:
			fprintf(fp,"ソロ");
			break;
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	return 0;
}
*/
static int WhatTodo_TEST(int PassageID,int time)
{
	int x;
	switch(PassageID)
	{
	case 1:case 6:x = 111;break;
	case 2:x = 222;break;
	case 3:x = 333;break;
	case 4:x = 444;break;
	case 5:x = 555;break;
	}
	return x;
}

static int Maker_JANICK_GERS_Passage(int PassageID,int time)
{
	int role;
	role = WhatTodo_TEST(PassageID,time);
	printf("ROAL = [%d]\n",role);
	return 0;
}


static int TestCall(){}
static char *IDtoFn_Movement(int ID){static char Fn[260];sprintf(Fn,"MOVEMENT_%d.txt",ID);return Fn;}

int Maker_JANICK_GERS_Movement(int MovementID)
{FILE *fp;char fn[260];
	FORMAT_MOVEMENT m;
	
	int i;
	TRACKS x;
	int NofTrack = 6;
	char *TrackName[] = {"ギター_1st","ギター_2st","ギター_3st","ギター_4st","ギター_5st","ギター_6st"};
	LoadMovementInfo(&m,MovementID);
	CreateTracks(&x,NofTrack,TrackName);
	
	
	for(i=0;i<NofTrack;i++){sprintf(fn,"%s.txt",TrackName[i]);fp = fopen(fn,"w");fprintf(fp,"1920,%d\n",i+60);fclose(fp);}
	
	///////////これまでにベルトファイルを作らなければならない
	AllTracksOut(&x);
	DestroyTracks(&x);
	UnloadMovementInfo(&m);
	return 0;
}
