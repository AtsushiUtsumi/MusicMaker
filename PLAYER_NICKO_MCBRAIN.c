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

#define BassDrum_MIDI     29
#define Snare_MIDI        40

#define HighTom_MIDI      45

#define LowTom_MIDI       45

#define FloorTom_MIDI     45

#define CrashCymbal_MIDI  49
#define RideCymbal_MIDI   51
#define OpenHiHat_MIDI    46
#define ClosedHiHat_MIDI  42

static const char *playername = "NICKO_MCBRAIN";
static const char *partname = "PLAYER_NICKO_MCBRAIN";
static int NOP;//起動時での他のパート数を記録しておく
static int NofS = 6;

//typedef struct{int length,note;}PART;//セッションで使用する
typedef struct{int wave,pre,note;}X;//個々のパートで使用
//typedef struct{int c_head,c_file,have_more_children;}CHILDREN_SIDE;


static FILE *PassageOpen(int PassageID)
{
	FILE *fp;char fn[260];
	switch(PassageID)
	{
	case 1:sprintf(fn,"PASSAGE[前奏].txt");break;
	case 2:sprintf(fn,"PASSAGE[Aメロ].txt");break;
	case 3:sprintf(fn,"PASSAGE[Bメロ].txt");break;
	case 4:sprintf(fn,"PASSAGE[サビ].txt");break;
	case 5:sprintf(fn,"PASSAGE[間奏].txt");break;
	case 6:sprintf(fn,"PASSAGE[後奏].txt");break;
	default:printf("¥n不明な楽節ID = [%d]¥n終了します¥n",PassageID);exit(1);break;
	}
	fp = fopen(fn,"r");
	if(fp==NULL){printf("[%s]が見つかりません by PassageOpen()¥n終了します¥n",fn);exit(1);}
	//else{printf("[%s]を開きます by PassageOpen()¥n",fn);}
	return fp;
}

int fill_children_side(int masterfilenumber,CHILDREN_SIDE *dest,int n)//マスターファイルを使う関数には便利かも
{
	MASTERFORMAT m,c;int x=(-1);//一枚岩なら全部が代入される
	int i,j;FILE *masterfp,*childfp;
	masterfp = masteropen(masterfilenumber);m = master_fscanf(masterfp);
	//if(n!=m.count){printf("¥n¥nfill_children_side()¥n[%d] <= m.count¥n[%d] <= n (引数)¥n終了します¥n¥n",m.count,n);exit(1);}
	for(i=0;i<n;i++){if(i%(n/m.linenumber) ==0){fscanf(masterfp,"%d",&x);dest[i].c_head = 1;dest[i].have_more_children = 1;}dest[i].c_file = x;}
	for(i=0;i<n;i++)
	{
		if(dest[i].c_head==1)//ヘッドキャッチ
		{
			childfp = masteropen(dest[i].c_file);c = master_fscanf(childfp);if(c.linenumber>0){dest[i].have_more_children = 1;}
			fclose(childfp);
			for(j=i;j<n;dest[i].c_head++,j++){if(j==n-1 || dest[j+1].c_head>0)break;}//実はこれ,そんなに難しいことではない
		}
	}
	fclose(masterfp);return 0;
}

static FILE *DrumsPatternOpen(int MovementID,int PassageID,int MasterID)
{
	int i;int beat_head,Hit_K,Hit_L,Hit_R;
	int setting[] = {ClosedHiHat_MIDI,Snare_MIDI,OpenHiHat_MIDI,HighTom_MIDI,LowTom_MIDI,FloorTom_MIDI,RideCymbal_MIDI,CrashCymbal_MIDI};
	FILE *patternfp;
	char patternfn[260];
	int NofS;//NUmberOfSet
	NofS = sizeof(setting) / sizeof(int);//ユニット計算
//printf("[%d]点セット by DrumsPatternOpen()¥n",NofS);
	sprintf(patternfn,"%s_%d_%d_%d.txt",partname,MovementID,PassageID,MasterID);
	patternfp = fopen(patternfn,"r");
	if(patternfp==NULL)
	{
		patternfp = fopen(patternfn,"w");
		for(i=0;i<64;i++)
		{beat_head = (i%2==0) ? 1:0;
			switch(PassageID)
			{
			case 1:Hit_K = BassDrum_MIDI;Hit_L = (i%2==1) ? setting[0]:0;Hit_R = 0;break;
			case 2:Hit_K = BassDrum_MIDI;Hit_L = (i%4==0) ? setting[1]:0;Hit_R = 0;break;
			case 3:Hit_K = BassDrum_MIDI;Hit_L = (i%2==1) ? setting[2]:0;Hit_R = 0;break;
			case 4:Hit_K = BassDrum_MIDI;Hit_L = (i%4==0) ? setting[3]:0;Hit_R = 0;break;
			case 5:Hit_K = BassDrum_MIDI;Hit_L = (i%2==1) ? setting[4]:0;Hit_R = 0;break;
			case 6:Hit_K = BassDrum_MIDI;Hit_L = (i%4==0) ? setting[5]:0;Hit_R = 0;break;
			}
			fprintf(patternfp,"2,%d,%d,%d,%d¥n",beat_head,Hit_K,Hit_L,Hit_R);
		}
		fclose(patternfp);
		patternfp = fopen(patternfn,"r");
	}
	return patternfp;
}

