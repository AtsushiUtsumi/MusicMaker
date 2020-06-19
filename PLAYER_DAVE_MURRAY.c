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

static const char *playername = "DAVE_MURRAY";
static const char *partname = "PLAYER_DAVE_MURRAY";
static int NOP;//起動時での他のパート数を記録しておく
static int NofS = 6;

//typedef struct{int length,note;}PART;//セッションで使用する
typedef struct
{
	int wave,pre,note;
}X;//個々のパートで使用
//typedef struct{int c_head,c_file,have_more_children;}CHILDREN_SIDE;

typedef struct{int nanika;}FEELING;//その時のプレーヤーの気持ち

static int NearWhite(int note,int whitefile,int pm)//好みのスケールでいいんじゃない?
{
	int x,y;x = note;y = (pm>0) ? 1:-1;
	while(x%12!=0 && x%12!=2 && x%12!=4 && x%12!=5 && x%12!=7 && x%12!=9 && x%12!=11)//気に食わない音
	{x+=y;}//printf("X = %d by NearWhite()\n",x);
	return x;
}



static int fill_children_side(int masterfilenumber,CHILDREN_SIDE *dest,int n)//マスターファイルを使う関数には便利かも
{
	MASTERFORMAT m,c;int x=(-1);//一枚岩なら全部が代入される
	int i,j;FILE *masterfp,*childfp;
	masterfp = masteropen(masterfilenumber);m = master_fscanf(masterfp);
	//if(n!=m.count){printf("\n\nfill_children_side()\n[%d] <= m.count\n[%d] <= n (引数)\n終了します\n\n",m.count,n);exit(1);}
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


static FILE *WaveFileOpen(int n,int WaveID)//使えるように音域が楽器によって違うことからPREは汎用性が低い
{//変化を出したい場所から書き換えることを考えると始まりの前の音プラスマイナスの方がいい
	int i;typedef struct{int pm;}WAVE;WAVE *w;
	FILE *wavefp;char wavefn[FILENAME_MAX];
	sprintf(wavefn,"WAVE_%d_%d.txt",n,WaveID);
	wavefp = fopen(wavefn,"r");
	if(wavefp==NULL)
	{
		wavefp = fopen(wavefn,"w");
		w = (WAVE *)calloc(n,sizeof(WAVE));
		for(i=0;i<n;i++){w[i].pm = (rand()%2==0) ? 1:-1;if(i==0){w[i].pm = 0;}}
		for(i=0;i<n;i++){fprintf(wavefp,"%d\n",w[i].pm);}
		free(w);
		fclose(wavefp);
		wavefp = fopen(wavefn,"r");
	}
	return wavefp;
}

static FILE *OpenRiff(int RiffID)//リフには材料が必要だってこと!!
{//小節未満は受付ないよ
	FILE *RiffFp=NULL;char RiffFn[260];sprintf(RiffFn,"RIFF_%d.txt",RiffID);
	RiffFp = fopen(RiffFn,"r");
	if(RiffFp==NULL)
	{
		RiffFp = fopen(RiffFn,"w");fprintf(RiffFp,"2,\n");
		fclose(RiffFp);
		RiffFp = fopen(RiffFn,"r");
	}
	return RiffFp;
}

static char *IDtoFn_Nanika(int ID){static char Fn[260];sprintf(Fn,"ギター_NANIKA_%d.txt",ID);return Fn;}

static int NewNanika()
{
	FILE *Fp=NULL;
	int static t=0;
	
	t++;
	Fp = fopen(IDtoFn_Nanika(t),"w");
	fprintf(Fp,"2,%d,リフ\n",t);
	fclose(Fp);
	return t;
}



























static int Maker_DAVE_MURRAY_Master(int MasterNumber,SCORE_LINE *s,X **x,int n,int PassageID,int NofS,int NofOP,PART **OP)
{
	FILE *wavefp;
	CHILDREN_SIDE *cside;int i,j;CHORD nowchord;int lastpre;
	cside = (CHILDREN_SIDE *)calloc(n,sizeof(CHILDREN_SIDE));
	fill_children_side(1,cside,n);

/////////////////メロディアスにいこう	
	wavefp = WaveFileOpen(n,MasterNumber);
	for(i=0;i<n;i++){if(cside[i].c_head>0){rewind(wavefp);x[i][2].pre = 60;lastpre = 60;}if(s[i].rh>0){fscanf(wavefp,"%d",&x[i][2].wave);x[i][2].pre = lastpre+x[i][2].wave;}}
	fclose(wavefp);

	for(i=0;i<n;i++)
	{
		nowchord.dia = s[i].chord_root;nowchord.kind= s[i].chord_kind;
		if(x[i][2].pre>0)
		{
			x[i][2].note = (s[i].ah>0) ? NearChordTone(x[i][2].pre,0,1,&nowchord,0):NearWhite(x[i][2].pre,1,1);
		}else{x[i][2].note = -1;}
	}



	for(i=0;i<n;i++)


/////////////////以上メロディアス

	switch(PassageID){case 1:case 5:case 6:
	for(i=0;i<n;i++){x[i][0].note = 52;x[i][1].note = 0;}
	
	for(i=0;i<n;i++){nowchord.dia = s[i].chord_root;nowchord.kind = s[i].chord_kind;
		if(s[i].ah>0){x[i][0].note = NearChordTone(1+x[i][0].note,0,1,&nowchord,0);}}break;
	
	case 2:case 3:case 4://ひたすらパワーコード
		for(i=0;i<n;i++){x[i][0].note = 52;x[i][1].note = 0;}
		for(i=0;i<n;i++)
		{
			nowchord.dia = s[i].chord_root;nowchord.kind= s[i].chord_kind;
			if(s[i].ch>0)
			{
				if(i!=0){x[i-1][0].note = 0;x[i-1][1].note = 0;}
				x[i][0].note = NearChordTone(x[i][0].note,0,1,&nowchord,0);x[i][1].note = x[i][0].note+7;
			}else{x[i][0].note = -1;x[i][1].note = -1;}
		}break;
	default:printf("どうしろと!? => [%d]\n",PassageID);exit(1);break;}

	free(cside);
	return 0;
}


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
	default:printf("\n不明な楽節ID = [%d]\n終了します\n",PassageID);exit(1);break;
	}
	fp = fopen(fn,"r");
	if(fp==NULL){printf("[%s]が見つかりません by PassageOpen()\n終了します\n",fn);exit(1);}
	//else{printf("[%s]を開きます by PassageOpen()\n",fn);}
	return fp;
}
//マスター割ができてない
//マスターが同じなら同じリズムだと忘れてないか?

