//パート無しのスコアファイル作成関数
#define SCORE_C
#include <stdio.h>
#include <stdlib.h>
#include "SCORE.h"
#include "PASSAGE.h"
#include "masterfile.h"

#define LINEKIND_HEADER 1
#define LINEKIND_BODY   2

int score_line_sscanf(char *line,SCORE_LINE *dest)
{
	//本来なら文字列ポインタを引数にすべき
	//そもそもこの関数を呼んでいるということは LINEKIND は既知のはず
	//など考えてるうちに修正完了
	
	SCORE_LINE x;
	sscanf(line,"%d,%d,%d,%d,%d,%d,%d,%d,%d",&x.length,&x.bh,&x.ch,&x.ah,&x.rh,&x.chord_root,&x.chord_kind,&x.passage_head,&x.passage_number);
	*dest = x;
	return 0;
}

static int score_line_fprintf(FILE *fp,SCORE_LINE *sour)
{
	SCORE_LINE x;
	x = *sour;
	fprintf(fp,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",LINEKIND_BODY,x.length,x.bh,x.ch,x.ah,x.rh,x.chord_root,x.chord_kind,x.passage_head,x.passage_number);
	return 0;
}

static int ScoreMaker_Master(FILE *scorefp,int masterfilenumber)//Mファイルから得られるスコアをFPの末尾に追加
{
	int t;//適当
	
	int linekind;char line[1280];
	FILE *mapfp;
	char mapfn[FILENAME_MAX];
	
	SCORE_LINE s = {-1,-2,-3,-4,-5,-6,-7};
	
	MASTERFORMAT m;
	FILE *masterfp;
	masterfp = masteropen(masterfilenumber);
	m = master_fscanf(masterfp);
	fclose(masterfp);

	//fprintf(scorefp,"M_[%d]のスコア追加します by ScoreMaker_Master()\n",masterfilenumber);

	sprintf(mapfn,"MAP_%d_NEW.txt",masterfilenumber);//今は MAP_NEW ファイル参照
	mapfp = fopen(mapfn,"r");
	if(mapfp!=NULL)//if(ここで埋める,つまりコード装填済みのファイルがある)
	{
		t=1;//今は楽節ヘッドに 1 を使用本来なら他のように長さにすべき

		while(fscanf(mapfp,"%d,%s",&linekind,line)>0)
		{
			if(linekind==LINEKIND_BODY)
			{
				fprintf(scorefp,"2,%s,%d,%d\n",line,t,masterfilenumber);//今は楽節固有の値にマスターファイルのナンバーを使用
				t=0;
			}
			//score_line_fprintf(scorefp,&s);
		}
		fclose(mapfp);
	}//else if(子供達に埋めてもらう){未開発;}
	
	

	return 0;
}

static int ScoreMaker_Passage(FILE *scorefp,char *passagefn)//楽節ファイルから得られるスコアをFPの末尾に追加
{
	FORMAT_PASSAGE p;
	FILE *passagefp;
	passagefp = fopen(passagefn,"r");
	p = passage_fscanf(passagefp);
	
	//fprintf(scorefp,"\n[%s]のスコア追加します by ScoreMaker_Passage()\n",passagefn);
	
	ScoreMaker_Master(scorefp,p.masterfile);
	
fclose(passagefp);
	return 0;
}

int ScoreMaker_Movement(char *movementfn)//楽章(1曲)のスコアファイル作成関数
{
	char passagefn[FILENAME_MAX];
	
	FILE *scorefp;
	char scorefn[FILENAME_MAX];
	
	FILE *movementfp;
	
	sprintf(scorefn,"スコア.txt");
	scorefp = fopen(scorefn,"w");
	fprintf(scorefp,"%d,120\n",LINEKIND_HEADER);
	
	movementfp = fopen(movementfn,"r");
	if(movementfp==NULL){printf("[%s]が見つかりません=>終了します\n",movementfn);exit(1);}
	while(fscanf(movementfp,"%s",passagefn)>0){ScoreMaker_Passage(scorefp,passagefn);}
	fclose(movementfp);
	
	fclose(scorefp);
	return 0;
}

/*
//バックアップ
static int ScoreMaker_Master(FILE *scorefp,int masterfilenumber)//Mファイルから得られるスコアをFPの末尾に追加
{
	SCORE_LINE scoreline = {-1,-2,-3,-4,-5,-6,-7};
	
	MASTERFORMAT m;
	FILE *masterfp;
	masterfp = masteropen(masterfilenumber);
	m = master_fscanf(masterfp);
	fclose(masterfp);

	fprintf(scorefp,"M_[%d]のスコア追加します by ScoreMaker_Master()\n",masterfilenumber);
	//if(ここで埋める)
	{score_line_fprintf(scorefp,&scoreline);}
	//else if(子供達に埋めてもらう){未開発;}
	

	return 0;
}
*/