static int LR_Hit(int *dest_L,int *dest_R,int passageID)
{int NofS;
	int setting[] = {ClosedHiHat_MIDI,Snare_MIDI,OpenHiHat_MIDI,HighTom_MIDI,LowTom_MIDI,FloorTom_MIDI,RideCymbal_MIDI,CrashCymbal_MIDI};
	NofS = sizeof(setting)/sizeof(int);
	switch(passageID)
	{
	case 1:*dest_L = Snare_MIDI;break;
	case 2:*dest_L = LowTom_MIDI;break;
	case 3:*dest_L = RideCymbal_MIDI;break;
	case 4:*dest_L = OpenHiHat_MIDI;break;
	case 5:*dest_L = ClosedHiHat_MIDI;break;
	case 6:*dest_L = CrashCymbal_MIDI;break;
	}*dest_L = setting[rand()%NofS];
	return 0;
}

static int Maker_NICKO_MCBRAIN_Master(int MasterNumber,SCORE_LINE *s,X **x,int n,int PassageID,int NofS,int NofOP,PART **OP)
{
	CHILDREN_SIDE *cside;int i,j,k;
	
	int linekind,beathead;FILE *patternfp;patternfp = DrumsPatternOpen(1,PassageID,MasterNumber);

	cside = (CHILDREN_SIDE *)calloc(n,sizeof(CHILDREN_SIDE));
	fill_children_side(1,cside,n);

	//ここでひとくくりにする
	for(i=0;i<n;i++)
	{
		if(cside[i].c_head>0){rewind(patternfp);}LABEL_REWIND:
		if(fscanf(patternfp,"%d,%d,%d,%d,%d",&linekind,&beathead,&x[i][0].note,&x[i][1].note,&x[i][2].note)<=0){rewind(patternfp);goto LABEL_REWIND;}
	}
	//for(i=0;i<n;i++){x[i][0].note = (1%2==0) ? BassDrum_MIDI:0;}//足
	for(i=0;i<n;i++){if((i<(n/2)&&i>(n*3/8)) || i>(n*7/8)){LR_Hit(&(x[i][1].note),&(x[i][2].note),PassageID);}}//手
	fclose(patternfp);
	//ここでひとくくりにしない
	//Maker_NICKO_MCBRAIN_Master(int MasterNumber,SCORE_LINE *s,X **x,int n,int PassageID,int NofS,int NofOP,PART **OP);//回帰呼び出し
//for(i=0;i<n;i++){x[i][0].note = i;}
	//同じマスターナンバーにコピー(よろしければアレンジします)
///*
	for(i=0;i<n;i++)
	{
		if(cside[i].c_head>0)//1st ヘッドキャッチ
		{
			for(j=i;j<n;j++)
			{
				if(cside[j].c_head>0 && cside[j].c_file==cside[i].c_file)//2nd ヘッドキャッチ
				{
					for(k=0;j+k<n;k++)
					{
						if(cside[i+k].c_file==cside[j+k].c_file)
						{
							x[j+k][0].note = x[i+k][0].note;
							x[j+k][1].note = x[i+k][1].note;
							x[j+k][2].note = x[i+k][2].note;
							x[j+k][3].note = x[i+k][3].note;
							//*x[j+k] = *x[i+k];
							//printf("コピーします by %s()¥n",playername);
						}else{break;}
					}
				}
			}
		}
	}
//*/
	//for(i=0;i<n;i++){printf("[%3d] => [%3d] by %s [%d,%d]¥n",i,x[i][0].note,playername,cside[i].c_head,cside[i].c_file);}
	free(cside);
	return 0;
}

