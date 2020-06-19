#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SCORE.h"
#include "MUSIC.h"
#include "UST.h"
#include "SESSION.h"
#include "masterfile.h"
#include "PASSAGE.h"
#define LINEKIND_BODY 2

static const char *playername = "DAVE_MURRAY";
static const char *partname = "PLAYER_DAVE_MURRAY";
static int NOP;//�N�����ł̑��̃p�[�g�����L�^���Ă���
static int NofS = 6;

//typedef struct{int length,note;}PART;//�Z�b�V�����Ŏg�p����
typedef struct
{
	int wave,pre,note;
}X;//�X�̃p�[�g�Ŏg�p
//typedef struct{int c_head,c_file,have_more_children;}CHILDREN_SIDE;

typedef struct{int nanika;}FEELING;//���̎��̃v���[���[�̋C����

static int NearWhite(int note,int whitefile,int pm)//�D�݂̃X�P�[���ł����񂶂�Ȃ�?
{
	int x,y;x = note;y = (pm>0) ? 1:-1;
	while(x%12!=0 && x%12!=2 && x%12!=4 && x%12!=5 && x%12!=7 && x%12!=9 && x%12!=11)//�C�ɐH��Ȃ���
	{x+=y;}//printf("X = %d by NearWhite()\n",x);
	return x;
}



static int fill_children_side(int masterfilenumber,CHILDREN_SIDE *dest,int n)//�}�X�^�[�t�@�C�����g���֐��ɂ͕֗�����
{
	MASTERFORMAT m,c;int x=(-1);//�ꖇ��Ȃ�S������������
	int i,j;FILE *masterfp,*childfp;
	masterfp = masteropen(masterfilenumber);m = master_fscanf(masterfp);
	//if(n!=m.count){printf("\n\nfill_children_side()\n[%d] <= m.count\n[%d] <= n (����)\n�I�����܂�\n\n",m.count,n);exit(1);}
	for(i=0;i<n;i++){if(i%(n/m.linenumber) ==0){fscanf(masterfp,"%d",&x);dest[i].c_head = 1;dest[i].have_more_children = 1;}dest[i].c_file = x;}
	for(i=0;i<n;i++)
	{
		if(dest[i].c_head==1)//�w�b�h�L���b�`
		{
			childfp = masteropen(dest[i].c_file);c = master_fscanf(childfp);if(c.linenumber>0){dest[i].have_more_children = 1;}
			fclose(childfp);
			for(j=i;j<n;dest[i].c_head++,j++){if(j==n-1 || dest[j+1].c_head>0)break;}//���͂���,����Ȃɓ�����Ƃł͂Ȃ�
		}
	}
	fclose(masterfp);return 0;
}


static FILE *WaveFileOpen(int n,int WaveID)//�g����悤�ɉ��悪�y��ɂ���ĈႤ���Ƃ���PRE�͔ėp�����Ⴂ
{//�ω����o�������ꏊ���珑�������邱�Ƃ��l����Ǝn�܂�̑O�̉��v���X�}�C�i�X�̕�������
	int i;typedef struct{int pm;}WAVE;WAVE *w;
	FILE *wavefp;char wavefn[FILENAME_MAX];
	sprintf(wavefn,"WAVE_%d_%d.txt",n,WaveID);
	wavefp = fopen(wavefn,"r");
	if(wavefp==NULL)
	{
		wavefp = fopen(wavefn,"w");
		w = (WAVE *)calloc(n,sizeof(WAVE));
		for(i=0;i<n;i++){w[i].pm = (rand()%2==0) ? 1:-1;if(i==0){w[i].pm = 0;}}
		for(i=0;i<n;i++){fprintf(wavefp,"%d\n",w[i].pm);}
		free(w);
		fclose(wavefp);
		wavefp = fopen(wavefn,"r");
	}
	return wavefp;
}

static FILE *OpenRiff(int RiffID)//���t�ɂ͍ޗ����K�v�����Ă���!!
{//���ߖ����͎�t�Ȃ���
	FILE *RiffFp=NULL;char RiffFn[260];sprintf(RiffFn,"RIFF_%d.txt",RiffID);
	RiffFp = fopen(RiffFn,"r");
	if(RiffFp==NULL)
	{
		RiffFp = fopen(RiffFn,"w");fprintf(RiffFp,"2,\n");
		fclose(RiffFp);
		RiffFp = fopen(RiffFn,"r");
	}
	return RiffFp;
}

static char *IDtoFn_Nanika(int ID){static char Fn[260];sprintf(Fn,"�M�^�[_NANIKA_%d.txt",ID);return Fn;}

static int NewNanika()
{
	FILE *Fp=NULL;
	int static t=0;
	
	t++;
	Fp = fopen(IDtoFn_Nanika(t),"w");
	fprintf(Fp,"2,%d,���t\n",t);
	fclose(Fp);
	return t;
}



























