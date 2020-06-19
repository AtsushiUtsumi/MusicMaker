//スコアを読んで各パートの共通事項
//どんなスクラップファイルを作るかは各パート専用の関数にお任せ
//スコアにパートスクラップを引っ付けるだけのシケた関数
//しかしながらこの関数のおかげでアウフタクトを意識せずにすむようになる
//各パートスコア各パートとスコアを通すことで,
//パート間での横のつながりを考える事ができ,
//冒頭の小節数のように不確定要素による修正が行える
#define SCRAP_C
#include "SCRAP.h"
#include <stdio.h>
#include <stdlib.h>
#include "SCORE.h"
#include "MUSIC.h"
#include "UST.h"
#define LINEKIND_BODY 2

static int maxnote = 101,minnote = 70;

typedef struct{int wave,pre,note;}X;

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
	FILE *fp;char fn[FILENAME_MAX];if(ID<0){printf("負のID = [%d] は渡せません by MakeNewScrapFile()\n",ID);exit(1);}sprintf(fn,"SCRAP_%d.txt",ID);
	fp = fopen(fn,"r");if(fp==NULL){exit(1);}if(fscanf(fp,"%s",line)>0){E=0;}fclose(fp);
	if(E==1){fp = fopen(fn,"a");return fp;}else{return NULL;}
}

static FILE *OpenScrapFileR(int ID)//スクラップファイルが存在するときのみ Rモードで開く
{
	FILE *fp;char fn[FILENAME_MAX];if(ID<0){printf("負のID = [%d] は渡せません by MakeNewScrapFile()\n",ID);exit(1);}sprintf(fn,"SCRAP_%d.txt",ID);
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
	sprintf(fn,"SCRAP_%d.txt",ID);fp = fopen(fn,"r");
	if(fp!=NULL){//printf("[%s]は既に存在します by MakeNewScrapFile()\n",fn);
	fclose(fp);return (-1);}
	else{//printf("[%s]を作成します by MakeNewScrapFile()\n",fn);
	fp = fopen(fn,"w");fclose(fp);return ID;}
}

static int Maker_Oboe_Passage(int passageID,SCORE_LINE *s,X *x,int n)//横並びファイル作成すべき(音高のみ)
{
#define Oboe_MODE_SOLO 1
#define Oboe_MODE_SOLI 2
#define Oboe_MODE_CHORD_BACKING 3
	int mode =(-1);
	char *testline = "-1,-2,-3,-4,-5,-6,-7";int testint[8]={0};int outnote;
	typedef struct{int length,note;}PART;char PartsScaner[1000]={0};
	PART **OP;//OtherPartsの略(ポインタテーブル)
	int NofOP=4;
	int i;CHORD nowchord;int ID;FILE *scrapfp;
	OP = (PART**)calloc(n,sizeof(PART*));//行数分ポインタを確保
	for(i=0;i<n;i++)
	{
		OP[i] = (PART*)calloc(NofOP,sizeof(PART));//それぞれの行にパート生成
		if(OP[i]==NULL){printf("メモリ確保失敗\n");}
		//else{strcat(PartsScaner,"%d,");printf(PartsScaner,1,2,3,4,5,6,8);
		//sscanf(testline,PartsScaner,&testint[0],&testint[1],&testint[2],&testint[3],&testint[4],&testint[5],&testint[6],&testint[7]);
		//printf("\nこうなる => %d/%d/%d/%d/%d/%d/%d/%d",testint[0],testint[1],testint[2],testint[3],testint[4],testint[5],testint[6],testint[7]);
		//printf("パートナンバー[%d]の先頭アドレス = [%X]\n",i,OP[i]);}
	}
	for(i=0;i<NofOP;i++){free(OP[i]);OP[i]=NULL;}
	free(OP);OP=NULL;

	for(i=0;i<n;i++){x[i].wave = i;}
	for(i=0;i<n;i++)
	{
		if(i==0){x[i].pre = minnote + rand()%(maxnote-minnote);}else{x[i].pre = x[i-1].pre + (rand()%3-1);}
		if(x[i].pre>maxnote){x[i].pre = x[i-1].pre-2;}
		if(x[i].pre<minnote){x[i].pre = x[i-1].pre+2;}
		
	}//この時点で音域に無理がないように
	for(i=0;i<n;i++)
	{
		if(s[i].rh>0)//アタック
		{
			nowchord.dia = s[i].chord_root;nowchord.kind= s[i].chord_kind;
			if(s[i].ah>0){x[i].note = NearChordTone(x[i].pre,0,1,&nowchord,0);}
			else{x[i].note = NearWhite(x[i].pre,0,1);}
		}//else{x[i].note = 0;}//休符(0でいいのか?)
		//else{x[i].note = x[i-1].note;}//今は休符無し前の音と同じ音
		else{x[i].note = -1;}//
	}for(i=n-1;i>0;i--){if(x[i].note>0){x[i].note = -1;break;}}
	for(i=n-1;i>0;i--){if(x[i].note>0){x[i].note = 60;break;}}//強引に最後の音を C にする
if(passageID==1|| passageID==5 || passageID==6){for(i=0;i<n;i++){x[i].wave = 0;x[i].pre = 0;x[i].note = 0;}}
	ID = MakeNewScrapFile(s[i].passage_number);
			if(ID>0)
			{
				scrapfp = OpenScrapFileW(ID);
				for(i=0;s[i].passage_number==ID;i++){fprintf(scrapfp,"%d,%d,%d\n",x[i].pre,x[i].wave,x[i].note);}
				fclose(scrapfp);
			}else{ScrapLoad(&x[i],s[i].passage_number,n);}
	for(i=0,outnote=0;i<n;i++){if(x[i].note>0){outnote=x[i].note;}/*printf("%2d = %d こんなのを横並びファイルにすれば? by Maker_Oboe_Passage()\n",i,outnote);*/}
	return 0;
}

