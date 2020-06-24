#define BORDERSMAKER_C
#include "BORDERSMAKER.h"
#include "PASSAGE.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "masterfile.h"
#define LINEKIND_HEADER 1
#define LINEKIND_BODY   2



static FILE *patternfile_ab(int patternfilenumber)
{
	int i;FILE *patternfp=NULL;char patternfn[FILENAME_MAX];printf("[%d]を使います\n",patternfilenumber);
	sprintf(patternfn,"PATTERN_AB_%d.txt",patternfilenumber);patternfp = fopen(patternfn,"r");
	if(patternfp==NULL)
	{
		printf("パターン新規作成\n");
		patternfp = fopen(patternfn,"w");/*
		if(patternfilenumber==1){fprintf(patternfp,"1\n0\n0\n1\n0\n0\n1\n0\n");}
		else if(patternfilenumber==2){fprintf(patternfp,"1\n0\n0\n0\n1\n0\n0\n0\n");}
		else if(patternfilenumber==3){fprintf(patternfp,"1\n0\n");}*/fprintf(patternfp,"1\n0\n0\n1\n0\n0\n1\n0\n");//ひとまずメタル
		fclose(patternfp);patternfp = fopen(patternfn,"r");
	}
	return patternfp;
}

static int fill_ab(BORDERS *manaita,int number)
{//この時点で CH が決まってるとは限らない
	int i,j;FILE *patternfp=NULL;//for(i=0;i<number;i++){manaita[i].ab = 0;}return 0;//今はまだ RETURN

	for(i=0;i<number;i++){if(manaita[i].ch>0){manaita[i].ab = 1+rand()%3;}else{manaita[i].ab = 0;}}//これだと CH 未設定の時ひどいことになる
	for(i=0;i<number;i++)
	{
		if(manaita[i].ab>0){if(patternfp!=NULL){fclose(patternfp);}patternfp = patternfile_ab(manaita[i].ab);}
		if(patternfp==NULL){manaita[i].ah=0;}else{PATTERN_REWIND:if(fscanf(patternfp,"%d",&manaita[i].ah)<=0){rewind(patternfp);goto PATTERN_REWIND;}}
	}if(patternfp!=NULL){fclose(patternfp);}
	//for(i=0;i<number;i++){if(i%8==0 || i%8==3 || i%8==6 ||manaita[i].ch>0){manaita[i].ah = 1;}else{manaita[i].ah = 0;}}
	for(i=0;i<number;i++){if(manaita[i].ah > 0){for(j=i+1;j<number;j++){if(manaita[j].ah==0){manaita[i].ah++;}else{break;}}}}
	return 0;
}

static int fill_rb(BORDERS *manaita,int number)
{
	int i,j;//for(i=0;i<number;i++){manaita[i].rb = 0;}return 0;//今はまだ RETURN

	for(i=0;i<number;i++){if(rand()%3==0 || manaita[i].ah>0){manaita[i].rh = 1;}else{manaita[i].rh = 0;}}
	for(i=0;i<number;i++){if(manaita[i].rh > 0){for(j=i+1;j<number;j++){if(manaita[j].rh==0){manaita[i].rh++;}else{break;}}}}
	return 0;
}

