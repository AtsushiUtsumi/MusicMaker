//フォーマット構造体とメンバー数の定義
//ファイルから1行読んで構造体を返す関数
//構造体を読んでファイルに1行出力する関数
//マスターナンバーを引き数にしてボーダーの個数カウントする回帰呼出関数
//中身を作る関数

//行数の不明なファイルで構造体を返す場合ポインタを使う(スキャンエラーなのか分からないので)
//ヘッダー情報みたいな必ず有ると分かっている場合のみ
#define CHORD_BORDERC
#include "chord_border.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "masterfile.h"

FILE *new_chord_border_open(int number)
{
	FILE *fp;
	char fn[FILENAME_MAX];
	sprintf(fn,"CHORDBORDER_%d.txt",number);
	//sprintf(fn,"CHORDBORDER_%d.txt",number);
	fp = fopen(fn,"r");
	if(fp != NULL)
	{
		//printf("既に存在するナンバーです\n");
		fclose(fp);
		return NULL;
	}
	fp = fopen(fn,"w");
	return fp;
}

FILE *chord_border_open(int number)
{
	FILE *fp;
	char fn[FILENAME_MAX];
	sprintf(fn,"BORDER_C_[%d].txt",number);
	//sprintf(fn,"CHORDBORDER_%d.txt",number);
	fp = fopen(fn,"r");
	return fp;
}

//コードボーダーファイルの回帰呼出作成
	//マスターファイルを開いてファイルナンバー通りのファイルを作る
	//引き数はマスターファイルのナンバーでありこの関数特有のものでは無い
	//コードボーダーのファイル名はマスターファイルに含まれている


static int test_border_out(FILE *fp,int length)
{
	const int bar = 1920;
	int lest;
	int i,n;
	FORMAT_CHORD_BORDER x = {0};
	
	n = length /bar;
	lest = length %bar;
	for(i=0;i<n;i++){x.length = bar;x.chord.dia = (2+rand()%5);x.chord.kind = 3;fprintf_chord_border(fp,&x);}
	if(lest > 0){x.length = lest;x.chord.dia = 1;x.chord.kind = 4;fprintf_chord_border(fp,&x);}
	//static int tekito = 1;
	//fprintf(fp,"%d,%d\n",1+(rand()%7),length);//いくら何でも適当過ぎる出力
/*
	int i;
	int n,lest;
	
	for(i=0;i<n;i++,tekito++){fprintf(fp,"%d,%d\n",tekito,1920);}
	
*/
	return 0;
}

static int kaiki_chord_border(int masternumber)//コードボーダーファイルを出すのはコイツ使わないのでクビ
{
	extern int map(int);

	int i,j;
	int child;//マスターファイルの子供のファイルナンバー
	MASTERFORMAT m;
	FILE *masterfp;FILE *myfp;
	masterfp = masteropen(masternumber);
	m = master_fscanf(masterfp);
	fclose(masterfp);
printf("M_[%d]を手にかける前にマッピング\n",masternumber);
	//今から作るのだからファイルが無いのは当然
	map(masternumber);

	if(m.chord_border_painted==0)/*未塗装ならその子供達を襲う*/
	{
		//printf("未塗装\n");
		for(i=0;i<m.linenumber;i++)
		{
			masterfp = masteropen(masternumber);
			m = master_fscanf(masterfp);

			for(j=0;j<i;j++){fscanf(masterfp,"%d",&child);}
			if(fscanf(masterfp,"%d",&child)>0){fclose(masterfp);kaiki_chord_border(child);}
			else{fclose(masterfp);}
		}
	}
	
	else if(m.chord_border_file>0 && m.chord_border_painted>0)/*この時は作らなければならない*/
	{
		myfp = new_chord_border_open(m.chord_border_file);
		if(myfp==NULL){//printf("CB強襲済み\n");
		return 0;}
		//printf("長さ(%d)のCBファイル(%d)を作ります\n",m.length,m.chord_border_file);
		
		test_border_out(myfp,m.length);//いくら何でも適当過ぎる出力
		
		fclose(myfp);
	}
	
	else if(m.chord_border_painted>0)
	{printf("これ以上考える必要は無い(%d,%d)\n",m.chord_border_painted,m.chord_border_file);return 0;}
	
	
	return 0;
}

