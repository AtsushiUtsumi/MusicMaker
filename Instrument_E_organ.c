//�X�R�A��ǂ�Ŋe�p�[�g�̋��ʎ���
//�ǂ�ȃX�N���b�v�t�@�C������邩�͊e�p�[�g��p�̊֐��ɂ��C��
//�X�R�A�Ƀp�[�g�X�N���b�v�������t���邾���̃V�P���֐�
//�������Ȃ��炱�̊֐��̂������ŃA�E�t�^�N�g���ӎ������ɂ��ނ悤�ɂȂ�
//�e�p�[�g�X�R�A�e�p�[�g�ƃX�R�A��ʂ����Ƃ�,
//�p�[�g�Ԃł̉��̂Ȃ�����l���鎖���ł�,
//�`���̏��ߐ��̂悤�ɕs�m��v�f�ɂ��C�����s����
#define SCRAP_C
#include "SCRAP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SCORE.h"
#include "MUSIC.h"
#include "UST.h"
#define LINEKIND_BODY 2

static const char *partname = "Organ";static int NOP;//�N�����ł̑��̃p�[�g�����L�^���Ă���

typedef struct{int length,note;}PART;//�Z�b�V�����Ŏg�p����
typedef struct{int wave,pre,note;}X;//�X�̃p�[�g�Ŏg�p

static int NearWhite(int note,int whitefile,int pm)
{
	int x,y;x = note;y = (pm>0) ? 1:-1;
	while(x%12!=0 && x%12!=2 && x%12!=4 && x%12!=5 && x%12!=7 && x%12!=9 && x%12!=11)//�C�ɐH��Ȃ���
	{x+=y;}//printf("X = %d by NearWhite()\n",x);
	return x;
}
//���߂�"�g���b�N"�ɂ��Ƃ���

static FILE *OpenScrapFileW(int ID)//��̃X�N���b�v�t�@�C�������݂���Ƃ��̂� A���[�h�ŊJ��
{
	char line[1000];int E=1;
	FILE *fp;char fn[FILENAME_MAX];if(ID<0){printf("����ID = [%d] �͓n���܂��� by MakeNewScrapFile()\n",ID);exit(1);}sprintf(fn,"SCRAP_%s_%d.txt",partname,ID);
	fp = fopen(fn,"r");if(fp==NULL){exit(1);}if(fscanf(fp,"%s",line)>0){E=0;}fclose(fp);
	if(E==1){fp = fopen(fn,"a");return fp;}else{return NULL;}
}

static FILE *OpenScrapFileR(int ID)//�X�N���b�v�t�@�C�������݂���Ƃ��̂� R���[�h�ŊJ��
{
	FILE *fp;char fn[FILENAME_MAX];if(ID<0){printf("����ID = [%d] �͓n���܂��� by MakeNewScrapFile()\n",ID);exit(1);}sprintf(fn,"SCRAP_%s_%d.txt",partname,ID);
	fp = fopen(fn,"r");if(fp==NULL){exit(1);}return fp;
}

static int ScrapLoad(X *dest,int ID,int n)//dest �� SCRAP_ID.txt �� n�s���[�h����֐�
{
	//�{���Ȃ璆�r���[�ȃ��[�h��h�����߂ɂ܂Ȕ����ĂɈڂ��ׂ�
	X x;
	FILE *scrapfp;
	int i;
	scrapfp = OpenScrapFileR(ID);
	for(i=0;i<n;i++)
	{
		if(fscanf(scrapfp,"%d,%d,%d",&x.pre,&x.wave,&x.note)>0){dest[i] = x;}
		else{fclose(scrapfp);return -1;}
	}
	fclose(scrapfp);return 1;
}

static int MakeNewScrapFile(int ID)//�V�K�쐬�� ID ��Ԃ�
{
	FILE *fp;char fn[FILENAME_MAX];
	if(ID<0){printf("����ID = [%d] �͓n���܂��� by MakeNewScrapFile()\n",ID);exit(1);}
	sprintf(fn,"SCRAP_%s_%d.txt",partname,ID);fp = fopen(fn,"r");
	if(fp!=NULL){//printf("[%s]�͊��ɑ��݂��܂� by MakeNewScrapFile()\n",fn);
	fclose(fp);return (-1);}
	else{//printf("[%s]���쐬���܂� by MakeNewScrapFile()\n",fn);
	fp = fopen(fn,"w");fclose(fp);return ID;}
}

