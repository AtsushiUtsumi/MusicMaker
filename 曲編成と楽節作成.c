
#include <stdio.h>
#include <stdlib.h>
#define PASSAGE_C
#include "PASSAGE.h"

#include "music.h"

//typedef struct{int masterfile,length,chordlength;CHORD start,end;}FORMAT_PASSAGE;



static int myfprintf(FILE *fp,FORMAT_PASSAGE *fpf)//フォーマットをファイルに1行書き込む関数
{
	fprintf(fp,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n"
		,fpf->masterfile
		,fpf->length
		,fpf->chordlength
		,fpf->start.dia
		,fpf->start.kind
		,fpf->end.dia
		,fpf->end.kind
		,fpf->kind
		,fpf->NofBar
		,fpf->count
		,fpf->size
		,fpf->auftaktID,fpf->Unit,fpf->CoreID);//printf("[%d]のアウフタクトは[%d] by myfprintf()\n",fpf->masterfile,fpf->auftaktID);
	fprintf(fp,"MF,length,chordlength,コードの最初最後,種類,小節数,メモリ数,1メモリの長さ\n");
	return 0;
}

static int make_passage_file(char *pfn,FORMAT_PASSAGE *fpf)//単体の楽節ファイルを作成する関数
{
	FILE *pfp;
	pfp = fopen(pfn,"w");
	if(pfp==NULL){printf("[%s]が見つかりません\n",pfn);return (-1);}
	else{myfprintf(pfp,fpf);fclose(pfp);return 0;}
}

static int string_to_kind(char *pfn)
{
	int x = 0;
	if(strcmp(pfn,"PASSAGE[Aメロ].txt")==0){x = 1;}
	else if(strcmp(pfn,"PASSAGE[Bメロ].txt")==0){x = 2;}
	else if(strcmp(pfn,"PASSAGE[サビ].txt")==0){x = 3;}
	else if(strcmp(pfn,"PASSAGE[前奏].txt")==0){x = 4;}
	else if(strcmp(pfn,"PASSAGE[間奏].txt")==0){x = 5;}
	else if(strcmp(pfn,"PASSAGE[後奏].txt")==0){x = 6;}
	return x;
}

static int make_passage_files(char *kyokuhennseifilename)//曲編成のファイル名を引数にして複数の楽節ファイルを出力する関数
{
	FILE *khfp;FILE *pfp;char pfn[128];
	extern int newmynumber();
	int i;
	FORMAT_PASSAGE fpf;





	khfp = fopen(kyokuhennseifilename,"r");
	
	while(fscanf(khfp,"%s",pfn)>0)
	{
		pfp = fopen(pfn,"r");
		if(pfp!=NULL)
		{
			//printf("[%s]は既に存在する楽節ファイル名です\n",pfn);
			fclose(pfp);
		}
		else
		{//この初期化きっと後悔する
			fpf.kind = string_to_kind(pfn);
			
			fpf.size = 1920 / 8;//ひとまず♪で考える(ここをにするとクラッシュ)
			fpf.NofBar = 8;//ひとまず8小節
			fpf.length = 1920 * fpf.NofBar;
			fpf.count = fpf.length / fpf.size;
			
			
			fpf.auftaktID = (fpf.masterfile==3) ? 1:0;
			
			fpf.CoreID = 77;
			fpf.Unit = (rand()%2==0) ? 2:4;


			fpf.masterfile = newmynumber();
			switch(fpf.masterfile)
			{
			case 1:case 5:case 6:fpf.Unit = 4;break;
			case 2:fpf.Unit = 8;break;
			case 3:fpf.Unit = 4;break;
			case 4:fpf.Unit = 8;break;
			default:printf("不明なマスター_[%d]\n終了します by make_passage_files()\n",fpf.masterfile);exit(1);break;
			}
			fpf.chordlength = 1920;
			
			//////ひとまずトライアドトニック
			switch(fpf.kind)
			{
			case 4:fpf.start.dia = 5;fpf.start.kind = 1;fpf.end.dia = 4;fpf.end.kind = 1;break;
			case 1:fpf.start.dia = 1;fpf.start.kind = 1;fpf.end.dia = 4;fpf.end.kind = 1;break;
			case 2:fpf.start.dia = 3;fpf.start.kind = 2;fpf.end.dia = 5;fpf.end.kind = 1;break;
			case 3:fpf.start.dia = 1;fpf.start.kind = 1;fpf.end.dia = 1;fpf.end.kind = 1;break;
			case 5:fpf.start.dia = 3;fpf.start.kind = 2;fpf.end.dia = 5;fpf.end.kind = 1;break;
			case 6:fpf.start.dia = 1;fpf.start.kind = 1;fpf.end.dia = 1;fpf.end.kind = 1;break;
			}//fpf.start.dia = 1;fpf.start.kind = 3;fpf.end.dia = 1;fpf.end.kind = 3;

			
			//printf("fpf.kind = %d\n",fpf.kind);
			make_passage_file(pfn,&fpf);
		}
	}
	
	
	fclose(khfp);
	return 0;
}

static int make_kyokuhennsei_file(char *fn)//曲編成ファイル出力関数
{
	char *table[] = {"PASSAGE[前奏].txt","PASSAGE[Aメロ].txt","PASSAGE[Bメロ].txt","PASSAGE[サビ].txt","PASSAGE[間奏].txt","PASSAGE[Aメロ].txt","PASSAGE[Bメロ].txt","PASSAGE[サビ].txt","PASSAGE[後奏].txt"};
	
	int i;
	FILE *fp;
	
	fp = fopen(fn,"w");
	for(i=0;i<9;i++){fprintf(fp,"%s\n",table[i]);}

	fclose(fp);
	return 0;
}

int test_HandP()//曲編成と楽節ファイルのテスト関数,どうせ main() からしか呼ばれない
{char *kfn = "曲編成.txt";make_kyokuhennsei_file(kfn);make_passage_files(kfn);return 0;}

FORMAT_PASSAGE passage_fscanf(FILE *fp)
{
	FORMAT_PASSAGE p;
	char cmd[1280];
	fscanf(fp,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s"
		,&p.masterfile
		,&p.length
		,&p.chordlength
		,&p.start.dia,&p.start.kind
		,&p.end.dia,&p.end.kind
		
		
		,&p.kind
		,&p.NofBar
		,&p.count
		,&p.size
		,&p.auftaktID,&p.Unit,&p.CoreID
		,cmd);
	return p;
}