static int Maker_DAVE_MURRAY_Master(int MasterNumber,SCORE_LINE *s,X **x,int n,int PassageID,int NofS,int NofOP,PART **OP)
{
	FILE *wavefp;
	CHILDREN_SIDE *cside;int i,j;CHORD nowchord;int lastpre;
	cside = (CHILDREN_SIDE *)calloc(n,sizeof(CHILDREN_SIDE));
	fill_children_side(1,cside,n);

/////////////////�����f�B�A�X�ɂ�����	
	wavefp = WaveFileOpen(n,MasterNumber);
	for(i=0;i<n;i++){if(cside[i].c_head>0){rewind(wavefp);x[i][2].pre = 60;lastpre = 60;}if(s[i].rh>0){fscanf(wavefp,"%d",&x[i][2].wave);x[i][2].pre = lastpre+x[i][2].wave;}}
	fclose(wavefp);

	for(i=0;i<n;i++)
	{
		nowchord.dia = s[i].chord_root;nowchord.kind= s[i].chord_kind;
		if(x[i][2].pre>0)
		{
			x[i][2].note = (s[i].ah>0) ? NearChordTone(x[i][2].pre,0,1,&nowchord,0):NearWhite(x[i][2].pre,1,1);
		}else{x[i][2].note = -1;}
	}



	for(i=0;i<n;i++)


/////////////////�ȏチ���f�B�A�X

	switch(PassageID){case 1:case 5:case 6:
	for(i=0;i<n;i++){x[i][0].note = 52;x[i][1].note = 0;}
	
	for(i=0;i<n;i++){nowchord.dia = s[i].chord_root;nowchord.kind = s[i].chord_kind;
		if(s[i].ah>0){x[i][0].note = NearChordTone(1+x[i][0].note,0,1,&nowchord,0);}}break;
	
	case 2:case 3:case 4://�Ђ�����p���[�R�[�h
		for(i=0;i<n;i++){x[i][0].note = 52;x[i][1].note = 0;}
		for(i=0;i<n;i++)
		{
			nowchord.dia = s[i].chord_root;nowchord.kind= s[i].chord_kind;
			if(s[i].ch>0)
			{
				if(i!=0){x[i-1][0].note = 0;x[i-1][1].note = 0;}
				x[i][0].note = NearChordTone(x[i][0].note,0,1,&nowchord,0);x[i][1].note = x[i][0].note+7;
			}else{x[i][0].note = -1;x[i][1].note = -1;}
		}break;
	default:printf("�ǂ������!? => [%d]\n",PassageID);exit(1);break;}

	free(cside);
	return 0;
}


static FILE *PassageOpen(int PassageID)
{
	FILE *fp;char fn[260];
	switch(PassageID)
	{
	case 1:sprintf(fn,"PASSAGE[�O�t].txt");break;
	case 2:sprintf(fn,"PASSAGE[A����].txt");break;
	case 3:sprintf(fn,"PASSAGE[B����].txt");break;
	case 4:sprintf(fn,"PASSAGE[�T�r].txt");break;
	case 5:sprintf(fn,"PASSAGE[�ԑt].txt");break;
	case 6:sprintf(fn,"PASSAGE[��t].txt");break;
	default:printf("\n�s���Ȋy��ID = [%d]\n�I�����܂�\n",PassageID);exit(1);break;
	}
	fp = fopen(fn,"r");
	if(fp==NULL){printf("[%s]��������܂��� by PassageOpen()\n�I�����܂�\n",fn);exit(1);}
	//else{printf("[%s]���J���܂� by PassageOpen()\n",fn);}
	return fp;
}
//�}�X�^�[�����ł��ĂȂ�
//�}�X�^�[�������Ȃ瓯�����Y�����ƖY��ĂȂ���?

