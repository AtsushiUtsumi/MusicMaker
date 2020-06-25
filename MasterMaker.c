//同じ ID ならルーツは同じ(しかし完全コピーとは限らない)
#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include "PASSAGE.h"
#include "masterfile.h"

static int pow(unsigned int x,unsigned int n){
	int ret=1;
	int i;
	for(i=0;i<n;i++){ret *= x;}
	return ret;
}

static int pow2(const int big,const int small)
{
	int i;
	for(i=0;;i++){if(small*pow(2,i)==big){return i;}else if(small*pow(2,i)>big){printf("[%d] IS NOT POW\n",big);return -1;exit(1);}}
}

static int TestBaiCore(int CoreID,int NofBar)
{
	extern int newmynumber();MASTERFORMAT m={0};
	FILE *MasterFp;char MasterFn[260];int HowManyTimesBai;
	HowManyTimesBai = pow2(NofBar,TestCoreBarNumber(CoreID));
	printf("CORE[%d] を [%d] 回倍して[%d]小節分のマスターファイル作成 by TestBaiCore()\n",CoreID,HowManyTimesBai,NofBar);
	sprintf(MasterFn,"MASTER_仮_%d.txt",newmynumber());MasterFp = fopen(MasterFn,"w");master_fprintf(MasterFp,&m);fclose(MasterFp);
	return 0;
}

int TestCoreMaker(int CoreID,int NofBar){
	int i;int NofBeatPerBar = 8;
	int Length,BH,CH,AH,RH;int pucchoID;
	
	FILE *Corefp;char Corefn[260];sprintf(Corefn,"CORE_%d.txt",CoreID);
	Corefp = fopen(Corefn,"w");
	fprintf(Corefp,"0,小節数\n1,%d\n",NofBar);
	for(i=0;i<NofBar*NofBeatPerBar;i++)
	{
		Length = 240;
		BH = (i%NofBeatPerBar==0)?1:0;
		CH = 1;AH = 2;RH = 3;
		fprintf(Corefp,"2,%d,%d,%d,%d,%d\n",Length,BH,CH,AH,RH);
	}
	fclose(Corefp);//printf("終了します\n");exit(1);
	return 0;
}

int TestCoreBarNumber(int CoreID){
	int LineKind,x;char Line[1280];
	FILE *Corefp;char Corefn[260];
	sprintf(Corefn,"CORE_%d.txt",CoreID);
	Corefp = fopen(Corefn,"r");
	while(fscanf(Corefp,"%d,%s",&LineKind,Line)>0)
	{
		//printf("[%d,%s] by TestCoreBarNumber()\n",LineKind,Line);
		if(LineKind==1){fclose(Corefp);sscanf(Line,"%d",&x);
		//printf("CORE[%d]に[%d]個の小節 by TestCoreBarNumber()\n",CoreID,x);
		return x;}
	}
}

int TEST_MASTER_PARENT_Maker(int PassageID,int NofBar)//NofBar個の小節を含むマスターファイルを作成してその ID を返す
{FORMAT_PASSAGE p;
	int MasterID;printf("TEST_MASTER_PARENT_Maker(%d) 開始\n",PassageID);
	PassageLoad(&p,PassageID);
	TestCoreMaker(p.CoreID,NofBar/4);//ひとまず全体の 1/4 のコアにしとく
	printf("CORE[%d]に[%d]個の小節 by TEST_MASTER_PARENT_Maker()\n",p.CoreID,TestCoreBarNumber(p.CoreID));
	TestBaiCore(p.CoreID,NofBar);
	//printf("終了します by TEST_MASTER_PARENT_Maker(%d)\n",PassageID);exit(1);
	return MasterID;
}

int PassageLoad(FORMAT_PASSAGE *dest,int PassageID)
{
	char *table[] = {"PASSAGE[前奏].txt","PASSAGE[Aメロ].txt","PASSAGE[Bメロ].txt","PASSAGE[サビ].txt","PASSAGE[間奏].txt","PASSAGE[後奏].txt"};
	FILE *PassageFp;char PassageFn[260];FORMAT_PASSAGE x;sprintf(PassageFn,"%s",table[PassageID-1]);
	PassageFp = fopen(PassageFn,"r");x = passage_fscanf(PassageFp);fclose(PassageFp);
	*dest = x;
	//printf("終了します\n");exit(1);
	return 1;
}

int MasterMaker_Passage(int MovementID,int PassageID)
{
	int LineKind;char Line[1280];FILE *PassageFp;char PassageFn[260];printf("MasterMaker_Passage(%d)開始\n",PassageID);
	printf("MasterMaker_Passage(%d)開始\n",PassageID);sprintf(PassageFn,"PASSAGE_%d_%d.txt",MovementID,PassageID);

TEST_MASTER_PARENT_Maker(PassageID,16);
//exit(1);
/*
	PassageFp = fopen(PassageFn,"r");if(PassageFp==NULL){printf("[%s]が見つかりません\n\n",PassageFn);exit(1);}
	while(fscanf(PassageFp,"%d,%s",&LineKind,Line)>0)
	{
		switch(LineKind)
		{
		case 1:default://今は何でも表示
			printf("[%s]をスキャンしました by MasterMaker_Passage()\n",Line);
			break;//どうでもいい日本語情報は0でスルーってことで
		}
	}
	fclose(PassageFp);
*/
//printf("終了します by MasterMaker_Passage()\n");exit(1);
	return 0;
}

int MasterMaker_Movement(int MovementID)
{int count,time,PassageID;
	int LineKind;char Line[1280];FILE *MovementFp;char MovementFn[260];printf("MasterMaker_Movement(%d) 開始\n",MovementID);

	sprintf(MovementFn,"MOVEMENT_%d.txt",MovementID);
	MovementFp = fopen(MovementFn,"r");if(MovementFp==NULL){printf("[%s]が見つかりません\n",MovementFn);return (-1);}
	while(fscanf(MovementFp,"%d,%s",&LineKind,Line)>0)
	{
		switch(LineKind)
		{
		case 2:
			sscanf(Line,"%d,%d,%d",&count,&time,&PassageID);//本来は専用のスキャン関数を使うべき
			MasterMaker_Passage(MovementID,PassageID);break;
		case 1:default://今は何でも表示
			printf("[%s]をスキャンしました by MasterMaker_Movement()\n",Line);
			break;/*どうでもいい日本語情報は0でスルーってことで*/
		}
	}
	fclose(MovementFp);
	//printf("終了します by MasterMaker_Movement()\n");exit(1);
	return 0;
}
