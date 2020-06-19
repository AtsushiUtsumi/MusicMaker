#include "MUSIC.h"
#include <stdio.h>
#include <stdlib.h>

const char *IntToString(int note)
{
	switch(note%12)
	{
	case 0:return "C";
	case 1:return "C♯";
	case 2:return "D";
	case 3:return "D♯";
	case 4:return "E";
	case 5:return "F";
	case 6:return "G♭";
	case 7:return "G";
	case 8:return "A♭";
	case 9:return "A";
	case 10:return "B♭";
	case 11:return "B";
	}
}

//case CHORD_KIND_M:break;case CHORD_KIND_m:break;case CHORD_KIND_aug:break;case CHORD_KIND_dim:break;case CHORD_KIND_6:break;case CHORD_KIND_m6:break;case CHORD_KIND_maj7:break;case CHORD_KIND_mmaj7:break;case CHORD_KIND_7:break;case CHORD_KIND_m7:break;case CHORD_KIND_7aug:break;case CHORD_KIND_7f5:break;case CHORD_KIND_m7aug:break;case CHORD_KIND_m7f5:break;case CHORD_KIND_sus4:break;case CHORD_KIND_7sus4:break;default:break;
static int SetChord(int *x,int root,int ChordKind)
{int i;for(i=0;i<12;i++){x[i] = 0;}
	x[(root)%12] = 1;//根音は絶対
	switch(ChordKind)
	{
	case CHORD_KIND_M:x[(root+4)%12]=1;x[(root+7)%12]=1;break;
	case CHORD_KIND_m:x[(root+3)%12]=1;x[(root+7)%12]=1;break;
	case CHORD_KIND_aug://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_dim://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_6://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_m6://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_maj7://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_mmaj7://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_7://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_m7://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_7aug://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_7f5://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_m7aug://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_m7f5://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_sus4://x[(root+)%12]=1;x[(root+)%12]=1;break;
	case CHORD_KIND_7sus4://x[(root+)%12]=1;x[(root+)%12]=1;break;
	default:printf("ChordKind = [%d] は対応していません by SetChord()\n",ChordKind);break;
	}return 0;
}
static int Candidate()
{char test[234];
	int i,j,root,chordkind;
	int pin[12];//ボーリングのピンと同じ
	int chord[12];//これにコード構成音をセットして倒す
	
	for(i=0;i<12;i++){pin[i] = (i==0 || i==4 || i==7 || i==11) ? 1:0;}//入力音にピンを立てる
	for(root=0;root<12;root++)
	{
		printf("\n[%s]についてチェック\n",IntToString(root));
		for(chordkind=1;chordkind<3;)
		{
			SetChord(chord,root,chordkind);
			for(i=0;i<12;i++){if(pin[i]-chord[i]>0){printf("このコードは候補になりません\n");goto GUTTER;}}
			printf("このコードは候補になります\n");
GUTTER:chordkind++;
		}
	}return 0;
}




int main2()
{
	int MakeCanUseNoteFile(int,int);
	int root;
	for(root=0;root<12;root++)
	{
		switch(root)
		{
		case 0:case 5:case 7:MakeCanUseNoteFile(root,CHORD_KIND_M);break;
		case 2:case 4:case 9:MakeCanUseNoteFile(root,CHORD_KIND_m);break;
		case 11:MakeCanUseNoteFile(root,CHORD_KIND_dim);break;
		default:break;
		}
	}
	
	
	Candidate();
	return 0;
}

static int MakeCanUseNoteFile(int root,int chordkind)//Avalableな音をファイルにするchoarkind>0ならリクエストに応える
{//アボイドノートであってもメロディで普通に短い音として使う
#define NONCHORDTONE (-1)
#define AVOIDNOTE    (-2)
#define NOTWHITE     (-3)//不本意ながら

	FILE *CanUseNoteFp;char CanUseNoteFn[260];int i;
	typedef struct{int degree;int note;}ALLNOTE;
	
	ALLNOTE x[12];
	//printf("\n\n[%s]を根音とするコード X って方針で by MakeCanUseNoteFile()\n",IntToString(root));
	printf("コード[%s",IntToString(root));
	switch(chordkind){case CHORD_KIND_M:printf("M");break;case CHORD_KIND_m:printf("m");break;case CHORD_KIND_dim:printf("dim");break;}
	printf("]って方針で by MakeCanUseNoteFile()\n");

	sprintf(CanUseNoteFn,"CanUseNoteFile.txt");

	for(i=0;i<12;i++){x[i].note = i;x[(i+root)%12].degree=i;}
	for(i=0;i<12;i++){if(x[i].note!=0 && x[i].note!=2 && x[i].note!=4 && x[i].note!=5 && x[i].note!=7 && x[i].note!=9 && x[i].note!=11){x[i].degree = NOTWHITE;}}
	//for(i=0;i<12;i++){if(x[i].degree%2==1){x[i].degree = NONCHORDTONE;}}
	//for(i=0;i<12;i++){if(x[i].degree%3==2){x[i].degree = AVOIDNOTE;}}

//for(i=0;i<12;i++){printf("[%2d] = %2d\n",i,x[i].degree);}
	CanUseNoteFp = fopen(CanUseNoteFn,"w");
	fprintf(CanUseNoteFp,"1,Header,[%s]を根音とするコードX\n",IntToString(root));
	for(i=0;i<12;i++){if(x[i].degree>=0){
fprintf(CanUseNoteFp,"2,%d,%d,%s\n",x[i].note,x[i].degree,IntToString(x[i].note));
printf("%d,%d,%s",x[i].note,x[i].degree,IntToString(x[i].note));

switch(x[i].degree){
case 0:printf(" <= [ROOT]");break;
case 3:printf(" <= [m3]");break;
case 4:printf(" <= [M3]");break;
case 7:printf(" <= [P5]");break;}
printf("\n");

}}
	fclose(CanUseNoteFp);
	return 0;
}
