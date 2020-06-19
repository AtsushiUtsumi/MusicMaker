//ボーダーファイルを量子マッピング
//ひょっとしてこのファイル不要!?

#include <stdio.h>
#include "masterfile.h"
#include "chord_border.h"
#include "accent_border.h"
#include "rythm_border.h"


static int cb_connect(int masternumber,int unit,char *outputfilename){FILE *outfp = NULL;char *outfn;
if(outputfilename==NULL){outfn = "DEFAULT_OUT_CB.txt";}else{outfn = outputfilename;}
outfp = fopen(outfn,"w");
//printf("開始\n");
reader_kaiki_cb(outfp,masternumber);

fclose(outfp);
////////////////////////本来はここで量子化すべき
q(outfn,unit);remove(outfn);rename("量子化.txt",outfn);
////////////////////////
return 0;}
static int ab_connect(int masternumber,int unit,char *outputfilename){FILE *outfp = NULL;char *outfn;
if(outputfilename==NULL){outfn = "DEFAULT_OUT_AB.txt";}else{outfn = outputfilename;}
outfp = fopen(outfn,"w");reader_kaiki_ab(outfp,masternumber);fclose(outfp);
////////////////////////本来はここで量子化すべき
q(outfn,unit);remove(outfn);rename("量子化.txt",outfn);
////////////////////////
return 0;}
static int rb_connect(int masternumber,int unit,char *outputfilename){FILE *outfp = NULL;char *outfn;
if(outputfilename==NULL){outfn = "DEFAULT_OUT_RB.txt";}else{outfn = outputfilename;}
outfp = fopen(outfn,"w");reader_kaiki_rb(outfp,masternumber);fclose(outfp);
////////////////////////本来はここで量子化すべき
q(outfn,unit);remove(outfn);rename("量子化.txt",outfn);
////////////////////////
return 0;}

static int reader_kaiki_cb(FILE *fp,int masternumber)//ファイルの末尾にマスターファイルの中身のコードボーダーを付け足す回帰関数
{
	int kari,rl;
	extern FILE *chord_border_open(int number);
	FILE *cbfp;
	int i,j;
	int child;//マスターファイルの子供のファイルナンバー
	
	MASTERFORMAT m;
	FILE *masterfp;FILE *myfp;

printf("\n\nNEWKAIKI_CB\n");
	masterfp = masteropen(masternumber);
	m = master_fscanf(masterfp);
	fclose(masterfp);
	
	

	if(m.chord_border_painted==0)/*未塗装ならその子供達を襲う*/
	{
		for(i=0;i<m.linenumber;i++)
		{
			masterfp = masteropen(masternumber);
			m = master_fscanf(masterfp);

			for(j=0;j<i;j++){fscanf(masterfp,"%d",&child);}
			if(fscanf(masterfp,"%d",&child)>0){fclose(masterfp);reader_kaiki_cb(fp,child);}
			else{fclose(masterfp);}
		}
	}
	
	else if(m.chord_border_file>0 && m.chord_border_painted>0)/*この時は付け足さなくてはならない*/
	{
		cbfp = chord_border_open(m.chord_border_file);
printf("以下RBファイル[%d]\n",m.chord_border_file);
		while(1)
		{
			kari = 0;rl = 0;
			fscanf(cbfp,"%d,%d",&kari,&rl);
//printf("RB => (%d,%d)\n",kari,rl);
			if(rl<=0){fclose(cbfp);break;}
			fprintf(fp,"%d,%d\n",kari,rl);
		}
	}
	
	else if(m.chord_border_painted>0)
	{printf("これ以上考える必要は無い(%d,%d)\n",m.chord_border_painted,m.chord_border_file);return 0;}


	return 0;
}