static int bordermaker(int masternumber,BORDERS *start,int number,int onelength)//onelength は事実上のqwontize number = メモリのcount
{
	int NofCH=0,NofAH=0,NofRH=0;

	FILE *mapfp;
	char mapfn[FILENAME_MAX];
	static int t=60;
	FILE *out;
	char shouldmakecbfn[FILENAME_MAX];
	char shouldmakeabfn[FILENAME_MAX];
	char shouldmakerbfn[FILENAME_MAX];
	int i,j;
	BORDERS *manaita=NULL;
	
	int cmdn;char cmd[1280];

	FILE *todofp=NULL;char todofn[FILENAME_MAX];
	
	MASTERFORMAT minf;
	FILE *mf=NULL;

	MASTERFORMAT cinf[4] = {0};
	int cnumber[4];
	FILE *cf[4] = {0};
	
	sprintf(todofn,"TODOLIST_[%d].txt",masternumber);
	todofp = fopen(todofn,"r");if(todofp!=NULL){fclose(todofp);return 0;}
	mf = masteropen(masternumber);
	minf = master_fscanf(mf);if(number==0){printf("個数 0 とは何事か[%d]\n",onelength);exit(1);}

	for(i=0;i<minf.linenumber;i++)
	{
		if(fscanf(mf,"%d",&cnumber[i])>0)
		{
			cf[i] = masteropen(cnumber[i]);
			cinf[i] = master_fscanf(cf[i]);
		}else{printf("誰だ!?無いのに行数を[%d]にしたのは!? 終了するぞ => M_[%d]\n",minf.linenumber,masternumber);exit(1);}
	}
	fclose(mf);
	
	if(minf.chord_border_file>0 || minf.accent_border_file>0 || minf.rythm_border_file>0)
	{
		if(minf.chord_border_file > 0){sprintf(shouldmakecbfn,"BORDER_C_[%d].txt",minf.chord_border_file);}else{sprintf(shouldmakecbfn,"BORDER_C_[%d](出たらダメなファイル).txt");}
		if(minf.accent_border_file> 0){sprintf(shouldmakeabfn,"BORDER_A_[%d].txt",minf.accent_border_file);}else{sprintf(shouldmakeabfn,"BORDER_A_[%d](出たらダメなファイル).txt");}
		if(minf.rythm_border_file > 0){sprintf(shouldmakerbfn,"BORDER_R_[%d].txt",minf.rythm_border_file);}else{sprintf(shouldmakerbfn,"BORDER_R_[%d](出たらダメなファイル).txt");}
	}else if(minf.chord_border_painted==1 && minf.accent_border_painted==1 && minf.rythm_border_painted==1){printf("全て作成済みなので RETURN\n");return 0;}
	
	todofp = fopen(todofn,"w");
	if(minf.chord_border_file>0){fprintf(todofp,"1,コード作成\n");}
	if(minf.accent_border_file>0){fprintf(todofp,"2,アクセント作成\n");}
	if(minf.rythm_border_file>0){fprintf(todofp,"3,リズム作成\n");}
	if(minf.chord_border_file<=0 && minf.accent_border_file<=0 && minf.rythm_border_file<=0){fprintf(todofp,"0,何も作成しないで子供達に分裂");}
	fclose(todofp);
//以上TODOリスト出力

/////////////////////////////この関数内でしなければならないこと列挙完了!!
	manaita = (BORDERS*)calloc(number,sizeof(BORDERS));if(manaita==NULL){printf("まな板メモリ確保失敗\n");exit(1);}

	for(i=0;i<number;i++)//まな板に依頼主をコピー
	{manaita[i] = start[i];}

//for(i=0;i<number;i++){printf("まな板(加工前) => [%d,%d,%d,%d,%d,%d]\n",manaita[i].cb,manaita[i].ch,manaita[i].ab,manaita[i].ah,manaita[i].rb,manaita[i].rh);}


////////////////////////////////以下TODOリストと横を見ながらファイル作成
todofp = fopen(todofn,"r");
	while(fscanf(todofp,"%d,%s",&cmdn,cmd)>0)
	{
//printf("CMD[%s]を読み取った!!\n",cmd);
		switch(cmdn)
		{
		case 1:
			for(i=0;i<number;i++)
			{
				if(i%8==0){manaita[i].ch = 1;}
				//これだと子供の480の頭は全てに1なる
				//別に8個毎にが現れるわけではない
				//ここは小節の頭==1でch=1とすべき

				else{manaita[i].ch = 0;}
			}
			for(i=0;i<number;i++){if(manaita[i].ch > 0){for(j=i+1;j<number;j++){if(manaita[j].ch==0){manaita[i].ch++;}else{break;}}}}
			//out = fopen(shouldmakecbfn,"w");for(i=0;i<number;i++){fprintf(out,"%d,%d\n",manaita[i].ch * (1920/8),manaita[i].cb);}fclose(out);out = NULL;
			break;
		case 2://for(i=0;i<number;i++){manaita[i].ab = 0;manaita[i].ah = 0;}
			fill_ab(manaita,number);
			//for(i=0;i<number;i++){if(i%8==0 || i%8==3 || i%8==6 ||manaita[i].ch>0){manaita[i].ah = 1;}else{manaita[i].ah = 0;}}
			//for(i=0;i<number;i++){if(manaita[i].ah > 0){for(j=i+1;j<number;j++){if(manaita[j].ah==0){manaita[i].ah++;}else{break;}}}}
			//out = fopen(shouldmakeabfn,"w");for(i=0;i<number;i++){fprintf(out,"%d,%d\n",manaita[i].ah * (1920/8),manaita[i].ab);}fclose(out);out = NULL;
			break;
		case 3://for(i=0;i<number;i++){manaita[i].rb = 0;manaita[i].rh = 0;}
			fill_rb(manaita,number);
			//for(i=0;i<number;i++){if(rand()%3==0 || manaita[i].ah>0){manaita[i].rh = 1;}else{manaita[i].rh = 0;}}
			//for(i=0;i<number;i++){if(manaita[i].rh > 0){for(j=i+1;j<number;j++){if(manaita[j].rh==0){manaita[i].rh++;}else{break;}}}}
			//out = fopen(shouldmakerbfn,"w");for(i=0;i<number;i++){fprintf(out,"%d,%d\n",manaita[i].rh * (1920/8),++t,manaita[i].rb);}fclose(out);out = NULL;
			break;
		default:break;
		}
	}
	fclose(todofp);
////////////////////////////////以上リストと横を見ながらファイル作成
	for(i=0;i<minf.linenumber;i++){bordermaker(cnumber[i],&manaita[i*(number/minf.linenumber)],number/minf.linenumber,onelength);}
	if(minf.linenumber==4){for(i=0;i<number/4;i++){manaita[i+(number/2)] = manaita[i];}}//こんな適当な方法だがひとまずコピー

/////////////////////////////////////////以下マッピング出力

//printf("\nマスター[%d]のマッピング結果 by bordermaker()\n",masternumber);
//for(i=0;i<number;i++){printf("[%d,%d,%d,%d,%d,%d]\n",manaita[i].cb,manaita[i].ch,manaita[i].ab,manaita[i].ah,manaita[i].rb,manaita[i].rh);}
sprintf(mapfn,"MAP_%d.txt",masternumber);
mapfp = fopen(mapfn,"w");
for(i=0;i<number;i++){if(manaita[i].ch>0){NofCH++;}if(manaita[i].ah>0){NofAH++;}if(manaita[i].rh>0){NofRH++;}}
fprintf(mapfp,"%d,%d,%d,%d,(NofCH,NofAH,NofRH)\n",LINEKIND_HEADER,NofCH,NofAH,NofRH);
for(i=0;i<number;i++){fprintf(mapfp,"%d,%d,%d,%d,%d,%d\n",LINEKIND_BODY,manaita[i].length,manaita[i].barh,manaita[i].ch,manaita[i].ah,manaita[i].rh);}
fclose(mapfp);
/////////////////////////////////////////以上マッピング出力

for(i=0;i<number;i++){start[i] = manaita[i];}free(manaita);return 0;//依頼主にまな板をコピーして終了
	
}

