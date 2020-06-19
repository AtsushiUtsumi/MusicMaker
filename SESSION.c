#define SESSION_C
#include "SESSION.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int NumberOfParts(char *sessionfn,int movementID);
int AddSession_Passage(int movementID,int passageID,char *addpartfn);
int LoadSession_Passage(PART **part,int NofPart,int movementID,int passageID);

int NumberOfParts(char *sessionfn,int movementID)
{int passageID = 1;
	char line[1000];char*pointer;
	FILE *fp;char fn[260];int x;
	if(sessionfn==NULL){sprintf(fn,"SESSION_%d_%d.txt",movementID,passageID);}else{sprintf(fn,"%s",sessionfn);}
	fp = fopen(fn,"r");if(fp==NULL){printf("[%s]が見つかりません by NumberOfParts()\n",fn);return 0;}
	if(fscanf(fp,"%s",line)>0){for(x=1,pointer=line;pointer!=NULL;x++){pointer = strstr(pointer,",");if(pointer==NULL){break;}else{pointer++;}}}
	else{x = 0;}
	fclose(fp);printf("X = [%d] by NumberOfParts(%d,%d)\n",x,movementID,passageID);
	return x;
}

int AddSession_Passage(int movementID,int passageID,char *addpartfn)
{
	char Line1[1000] = {0};
	char Line2[1000] = {0};
	FILE *addpartfp=NULL;
	FILE *tempfp=NULL;char *tempfn = "SESSION_TEMP.txt";
	FILE *sessionfp;
	char sessionfn[FILENAME_MAX];
	
	//if(NOP!=NumberOfParts(NULL,movementID)){printf("最初と異なる NOP のファイルには参加出来ません\n");return 0;}
	
	sprintf(sessionfn,"SESSION_%d_%d.txt",movementID,passageID);
	addpartfp = fopen(addpartfn,"r");sessionfp = fopen(sessionfn,"r");
	if(sessionfp==NULL)
	{
		sessionfp = fopen(sessionfn,"w");
		while(fscanf(addpartfp,"%s",Line1)>0){fprintf(sessionfp,"%s\n",Line1);}
		fclose(sessionfp);
		//printf("まだ独奏です by AddSession_Passage()\n");
		return 0;
	}//else{printf("セッションです\n");}
	tempfp = fopen(tempfn,"w");//全ファイルオープン
	while(fscanf(sessionfp,"%s",Line1)>0){fscanf(addpartfp,"%s",Line2);fprintf(tempfp,"%s,%s\n",Line1,Line2);}
	fclose(tempfp);fclose(sessionfp);fclose(addpartfp);tempfp = NULL;sessionfp = NULL;addpartfp = NULL;//全ファイルクローズ
	remove(sessionfn);rename(tempfn,sessionfn);
	return 0;
}

int LoadSession_Passage(PART **part,int NofPart,int movementID,int passageID)
{
	char *pointer;char sessionline[1000];
	int i,j,m=64,n;//mは行数,nはパート数
	FILE *sessionfp;char sessionfn[FILENAME_MAX];

	//n = NumberOfParts(NULL,movementID,passageID);
	//n = NOP;
	n = NofPart;
	sprintf(sessionfn,"SESSION_%d_%d.txt",movementID,passageID);sessionfp = fopen(sessionfn,"r");
	if(sessionfp==NULL){/*printf("[%s]が見つかりません by LoadSession()\n",sessionfn);*/return -1;}else{if(n!=NofPart){printf("引数[%d]と実際のパート数[%d]が合いません\n",NofPart,n);exit(1);}}
	for(i=0;i<m;i++)
	{
		fscanf(sessionfp,"%s",sessionline);
		for(j=0,pointer=sessionline;j<n && pointer!=NULL;j++)
		{
			sscanf(pointer,"%d",&part[i][j].note);
			//printf("%2d行_%dパート = [%d](%x,%s)\n",i,j,part[i][j].note,pointer,pointer);
			pointer = strstr(pointer,",");if(pointer==NULL){/*printf("この行にこれ以上のパートはありません\n");*/break;}else{pointer++;}
			
		}
	}
	fclose(sessionfp);
	return 0;
}