//writer(void) : writer_kaiki()
//reader(void) : reader_kaiki()
//���ꂪ���z�`

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <Windows.h>

#include "MOVEMENT.h"

#include "masterfile.h"
#include "chord_border.h"
#include "accent_border.h"
#include "rythm_border.h"
#include "UST.h"
#include "PASSAGE.h"
#include "SCRAP.h"

//�}�X�^�[�t�@�C���̔ԍ����������ɂ���
//���l�����t�@�C���o�͂���֐��͍��ׂ�
//�}�X�^�[�t�@�C���̃|�C���^��Ԃ���

//const char *kari = "������.txt";
const char *kari = "GUITAR.txt";
int main()
{
	
	
	MOVEMENT_INFO movementinfo = {0};
	extern int bordersmaker(char*);
	extern int chord_nakami_maker(char*);
	//extern int movement_loader(char*);
	//extern int guitar_passage_linker(char*);
	//sleep(1);
	srand(time(0));
	
	MovementMaker(&movementinfo);
	//MasterMaker_Movement(1);
	writer();
	BordersMaker_Movement(NULL);
	//bordersmaker(NULL);
	
	chord_nakami_maker("�ȕҐ�.txt");//�R�C�c�����g������ăR�C�c���g���A�T�C�����Ă�(������cat_chord���Ă�)
	
	cat_chord(1);//movement_loader("�ȕҐ�.txt");//reader();
	chord_passage_linker("�ȕҐ�.txt");//�R�[�h�p�[�g�o�� X 3
	//guitar_passage_linker("�ȕҐ�.txt");
	ScoreMaker_Movement("�ȕҐ�.txt");
	
	//Maker_Oboe("�X�R�A.txt");
	//Maker_Ebase("�X�R�A.txt");
	//Maker_Eguitar("�X�R�A.txt");
	//Maker_Organ("�X�R�A.txt");
	
	//Maker_DAVE_MURRAY_Movement("�X�R�A.txt");
	//Maker_NICKO_MCBRAIN_Movement("�X�R�A.txt");
	//Maker_BRUCE_DICKINSON_Movement("�X�R�A.txt");
	Maker_JANICK_GERS_Movement(1);
	//ScaleTest();
	//�}�X�^�����O�֐�
	//TEST_N_Lyrics(17,0);
	//SetsunaMotion(57,-1,0);
	printf("�����I��\n");
	return 0;
}





int newmynumber()
{static int x = 0;x++;return x;}


//count_chord_border(masternumber); <= ���̊֐��͐���!!

//���~�����̂͂���ȓ_�Ńo���o���ȃ{�[�_�[�쐬�ł͂Ȃ�,
//���ꂼ�ꂪ���݂Ƀ`�F�b�N���Ȃ��烁������Ŏ��g���쐬����
//�t�@�C���o�̓{�[�_�[�쐬�֐�
//const BORDERS default = {-1,-1,-1,-1,-1,-1};
//�v���O�����̊J�n���Ɍ��󂗌��Ď��g��RETURN����܂łɂ��Ȃ���΂Ȃ�Ȃ�����
//TODO���X�g[mastarnumber].txt�ɏ����o���Č㔼�ł�������s
//���セ����폜


int map(int masterfile)//������̂Ƃ����-1
{
	//���̉�A�̐i�s�ɍ��킹�ă��A���^�C���ōX�V����K�v����
	//���̏�Ń}�X�^�[�t�@�C����ǂ񂾂Ƃ��Ă����̊֐������ɍX�V���Ă���\��������
	extern int borders_mapper(int,int,char *);
	char mapfn[FILENAME_MAX];
	sprintf(mapfn,"����_[%d].txt",masterfile);

	printf("[%s]���}�b�s���O�O\n",mapfn);

	//���͕s���S�Ȃ̂Ŏg���Ȃ��}�b�s���O�֐�
	//borders_mapper(masterfile,(1920/16),mapfn);
	
	printf("[%s]���}�b�s���O��\n",mapfn);
	
	return 0;
}

int writer()//�}�X�^�[�t�@�C���̍��
{FORMAT_PASSAGE p;char cmd[1280];FILE *pfp;char pfn[128];
	FILE *khfp;char *khfn = "�ȕҐ�.txt";
	
	khfp = fopen(khfn,"r");if(khfp==NULL){printf("[%s]��������܂��� by writer()\n",khfn);exit(1);}
	while(fscanf(khfp,"%s",pfn)>0)
	{
		pfp = fopen(pfn,"r");//�T�r�J��
		p = passage_fscanf(pfp);
		fclose(pfp);
		if(NewMasterMake(&p)>0)//�V�K�쐬�������Ă���
		{writer_kaiki(p.masterfile);}
	}fclose(khfp);
	//printf("��U�I�� by writer()\n");exit(1);
	return 0;
}





//�ȉ��ʃv���O�����ɂďo�͂��邱��
//�d�v�Ȋ֐�!!

//�}�X�^�[�t�@�C�����ǂ����߂��悤���ǎ҂̏���
//fp �� UST �����O�̉����t�@�C��(���������f�B)

//��ΓI����ɂ��}�X�^�[�t�@�C���̉���
//���y�͂����܂ł��̊��z��

int reader(){readtest();return 0;}

