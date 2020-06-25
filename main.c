//writer(void) : writer_kaiki()
//reader(void) : reader_kaiki()
//これが理想形

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <Windows.h>

#include "MOVEMENT.h"

#include "masterfile.h"
#include "chord_border.h"
#include "accent_border.h"
#include "rythm_border.h"
#include "UST.h"
#include "PASSAGE.h"
#include "SCRAP.h"

//マスターファイルの番号を引き数にして
//即値情報をファイル出力する関数は作るべき
//マスターファイルのポインタを返して

//const char *kari = "仮メロ.txt";
const char *kari = "GUITAR.txt";
int main()
{
	
	
	MOVEMENT_INFO movementinfo = {0};
	extern int bordersmaker(char*);
	extern int chord_nakami_maker(char*);
	//extern int movement_loader(char*);
	//extern int guitar_passage_linker(char*);
	//sleep(1);
	srand(time(0));
	
	MovementMaker(&movementinfo);
	//MasterMaker_Movement(1);
	writer();
	BordersMaker_Movement(NULL);
	//bordersmaker(NULL);
	
	chord_nakami_maker("曲編成.txt");//コイツが中身を作ってコイツ自身がアサインしてる(内部でcat_chordしてる)
	
	cat_chord(1);//movement_loader("曲編成.txt");//reader();
	chord_passage_linker("曲編成.txt");//コードパート出力 X 3
	//guitar_passage_linker("曲編成.txt");
	ScoreMaker_Movement("曲編成.txt");
	
	//Maker_Oboe("スコア.txt");
	//Maker_Ebase("スコア.txt");
	//Maker_Eguitar("スコア.txt");
	//Maker_Organ("スコア.txt");
	
	//Maker_DAVE_MURRAY_Movement("スコア.txt");
	//Maker_NICKO_MCBRAIN_Movement("スコア.txt");
	//Maker_BRUCE_DICKINSON_Movement("スコア.txt");
	Maker_JANICK_GERS_Movement(1);
	//ScaleTest();
	//マスタリング関数
	//TEST_N_Lyrics(17,0);
	//SetsunaMotion(57,-1,0);
	printf("無事終了\n");
	return 0;
}





int newmynumber()
{static int x = 0;x++;return x;}


//count_chord_border(masternumber); <= この関数は凄い!!

//今欲しいのはこんな点でバラバラなボーダー作成ではなく,
//それぞれが相互にチェックしながらメモリ上で自身を作成する
//ファイル出力ボーダー作成関数
//const BORDERS default = {-1,-1,-1,-1,-1,-1};
//プログラムの開始時に現状ｗ見て自身がRETURNするまでにしなければならない事を
//TODOリスト[mastarnumber].txtに書き出して後半でそれを実行
//事後それを削除


int map(int masterfile)//未決定のところは-1
{
	//他の回帰の進行に合わせてリアルタイムで更新する必要あり
	//この場でマスターファイルを読んだとしても他の関数が既に更新している可能性がある
	extern int borders_mapper(int,int,char *);
	char mapfn[FILENAME_MAX];
	sprintf(mapfn,"結合_[%d].txt",masterfile);

	printf("[%s]をマッピング前\n",mapfn);

	//今は不完全なので使えないマッピング関数
	//borders_mapper(masterfile,(1920/16),mapfn);
	
	printf("[%s]をマッピング後\n",mapfn);
	
	return 0;
}

int writer()//マスターファイルの作者
{FORMAT_PASSAGE p;char cmd[1280];FILE *pfp;char pfn[128];
	FILE *khfp;char *khfn = "曲編成.txt";
	
	khfp = fopen(khfn,"r");if(khfp==NULL){printf("[%s]が見つかりません by writer()\n",khfn);exit(1);}
	while(fscanf(khfp,"%s",pfn)>0)
	{
		pfp = fopen(pfn,"r");//サビ開く
		p = passage_fscanf(pfp);
		fclose(pfp);
		if(NewMasterMake(&p)>0)//新規作成したってこと
		{writer_kaiki(p.masterfile);}
	}fclose(khfp);
	//printf("一旦終了 by writer()\n");exit(1);
	return 0;
}





//以下別プログラムにて出力すること
//重要な関数!!

//マスターファイルをどう解釈しようが読者の勝手
//fp は UST 一歩手前の音高ファイル(実質メロディ)

//絶対的自我によるマスターファイルの解釈
//音楽はあくまでその感想文

int reader(){readtest();return 0;}

