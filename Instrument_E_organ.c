//スコアを読んで各パートの共通事項
//どんなスクラップファイルを作るかは各パート専用の関数にお任せ
//スコアにパートスクラップを引っ付けるだけのシケた関数
//しかしながらこの関数のおかげでアウフタクトを意識せずにすむようになる
//各パートスコア各パートとスコアを通すことで,
//パート間での横のつながりを考える事ができ,
//冒頭の小節数のように不確定要素による修正が行える
#define SCRAP_C
#include "SCRAP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SCORE.h"
#include "MUSIC.h"
#include "UST.h"
#define LINEKIND_BODY 2

static const char *partname = "Organ";static int NOP;//起動時での他のパート数を記録しておく

typedef struct{int length,note;}PART;//セッションで使用する
typedef struct{int wave,pre,note;}X;//個々のパートで使用

static int NearWhite(int note,int whitefile,int pm)
{
	int x,y;x = note;y = (pm>0) ? 1:-1;
	while(x%12!=0 && x%12!=2 && x%12!=4 && x%12!=5 && x%12!=7 && x%12!=9 && x%12!=11)//気に食わない音
	{x+=y;}//printf("X = %d by NearWhite()\n",x);
	return x;
}
//せめて"トラック"にしとけよ

static FILE *OpenScrapFileW(int ID)//空のスクラップファイルが存在するときのみ Aモードで開く
{
	char line[1000];int E=1;
	FILE *fp;char fn[FILENAME_MAX];if(ID<0){printf("負のID = [%d] は渡せません by MakeNewScrapFile()\n",ID);exit(1);}sprintf(fn,"SCRAP_%s_%d.txt",partname,ID);
	fp = fopen(fn,"r");if(fp==NULL){exit(1);}if(fscanf(fp,"%s",line)>0){E=0;}fclose(fp);
	if(E==1){fp = fopen(fn,"a");return fp;}else{return NULL;}
}

static FILE *OpenScrapFileR(int ID)//スクラップファイルが存在するときのみ Rモードで開く
{
	FILE *fp;char fn[FILENAME_MAX];if(ID<0){printf("負のID = [%d] は渡せません by MakeNewScrapFile()\n",ID);exit(1);}sprintf(fn,"SCRAP_%s_%d.txt",partname,ID);
	fp = fopen(fn,"r");if(fp==NULL){exit(1);}return fp;
}

static int ScrapLoad(X *dest,int ID,int n)//dest に SCRAP_ID.txt を n行ロードする関数
{
	//本来なら中途半端なロードを防ぐためにまな板から一斉に移すべき
	X x;
	FILE *scrapfp;
	int i;
	scrapfp = OpenScrapFileR(ID);
	for(i=0;i<n;i++)
	{
		if(fscanf(scrapfp,"%d,%d,%d",&x.pre,&x.wave,&x.note)>0){dest[i] = x;}
		else{fclose(scrapfp);return -1;}
	}
	fclose(scrapfp);return 1;
}

static int MakeNewScrapFile(int ID)//新規作成で ID を返す
{
	FILE *fp;char fn[FILENAME_MAX];
	if(ID<0){printf("負のID = [%d] は渡せません by MakeNewScrapFile()\n",ID);exit(1);}
	sprintf(fn,"SCRAP_%s_%d.txt",partname,ID);fp = fopen(fn,"r");
	if(fp!=NULL){//printf("[%s]は既に存在します by MakeNewScrapFile()\n",fn);
	fclose(fp);return (-1);}
	else{//printf("[%s]を作成します by MakeNewScrapFile()\n",fn);
	fp = fopen(fn,"w");fclose(fp);return ID;}
}

static int NumberOfParts(char *sessionfn,int movementID,int passageID)
{
	char line[1000];char*pointer;
	FILE *fp;char fn[260];int x;
	if(sessionfn==NULL){sprintf(fn,"SESSION_%d_%d.txt",movementID,passageID);}else{sprintf(fn,"%s",sessionfn);}
	fp = fopen(fn,"r");if(fp==NULL){printf("[%s]が見つかりません by NumberOfParts()\n",fn);return 0;}
	if(fscanf(fp,"%s",line)>0){for(x=1,pointer=line;pointer!=NULL;x++){pointer = strstr(pointer,",");if(pointer==NULL){break;}else{pointer++;}}}
	else{x = 0;}
	fclose(fp);printf("X = [%d] by NumberOfParts(%d,%d)\n",x,movementID,passageID);
	return x;
}

