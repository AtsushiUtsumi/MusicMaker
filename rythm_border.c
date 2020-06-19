#define RYTHM_BORDER

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "masterfile.h"


FILE *rythm_border_open(int number)
{
	FILE *fp;
	char fn[FILENAME_MAX];
	sprintf(fn,"RYTHMBORDER_%d.txt",number);
	sprintf(fn,"BORDER_R_[%d].txt",number);
	fp = fopen(fn,"r");
	if(fp==NULL){printf("ファイル[%s]が見つかりません\n",fn);exit(1);return NULL;}
	else{return fp;}
}
FILE *new_rythm_border_open(int number)
{
	FILE *fp;
	char fn[FILENAME_MAX];
	sprintf(fn,"RYTHMBORDER_%d.txt",number);
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

static int test_rb_out(FILE *fp,int length)//空のCBファイルに長さ分の中身を書き込む(1発で全て書き込む)
{static int a=0;
	int *box;
	int i,j;
	int tubu;//粒の個数
	int to;
	to = (1920/8);
	tubu = length / to;

printf("♪%d個分の長さです\n",tubu);
if(tubu==0 && length>0)//1粒分もなければ長さ分の音で埋める
{
	fprintf(fp,"%d,%d\n",length,9);//仮に9
	return 0;
}

box = (int*)calloc(tubu,sizeof(int));
for(i=0;i<tubu;i++){printf("%d-",box[i]);}printf("\n");
for(i=0;i<tubu;i++){if(rand()%2==0 || i==0){box[i] = 1;}}
for(i=0;i<tubu;i++){printf("%d-",box[i]);}printf("\n");
for(i=0;i<tubu;i++)
{
	if(box[i]>0)
	{
		j=i+1;if(i==tubu||j==tubu){break;}
		while(1){if(box[j]==0){box[i]++;j++;}else{break;}}
	}
}
for(i=0;i<tubu;i++){printf("%d-",box[i]);}printf("\n");

for(i=0;i<tubu;i++){if(box[i]>0){fprintf(fp,"%d,%d\n",to*(box[i]),7);}}//仮に7


free(box);
	return 0;
}
int kaiki_rythm_border(int masternumber)
{
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
			if(fscanf(masterfp,"%d",&child)>0){fclose(masterfp);kaiki_rythm_border(child);}
			else{fclose(masterfp);}
		}
	}
	
	else if(m.rythm_border_file>0 && m.rythm_border_painted>0)/*この時は作らなければならない*/
	{
		myfp = new_rythm_border_open(m.rythm_border_file);

		if(myfp==NULL){//printf("RB強襲済み\n");
		return 0;}
		
		//printf("長さ(%d)のRBファイル(%d)を作ります\n",m.length,m.rythm_border_file);


test_rb_out(myfp,m.length);//いくら何でも適当過ぎる出力
		//fprintf(myfp,"試しに出力\n");
		
		fclose(myfp);
	}
	
	else if(m.rythm_border_painted>0)
	{printf("これ以上考える必要は無い(%d,%d)\n",m.rythm_border_painted,m.rythm_border_file);return 0;}
	
	
	return 0;
}