static int Maker_NICKO_MCBRAIN_Passage(int passageID,SCORE_LINE *s,X **x,int n)//横並びファイル作成すべき(音高のみ)
{
	FORMAT_PASSAGE p;FILE *passagefp;
	
#define POWERCHORD 1
	int WhatToDo = POWERCHORD;
	//int solo,powerchord;
	//FILE *missionfp;char missionfn[260];
	//Xパートの補佐,ソロ,バック等の役割をファイルに保存mission_X_Y.txt(あれば開くこのパートが必要に応じて別パートに助けを求めるのもあり既に存在するなら作るべきではない)
	FILE *mysoundfp;char mysoundfn[260];
	//char *testline = "-1,-2,-3,-4,-5,-6,-7";
	int testint[8]={0};
int outnote[6];
	
	char PartsScaner[1000]={0};
	PART **OP;//OtherPartsの略(ポインタテーブル)
	int NofOP;
	int i,j;CHORD nowchord;int ID;FILE *scrapfp;passagefp = PassageOpen(passageID);p= passage_fscanf(passagefp);fclose(passagefp);

	//NofOP = NumberOfParts(NULL,1,passageID);
	NofOP = NOP;
	
	sprintf(mysoundfn,"mysound_%s_%d.txt",partname,passageID);
	mysoundfp = fopen(mysoundfn,"w");
	OP = (PART**)calloc(n,sizeof(PART*));//行数分ポインタを確保
	for(i=0;i<n;i++)
	{
		OP[i] = (PART*)calloc(NofOP,sizeof(PART));//それぞれの行にパート生成
		if(OP[i]==NULL){printf("メモリ確保失敗¥n");exit(1);}
	}

LoadSession_Passage(OP,NofOP,1,passageID);//for(i=0;i<64;i++){printf("%s [%2d] => ",partname,i);for(j=0;j<NofOP;j++){printf(",%3d",OP[i][j].note);}printf("¥n");}

Maker_NICKO_MCBRAIN_Master(p.masterfile,s,x,n,passageID,NofS,NofOP,OP);
	
for(i=0;i<NofOP;i++){free(OP[i]);OP[i]=NULL;}
	free(OP);OP=NULL;

	
	for(i=0;i<NofS;i++){outnote[i] = 0;}
	for(i=0;i<n;i++)
	{
		for(j=0;j<NofS;j++)
		{
			if(x[i][j].note>0){outnote[j] = x[i][j].note;}
			fprintf(mysoundfp,"%d",outnote[j]);
			if(j==NofS-1){fprintf(mysoundfp,"¥n");}else{fprintf(mysoundfp,",");}
		}
	}
	fclose(mysoundfp);
	AddSession_Passage(1,passageID,mysoundfn);
	return 0;
}

       int Maker_NICKO_MCBRAIN_Movement(char *scorefn)//TrackMaker(scorefn,partname)にすべき?
{char trackname[6][FILENAME_MAX];int NofPL;
	char test[1000];int ID;//必ずしも楽節ひとまとまりとは限らないので ID としておく//int now_passage=(-1);
	X **x;
	USTFILEINFO finf = {120,1};

FILE *tfp=NULL;char tfn[260];//これ6ファイル出力すべき

	int linekind;char line[1000];SCORE_LINE *s;int *parts;int m=0,n,i,j;//パートの行列
	FILE *scorefp=NULL;
NOP = NumberOfParts(NULL,1);printf("Maker_%s() 起動時 => 既に[%d]パート¥n",partname,NOP);
	sprintf(tfn,"%s.txt",partname);

	scorefp = fopen(scorefn,"r");while(fscanf(scorefp,"%d,%s",&linekind,line)>0){if(linekind==LINEKIND_BODY)m++;}fclose(scorefp);scorefp=NULL;
	x = (X**)calloc(m,sizeof(X*));//行数分Xのポインタテーブル確保
	for(i=0;i<m;i++){x[i] = (X*)calloc(NofS,sizeof(X));}//全行に6弦作成

	if(x==NULL){printf("[%d]個の X メモリ確保 => 失敗 by Maker_%s()¥n",m,partname);exit(1);}else{printf("[%d]個の X メモリ確保　=> 成功 by Maker_%s()¥n",m,partname);}
	s = (SCORE_LINE*)calloc(m,sizeof(SCORE_LINE));
	if(s==NULL){printf("[%d]個の SCORE_LINE メモリ確保 => 失敗 by Maker_%s()¥n",m,partname);exit(1);}else{printf("[%d]個の SCORE_LINE メモリ確保　=> 成功 by Maker_%s()¥n",m,partname);}

	scorefp = fopen(scorefn,"r");fscanf(scorefp,"%d,%s",&linekind,line);//ヘッダー読み飛ばし
	for(i=0;i<m;i++){fscanf(scorefp,"%d,%s",&linekind,line);score_line_sscanf(line,&s[i]);}//読み込み
	fclose(scorefp);scorefp=NULL;
//////////////////////////////////////////////////スコアファイル読み込み完了

//////////////////////////////////メモリ上の操作はここから
///*
	for(i=0;i<m;i++)
	{
		if(s[i].passage_head==1)//名付けてヘッドキャッチ
		{
			for(j=i,NofPL=1;j!=m-1 && s[j+1].passage_head==0;j++,NofPL++);
//printf("PASSAGE[%d]は[%d]行で1セット by Maker_%s()¥n",s[i].passage_number,NofPL,playername);
			Maker_NICKO_MCBRAIN_Passage(s[i].passage_number,&s[i],&x[i],NofPL);
		}
	}
//*/
//////////////////////////////////メモリ上の操作はここまで

//////////////////////////////////////////////////以下出力
	for(j=0;j<2;j++){sprintf(tfn,"%s_%d弦.txt",playername,j+1);tfp = fopen(tfn,"w");for(i=0;i<m;i++){fprintf(tfp,"%d,%d¥n",s[i].length,x[i][j].note);}fclose(tfp);}
	
	for(i=0;i<m;i++){free(x[i]);x[i] = NULL;}
	free(x);x=NULL;free(s);s=NULL;
	for(j=0;j<2;j++){sprintf(trackname[j],"%s_%d弦",playername,j+1);ust_file_out(trackname[j],&finf);}
	return 0;
}