static int AddSession(int movementID,int passageID,char *addpartfn)
{
	char Line1[1000] = {0};
	char Line2[1000] = {0};
	FILE *addpartfp=NULL;
	FILE *tempfp=NULL;char *tempfn = "SESSION_TEMP.txt";
	FILE *sessionfp;
	char sessionfn[FILENAME_MAX];
	
	if(NOP!=NumberOfParts(NULL,movementID,passageID)){printf("最初と異なる NOP のファイルには参加出来ません\n");return 0;}
	
	sprintf(sessionfn,"SESSION_%d_%d.txt",movementID,passageID);
	addpartfp = fopen(addpartfn,"r");sessionfp = fopen(sessionfn,"r");
	if(sessionfp==NULL)
	{
		sessionfp = fopen(sessionfn,"w");
		while(fscanf(addpartfp,"%s",Line1)>0){fprintf(sessionfp,"%s\n",Line1);}
		fclose(sessionfp);printf("まだ独奏です\n");return 0;
	}//else{printf("セッションです\n");}
	tempfp = fopen(tempfn,"w");//全ファイルオープン
	while(fscanf(sessionfp,"%s",Line1)>0){fscanf(addpartfp,"%s",Line2);fprintf(tempfp,"%s,%s\n",Line1,Line2);}
	fclose(tempfp);fclose(sessionfp);fclose(addpartfp);tempfp = NULL;sessionfp = NULL;addpartfp = NULL;//全ファイルクローズ
	remove(sessionfn);rename(tempfn,sessionfn);
	return 0;
}

static int LoadSession(PART **part,int NofPart,int movementID,int passageID)
{
	char *pointer;char sessionline[1000];
	int i,j,m=64,n;//mは行数,nはパート数
	FILE *sessionfp;char sessionfn[FILENAME_MAX];

	//n = NumberOfParts(NULL,movementID,passageID);
	n = NOP;
	sprintf(sessionfn,"SESSION_%d_%d.txt",movementID,passageID);sessionfp = fopen(sessionfn,"r");
	if(sessionfp==NULL){/*printf("[%s]が見つかりません by LoadSession()\n",sessionfn);*/return -1;}else{if(n!=NofPart){printf("引数[%d]と実際のパート数[%d]が合いません\n",NofPart,n);exit(1);}}
	for(i=0;i<m;i++)
	{
		fscanf(sessionfp,"%s",sessionline);
		for(j=0,pointer=sessionline;j<n && pointer!=NULL;j++)
		{
			sscanf(pointer,"%d",&part[i][j].note);
			//printf("%2d行_%dパート = [%d](%x,%s)\n",i,j,part[i][j].note,pointer,pointer);
			pointer = strstr(pointer,",");if(pointer==NULL){/*printf("この行にこれ以上のパートはありません\n");*/break;}else{pointer++;}
			
		}
	}
	fclose(sessionfp);
	return 0;
}

static int cat_scrap(char *scorefn,char *scrapfn)//出力をTEMPファイルに保存して最後に確認してから
{
	//行数と要所で合っているかを確認
	//場合によってはファイルを修正
	//(アウフタクトによる出だしの不確定などによる)FILE fp;fopen();fclose();
	FILE *scorefp=NULL;
	FILE *scrapfp=NULL;
	FILE *tempfp =NULL;
	char tempfn[FILENAME_MAX]={0};
	
	scorefp = fopen(scorefn,"r");//if(scorefp==NULL){printf("スコアファイル[%s]が見つかりません\n",scorefn);exit(1);}
	scrapfp = fopen(scrapfn,"r");//if(scrapfp==NULL){printf("スクラップファイル[%s]が見つかりません\n",scrapfn);fclose(scorefp);exit(1);}
	
	fclose(scrapfp);
	fclose(scorefp);

	sprintf("[%s]のTEMP出力.txt",scrapfn);
	tempfp = fopen(tempfn,"w");
	fclose(tempfp);

	//ここでを旧スコアファイルを削除
	//TEMPをリネームして新スコアファイルとする

	return 0;
}

