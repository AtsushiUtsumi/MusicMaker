/*
#include <stdio.h>
#include "PASSAGE.h"
#include "UST.h"
//音高を付ける時にツリー構造を解釈プログラムが把握する必要がある以上,
//ここでの作業は READER() 側になる
//生成するファイルはUSTに近い形になる
//問題点は音高がつかない状態をマスターファイルとして良いかということ
//他のパートが追加できるようにするためには最低コードは音高生成しておく必要がある
//結論：コードだけはマスターファイルでつけておく!!
//コンパイラとリンカは各楽器毎にそれぞれ一つずつ持つことになる?
//このファイルは本来は独立した main() を持つことになるので,
//本来のmain()をmain2()とかにしてビルドすること(作業ディレクトリも変えること)


static FILE *outfp = NULL;
static char *outfn = "ギター.ust";

int guitar_passage_compiler(char *beforefn,char *afterfn)//1つの楽節ファイルをコンパイル//本来はこの関数で音高を決める
{
	int i;
	int head;
	int tubunum;
	typedef struct{int note,length;}TUBU;
	TUBU *x;

	char cmd[1280];
	int length=-1,note=-1;
////////////////量子化
	//int t = 0;
	FILE *ryofp;
	char *ryofn = "量子化.txt";
////////////////

////////////////滑らか化
	int t = 0;
	FILE *ngfp;
	char *ngfn = "滑らか後.txt";

	FILE *nfp;
	char *nfn = "滑らか.txt";
	int c = 0;
////////////////

	FILE *kaikifp = NULL;char *kaikifn = "回帰出力.txt";

	FILE *beforefp=NULL,*afterfp=NULL;
	FORMAT_PASSAGE pinf;
	extern FORMAT_PASSAGE passage_fscanf(FILE*);
	extern int reader_kaiki(FILE*,int);
	extern int q(char*,int);
	extern int nameraka(int,int,int);

//printf("\nBEFORE_FILENAME\n%s\n\n",beforefn);
//printf("\nAFTER_FILENAME\n%s\n\n",afterfn);


	beforefp = fopen(beforefn,"r");if(beforefp==NULL){printf("[%s]が見つかりません\n",beforefn);return (-1);}
	else{pinf = passage_fscanf(beforefp);fclose(beforefp);}
	
	kaikifp = fopen(kaikifn,"w");//if(kaikifp==NULL){printf("[%s]が開けません\n");return (-1);}
	reader_kaiki(kaikifp,pinf.masterfile);
	fclose(kaikifp);kaikifp = NULL;

	
	
////////////////量子化

	q(kaikifn,(1920/4));
	afterfp = fopen(afterfn,"w");
	ryofp = fopen(ryofn,"r");
	t = tubunum = 0;
	while(fscanf(ryofp,"%d,%d,%s",&head,&note,cmd)>0)
	{t++;
		if(head>0){fprintf(afterfp,"%d,%d\n",head*(1920/4),note);tubunum++;}
	}fclose(ryofp);
//printf("%d行中 %d個の音があります\n",t,tubunum);
	fclose(afterfp);afterfp = NULL;

//printf("終了2\n");exit(1);


nameraka(tubunum,60,60);
x = (TUBU*)calloc(tubunum,sizeof(TUBU));if(x==NULL){printf("メモリ確保に失敗しました\n");exit(1);}
nfp = fopen(nfn,"r");if(nfp==NULL){printf("[%s]が見つかりません\n",nfn);exit(1);}
for(i=0;i<tubunum;i++){fscanf(nfp,"%d,%d",&t,&x[i].note);}
fclose(nfp);nfp = NULL;
////////////////////////////////////
afterfp = fopen(afterfn,"r");
for(i=0;i<tubunum;i++){fscanf(afterfp,"%d,%d\n",&x[i].length,&t);}
fclose(afterfp);afterfp = NULL;




////////////////////////////////////
//for(i=0;i<tubunum;i++){printf("N = %d,L = %d\n",x[i].note,x[i].length);}

afterfp = fopen(afterfn,"w");
if(afterfp==NULL){printf("ファイル作成に失敗しました\n");exit(1);}
for(i=0;i<tubunum;i++){fprintf(afterfp,"%d,%d\n",x[i].length,x[i].note);}
fclose(afterfp);afterfp = NULL;

free(x);return 0;
}

int guitar_passage_linker(char *kyokuhennseifn)//コンパイル後の楽節ファイル複数をつなげて1曲に
{
	extern int ust_file_out(char *,USTFILEINFO *);
	
	USTFILEINFO finf = {120,1};
	int c=0;
	char line[1280];
	
char *testfn = "GUITAR.txt";	
	FILE *testfp = NULL;
	
	FILE *cpfp;
	char cpfn[128];
	
	FILE *pfp;
	char pfn[128];
	FILE *khfp;

	printf("GUITAR[%s]リンクします\n",kyokuhennseifn);
	
	testfp = fopen(testfn,"w");
	khfp = fopen(kyokuhennseifn,"r");
	while(fscanf(khfp,"%s",pfn)>0)
	{
		sprintf(cpfn,"GUITAR[%d].txt",c++);
		
//printf("[%s]コンパイル前\n",cpfn);
		guitar_passage_compiler(pfn,cpfn);
//printf("[%s]コンパイル後\n",cpfn);

//今は作ったそばから接合開始
cpfp = fopen(cpfn,"r");
if(cpfp==NULL){printf("[%s]が見つかりません\n",cpfn);}
//else{printf("[%s]を開きます\n",cpfn);}
while(fscanf(cpfp,"%s",line)>0)
{fprintf(testfp,"%s\n",line);}
fclose(cpfp);
//接合終了

	}
fclose(testfp);
	fclose(khfp);




	
ust_file_out("GUITAR",&finf);

	return 0;
}
*/
