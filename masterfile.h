#ifndef MASTERFILEH
#define MASTERFILEH
//�e�̃}�C�i���o�[,����,�t���[�Y,���g,�R�[�h,���g,�A�N�Z���g => f[6] �Ƃ���(�\���̂ɂ��Ă��̂܂�܂̏��ŏ������ނׂ�)
//�w�b�_�[����2�s(1�t�H�[�}�b�g,2����)
#define MASTERFORMATNUMBER 8
typedef struct
{
	int parent,length,chord_border_painted,chord_border_file
		,rythm_border_painted,rythm_border_file
		,accent_border_painted,accent_border_file
		,linenumber,passage,position;
	int NofBar,count,size;
	int barpropaty,joint;//���߂̓���>0�Ńv���p�e�B�[�t�@�C���i���o�[,JOINT>0�Ŋy�ߍŌ�̃}�X�^�[�t�@�C���Ȃ̂Œ����͎��R����Ŋy�ߊԂ����炩�ɂȂ�

	int barhead,file_bar;
	int file_chord,file_rhythm;
	int auftaktID;//�擪�̃A�E�t�^�N�g���L��(�q���Ɋ��������邩�͎��R)
	int Unit,CoreID;

	int *ChildID;
}MASTERFORMAT;

typedef struct{int c_head,c_file,have_more_children;}CHILDREN_SIDE;
//NofBar,count,size;//�ǉ�

#ifndef MASTERFILEC
extern int fill_children_side(int masterfilenumber,CHILDREN_SIDE *dest,int n);
extern FILE *masteropen(int masterfilenumber);
extern MASTERFORMAT master_fscanf(FILE *masterfp);
extern int master_fprintf(FILE *masterfp,MASTERFORMAT *source);
extern int writer_kaiki(int mynumber);//�}�X�^�[�t�@�C���̉�A�ďo�쐬
#endif
#endif