int reader_kaiki(FILE *fp,int masternumber)//ファイルの末尾にマスターファイルの中身を付け足すテスト用回帰関数
{
	//この期に及んでマッピング出来ないなんて嘘!!
	int kari,rl;
	extern FILE *rythm_border_open(int number);
	FILE *cbfp;
	int i,j;
	int child;//マスターファイルの子供のファイルナンバー
	
	MASTERFORMAT m;
	FILE *masterfp;FILE *myfp;
	masterfp = masteropen(masternumber);
	m = master_fscanf(masterfp);
	fclose(masterfp);
	
	

	if(m.rythm_border_painted==0)/*未塗装ならその子供達を襲う*/
	{
		for(i=0;i<m.linenumber;i++)
		{
			masterfp = masteropen(masternumber);
			m = master_fscanf(masterfp);

			for(j=0;j<i;j++){fscanf(masterfp,"%d",&child);}
			if(fscanf(masterfp,"%d",&child)>0){fclose(masterfp);reader_kaiki(fp,child);}
			else{fclose(masterfp);}
		}
	}
	
	else if(m.rythm_border_file>0 && m.rythm_border_painted>0)/*この時は付け足さなくてはならない*/
	{
		cbfp = rythm_border_open(m.rythm_border_file);
//printf("以下RBファイル[%d]\n",m.rythm_border_file);
		while(1)
		{
			kari = 0;rl = 0;
			if(fscanf(cbfp,"%d,%d",&kari,&rl)<0){fclose(cbfp);break;}
//printf("RB = (%d,%d) by reader_kaiki()\n",kari,rl);
			//if(rl<=0){fclose(cbfp);break;}
if(rl>0){fprintf(fp,"%d,%d\n",kari,rl);}
		}
	}
	
	else if(m.rythm_border_painted>0)
	{printf("これ以上考える必要は無い(%d,%d)\n",m.rythm_border_painted,m.rythm_border_file);return 0;}


	return 0;
}

int readtest(){
	int masternumber;
	
	char cmd[1280];

	FILE *pfp;char pfn[128];
	FILE *khfp;char *khfn = "曲編成.txt";
	
	
	FILE *fp;//これに付け足していく
	fp = fopen(kari,"w");
//printf("WHILE開始\n");
khfp = fopen(khfn,"r");
if(khfp==NULL){printf("%sが見つかりません\n",khfn);exit(1);}
//else{printf("%sを開きます\n",khfn);}
	while(fscanf(khfp,"%s",pfn)>0)
	{
		pfp = fopen(pfn,"r");if(pfp==NULL){printf("%sが見つかりません\n",pfn);return (-1);}
		fscanf(pfp,"%d,%s",&masternumber,cmd);
		//printf("[%d]が記されていた\n",masternumber);
		fclose(pfp);

//border(masternumber);//exit(1);

		reader_kaiki(fp,masternumber);

		

	}
//printf("WHILE終了\n");
	//reader_kaiki(fp,1);//本来は編成ファイルを開いて,以下略
	fclose(fp);
	//testustout();
	return 0;
}

//const char *kari = "仮メロ.txt";
//const char *kari = "GUITAR.txt";

int testustout(){
	//extern int q(char *,int);
	extern int nameraka(int,int,int);
	char cmd[1200];
	int t=0;
	FILE *ngfp;
	char *ngfn = "滑らか後.txt";
	FILE *nfp;
	char *nfn = "滑らか.txt";
	int c = 0;
	
	FILE *txtfp;
	int i;
	USTFILEINFO x;
	USTNOTEINFO n;
	FILE *ustfp;
	
	x.Tempo = 120;

//滑らかテスト開始
	txtfp = fopen(kari,"r");
	while(fscanf(txtfp,"%d,%d",&n.note,&n.length)>0){t++;}
	fclose(txtfp);
//printf("%d個の音があります\n",t);
nameraka(t,60,60);

	txtfp = fopen(kari,"r");nfp = fopen(nfn,"r");ngfp = fopen(ngfn,"w");
	while(fscanf(txtfp,"%d,%d",&n.note,&n.length)>0)
	{
		fscanf(nfp,"%d,%d",&t,&n.note);
		fprintf(ngfp,"%d,%d\n",n.note,n.length);
	}
	fclose(txtfp);fclose(nfp);fclose(ngfp);
//滑らかテスト終了
txtfp = fopen(ngfn,"r");
	
	ustfp = ust_fopen("ひとまず",&x);
	while(fscanf(txtfp,"%d,%d",&n.note,&n.length)>0)
	{
		//n.note += 60;
		n.counter = c;c++;
		sprintf(n.lylic,"ら");
		ust_fprintf(ustfp,&n);
	}
	ust_fclose(ustfp);
	
fclose(txtfp);

//q(ngfn,(1920/4));


	
txtfp = fopen("量子化.txt","r");
ustfp = ust_fopen("ひとまず2",&x);c=0;
	while(fscanf(txtfp,"%d,%d",&n.note,&n.length)>0)
	{n.length = (1920/4);
		n.note += 60;
		n.counter = c;c++;
		sprintf(n.lylic,"ら");
		ust_fprintf(ustfp,&n);
	}fclose(txtfp);
ust_fclose(ustfp);





	return 0;
}
//fscanfは1文字でもスキャンしたら 1 ,以外は -1 を返すらしい
