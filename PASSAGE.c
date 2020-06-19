#define PASSAGE_C
#include "PASSAGE.h"
#include "MOVEMENT.h"
#include <stdlib.h>


static int passage_header_sscanf(PASSAGE_HEADER *dest,char *line){return (-1);}

static int PassageMaker(char *movementfn,int linenumber)
{
	int linekind,linenum,time,ID;char name[250];int next;
	char passagefn[FILENAME_MAX];
	FILE *passagefp;
	FILE *movementfp;

	movementfp = fopen(movementfn,"r");if(movementfp==NULL){printf("[%s]が見つかりません\n終了します\n",movementfn);exit(1);}

	fscanf(movementfp,"%s",name);
	while(fscanf(movementfp,"%d,%d,%d,%d,%s",&linekind,&linenum,&time,&ID,name)>0){if(linenum==linenumber){sprintf(passagefn,"新しいタイプの_[%s].txt",name);break;}}
	fclose(movementfp);

	if(time==1)//1st time
	{
		passagefp = fopen(passagefn,"w");
		fprintf(passagefp,"[%d,%d,%d,%s]から得られる情報をここに出力\n",linenum,time,ID,name);
		switch(ID)
		{
		case 1:case 2:case 3:fprintf(passagefp,"3,%d,<=(LINEKIND,続きうる楽節)\n",ID+1);break;
		case 4:fprintf(passagefp,"3,%d,<=(LINEKIND,続きうる楽節)\n3,%d,<=(LINEKIND,続きうる楽節)\n",5,6);break;
		case 5:fprintf(passagefp,"3,2,<=(LINEKIND,続きうる楽節)\n");
		case 6:fprintf(passagefp,"3,NoFuture!!,<=(LINEKIND,続きうる楽節)\n");break;}//,<=続きうる楽節
		fclose(passagefp);
		return 1;
	}else{return (-1);}
}

int PassagesMaker(char *movementfn)
{
	int linenumber;

	MOVEMENT_INFO x={120,0,9};
	for(linenumber=1;linenumber<=x.NofPassage;linenumber++){PassageMaker(movementfn,linenumber);}
	//printf("終了します by PassageMaker()\n");exit(1);
	return 0;
}
