#define PARTS_COMMON_C
#include "PARTS_COMMON.h"
#include "UST.h"

int DIAtoMIDI(int dia)
{
	switch(dia)
	{
	case 1:return 0;
	case 2:return 2;
	case 3:return 4;
	case 4:return 5;
	case 5:return 7;
	case 6:return 9;
	case 7:return 11;
	default:printf("無効なDIA[%d]終了します\n");exit(1);
	}
}

char *TestScaleFn(int root)
{
	FILE *ScaleFp;int midi;
	static char ScaleFn[260];
	
	switch(root)
	{
	case 0:case 2:case 4:case 5:case 7:case 9:case 11:sprintf(ScaleFn,"[%d]スケール.txt",root);break;
	default:printf("あり得ない ROOT[%d] 終了します",root);exit(1);
	}
	ScaleFp = fopen(ScaleFn,"r");
	if(ScaleFp==NULL)
	{
		ScaleFp = fopen(ScaleFn,"w");
		for(midi=0;midi<12;midi++)
		{
			switch((midi+root)%12){case 0:case 2:case 4:case 5:case 7:case 9:case 11:fprintf(ScaleFp,"%d\n",(midi+root)%12);break;default:break;}
			
		}
	}
	fclose(ScaleFp);return ScaleFn;
}

int IsScaleNote(int note,int root,char *ScaleFn)
{int midi;
	FILE *ScaleFp;
	int ScaleNote;
ScaleFp = fopen(ScaleFn,"r");
	
	if(ScaleFp==NULL)//なければ新規作成
	{
		
		ScaleFp = fopen(ScaleFn,"w");
		//printf(ScaleFp,"0\n1\n5\n7\n8\n");//都節音階
		fprintf(ScaleFp,"0\n1\n4\n5\n7\n8\n11\n");//中近東音階
		/*for(midi=0;midi<12;midi++)
		{
			switch((midi+root)%12)
			{
			case 0:case 2:case 4:case 5:case 7:case 9:case 11:fprintf(ScaleFp,"%d\n",(midi+root)%12);break;
			default:break;
			}
		*/

		fclose(ScaleFp);
		ScaleFp = fopen(ScaleFn,"r");
	}
	
	while(fscanf(ScaleFp,"%d",&ScaleNote)>0)
	{
		if((ScaleNote+root)%12 == note%12){fclose(ScaleFp);return 1;}
	}
	fclose(ScaleFp);
	return -1;
}

//この音を基準に上下にルート D のスケールモーションを許可する
int ScaleNote(int base,int updown,int root,char *ScaleFn)//root<0ならスケールファイルは絶対音
{
	int x;int pm;
	pm = (updown>0)?1:-1;for(x=base;IsScaleNote(x,root,ScaleFn)<0;x+=pm);
	return x;
}

int ScaleTest()
{
#define NUMBER (32*8)
	int jyunnji = -1;int key=0;
	USTFILEINFO finf = {120,1};
	char TestFn[260];FILE *TestFp;int note[NUMBER];
	char *partname = "MIYAKO";int i;
	
	jyunnji = (rand()%2==0)?1:-1;
	for(note[0]=60,i=1;i<NUMBER;i++)
	{
		if(i%16==0){key++;}
		if(rand()%3==0){jyunnji *=-1;}
		note[i] = ScaleNote(note[i-1]+jyunnji,jyunnji,key,"TESTSCALE.txt");
	}

	for(note[0]=60,i=1;i<NUMBER;i++)
	{jyunnji = (rand()%2==0)?1:-1;
		note[i] = SetsunaMotion(note[i-1],jyunnji,0);
	}

	sprintf(TestFn,"%s.txt",partname);TestFp = fopen(TestFn,"w");
	for(i=0;i<NUMBER;i++){fprintf(TestFp,"%d,%d\n",240,note[i]);}//*(1+rand()%7)
	fclose(TestFp);

	ust_file_out(partname,&finf);
	system("MIYAKO.mid");

	return 0;
}




















static int HaveNote(int note,int CanUseNotesID)//この音は使える音に入っているのか
{
	char line[1000];
	int have =-1;
	int CanUseNote;//MIDI(0to11)
	FILE *CanUseNotesFp;
	char CanUseNotesFn[260];sprintf(CanUseNotesFn,"音名.txt");
	CanUseNotesFp = fopen(CanUseNotesFn,"r");if(CanUseNotesFp==NULL){printf("[%s]が見つかりません => 終了します by HaveNote()\n",CanUseNotesFn);exit(1);}
	while(fscanf(CanUseNotesFp,"%d,%s",&CanUseNote,line)>0)
	{
		if( (CanUseNote%12)==(note%12) ){have = 1;break;}
	}
	fclose(CanUseNotesFp);
	if(have>0){printf("NOTE[%d]持ってますby HaveNote()\n",note);}
	else{printf("NOTE[%d]持ってませんby HaveNote()\n",note);}
	return have;
}

static int testONN()
{
	FILE *CanUseNotesFp;char CanUseNotesFn[260];sprintf(CanUseNotesFn,"音名.txt");
	CanUseNotesFp = fopen(CanUseNotesFn,"w");fprintf(CanUseNotesFp,"0\n2\n4\n5\n7\n9\n11\n");
	fclose(CanUseNotesFp);return 0;
}

//めちゃ便利な関数
int SetsunaMotion(int base,int updown,int CanUseNotesID)//モーションというからには base から絶対変わる
{
	
	int i,pm,x;testONN();
	
	pm = (updown>0) ? 1:-1;
	for(x=base,i=0;i<12;i++)
	{
		x += pm;if(HaveNote(x,0)>0){printf("[%d]が使える by SetsunaMotion()\n",x);return x;}
	}
	printf("使える音が見つかりませんでした => 終了します by SetsunaMotion()\n");
	exit(1);
}
