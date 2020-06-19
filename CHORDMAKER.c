#define CHORDMAKER_C
#include <stdio.h>
#include <stdlib.h>
#include "MUSIC.h"
#include "PASSAGE.h"
#include "masterfile.h"


static int ChordPoolMaker(int passageID)
{
	FILE *cpfp;char cpfn[260];sprintf(cpfn,"CHORD_POOL_%d.txt",passageID);
	cpfp = fopen(cpfn,"w");fprintf(cpfp,"優先して使うべきコード進行\n");
	fclose(cpfp);return 0;
}


//マスターファイル毎に候補を作っていると
static int ChordMaker_Master(int masterfilenumber,char *outputfn,CHORD *start,CHORD *end,int chordpool,int passageID)//今はpIDは最初のマスターで chordpool = pID
{//if(requestfp!=NULL){リクエストにお応えします;}else{好き勝手に決めます;}
	char kouhofn[260];typedef struct{int c_masterfilenumber;char c_outputfn[260];CHORD c_start;CHORD c_end;}CHILD;CHILD *c;
	typedef struct{int length,bh,ch,ah,rh;}MAP_LINE;MAP_LINE *map;int NofLine=0;int linekind;char line[1000];int i=0;int NofC=0;
	FILE *mapfp;char mapfn[FILENAME_MAX];
	FILE *masterfp;MASTERFORMAT m;
//printf("[POOL,pID] = [%d,%d]\n",chordpool,passageID);
//printf("ChordMaker_Master(%d)\n",masterfilenumber);
	masterfp = masteropen(masterfilenumber);m = master_fscanf(masterfp);//以後マスターにアクセス可能
//////////////////////////////////////////////////////////////////MAP読み込み開始
	sprintf(mapfn,"MAP_%d.txt",masterfilenumber);
	mapfp = fopen(mapfn,"r");while(fscanf(mapfp,"%d,%s",&linekind,line)>0){switch(linekind){case 1:break;case 2:NofLine++;break;}}fclose(mapfp);mapfp=NULL;
	map = (MAP_LINE*)calloc(NofLine,sizeof(MAP_LINE));
	mapfp = fopen(mapfn,"r");
	while(fscanf(mapfp,"%d,%s",&linekind,line)>0){switch(linekind){case 1:break;case 2:sscanf(line,"%d,%d,%d,%d,%d",&map[i].length,&map[i].bh,&map[i].ch,&map[i].ah,&map[i].rh);if(map[i].ch>0){NofC++;}i++;break;}}
	fclose(mapfp);mapfp=NULL;
//////////////////////////////////////////////////////////////////MAP読み込み終了

	//ここで腹をくくった世界と,くくらなかった世界の両方を実行
	//つまりここでケーデンスとして出力しつつ,子供達にそれぞれ決めてもらう
///////////////////////////////////////////////////////////////////////////////世界1の始まり(ここで決める)
	nchord(NofC,start,end);sprintf(kouhofn,"コード候補_[%d].txt",masterfilenumber);rename("Nコード.txt",kouhofn);
///////////////////////////////////////////////////////////////////////////////世界1の終わり

///////////////////////////////////////////////////////////////////////////////世界2の始まり(子供達に決めてもらう)
	c = (CHILD*)calloc(m.linenumber,sizeof(CHILD));
	for(i=0;i<m.linenumber;i++)
	{
		fscanf(masterfp,"%d",&c[i].c_masterfilenumber);
		if(i%2==0){c[i].c_start = *start;c[i].c_end.dia = 5;c[i].c_end.kind = 1;}
		else if(i==m.linenumber-1){c[i].c_start.dia = 4;c[i].c_start.kind = 2;c[i].c_end = *end;}
		else{c[i].c_start.dia = 4;c[i].c_start.kind = 2;c[i].c_end.dia = 1;c[i].c_end.kind = 1;}
	}
	//for(i=0;i<m.linenumber;i++){printf("[%d]の子供は[%d] by ChordMaker_Master()\n",masterfilenumber,c[i].c_masterfilenumber);}
for(i=0;i<m.linenumber;i++){ChordMaker_Master(c[i].c_masterfilenumber,c[i].c_outputfn,&c[i].c_start,&c[i].c_end,chordpool,passageID);}
	free(c);
///////////////////////////////////////////////////////////////////////////////世界2の終わり
	
	//for(i=0;i<NofLine;i++){printf("%2d = %d\n",i,map[i].bh);}
	free(map);map=NULL;fclose(masterfp);masterfp=NULL;
	return 0;
}