static int borders_passage_maker(char *passagefn)//楽節ファイル単位
{
	int i,number;
	
	FILE *pfp;
	char pfn[FILENAME_MAX];
	FORMAT_PASSAGE p;
	BORDERS undef = {0,0,-1,-1,-1,-1,-1,-1};
	BORDERS *start;
	
	sprintf(pfn,"%s",passagefn);pfp = fopen(pfn,"r");
	if(pfp==NULL){printf("楽節[%s]が見つかりません by borders_passage_maker()\n",pfn);exit(1);}
	p = passage_fscanf(pfp);fclose(pfp);
number = (p.length/(1920/16));//16分音符何個分か
//start = (BORDERS*)calloc(p.length/16,sizeof(BORDERS));//適当
start = (BORDERS*)calloc(number,sizeof(BORDERS));//適当
printf("[%d,%d] by borders_passage_maker()\n",number,p.length);
	for(i=0;i<number;i++){start[i] = undef;}
	for(i=0;i<number;i++)
	{start[i].length = p.size;
		if(i%(p.count/p.NofBar)==0)
		{
			start[i].barh = 8;//今は8ビート
		}
	}
	
	bordermaker(p.masterfile,start,number,p.length/number);
	
	free(start);return 0;
}

int bordersmaker(char *kyokuhennseifn)//曲単位で
{
	
	char cmd[1280];

	
	FILE *khfp;
	char khfn[FILENAME_MAX];
	char pfn[FILENAME_MAX];

	if(kyokuhennseifn==NULL){sprintf(khfn,"曲編成.txt");}else{sprintf(khfn,"%s",kyokuhennseifn);}

printf("bordersmaker(%s) 開始\n",khfn);

	khfp = fopen(khfn,"r");if(khfp==NULL){printf("[%s]が見つかりません by bordersmaker()\n",khfn);exit(1);}
	while(fscanf(khfp,"%s",pfn)>0){borders_passage_maker(pfn);}fclose(khfp);

printf("bordersmaker() 終了\n");return 0;
}

