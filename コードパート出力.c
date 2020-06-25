#include <stdio.h>
#include "PASSAGE.h"
#include "UST.h"

//�v�� chord_passage_compiler() ���Ė��O�̊֐���
//�x���g���o�͂���΂Ȃ�ł�������i�͑I�΂Ȃ�
//��œ������O�̖{�Ԃ̊֐����o������e�X�g�p�֐��͍폜����

int chord_passage_compiler(char *passagefn,char *outputfn)//�e�X�g�p�֐�
{int linekind,length,barh,ch,ah,rh,root,kind;
	char header[1280];
	FILE *mapfp;char mapfn[FILENAME_MAX];//�R�[�h���U�ς݂̃t�@�C��

	//����A�����ł��W���C���g�͈قȂ�\������
	//�����1�x��ɂ������y�߂ł��x���g����蒼���K�v������

	static int t = 0;
	//�o���b�g�x���g(bullet belt)�݂�����
	//�ЂƂ܂��y�߂̒���[length,root,kind]�t�@�C�����쐬

	FORMAT_PASSAGE p;FILE *passagefp;
	FILE *beltfp;char beltfn[FILENAME_MAX];
	
	passagefp = fopen(passagefn,"r");p = passage_fscanf(passagefp);fclose(passagefp);
	//��������� p ���g���Ȃ��̂Œ���!!
	
	sprintf(mapfn,"MAP_%d_NEW.txt",p.masterfile);

	//printf("[%s]�̏��� by chord_passage_compiler()\n",passagefn);

mapfp = fopen(mapfn,"r");
//printf("%d�s�ڂŒ�~\n",__LINE__);exit(1);
if(mapfp==NULL){printf("[%s]��������܂���M_[%d] by chord_passage_compiler(%s)\n",mapfn,p.masterfile,passagefn);
	printf("%d�s�ڂŒ�~\n",__LINE__);exit(1);return 0;}
	else{fscanf(mapfp,"%s",header);}//�w�b�_�[��񏜋�

//printf("%d�s�ڂŒ�~\n",__LINE__);exit(1);

	
	
	sprintf(beltfn,"�R�[�h[%d].txt",t++);

	beltfp = fopen(beltfn,"w");
	while(fscanf(mapfp,"%d,%d,%d,%d,%d,%d,%d,%d",&linekind,&length,&barh,&ch,&ah,&rh,&root,&kind) > 0)
	{
		//printf("%d,%d,%d,%d,%d,%d,%d,%d\n",linekind,length,barh,ch,ah,rh,root,kind);
		//printf("%d�s�ڂŒ�~\n",__LINE__);//exit(1);
		if(ch>0){fprintf(beltfp,"%d,%d,%d\n",ch*length,root,kind);}
	}
	fclose(beltfp);
fclose(mapfp);
//printf("%d�s�ڂŒ�~\n",__LINE__);exit(1);
	return 0;
}

int chord_passage_linker(char *movementfn)
{
	FILE *movementfp;
	static int t=0;char line[1280];

	FILE *pbeltfp;char pbeltfn[FILENAME_MAX];
	FILE *mbeltfp;char mbeltfn[FILENAME_MAX];
	
	char passagefn[FILENAME_MAX];//�ʂɊy�߃t�@�C�����J���킯�ł͂Ȃ�����X�̂���
	
	
	

	sprintf(mbeltfn,"1�ȕ��̃R�[�h�i�s.txt");
	mbeltfp = fopen(mbeltfn,"w");
	movementfp = fopen(movementfn,"r");
	while(fscanf(movementfp,"%s",passagefn)>0)
	{
		chord_passage_compiler(passagefn,NULL);//�����ō�����t�@�C����
		sprintf(pbeltfn,"�R�[�h[%d].txt",t++);
		pbeltfp = fopen(pbeltfn,"r");if(pbeltfp==NULL){printf("[%s]��������܂��� by chord_passage_linker()\n",pbeltfn);}
		while(fscanf(pbeltfp,"%s",line)>0){fprintf(mbeltfp,"%s\n",line);}//���t������
		fclose(pbeltfp);
		
	}
	fclose(movementfp);
	fclose(mbeltfp);

//////////////////////////////////////�ȏ�1�y��(1��)���̃x���g�t�@�C���쐬����!!
	
	ChordSplitter(mbeltfn);

	return 0;
}

int ChordSplitter(char *chordbeltfn)//�O�҂ݕ����֐�
{
	USTFILEINFO finf = {120,1};
	
	int length,root,kind;
	FILE *chordbeltfp;

	char *partname[3] = {"����","3��","5��"};
	char prefn[3][FILENAME_MAX];
	FILE *prefp[3];

	int i;

	//printf("�O�҂݂�[%s]�𕪉����܂�\n",chordbeltfn);//return 0;

	for(i=0;i<3;i++){sprintf(prefn[i],"%s.txt",partname[i]);}

	for(i=0;i<3;i++)
	{
		prefp[i] = fopen(prefn[i],"w");
		if(prefp[i]==NULL){printf("[%s]���쐬�o���܂���ł���\n",prefn[i]);}
		else{printf("[%s]���쐬���܂���\n",prefn[i]);}
	}
	for(i=0;i<3;i++)
	{
		chordbeltfp = fopen(chordbeltfn,"r");
		while(fscanf(chordbeltfp,"%d,%d,%d",&length,&root,&kind)>0)
		{
			fprintf(prefp[i],"%d,%d\n",length,testc(root,i));
		}
		fclose(chordbeltfp);
		
	}
	for(i=0;i<3;i++){fclose(prefp[i]);}
	
	for(i=0;i<3;i++){ust_file_out(partname[i],&finf);}
	
	return 0;
}


int testc(int root,int i)
{
	int x;
	x=root+2*i;
	
	switch(x%7)
	{
	case 0:x = 59;break;
	case 1:x = 60;break;//C
	case 2:x = 62;break;//D
	case 3:x = 64;break;//E
	case 4:x = 65;break;//F
	case 5:x = 67;break;//G
	case 6:x = 69;break;//A
	default:x = 0;break;
	}
	
	return x;
}