//楽節ファイルに合うコード進行作成手段は選ばない
static int ChordMaker_Passage(char *passagefn,char *outputfn)//別に 出力==NULL なら出力不要 ?
{int passageID;
	//CHORD start={1,CHORD_KIND_M},end={1,CHORD_KIND_M};
	typedef struct{int length,bh,ch,ah,rh;}MAP_LINE;MAP_LINE *map;int NofLine=0;int linekind;char line[1000];int i=0;int NofC=0;
	FILE *mapfp;char mapfn[FILENAME_MAX];
	FILE *passagefp,*outputfp;FORMAT_PASSAGE p;
	//printf("[%s]に合うコード進行を考えます\n出力 => [%s] by ChordMaker()\n",passagefn,outputfn);
	passagefp = fopen(passagefn,"r");p = passage_fscanf(passagefp);
	fclose(passagefp);passagefp=NULL;passageID = p.masterfile;
ChordPoolMaker(passageID);
	ChordMaker_Master(p.masterfile,NULL,&p.start,&p.end,p.masterfile,passageID);
/*
	sprintf(mapfn,"MAP_%d.txt",p.masterfile);
	mapfp = fopen(mapfn,"r");while(fscanf(mapfp,"%d,%s",&linekind,line)>0){switch(linekind){case 1:break;case 2:NofLine++;break;}}fclose(mapfp);
	map = (MAP_LINE*)calloc(NofLine,sizeof(MAP_LINE));
	
	mapfp = fopen(mapfn,"r");
	while(fscanf(mapfp,"%d,%s",&linekind,line)>0)
	{
		switch(linekind)
		{
		case 1:break;
		case 2:
			sscanf(line,"%d,%d,%d,%d,%d",&map[i].length,&map[i].bh,&map[i].ch,&map[i].ah,&map[i].rh);if(map[i].ch>0){NofC++;}
			i++;break;
		}
	}
	fclose(mapfp);//for(i=0;i<NofLine;i++){printf("%2d = %d\n",i,map[i].bh);}
	
	outputfp = fopen(outputfn,"w");
	//printf("[%d]行です =>[%d]個のコード\n",NofLine,NofC);
	fclose(outputfp);outputfp=NULL;free(map);
*/
	return 0;
}

int ChordMaker_Movement(char *movementfn)
{//まず曲全体を見てそれぞれの楽節で使うコードプールを作成すること
	int m,time,id;char name[20];
	int linekind;char line[1000];
	FILE *movementfp;int i=0;
	char passagefn[FILENAME_MAX];
	char chordfn[FILENAME_MAX];
	movementfp = fopen(movementfn,"r");
	if(movementfp==NULL){printf("[]が見つかりません\n終了します by ChordMaker_Movement()\n",movementfn);exit(1);}
	while(fscanf(movementfp,"%d,%s",&linekind,line)>0)
	{
		switch(linekind)
		{
		case 1:break;
		case 2:
			sscanf(line,"%d,%d,%d,%s",&m,&time,&id,name);
			sprintf(chordfn,"[%s(%d回目)]のコード進行.txt",name,time);
			sprintf(passagefn,"PASSAGE[%s].txt",name);
			ChordMaker_Passage(passagefn,chordfn);
			break;
		}
	}
	fclose(movementfp);movementfp=NULL;
	return 0;
}