static int ChordAssign(char *outputfn,char *inputfn,char *chordfn)
{
	FILE *outputfp,*inputfp,*chordfp;

	int linekind;char line[1000];int barh,length,ch;int root,kind;char add[120];
	
//printf("[%s]に[%s]をアサインして[%s]を出力します by ChordAssign()\n",inputfn,chordfn,outputfn);
	outputfp = fopen(outputfn,"w");
	inputfp = fopen(inputfn,"r");if(inputfp==NULL){printf("[%s]が見つかりません\n終了します by ChordAssign()\n",inputfn);exit(1);}
	chordfp = fopen(chordfn,"r");if(chordfp==NULL){printf("[%s]が見つかりません\n終了します by ChordAssign()\n",chordfp);fclose(inputfp);exit(1);}
///////////////////////////////////////
	while(fscanf(inputfp,"%d,%s",&linekind,line)>0)
	{
		switch(linekind)
		{
		case LINEKIND_HEADER:
			fprintf(outputfp,"%d,%s,(ROOT,KIND)\n",linekind,line);
			break;
		case LINEKIND_BODY:
			sscanf(line,"%d,%d,%d",&barh,&length,&ch);
			if(ch>0){if(fscanf(chordfp,"%d,%d",&root,&kind)>0){/*printf("[%d,%d] にコードチェンジ\n",root,kind);*/}
			else{printf("コードの中身枯渇\nカウント関数に恐らく不備があります => [%s]\n",chordfn);exit(1);}}
			sprintf(add,",%d,%d",root,kind);strcat(line,add);fprintf(outputfp,"%d,%s\n",linekind,line);
			break;
		default:printf("何だこの行は!?(KIND=%d) => %s\n",linekind,line);exit(1);break;
		}
	}
///////////////////////////////////////
	fclose(outputfp);fclose(inputfp);fclose(chordfp);
	return 0;
}

int cat_chord(int masterfilenumber)//Nコード と MAP から NEW を出力してるのはコイツ
{
	//本来このマスターファイルの子供が全員コードの候補
	//になるファイルを持つかどうかを確認して
	char mapfn[FILENAME_MAX];char chordfn[FILENAME_MAX];char newfn[FILENAME_MAX];
	sprintf(chordfn,"Nコード_[%d].txt",masterfilenumber);
	sprintf(mapfn,"MAP_%d.txt",masterfilenumber);
	sprintf(newfn,"MAP_%d_NEW.txt",masterfilenumber);
	ChordAssign(newfn,mapfn,chordfn);
	return 0;
}