static int reader_kaiki_ab(FILE *fp,int masternumber)//ファイルの末尾にマスターファイルの中身を付け足すテスト用回帰関数
{
	int kari,rl;
	extern FILE *accent_border_open(int number);
	FILE *cbfp;
	int i,j;
	int child;//マスターファイルの子供のファイルナンバー
	
	MASTERFORMAT m;
	FILE *masterfp;FILE *myfp;
	masterfp = masteropen(masternumber);
	m = master_fscanf(masterfp);
	fclose(masterfp);
	
	

	if(m.accent_border_painted==0)/*未塗装ならその子供達を襲う*/
	{
		for(i=0;i<m.linenumber;i++)
		{
			masterfp = masteropen(masternumber);
			m = master_fscanf(masterfp);

			for(j=0;j<i;j++){fscanf(masterfp,"%d",&child);}
			if(fscanf(masterfp,"%d",&child)>0){fclose(masterfp);reader_kaiki_ab(fp,child);}
			else{fclose(masterfp);}
		}
	}
	
	else if(m.accent_border_file>0 && m.accent_border_painted>0)/*この時は付け足さなくてはならない*/
	{
		cbfp = accent_border_open(m.accent_border_file);
		while(1)
		{
			kari = 0;rl = 0;
			fscanf(cbfp,"%d,%d",&rl,&kari);
//printf("(%d,%d)\n",kari,rl);
			if(rl<=0){fclose(cbfp);break;}
			fprintf(fp,"%d,%d\n",rl,kari);
		}
	}
	
	else if(m.accent_border_painted>0)
	{printf("これ以上考える必要は無い(%d,%d)\n",m.accent_border_painted,m.accent_border_file);return 0;}


	return 0;
}

static int reader_kaiki_rb(FILE *fp,int masternumber)//ファイルの末尾にマスターファイルの中身のリズムを付け足すテスト用回帰関数
{
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
			if(fscanf(masterfp,"%d",&child)>0){fclose(masterfp);reader_kaiki_rb(fp,child);}
			else{fclose(masterfp);}
		}
	}
	
	else if(m.rythm_border_file>0 && m.rythm_border_painted>0)/*この時は付け足さなくてはならない*/
	{
		cbfp = rythm_border_open(m.rythm_border_file);
		while(1)
		{
			kari = 0;rl = 0;
			fscanf(cbfp,"%d,%d",&rl,&kari);
			if(rl<=0){fclose(cbfp);break;}
			fprintf(fp,"%d,%d\n",rl,kari);
		}
	}
	
	else if(m.rythm_border_painted>0)
	{printf("これ以上考える必要は無い(%d,%d)\n",m.rythm_border_painted,m.rythm_border_file);return 0;}


	return 0;
}

static int borders_mapper(int masternumber,int unit,char *outputfilename)//ボーダーファイルを量子マッピングする関数
{
	char *defaultoutputfn = "マッピングテスト.txt";
	char *outputfn;
	FILE *outfp = NULL;
	FILE *cbf=NULL,*abf=NULL,*rbf=NULL;
	char *cbfn="DEFAULT_OUT_CB.txt",*abfn="DEFAULT_OUT_AB.txt",*rbfn="DEFAULT_OUT_RB.txt";
	typedef struct{int ch,cb,ah,ab,rh,rb;}OUT;
	const OUT ini ={-1,-1,-1,-1,-1,-1};
	OUT *x;
	int i,n;
	FILE *mfp;MASTERFORMAT m;
	
	if(outputfilename==NULL){outputfn = defaultoutputfn;}else{outputfn = outputfilename;}

	mfp = masteropen(masternumber);m = master_fscanf(mfp);fclose(mfp);
	n = (m.length) / unit;
//x = (OUT*)calloc(n,sizeof(BOX));for(i=0;i<n;i++){x[i] = ini;} <= なにこれ?
x = (OUT*)calloc(n,sizeof(OUT));for(i=0;i<n;i++){x[i] = ini;}

	cb_connect(masternumber,unit,NULL);cbf = fopen(cbfn,"r");for(i=0;i<n;i++){fscanf(cbf,"%d,%d",&x[i].ch,&x[i].cb);}fclose(cbf);
	ab_connect(masternumber,unit,NULL);abf = fopen(abfn,"r");for(i=0;i<n;i++){fscanf(abf,"%d,%d",&x[i].ah,&x[i].ab);}fclose(abf);
	rb_connect(masternumber,unit,NULL);rbf = fopen(rbfn,"r");for(i=0;i<n;i++){fscanf(rbf,"%d,%d",&x[i].rh,&x[i].rb);}fclose(rbf);
	outfp = fopen(outputfn,"w");
	for(i=0;i<n;i++){fprintf(outfp,"%d,%d,%d,%d,%d,%d\n"
		,x[i].ch,x[i].cb
		,x[i].ah,x[i].ab
		,x[i].rh,x[i].rb
		);}
	fclose(outfp);

free(x);
	return 0;
}
