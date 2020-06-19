#include <stdio.h>
#include <stdlib.h>
#include "PASSAGE.h"
#include "chord_border.h"
#include "masterfile.h"

static int HaveKouho(int masterID)
{
	MASTERFORMAT m;int i;int childID;
	FILE *childkouhofp;char childkouhofn[260];
	FILE *masterfp;char masterfn[260];

	masterfp = masteropen(masterID);m = master_fscanf(masterfp);
	if(m.linenumber<=0)
	{
		//printf("�������� M_[%d] �Ɏq���͂��܂��� by HaveKouho()\n",masterID);
		fclose(masterfp);
		return (-1);
	}
	for(i=0;i<m.linenumber;i++)
	{
		if(fscanf(masterfp,"%d",&childID)<=0){printf("������������ by HaveKouho()\n");exit(1);}
		sprintf(childkouhofn,"�R�[�h���_[%d].txt",childID);
		childkouhofp = fopen(childkouhofn,"r");
		if(childkouhofp==NULL)
		{
			//printf("[%d]�̎q���B���S�����������Ă���킯�ł͂Ȃ�����\n",masterID);
			fclose(masterfp);
			return (-1);
		}
		else{fclose(childkouhofp);}
	}
	fclose(masterfp);//printf("[%d]�̎q���B�����������Ă�!! by HaveKouho()\n",masterID);
	return 1;
}

static int ChordPickUp(int master,CHORD *chordlist,int n)//�����Ō��̏E���ă��X�g�𖄂߂邾���̊֐�
{
	int i,j,k,x,ok=0,c;MASTERFORMAT m;FILE *masterfp;
	FILE *chordfp;
	char chordfn[FILENAME_MAX];

	ok = HaveKouho(master);//if(ok>0){printf("�ʂɎq���B�ɔC���Ă����� by ChordPickUp(%d)\n\n",master);}else{printf("�q���B�Ɍ�₪�Ȃ� by ChordPickUp(%d)\n\n",master);}
	
	if(ok>0)
	{
		masterfp = masteropen(master);
		m = master_fscanf(masterfp);
		for(k=0,j=0;j<m.linenumber && k<n;j++)
		{
			fscanf(masterfp,"%d",&c);
			x = ChordPickUp(c,&chordlist[k],count_chord_border(c));//printf("PICKUP = [%d]\n",x);
			k += x;
		}
		fclose(masterfp);
	}else{
	sprintf(chordfn,"�R�[�h���_[%d].txt",master);
	chordfp = fopen(chordfn,"r");
	if(chordfp==NULL){printf("[%s]��������܂���\n�I�����܂� by ChordPickUp()\n",chordfn);exit(1);}
	//else{printf("OPEN => [%s] by ChordPickUp()\n",chordfn);}
	for(i=0;i<n;i++){fscanf(chordfp,"%d,%d",&chordlist[i].dia,&chordlist[i].kind);}
	fclose(chordfp);
	}
//for(i=0;i<n;i++){printf("%d,%d by ChordPickUp(%d)\n",chordlist[i].dia,chordlist[i].kind,master);}
	return n;
}

static int chord_passage_maker(char *passagefn)//�y�߃t�@�C���P��
{
	CHORD *chordlist;
	
	FILE *outfp=NULL;
	char outfn[FILENAME_MAX];
	
	CHORD mystart,myend;int x,i,number;FILE *pfp;char pfn[FILENAME_MAX];
	FORMAT_PASSAGE p;
	sprintf(pfn,"%s",passagefn);
	pfp = fopen(pfn,"r");
	if(pfp==NULL){printf("�y��[%s]��������܂��� by borders_passage_maker()\n",pfn);exit(1);}
	p = passage_fscanf(pfp);mystart = p.start;myend = p.end;fclose(pfp);x = count_chord_border(p.masterfile);

///////////////////////////////////////////////////////���͈ȉ��� N�R�[�h_[%d].txt �쐬�R�[�h�����肩���邱�ƂɏW������
	sprintf(outfn,"N�R�[�h_[%d].txt",p.masterfile);chordlist = (CHORD*)calloc(x,sizeof(CHORD));ChordPickUp(p.masterfile,chordlist,x);
	
	outfp = fopen(outfn,"w");
	for(i=0;i<x;i++)
	{
		fprintf(outfp,"%d,%d\n",chordlist[i],chordlist[i]);
		//printf("[%d,%d]",chordlist[i],chordlist[i]);
		//printf(" = [%d,%d]\n",chordlist[i].dia,chordlist[i].kind);
	}fclose(outfp);free(chordlist);
///////////////////////////////////////////////////////
//printf("�s�R�Ȓl���`�F�b�N => �I�����܂� by chord_passage_maker()\n");exit(1);
	cat_chord(p.masterfile);/*���͍�藧�ẴR�[�h�𑦑��U*/
	return 0;
}

int chord_nakami_maker(char *kyokuhennseifn)//�ȒP�ʂ�
{
	//char testfn[120];
	//FORMAT_PASSAGE passage_fscanf(FILE *fp);
	char cmd[1280];FILE *khfp;
	char khfn[FILENAME_MAX];char pfn[FILENAME_MAX];
	if(kyokuhennseifn==NULL){sprintf(khfn,"�ȕҐ�.txt");}else{sprintf(khfn,"%s",kyokuhennseifn);}

ChordMaker_Movement("MOVEMENT_1.txt");//���͌Œ�

printf("chord_nakami_maker(%s) �J�n\n",khfn);
	khfp = fopen(khfn,"r");if(khfp==NULL){printf("[%s]��������܂��� by bordersmaker()\n",khfn);exit(1);}
	while(fscanf(khfp,"%s",pfn)>0)
	{
		//sprintf(testfn,"[%s]�̃R�[�h�i�s�o��.txt",pfn);//ChordMaker(pfn,testfn);
		chord_passage_maker(pfn);
	}fclose(khfp);
printf("chord_nakami_maker() �I��\n");
//printf("�I�����܂� by chord_nakami_maker()\n");exit(1);
	return 0;
}