static int Maker_DAVE_MURRAY_Passage(int passageID,SCORE_LINE *s,X **x,int n)//横並びファイル作成すべき(音高のみ)
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
		if(OP[i]==NULL){printf("メモリ確保失敗\n");exit(1);}
	}

LoadSession_Passage(OP,NofOP,1,passageID);//for(i=0;i<64;i++){printf("%s [%2d] => ",partname,i);for(j=0;j<NofOP;j++){printf(",%3d",OP[i][j].note);}printf("\n");}

Maker_DAVE_MURRAY_Master(p.masterfile,s,x,n,passageID,NofS,NofOP,OP);
	
for(i=0;i<NofOP;i++){free(OP[i]);OP[i]=NULL;}
	free(OP);OP=NULL;

	
	for(i=0;i<NofS;i++){outnote[i] = 0;}
	for(i=0;i<n;i++)
	{
		for(j=0;j<NofS;j++)
		{
			if(x[i][j].note>0){outnote[j] = x[i][j].note;}
			fprintf(mysoundfp,"%d",outnote[j]);
			if(j==NofS-1){fprintf(mysoundfp,"\n");}else{fprintf(mysoundfp,",");}
		}
	}
	fclose(mysoundfp);
	AddSession_Passage(1,passageID,mysoundfn);
	return 0;
}

       int Maker_DAVE_MURRAY_Movement(char *scorefn)//TrackMaker(scorefn,partname)にすべき?
{char trackname[6][FILENAME_MAX];int NofPL;
	char test[1000];int ID;//必ずしも楽節ひとまとまりとは限らないので ID としておく//int now_passage=(-1);
	X **x;
	USTFILEINFO finf = {120,1};

FILE *tfp=NULL;char tfn[260];//これ6ファイル出力すべき

	int linekind;char line[1000];SCORE_LINE *s;int *parts;int m=0,n,i,j;//パートの行列
	FILE *scorefp=NULL;
NOP = NumberOfParts(NULL,1);printf("Maker_%s() 起動時 => 既に[%d]パート\n",partname,NOP);
	sprintf(tfn,"%s.txt",partname);

	scorefp = fopen(scorefn,"r");while(fscanf(scorefp,"%d,%s",&linekind,line)>0){if(linekind==LINEKIND_BODY)m++;}fclose(scorefp);scorefp=NULL;
	x = (X**)calloc(m,sizeof(X*));//行数分Xのポインタテーブル確保
	for(i=0;i<m;i++){x[i] = (X*)calloc(NofS,sizeof(X));}//全行に6弦作成

	if(x==NULL){printf("[%d]個の X メモリ確保 => 失敗 by Maker_%s()\n",m,partname);exit(1);}else{printf("[%d]個の X メモリ確保　=> 成功 by Maker_%s()\n",m,partname);}
	s = (SCORE_LINE*)calloc(m,sizeof(SCORE_LINE));
	if(s==NULL){printf("[%d]個の SCORE_LINE メモリ確保 => 失敗 by Maker_%s()\n",m,partname);exit(1);}else{printf("[%d]個の SCORE_LINE メモリ確保　=> 成功 by Maker_%s()\n",m,partname);}

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
//printf("PASSAGE[%d]は[%d]行で1セット by Maker_%s()\n",s[i].passage_number,NofPL,playername);
			Maker_DAVE_MURRAY_Passage(s[i].passage_number,&s[i],&x[i],NofPL);
		}
	}
//*/
//////////////////////////////////メモリ上の操作はここまで

//////////////////////////////////////////////////以下出力
	for(j=0;j<3;j++){sprintf(tfn,"%s_%d弦.txt",playername,j+1);tfp = fopen(tfn,"w");for(i=0;i<m;i++){fprintf(tfp,"%d,%d\n",s[i].length,x[i][j].note);}fclose(tfp);}
	
	for(i=0;i<m;i++){free(x[i]);x[i] = NULL;}
	free(x);x=NULL;free(s);s=NULL;
	for(j=0;j<3;j++){sprintf(trackname[j],"%s_%d弦",playername,j+1);ust_file_out(trackname[j],&finf);}
	return 0;
}