static int Maker_Organ_Passage(int passageID,SCORE_LINE *s,X *x,int n)//横並びファイル作成すべき(音高のみ)
{
	FILE *mysoundfp;char mysoundfn[260];
	char *testline = "-1,-2,-3,-4,-5,-6,-7";int testint[8]={0};int outnote;
	
	char PartsScaner[1000]={0};
	PART **OP;//OtherPartsの略(ポインタテーブル)
	int NofOP;
	int i,j;CHORD nowchord;int ID;FILE *scrapfp;

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

LoadSession(OP,NofOP,1,passageID);
	//for(i=0;i<64;i++){printf("%s [%2d] => ",partname,i);for(j=0;j<NofOP;j++){printf(",%3d",OP[i][j].note);}printf(" by Maker_Organ_Passage()\n",);}

	for(i=0;i<NofOP;i++){free(OP[i]);OP[i]=NULL;}
	free(OP);OP=NULL;

	for(i=0;i<n;i++){x[i].wave = i;}
	for(i=0;i<n;i++)
	{x[i].pre = 40;}//この時点で音域に無理がないように
	for(i=0;i<n;i++)
	{//if(s[i].rh>0)
		if(1)//アタック
		{
			nowchord.dia = s[i].chord_root;nowchord.kind= s[i].chord_kind;x[i].note = NearChordTone(x[i].pre,0,1,&nowchord,5);}else{x[i].note = -1;}}ID = MakeNewScrapFile(s[i].passage_number);
			if(ID>0)
			{
				scrapfp = OpenScrapFileW(ID);
				for(i=0;s[i].passage_number==ID;i++){fprintf(scrapfp,"%d,%d,%d\n",x[i].pre,x[i].wave,x[i].note);}
				fclose(scrapfp);
			}else{ScrapLoad(&x[i],s[i].passage_number,n);}
			for(i=0,outnote=0;i<n;i++){if(x[i].note>0){outnote=x[i].note;}fprintf(mysoundfp,"%d\n",outnote);}
			fclose(mysoundfp);AddSession(1,passageID,mysoundfn);return 0;}

int Maker_Organ(char *scorefn)//TrackMaker(scorefn,partname)にすべき?
{int NofPL;
	char test[1000];int ID;//必ずしも楽節ひとまとまりとは限らないので ID としておく//int now_passage=(-1);
	X *x;USTFILEINFO finf = {120,1};FILE *tfp=NULL;char tfn[260];
	int linekind;char line[1000];SCORE_LINE *s;int *parts;int m=0,n,i,j;//パートの行列
	FILE *scorefp=NULL;
NOP = NumberOfParts(NULL,1,1);printf("Maker_%s() 起動時 => 既に[%d]パート\n",partname,NOP);
	sprintf(tfn,"%s.txt",partname);

	scorefp = fopen(scorefn,"r");while(fscanf(scorefp,"%d,%s",&linekind,line)>0){if(linekind==LINEKIND_BODY)m++;}fclose(scorefp);scorefp=NULL;
	x = (X*)calloc(m,sizeof(X));
	if(x==NULL){printf("[%d]個の X メモリ確保 => 失敗 by Maker_%s()\n",m,partname);exit(1);}
	else{printf("[%d]個の X メモリ確保　=> 成功 by Maker_%s()\n",m,partname);}
	s = (SCORE_LINE*)calloc(m,sizeof(SCORE_LINE));
	if(s==NULL){printf("[%d]個の SCORE_LINE メモリ確保 => 失敗 by Maker_%s()\n",m,partname);exit(1);}
	else{printf("[%d]個の SCORE_LINE メモリ確保　=> 成功 by Maker_%s()\n",m,partname);};

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
printf("PASSAGE[%d]は[%d]行で1セット by Maker_%s()\n",s[i].passage_number,NofPL,partname);
			Maker_Organ_Passage(s[i].passage_number,&s[i],&x[i],NofPL);
		}
	}
//*/
//////////////////////////////////メモリ上の操作はここまで

//////////////////////////////////////////////////以下出力
	tfp = fopen(tfn,"w");for(i=0;i<m;i++){fprintf(tfp,"%d,%d\n",s[i].length,x[i].note);}fclose(tfp);
	free(x);x=NULL;free(s);s=NULL;
	ust_file_out(partname,&finf);
	return 0;
}
