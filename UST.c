#define UST_C
#include "UST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *ust_fopen(char *filename,USTFILEINFO *x){FILE *fp;char fn[FILENAME_MAX];sprintf(fn,"%s.ust",filename);fp = fopen(fn,"w");fprintf(fp,"[#VERSION]\nUST Version1.2\n[#SETTING]\nTempo=240.00\nTracks=1\nProjectName=新規ソング\nVoiceDir=%%VOICE%%櫻花アリス(通常)\nOutFile=\nCacheDir=コード自動生成.cache\nTool1=wavtool.exe\nTool2=resampler.exe\n");return fp;}
int ust_fclose(FILE *fp){fprintf(fp,"[#TRACKEND]\n");fclose(fp);return 0;}
int ust_fprintf(FILE *fp,USTNOTEINFO *x){
	fprintf(fp,"[#%04d]\nLength=%d\nLyric=%s\nNoteNum=%d\nPreUtterance=\nIntensity=100\nModulation=0\n",x->counter,x->length,x->lylic,x->note);
	return 0;}

static USTNOTEINFO pre_ust_fscanf(FILE *fp)
{char *IfLyric = NULL;
	static int totallength = 0;int x;
	char *table[] = {"R","あ","ど","れ","み","ふぁ","そ","ら","し"};
	static int count = 0;
	USTNOTEINFO ninf = {0};
	if(fscanf(fp,"%d,%d,%s",&ninf.length,&ninf.note,ninf.lylic)<0){ninf.counter = (-1);}
	else if(ninf.length>0)
	{
		totallength += ninf.length;
		ninf.counter = (count++);
		if(ninf.note==0){x=0;}
		else
		{
			switch(ninf.note%12)
			{
			case 0:x=2;break;
			case 2:x=3;break;
			case 4:x=4;break;
			case 5:x=5;break;
			case 7:x=6;break;
			case 9:x=7;break;
			case 11:x=8;break;
			default:x=1;break;
			}
		}
		if(strlen(ninf.lylic)>4){printf("歌詞長すぎ!! => [%s]\n終了します\n",ninf.lylic);exit(1);}
		if(strlen(ninf.lylic)==0){sprintf(ninf.lylic,"%s",table[x]);}
	}
	return ninf;
}

int ust_file_out(const char *partname,USTFILEINFO *finf)//-1なら伸ばしといういことで
{
	FILE *LongLyricsFp = NULL;char LongLyricsFn[260];char Liric[5];//sprintf(LongLyricsFn,"%sが歌う1曲分の歌詞.txt",partname);
	
	
	typedef struct{int length,note;}BELT;BELT *belt;int i;
	FILE *beltfp;char beltfn[260];int linenumber=0;char line[100];
	int count=0;
	FILE *ustfp;char ustfn[FILENAME_MAX];
	FILE *preustfp;char preustfn[FILENAME_MAX];
	
	USTNOTEINFO ninf;
	
	sprintf(preustfn,"%s.txt",partname);//このファイルはで -1 が伸ばし, 0 が休符(0 は詰められていない)
	sprintf(ustfn,"%s",partname);
	sprintf(beltfn,"%s_BELT.txt",partname);

preustfp = fopen(preustfn,"r");while(fscanf(preustfp,"%s",line)>0){linenumber++;}fclose(preustfp);preustfp=NULL;
belt = (BELT*)calloc(linenumber,sizeof(BELT));
preustfp = fopen(preustfn,"r");for(i=0;i<linenumber;i++){fscanf(preustfp,"%d,%d",&belt[i].length,&belt[i].note);}fclose(preustfp);preustfp=NULL;

for(i=linenumber-1;i>0;i--)//連続する -1 を前の音に上乗せ
{if(belt[i].note==-1){belt[i-1].length += belt[i].length;belt[i].length = 0;}}

for(i=0;i<linenumber-1;i++)//連続する 0 をひとまとめに
{
	if(belt[i].length>0 && belt[i].note==0 && belt[i+1].note==0){belt[i+1].length += belt[i].length;belt[i].length = 0;}
}

beltfp =fopen(beltfn,"w");
TEST_N_Lyrics(linenumber,0);sprintf(LongLyricsFn,"歌詞.txt");LongLyricsFp = fopen(LongLyricsFn,"r");if(LongLyricsFp==NULL){printf("[%s]が見つかりません\n終了します\n",LongLyricsFn);}
for(i=0;i<linenumber;i++)
{
	if(belt[i].length>0)
	{
		if(belt[i].note==0){belt[i].note = -1;}//MIDIでは 0 は休符ではないので休符を表す-1に変換
		fprintf(beltfp,"%d,%d",belt[i].length,belt[i].note);
		if(belt[i].note==-1){fprintf(beltfp,",R");}
		else if(LongLyricsFp!=NULL && belt[i].note>0){if(fscanf(LongLyricsFp,"%s",Liric)>0){fprintf(beltfp,",%s",Liric);}}
		fprintf(beltfp,"\n");
	}
}fclose(LongLyricsFp);
fclose(beltfp);beltfp=NULL;
preustfp = fopen(beltfn,"r");//入力ファイルオープン
	if(preustfp==NULL){printf("%sが見つかりません\n",preustfn);exit(1);}

	ustfp = ust_fopen(ustfn,finf);
	ninf = pre_ust_fscanf(preustfp);
	while(ninf.counter >= 0){ninf.counter = (count++);ust_fprintf(ustfp,&ninf);ninf = pre_ust_fscanf(preustfp);}
	ust_fclose(ustfp);
	
	fclose(preustfp);//入力ファイルクローズ
free(belt);

//printf("BELT\n");exit(1);
MidiFileOut(partname);
	return 0;
}