static int NumberOfParts(char *sessionfn,int movementID,int passageID)
{
	char line[1000];char*pointer;
	FILE *fp;char fn[260];int x;
	if(sessionfn==NULL){sprintf(fn,"SESSION_%d_%d.txt",movementID,passageID);}else{sprintf(fn,"%s",sessionfn);}
	fp = fopen(fn,"r");if(fp==NULL){printf("[%s]��������܂��� by NumberOfParts()\n",fn);return 0;}
	if(fscanf(fp,"%s",line)>0){for(x=1,pointer=line;pointer!=NULL;x++){pointer = strstr(pointer,",");if(pointer==NULL){break;}else{pointer++;}}}
	else{x = 0;}
	fclose(fp);printf("X = [%d] by NumberOfParts(%d,%d)\n",x,movementID,passageID);
	return x;
}

static int AddSession(int movementID,int passageID,char *addpartfn)
{
	char Line1[1000] = {0};
	char Line2[1000] = {0};
	FILE *addpartfp=NULL;
	FILE *tempfp=NULL;char *tempfn = "SESSION_TEMP.txt";
	FILE *sessionfp;
	char sessionfn[FILENAME_MAX];
	
	if(NOP!=NumberOfParts(NULL,movementID,passageID)){printf("�ŏ��ƈقȂ� NOP �̃t�@�C���ɂ͎Q���o���܂���\n");return 0;}
	
	sprintf(sessionfn,"SESSION_%d_%d.txt",movementID,passageID);
	addpartfp = fopen(addpartfn,"r");sessionfp = fopen(sessionfn,"r");
	if(sessionfp==NULL)
	{
		sessionfp = fopen(sessionfn,"w");
		while(fscanf(addpartfp,"%s",Line1)>0){fprintf(sessionfp,"%s\n",Line1);}
		fclose(sessionfp);printf("�܂��Ƒt�ł�\n");return 0;
	}//else{printf("�Z�b�V�����ł�\n");}
	tempfp = fopen(tempfn,"w");//�S�t�@�C���I�[�v��
	while(fscanf(sessionfp,"%s",Line1)>0){fscanf(addpartfp,"%s",Line2);fprintf(tempfp,"%s,%s\n",Line1,Line2);}
	fclose(tempfp);fclose(sessionfp);fclose(addpartfp);tempfp = NULL;sessionfp = NULL;addpartfp = NULL;//�S�t�@�C���N���[�Y
	remove(sessionfn);rename(tempfn,sessionfn);
	return 0;
}

static int LoadSession(PART **part,int NofPart,int movementID,int passageID)
{
	char *pointer;char sessionline[1000];
	int i,j,m=64,n;//m�͍s��,n�̓p�[�g��
	FILE *sessionfp;char sessionfn[FILENAME_MAX];

	//n = NumberOfParts(NULL,movementID,passageID);
	n = NOP;
	sprintf(sessionfn,"SESSION_%d_%d.txt",movementID,passageID);sessionfp = fopen(sessionfn,"r");
	if(sessionfp==NULL){/*printf("[%s]��������܂��� by LoadSession()\n",sessionfn);*/return -1;}else{if(n!=NofPart){printf("����[%d]�Ǝ��ۂ̃p�[�g��[%d]�������܂���\n",NofPart,n);exit(1);}}
	for(i=0;i<m;i++)
	{
		fscanf(sessionfp,"%s",sessionline);
		for(j=0,pointer=sessionline;j<n && pointer!=NULL;j++)
		{
			sscanf(pointer,"%d",&part[i][j].note);
			//printf("%2d�s_%d�p�[�g = [%d](%x,%s)\n",i,j,part[i][j].note,pointer,pointer);
			pointer = strstr(pointer,",");if(pointer==NULL){/*printf("���̍s�ɂ���ȏ�̃p�[�g�͂���܂���\n");*/break;}else{pointer++;}
			
		}
	}
	fclose(sessionfp);
	return 0;
}

static int cat_scrap(char *scorefn,char *scrapfn)//�o�͂�TEMP�t�@�C���ɕۑ����čŌ�Ɋm�F���Ă���
{
	//�s���Ɨv���ō����Ă��邩���m�F
	//�ꍇ�ɂ���Ă̓t�@�C�����C��
	//(�A�E�t�^�N�g�ɂ��o�����̕s�m��Ȃǂɂ��)FILE fp;fopen();fclose();
	FILE *scorefp=NULL;
	FILE *scrapfp=NULL;
	FILE *tempfp =NULL;
	char tempfn[FILENAME_MAX]={0};
	
	scorefp = fopen(scorefn,"r");//if(scorefp==NULL){printf("�X�R�A�t�@�C��[%s]��������܂���\n",scorefn);exit(1);}
	scrapfp = fopen(scrapfn,"r");//if(scrapfp==NULL){printf("�X�N���b�v�t�@�C��[%s]��������܂���\n",scrapfn);fclose(scorefp);exit(1);}
	
	fclose(scrapfp);
	fclose(scorefp);

	sprintf("[%s]��TEMP�o��.txt",scrapfn);
	tempfp = fopen(tempfn,"w");
	fclose(tempfp);

	//�����ł����X�R�A�t�@�C�����폜
	//TEMP�����l�[�����ĐV�X�R�A�t�@�C���Ƃ���

	return 0;
}