int count_chord_border(int masternumber)//マスターナンバーを引き数にしてボーダーの個数を返す(マップファイル参照型)
{
	int x;int linekind;
	char mapfn[FILENAME_MAX];
	FILE *mapfp = NULL;
	sprintf(mapfn,"MAP_%d.txt",masternumber);
	mapfp = fopen(mapfn,"r");
	fscanf(mapfp,"%d,%d",&linekind,&x);
	fclose(mapfp);
	//printf("M_[%d]には[%d]個のコード by count_chord_border()\n",masternumber,x);
	return x;
}

int count_chord_border_kubi(int masternumber)//マスターナンバーを引き数にしてボーダーの個数カウントする回帰呼出関数
{int x=0;//char mapfn[FILENAME_MAX];FILE *mapfp = NULL;
	char cmd[1280];
	int t;//ホントは構造体のメンバ
	static int n = 0;
	int nest;

	
	int i,j;
	int child;//マスターファイルの子供のファイルナンバー
	MASTERFORMAT m;
	FILE *masterfp;FILE *myfp;

	nest = n++;
	//for(i=0;i<nest;i++){printf("-");}printf("[%d]\n",masternumber);


	masterfp = masteropen(masternumber);
	m = master_fscanf(masterfp);
	fclose(masterfp);
	

	if(m.chord_border_painted==0)/*未塗装ならその子供達を襲ったはず*/
	{
		for(i=0;i<m.linenumber;i++)
		{
			masterfp = masteropen(masternumber);
			m = master_fscanf(masterfp);

			for(j=0;j<i;j++){fscanf(masterfp,"%d",&child);}
			if(fscanf(masterfp,"%d",&child)>0){fclose(masterfp);x += count_chord_border(child);}
			else{fclose(masterfp);}
		}
	}
	
	else if(m.chord_border_file>0 && m.chord_border_painted>0)/*この時は作ったはず(これを数えて返す)*/
	{
		myfp = chord_border_open(m.chord_border_file);

		//printf("BORFILE = (%d)\n",m.chord_border_file);

		while(fscanf(myfp,"%d,%s",&t,cmd) > 0){if(t>0){x++;}}//適当に数える
		
		fclose(myfp);
	}
	
	//else if(m.chord_border_painted>0){printf("これ以上考える必要は無い(%d,%d)\n",m.chord_border_painted,m.chord_border_file);return 0;}
	
	//for(i=0;i<nest;i++){printf("-");}
	printf("M_[%d]には%d個のボーダー\n",masternumber,x);
	return x;
}

FORMAT_CHORD_BORDER fscanf_chord_border(FILE *fp)//ファイルから1行読んで構造体を返す関数
{
	FORMAT_CHORD_BORDER x = {0};//:エラーの時何が返る?
	if(fscanf(fp,"%d,%d,%d",&x.length,&x.chord.dia,&x.chord.kind)<0)
	{
		printf("\nfscanf_chord_border()でのスキャンエラー\n");
		printf("%d,%d,%d\n",x.length,x.chord.dia,x.chord.kind);
	}else if(x.length<=0 || x.chord.dia<=0 || x.chord.kind<=0)
	{
		printf("\nスキャンは出来たが,明らかに異常\n");
		printf("異常 => [%d,%d,%d]\n",x.length,x.chord.dia,x.chord.kind);
	}
	return x;
}

int fprintf_chord_border(FILE *fp,FORMAT_CHORD_BORDER *format)//構造体を読んでファイルに1行出力する関数
{
	FORMAT_CHORD_BORDER x;

	x = *format;

	//fprintf(fp,"%d,%d\n",x.chord.dia,x.length);

	fprintf(fp,"%d,%d,%d\n",x.length,x.chord.dia,x.chord.kind);

	return 0;
}
