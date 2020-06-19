/*
#include <stdio.h>
#include <stdlib.h>

int q(char *lfn,int unit)
{
	//char value[1280];
int value;
	//char **x;
int *x;
	int *head;
	int i,j,lineN=0;
	int length,totallength,ko;
	FILE *qfp;FILE *lfp;
	totallength=0;
	
//printf("q(%s)\n",lfn);
	lfp = fopen(lfn,"r");
	if(lfp==NULL){printf("終了\n");return (-1);}
	while(fscanf(lfp,"%d,%d",&length,&value)>0){totallength += length;}
	fclose(lfp);
//printf("TOTAL = %d\n",totallength);
	
	if(totallength%unit!=0){printf("[%d]を[%d]で量子化出来ません\n",totallength,unit);return (-1);}
	else
	{
		ko = (totallength/unit);
//printf("[%d]を1個[%d]の[%d]個に量子化します\n",totallength,unit,ko);
		qfp = fopen("量子化.txt","w");
	
x = (int*)calloc(ko,sizeof(value));
		head = (int *)calloc(ko,sizeof(int));
		lfp = fopen(lfn,"r");
		
		for(i=0;i<ko;i++)
		{
			//printf("OFFSET = %d\n",i*unit);
			//この時点でオフセットを超える前の値を知りたい
while(fscanf(lfp,"%d,%d",&length,&value)>0)
//while(fscanf(lfp,"%d,%d",&value,&length)>0)
			{
				lineN++;//printf("%d行目を読み込みました\n",lineN);
				totallength += length;
				if(totallength>(i*unit)){break;}
			}
			//x[i] = length;
			x[i] = value;
			//x[i] = value;
			head[i] = lineN;
			lineN = 0;
			rewind(lfp);
			totallength = 0;
		}
		
		for(i=0;i<ko;i++){if(head[i]>0){for(j=i+1;j<ko;j++){if(head[j]==head[i]){head[j] = 0;}}}}
		for(i=0;i<ko;i++){if(head[i]>0)head[i] = 1;}
		for(i=0;i<ko;i++){if(head[i]==1){j = i+1;while(j<ko && head[j]==0){head[i]++;j++;}}}
for(i=0;i<ko;i++){fprintf(qfp,"%d,%d\n",head[i],x[i]);}fclose(lfp);
		free(x);
		free(head);
		fclose(qfp);
		return 0;
	}
}
*/
