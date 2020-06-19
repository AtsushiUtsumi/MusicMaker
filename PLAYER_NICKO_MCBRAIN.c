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

#define BassDrum_MIDI     29
#define Snare_MIDI        40

#define HighTom_MIDI      45

#define LowTom_MIDI       45

#define FloorTom_MIDI     45

#define CrashCymbal_MIDI  49
#define RideCymbal_MIDI   51
#define OpenHiHat_MIDI    46
#define ClosedHiHat_MIDI  42

static const char *playername = "NICKO_MCBRAIN";
static const char *partname = "PLAYER_NICKO_MCBRAIN";
static int NOP;//�N�����ł̑��̃p�[�g�����L�^���Ă���
static int NofS = 6;

//typedef struct{int length,note;}PART;//�Z�b�V�����Ŏg�p����
typedef struct{int wave,pre,note;}X;//�X�̃p�[�g�Ŏg�p
//typedef struct{int c_head,c_file,have_more_children;}CHILDREN_SIDE;


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

static FILE *DrumsPatternOpen(int MovementID,int PassageID,int MasterID)
{
	int i;int beat_head,Hit_K,Hit_L,Hit_R;
	int setting[] = {ClosedHiHat_MIDI,Snare_MIDI,OpenHiHat_MIDI,HighTom_MIDI,LowTom_MIDI,FloorTom_MIDI,RideCymbal_MIDI,CrashCymbal_MIDI};
	FILE *patternfp;
	char patternfn[260];
	int NofS;//NUmberOfSet
	NofS = sizeof(setting) / sizeof(int);//���j�b�g�v�Z
//printf("[%d]�_�Z�b�g by DrumsPatternOpen()\n",NofS);
	sprintf(patternfn,"%s_%d_%d_%d.txt",partname,MovementID,PassageID,MasterID);
	patternfp = fopen(patternfn,"r");
	if(patternfp==NULL)
	{
		patternfp = fopen(patternfn,"w");
		for(i=0;i<64;i++)
		{beat_head = (i%2==0) ? 1:0;
			switch(PassageID)
			{
			case 1:Hit_K = BassDrum_MIDI;Hit_L = (i%2==1) ? setting[0]:0;Hit_R = 0;break;
			case 2:Hit_K = BassDrum_MIDI;Hit_L = (i%4==0) ? setting[1]:0;Hit_R = 0;break;
			case 3:Hit_K = BassDrum_MIDI;Hit_L = (i%2==1) ? setting[2]:0;Hit_R = 0;break;
			case 4:Hit_K = BassDrum_MIDI;Hit_L = (i%4==0) ? setting[3]:0;Hit_R = 0;break;
			case 5:Hit_K = BassDrum_MIDI;Hit_L = (i%2==1) ? setting[4]:0;Hit_R = 0;break;
			case 6:Hit_K = BassDrum_MIDI;Hit_L = (i%4==0) ? setting[5]:0;Hit_R = 0;break;
			}
			fprintf(patternfp,"2,%d,%d,%d,%d\n",beat_head,Hit_K,Hit_L,Hit_R);
		}
		fclose(patternfp);
		patternfp = fopen(patternfn,"r");
	}
	return patternfp;
}

static int LR_Hit(int *dest_L,int *dest_R,int passageID)
{int NofS;
	int setting[] = {ClosedHiHat_MIDI,Snare_MIDI,OpenHiHat_MIDI,HighTom_MIDI,LowTom_MIDI,FloorTom_MIDI,RideCymbal_MIDI,CrashCymbal_MIDI};
	NofS = sizeof(setting)/sizeof(int);
	switch(passageID)
	{
	case 1:*dest_L = Snare_MIDI;break;
	case 2:*dest_L = LowTom_MIDI;break;
	case 3:*dest_L = RideCymbal_MIDI;break;
	case 4:*dest_L = OpenHiHat_MIDI;break;
	case 5:*dest_L = ClosedHiHat_MIDI;break;
	case 6:*dest_L = CrashCymbal_MIDI;break;
	}*dest_L = setting[rand()%NofS];
	return 0;
}