static int Maker_DAVE_MURRAY_Passage(int passageID,SCORE_LINE *s,X **x,int n)//�����уt�@�C���쐬���ׂ�(�����̂�)
{
	FORMAT_PASSAGE p;FILE *passagefp;
	
#define POWERCHORD 1
	int WhatToDo = POWERCHORD;
	//int solo,powerchord;
	//FILE *missionfp;char missionfn[260];
	//X�p�[�g�̕⍲,�\��,�o�b�N���̖������t�@�C���ɕۑ�mission_X_Y.txt(����ΊJ�����̃p�[�g���K�v�ɉ����ĕʃp�[�g�ɏ��������߂�̂�������ɑ��݂���Ȃ���ׂ��ł͂Ȃ�)
	FILE *mysoundfp;char mysoundfn[260];
	//char *testline = "-1,-2,-3,-4,-5,-6,-7";
	int testint[8]={0};
int outnote[6];
	
	char PartsScaner[1000]={0};
	PART **OP;//OtherParts�̗�(�|�C���^�e�[�u��)
	int NofOP;
	int i,j;CHORD nowchord;int ID;FILE *scrapfp;passagefp = PassageOpen(passageID);p= passage_fscanf(passagefp);fclose(passagefp);

	//NofOP = NumberOfParts(NULL,1,passageID);
	NofOP = NOP;
	
	sprintf(mysoundfn,"mysound_%s_%d.txt",partname,passageID);
	mysoundfp = fopen(mysoundfn,"w");
	OP = (PART**)calloc(n,sizeof(PART*));//�s�����|�C���^���m��
	for(i=0;i<n;i++)
	{
		OP[i] = (PART*)calloc(NofOP,sizeof(PART));//���ꂼ��̍s�Ƀp�[�g����
		if(OP[i]==NULL){printf("�������m�ێ��s\n");exit(1);}
	}

LoadSession_Passage(OP,NofOP,1,passageID);//for(i=0;i<64;i++){printf("%s [%2d] => ",partname,i);for(j=0;j<NofOP;j++){printf(",%3d",OP[i][j].note);}printf("\n");}

Maker_DAVE_MURRAY_Master(p.masterfile,s,x,n,passageID,NofS,NofOP,OP);
	
for(i=0;i<NofOP;i++){free(OP[i]);OP[i]=NULL;}
	free(OP);OP=NULL;

	
	for(i=0;i<NofS;i++){outnote[i] = 0;}
	for(i=0;i<n;i++)
	{
		for(j=0;j<NofS;j++)
		{
			if(x[i][j].note>0){outnote[j] = x[i][j].note;}
			fprintf(mysoundfp,"%d",outnote[j]);
			if(j==NofS-1){fprintf(mysoundfp,"\n");}else{fprintf(mysoundfp,",");}
		}
	}
	fclose(mysoundfp);
	AddSession_Passage(1,passageID,mysoundfn);
	return 0;
}

       int Maker_DAVE_MURRAY_Movement(char *scorefn)//TrackMaker(scorefn,partname)�ɂ��ׂ�?
{char trackname[6][FILENAME_MAX];int NofPL;
	char test[1000];int ID;//�K�������y�߂ЂƂ܂Ƃ܂�Ƃ͌���Ȃ��̂� ID �Ƃ��Ă���//int now_passage=(-1);
	X **x;
	USTFILEINFO finf = {120,1};

FILE *tfp=NULL;char tfn[260];//����6�t�@�C���o�͂��ׂ�

	int linekind;char line[1000];SCORE_LINE *s;int *parts;int m=0,n,i,j;//�p�[�g�̍s��
	FILE *scorefp=NULL;
NOP = NumberOfParts(NULL,1);printf("Maker_%s() �N���� => ����[%d]�p�[�g\n",partname,NOP);
	sprintf(tfn,"%s.txt",partname);

	scorefp = fopen(scorefn,"r");while(fscanf(scorefp,"%d,%s",&linekind,line)>0){if(linekind==LINEKIND_BODY)m++;}fclose(scorefp);scorefp=NULL;
	x = (X**)calloc(m,sizeof(X*));//�s����X�̃|�C���^�e�[�u���m��
	for(i=0;i<m;i++){x[i] = (X*)calloc(NofS,sizeof(X));}//�S�s��6���쐬

	if(x==NULL){printf("[%d]�� X �������m�� => ���s by Maker_%s()\n",m,partname);exit(1);}else{printf("[%d]�� X �������m�ہ@=> ���� by Maker_%s()\n",m,partname);}
	s = (SCORE_LINE*)calloc(m,sizeof(SCORE_LINE));
	if(s==NULL){printf("[%d]�� SCORE_LINE �������m�� => ���s by Maker_%s()\n",m,partname);exit(1);}else{printf("[%d]�� SCORE_LINE �������m�ہ@=> ���� by Maker_%s()\n",m,partname);}

	scorefp = fopen(scorefn,"r");fscanf(scorefp,"%d,%s",&linekind,line);//�w�b�_�[�ǂݔ�΂�
	for(i=0;i<m;i++){fscanf(scorefp,"%d,%s",&linekind,line);score_line_sscanf(line,&s[i]);}//�ǂݍ���
	fclose(scorefp);scorefp=NULL;
//////////////////////////////////////////////////�X�R�A�t�@�C���ǂݍ��݊���

//////////////////////////////////��������̑���͂�������
///*
	for(i=0;i<m;i++)
	{
		if(s[i].passage_head==1)//���t���ăw�b�h�L���b�`
		{
			for(j=i,NofPL=1;j!=m-1 && s[j+1].passage_head==0;j++,NofPL++);
//printf("PASSAGE[%d]��[%d]�s��1�Z�b�g by Maker_%s()\n",s[i].passage_number,NofPL,playername);
			Maker_DAVE_MURRAY_Passage(s[i].passage_number,&s[i],&x[i],NofPL);
		}
	}
//*/
//////////////////////////////////��������̑���͂����܂�

//////////////////////////////////////////////////�ȉ��o��
	for(j=0;j<3;j++){sprintf(tfn,"%s_%d��.txt",playername,j+1);tfp = fopen(tfn,"w");for(i=0;i<m;i++){fprintf(tfp,"%d,%d\n",s[i].length,x[i][j].note);}fclose(tfp);}
	
	for(i=0;i<m;i++){free(x[i]);x[i] = NULL;}
	free(x);x=NULL;free(s);s=NULL;
	for(j=0;j<3;j++){sprintf(trackname[j],"%s_%d��",playername,j+1);ust_file_out(trackname[j],&finf);}
	return 0;
}