static int Maker_Organ_Passage(int passageID,SCORE_LINE *s,X *x,int n)//�����уt�@�C���쐬���ׂ�(�����̂�)
{
	FILE *mysoundfp;char mysoundfn[260];
	char *testline = "-1,-2,-3,-4,-5,-6,-7";int testint[8]={0};int outnote;
	
	char PartsScaner[1000]={0};
	PART **OP;//OtherParts�̗�(�|�C���^�e�[�u��)
	int NofOP;
	int i,j;CHORD nowchord;int ID;FILE *scrapfp;

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

LoadSession(OP,NofOP,1,passageID);
	//for(i=0;i<64;i++){printf("%s [%2d] => ",partname,i);for(j=0;j<NofOP;j++){printf(",%3d",OP[i][j].note);}printf(" by Maker_Organ_Passage()\n",);}

	for(i=0;i<NofOP;i++){free(OP[i]);OP[i]=NULL;}
	free(OP);OP=NULL;

	for(i=0;i<n;i++){x[i].wave = i;}
	for(i=0;i<n;i++)
	{x[i].pre = 40;}//���̎��_�ŉ���ɖ������Ȃ��悤��
	for(i=0;i<n;i++)
	{//if(s[i].rh>0)
		if(1)//�A�^�b�N
		{
			nowchord.dia = s[i].chord_root;nowchord.kind= s[i].chord_kind;x[i].note = NearChordTone(x[i].pre,0,1,&nowchord,5);}else{x[i].note = -1;}}ID = MakeNewScrapFile(s[i].passage_number);
			if(ID>0)
			{
				scrapfp = OpenScrapFileW(ID);
				for(i=0;s[i].passage_number==ID;i++){fprintf(scrapfp,"%d,%d,%d\n",x[i].pre,x[i].wave,x[i].note);}
				fclose(scrapfp);
			}else{ScrapLoad(&x[i],s[i].passage_number,n);}
			for(i=0,outnote=0;i<n;i++){if(x[i].note>0){outnote=x[i].note;}fprintf(mysoundfp,"%d\n",outnote);}
			fclose(mysoundfp);AddSession(1,passageID,mysoundfn);return 0;}

int Maker_Organ(char *scorefn)//TrackMaker(scorefn,partname)�ɂ��ׂ�?
{int NofPL;
	char test[1000];int ID;//�K�������y�߂ЂƂ܂Ƃ܂�Ƃ͌���Ȃ��̂� ID �Ƃ��Ă���//int now_passage=(-1);
	X *x;USTFILEINFO finf = {120,1};FILE *tfp=NULL;char tfn[260];
	int linekind;char line[1000];SCORE_LINE *s;int *parts;int m=0,n,i,j;//�p�[�g�̍s��
	FILE *scorefp=NULL;
NOP = NumberOfParts(NULL,1,1);printf("Maker_%s() �N���� => ����[%d]�p�[�g\n",partname,NOP);
	sprintf(tfn,"%s.txt",partname);

	scorefp = fopen(scorefn,"r");while(fscanf(scorefp,"%d,%s",&linekind,line)>0){if(linekind==LINEKIND_BODY)m++;}fclose(scorefp);scorefp=NULL;
	x = (X*)calloc(m,sizeof(X));
	if(x==NULL){printf("[%d]�� X �������m�� => ���s by Maker_%s()\n",m,partname);exit(1);}
	else{printf("[%d]�� X �������m�ہ@=> ���� by Maker_%s()\n",m,partname);}
	s = (SCORE_LINE*)calloc(m,sizeof(SCORE_LINE));
	if(s==NULL){printf("[%d]�� SCORE_LINE �������m�� => ���s by Maker_%s()\n",m,partname);exit(1);}
	else{printf("[%d]�� SCORE_LINE �������m�ہ@=> ���� by Maker_%s()\n",m,partname);};

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
printf("PASSAGE[%d]��[%d]�s��1�Z�b�g by Maker_%s()\n",s[i].passage_number,NofPL,partname);
			Maker_Organ_Passage(s[i].passage_number,&s[i],&x[i],NofPL);
		}
	}
//*/
//////////////////////////////////��������̑���͂����܂�

//////////////////////////////////////////////////�ȉ��o��
	tfp = fopen(tfn,"w");for(i=0;i<m;i++){fprintf(tfp,"%d,%d\n",s[i].length,x[i].note);}fclose(tfp);
	free(x);x=NULL;free(s);s=NULL;
	ust_file_out(partname,&finf);
	return 0;
}