static int Maker_NICKO_MCBRAIN_Master(int MasterNumber,SCORE_LINE *s,X **x,int n,int PassageID,int NofS,int NofOP,PART **OP)
{
	CHILDREN_SIDE *cside;int i,j,k;
	
	int linekind,beathead;FILE *patternfp;patternfp = DrumsPatternOpen(1,PassageID,MasterNumber);

	cside = (CHILDREN_SIDE *)calloc(n,sizeof(CHILDREN_SIDE));
	fill_children_side(1,cside,n);

	//�����łЂƂ�����ɂ���
	for(i=0;i<n;i++)
	{
		if(cside[i].c_head>0){rewind(patternfp);}LABEL_REWIND:
		if(fscanf(patternfp,"%d,%d,%d,%d,%d",&linekind,&beathead,&x[i][0].note,&x[i][1].note,&x[i][2].note)<=0){rewind(patternfp);goto LABEL_REWIND;}
	}
	//for(i=0;i<n;i++){x[i][0].note = (1%2==0) ? BassDrum_MIDI:0;}//��
	for(i=0;i<n;i++){if((i<(n/2)&&i>(n*3/8)) || i>(n*7/8)){LR_Hit(&(x[i][1].note),&(x[i][2].note),PassageID);}}//��
	fclose(patternfp);
	//�����łЂƂ�����ɂ��Ȃ�
	//Maker_NICKO_MCBRAIN_Master(int MasterNumber,SCORE_LINE *s,X **x,int n,int PassageID,int NofS,int NofOP,PART **OP);//��A�Ăяo��
//for(i=0;i<n;i++){x[i][0].note = i;}
	//�����}�X�^�[�i���o�[�ɃR�s�[(��낵����΃A�����W���܂�)
///*
	for(i=0;i<n;i++)
	{
		if(cside[i].c_head>0)//1st �w�b�h�L���b�`
		{
			for(j=i;j<n;j++)
			{
				if(cside[j].c_head>0 && cside[j].c_file==cside[i].c_file)//2nd �w�b�h�L���b�`
				{
					for(k=0;j+k<n;k++)
					{
						if(cside[i+k].c_file==cside[j+k].c_file)
						{
							x[j+k][0].note = x[i+k][0].note;
							x[j+k][1].note = x[i+k][1].note;
							x[j+k][2].note = x[i+k][2].note;
							x[j+k][3].note = x[i+k][3].note;
							//*x[j+k] = *x[i+k];
							//printf("�R�s�[���܂� by %s()\n",playername);
						}else{break;}
					}
				}
			}
		}
	}
//*/
	//for(i=0;i<n;i++){printf("[%3d] => [%3d] by %s [%d,%d]\n",i,x[i][0].note,playername,cside[i].c_head,cside[i].c_file);}
	free(cside);
	return 0;
}

static int Maker_NICKO_MCBRAIN_Passage(int passageID,SCORE_LINE *s,X **x,int n)//�����уt�@�C���쐬���ׂ�(�����̂�)
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

Maker_NICKO_MCBRAIN_Master(p.masterfile,s,x,n,passageID,NofS,NofOP,OP);
	
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

       int Maker_NICKO_MCBRAIN_Movement(char *scorefn)//TrackMaker(scorefn,partname)�ɂ��ׂ�?
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
			Maker_NICKO_MCBRAIN_Passage(s[i].passage_number,&s[i],&x[i],NofPL);
		}
	}
//*/
//////////////////////////////////��������̑���͂����܂�

//////////////////////////////////////////////////�ȉ��o��
	for(j=0;j<2;j++){sprintf(tfn,"%s_%d��.txt",playername,j+1);tfp = fopen(tfn,"w");for(i=0;i<m;i++){fprintf(tfp,"%d,%d\n",s[i].length,x[i][j].note);}fclose(tfp);}
	
	for(i=0;i<m;i++){free(x[i]);x[i] = NULL;}
	free(x);x=NULL;free(s);s=NULL;
	for(j=0;j<2;j++){sprintf(trackname[j],"%s_%d��",playername,j+1);ust_file_out(trackname[j],&finf);}
	return 0;
}