int Maker_Oboe(char *scorefn)//TrackMaker(scorefn,partname)にすべき?
{int NofPL;
	char test[1000];int ID;//必ずしも楽節ひとまとまりとは限らないので ID としておく//int now_passage=(-1);
	X *x;USTFILEINFO finf = {120,1};FILE *tfp=NULL;char *tfn = "Oboe.txt";
	int linekind;char line[1000];SCORE_LINE *s;int *parts;int m=0,n,i,j;//パートの行列
	FILE *scorefp=NULL;
	scorefp = fopen(scorefn,"r");while(fscanf(scorefp,"%d,%s",&linekind,line)>0){if(linekind==LINEKIND_BODY)m++;}fclose(scorefp);scorefp=NULL;
x = (X*)calloc(m,sizeof(X));	
	s = (SCORE_LINE*)calloc(m,sizeof(SCORE_LINE));
	if(s==NULL){printf("[%d]個の SCORE_LINE メモリ確保 => 失敗 by Oboe()\n",m);exit(1);}
	//else{printf("[%d]個の SCORE_LINE メモリ確保　=> 成功 by Oboe()\n",m);};

	scorefp = fopen(scorefn,"r");fscanf(scorefp,"%d,%s",&linekind,line);//ヘッダー読み飛ばし
	for(i=0;i<m;i++){fscanf(scorefp,"%d,%s",&linekind,line);score_line_sscanf(line,&s[i]);}//読み込み
	fclose(scorefp);scorefp=NULL;
//////////////////////////////////////////////////スコアファイル読み込み完了(本来なら他のパートもここまでで読み込むべき)

//////////////////////////////////メモリ上の操作はここから

	for(i=0;i<m;i++)
	{
		if(s[i].passage_head==1)//名付けてヘッドキャッチ
		{
			for(j=i,NofPL=1;j!=m-1 && s[j+1].passage_head==0;j++,NofPL++);
//printf("PASSAGE[%d]は[%d]行で1セット by Maker_Oboe()\n",s[i].passage_number,NofPL);
			Maker_Oboe_Passage(s[i].passage_number,&s[i],&x[i],NofPL);
		}
	}
//////////////////////////////////メモリ上の操作はここまで

//////////////////////////////////////////////////以下出力
	tfp = fopen(tfn,"w");for(i=0;i<m;i++){fprintf(tfp,"%d,%d\n",s[i].length,x[i].note);}fclose(tfp);

	free(s);
free(x);

ust_file_out("Oboe",&finf);

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