int reader_kaiki(FILE *fp,int masternumber)//�t�@�C���̖����Ƀ}�X�^�[�t�@�C���̒��g��t�������e�X�g�p��A�֐�
{
	//���̊��ɋy��Ń}�b�s���O�o���Ȃ��Ȃ�ĉR!!
	int kari,rl;
	extern FILE *rythm_border_open(int number);
	FILE *cbfp;
	int i,j;
	int child;//�}�X�^�[�t�@�C���̎q���̃t�@�C���i���o�[
	
	MASTERFORMAT m;
	FILE *masterfp;FILE *myfp;
	masterfp = masteropen(masternumber);
	m = master_fscanf(masterfp);
	fclose(masterfp);
	
	

	if(m.rythm_border_painted==0)/*���h���Ȃ炻�̎q���B���P��*/
	{
		for(i=0;i<m.linenumber;i++)
		{
			masterfp = masteropen(masternumber);
			m = master_fscanf(masterfp);

			for(j=0;j<i;j++){fscanf(masterfp,"%d",&child);}
			if(fscanf(masterfp,"%d",&child)>0){fclose(masterfp);reader_kaiki(fp,child);}
			else{fclose(masterfp);}
		}
	}
	
	else if(m.rythm_border_file>0 && m.rythm_border_painted>0)/*���̎��͕t�������Ȃ��Ă͂Ȃ�Ȃ�*/
	{
		cbfp = rythm_border_open(m.rythm_border_file);
//printf("�ȉ�RB�t�@�C��[%d]\n",m.rythm_border_file);
		while(1)
		{
			kari = 0;rl = 0;
			if(fscanf(cbfp,"%d,%d",&kari,&rl)<0){fclose(cbfp);break;}
//printf("RB = (%d,%d) by reader_kaiki()\n",kari,rl);
			//if(rl<=0){fclose(cbfp);break;}
if(rl>0){fprintf(fp,"%d,%d\n",kari,rl);}
		}
	}
	
	else if(m.rythm_border_painted>0)
	{printf("����ȏ�l����K�v�͖���(%d,%d)\n",m.rythm_border_painted,m.rythm_border_file);return 0;}


	return 0;
}

int readtest(){
	int masternumber;
	
	char cmd[1280];

	FILE *pfp;char pfn[128];
	FILE *khfp;char *khfn = "�ȕҐ�.txt";
	
	
	FILE *fp;//����ɕt�������Ă���
	fp = fopen(kari,"w");
//printf("WHILE�J�n\n");
khfp = fopen(khfn,"r");
if(khfp==NULL){printf("%s��������܂���\n",khfn);exit(1);}
//else{printf("%s���J���܂�\n",khfn);}
	while(fscanf(khfp,"%s",pfn)>0)
	{
		pfp = fopen(pfn,"r");if(pfp==NULL){printf("%s��������܂���\n",pfn);return (-1);}
		fscanf(pfp,"%d,%s",&masternumber,cmd);
		//printf("[%d]���L����Ă���\n",masternumber);
		fclose(pfp);

//border(masternumber);//exit(1);

		reader_kaiki(fp,masternumber);

		

	}
//printf("WHILE�I��\n");
	//reader_kaiki(fp,1);//�{���͕Ґ��t�@�C�����J����,�ȉ���
	fclose(fp);
	//testustout();
	return 0;
}

//const char *kari = "������.txt";
//const char *kari = "GUITAR.txt";

int testustout(){
	//extern int q(char *,int);
	extern int nameraka(int,int,int);
	char cmd[1200];
	int t=0;
	FILE *ngfp;
	char *ngfn = "���炩��.txt";
	FILE *nfp;
	char *nfn = "���炩.txt";
	int c = 0;
	
	FILE *txtfp;
	int i;
	USTFILEINFO x;
	USTNOTEINFO n;
	FILE *ustfp;
	
	x.Tempo = 120;

//���炩�e�X�g�J�n
	txtfp = fopen(kari,"r");
	while(fscanf(txtfp,"%d,%d",&n.note,&n.length)>0){t++;}
	fclose(txtfp);
//printf("%d�̉�������܂�\n",t);
nameraka(t,60,60);

	txtfp = fopen(kari,"r");nfp = fopen(nfn,"r");ngfp = fopen(ngfn,"w");
	while(fscanf(txtfp,"%d,%d",&n.note,&n.length)>0)
	{
		fscanf(nfp,"%d,%d",&t,&n.note);
		fprintf(ngfp,"%d,%d\n",n.note,n.length);
	}
	fclose(txtfp);fclose(nfp);fclose(ngfp);
//���炩�e�X�g�I��
txtfp = fopen(ngfn,"r");
	
	ustfp = ust_fopen("�ЂƂ܂�",&x);
	while(fscanf(txtfp,"%d,%d",&n.note,&n.length)>0)
	{
		//n.note += 60;
		n.counter = c;c++;
		sprintf(n.lylic,"��");
		ust_fprintf(ustfp,&n);
	}
	ust_fclose(ustfp);
	
fclose(txtfp);

//q(ngfn,(1920/4));


	
txtfp = fopen("�ʎq��.txt","r");
ustfp = ust_fopen("�ЂƂ܂�2",&x);c=0;
	while(fscanf(txtfp,"%d,%d",&n.note,&n.length)>0)
	{n.length = (1920/4);
		n.note += 60;
		n.counter = c;c++;
		sprintf(n.lylic,"��");
		ust_fprintf(ustfp,&n);
	}fclose(txtfp);
ust_fclose(ustfp);





	return 0;
}
//fscanf��1�����ł��X�L���������� 1 ,�ȊO�� -1 ��Ԃ��炵��
