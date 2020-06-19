/*
#include <stdio.h>
#include "PASSAGE.h"
#include "chord_border.h"
#include "masterfile.h"

//�ǂ��ɂǂ�����Ē��g�𑕓U���邩??
//���܂ł̃}�X�^�[�t�@�C�����U�����Ă��Ƃɂ���
//MASTER_LOADED_[X].txt�������Ă��Ƃ�

typedef struct{int masterN,parentN,lineN;}LOADED_HEADER;

typedef struct{int ch,ah,rh;CHORD chord;int size;}LOADED_LINE;

static int loaded_header_fprintf(FILE *loadedfp,LOADED_HEADER *loaded_header)
{
	LOADED_HEADER x;
	x = *loaded_header;
	fprintf(loadedfp,"�w�b�_�[���o�� = [%d,%d]\n",x.masterN,x.lineN);
	return 0;
}

static int loaded_line_fprintf(FILE *loadedfp,LOADED_LINE *loaded_line)
{
	LOADED_LINE x;
	x = *loaded_line;
	fprintf(loadedfp,"1�s�o�� = [%d,%d,%d,%d %d]\n",x.ch,x.ah,x.rh,x.chord.dia,x.chord.kind);
	return 0;
}

int n2lfn(char *dest,int masternumber){sprintf(dest,"MASTER_LOADED_[%d].txt",masternumber);return 0;}

static int master_loader(int masterfilenumber)//�}�X�^�[�t�@�C���t�@�C���P��(��A�֐�)
{
	//�������������Ń��[�h���邩�͂ǂ�����Č��߂�?
	int i;
	LOADED_HEADER lh = {0};
	LOADED_LINE ll = {0};
	
	MASTERFORMAT minf;
	FILE *lfp=NULL;
	char lfn[FILENAME_MAX];

	FILE *mfp=NULL;
	char mfn[FILENAME_MAX];

	FILE *chordfp=NULL;
	char chordfn[FILENAME_MAX];
	
	n2lfn(lfn,masterfilenumber);


mfp = masteropen(masterfilenumber);minf = master_fscanf(mfp);minf.count;minf.size;

	sprintf(chordfn,"N�R�[�h_[%d].txt",masterfilenumber);chordfp = fopen(chordfn,"r");
	if(chordfp==NULL){printf("[%s]��������܂��� by master_loader()\n",chordfn);fclose(mfp);fclose(lfp);exit(1);}
	else{printf("�R�[�h[%s]�𑕓U���܂� by master_loader()\n",chordfn);}
	fclose(chordfp);

	
	lfp = fopen(lfn,"w");
	loaded_header_fprintf(lfp,&lh);
	for(i=0;i<minf.count;i++){ll.ch = i+1;//�e�X�g
	loaded_line_fprintf(lfp,&ll);}
	fclose(lfp);


fclose(mfp);return 0;}

static int passage_loader(char *passagefn)//�y�߃t�@�C���P��
{
	
	FILE *pfp;char pfn[FILENAME_MAX];FORMAT_PASSAGE p;
	
	sprintf(pfn,"%s",passagefn);
	pfp = fopen(pfn,"r");if(pfp==NULL){printf("�y��[%s]��������܂���\n",pfn);exit(1);}
	else{printf("�y��[%s]�ɒ��g�𑕓U���܂� by passage_loaderer()\n",pfn);
	p = passage_fscanf(pfp);fclose(pfp);pfp=NULL;}
	
	
	
	master_loader(p.masterfile);

	

	return 0;
}

int movement_loader(char *kyokuhennseifn)//�ȒP�ʂ�
{
	char cmd[1280];FILE *khfp;char khfn[FILENAME_MAX];char pfn[FILENAME_MAX];
	if(kyokuhennseifn==NULL){sprintf(khfn,"�ȕҐ�.txt");}else{sprintf(khfn,"%s",kyokuhennseifn);}
	printf("\nLOADER �J�n [%s]\n",khfn);khfp = fopen(khfn,"r");if(khfp==NULL){printf("[%s]��������܂��� by movement_loader()\n",khfn);exit(1);}
	while(fscanf(khfp,"%s",pfn)>0){passage_loader(pfn);}fclose(khfp);
	printf("LOADER �I��\n\n");
	return 0;
}
*/
