/*
#include <stdio.h>
#include "UST.h"

//�t�@�C�����̓i���o�[�̂�1�s�ڂ�1�ԍ��ɕK��,
//���g�����̃t�@�C���Ȃ̂����������̂Ƃ���
//2�s�ڂ�������͎Q�ƃt�@�C���ԍ�,�Ȃ���� -1

typedef struct
{
	int NofBeat;
	int count,size;//���̌���,1���̒���(count�����������m�ۂ���)
}BAR_HEADER;

static int id(){static int x = 0;return ++x;}

static int FileMaker_bar()
{
	BAR_HEADER x = {0};
	int i;FILE *bfp;char bfn[FILENAME_MAX];sprintf(bfn,"%d.txt",id());bfp = fopen(bfn,"w");fprintf(bfp,"4,BAR\n");

	x.NofBeat = 3;
	x.count = 6;
	x.size = (1920/x.count);
	
	for(i=0;i<x.count;i++){fprintf(bfp,"%d\n",i);}
	fclose(bfp);
	return 0;}

//3 = �y��?

static int FileMaker_passage()
{
	FILE *pfp;char pfn[FILENAME_MAX];sprintf(pfn,"%d.txt",id());pfp = fopen(pfn,"w");fprintf(pfp,"2,PASSAGE\n");fclose(pfp);
	return 0;}

static int FileMaker_movement()
{
	FILE *mfp;char mfn[FILENAME_MAX];mfp = fopen(mfn,"w");sprintf(mfn,"%d.txt",id());fprintf(mfp,"1,MOVEMENT\n");fclose(mfp);
	return 0;}


int FileReader(FILE *prefp,int filenumber)//�v���t�@�C���Ƀt�@�C�����瓾���鎖��t������
{
	return 0;
}

FILE *openid(int id){char fn[FILENAME_MAX];FILE *x=NULL;sprintf(fn,"%d.txt",id);if(x==NULL){printf("[%s]���J���܂���\n",fn);}else{return x;}}

static int ust_b_out(int id)
{
	FILE *ustfp;
	USTFILEINFO ustfi;
	USTNOTEINFO ustni;
	
	ustfp = ust_fopen("�e�X�g",&ustfi);
	ust_fprintf(ustfp,&ustni);
	ust_fclose(ustfp);
	return 0;
}

static int ust_p_out(int id)
{
	FILE *ustfp;
	USTFILEINFO ustfi;
	USTNOTEINFO ustni;
	
	ustfp = ust_fopen("�e�X�g",&ustfi);
	ust_fprintf(ustfp,&ustni);
	ust_fclose(ustfp);
	return 0;
}

static int ust_m_out(int id)
{
	FILE *ustfp;
	USTFILEINFO ustfi;
	USTNOTEINFO ustni;
	
	ustfp = ust_fopen("�e�X�g",&ustfi);
	ust_fprintf(ustfp,&ustni);
	ust_fclose(ustfp);
	return 0;
}


int main2()
{
	int i;
	printf("HELLO WORLD!!\n");
	for(i=0;i<4;i++){FileMaker_bar();}
	return 0;
}
*/
