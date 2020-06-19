#include "music.h"
#include <stdlib.h>
int ChordIsOk(CHORD *a,CHORD *b)//a->bの進行がなら 1 、なら -1 を返す
{
	int i;
	int ad,bd;
	int ak,bk;
	ad = a->dia;
	bd = b->dia;
	ak = a->kind;
	bk = b->kind;

	//return 1;//今は何でもOK

	if(a->dia==b->dia && a->kind==b->kind)
	{
		//printf("同じコードは連続で使えません [%d] => [%d] by ChordIsOk()\n",a->dia,b->dia);
		return (-1);
	}
	if(ad==2 && bd!=5){return (-1);}
	if(ad==5 && (bd!=1&&bd!=4)){return (-1);}

	return 1;
}


int nchord(int num,CHORD *start,CHORD *end)//この関数重要!!
{
	int i;CHORD *list;FILE *out;char outname[] = "Nコード.txt";
	if(num==1)
	{
		if(start->dia==end->dia && start->kind==end->kind){out = fopen(outname,"w");fprintf(out,"%d,%d\n",start->dia,start->dia);fclose(out);return 0;}
		else{
			//printf("明らかに不可能なコード進行1個で[%d]=>[%d]\n",start->dia,end->dia);
			return (-1);}
	}
	if(num==2 && ChordIsOk(start,end)<0){printf("明らかに不可能なコード進行2個で [%d] => [%d] by nchord()\n",start->dia,end->dia);exit(1);return (-1);}
	list = (CHORD*)calloc(num,sizeof(CHORD));list[0] = *start;list[num-1] = *end;
FROM_A:
	for(i=1;i<num-1;i++)
	{
		list[i].dia = 1+rand()%7;
		if(list[i].dia==1||list[i].dia==4||list[i].dia==5){list[i].kind = CHORD_KIND_M;}
		else if(list[i].dia==7){list[i].kind = CHORD_KIND_dim;}
		else{list[i].kind = CHORD_KIND_m;}//今はメジャーでなければマイナー
		if(ChordIsOk(&list[i],&list[i+1])<0){continue;}
	}if(ChordIsOk(&list[num-2],&list[num-1])<0){goto FROM_A;}
	if(1){out = fopen(outname,"w");for(i=0;i<num;i++){fprintf(out,"%d,%d\n",list[i].dia,list[i].kind);}fclose(out);free(list);return 1;}
	else{printf("%d個の %d -> %d は出来ませんでした\n",num,start->dia,end->dia);printf("nchord()の試行回数エラーです\n");free(list);exit(1);return (-1);}
}
/*
int nchord(int num,CHORD *start,CHORD *end)//この関数重要!!
{
	int count = 0;
	
	int i;
	int go;
	CHORD *list;
	FILE *out;
	char outname[] = "Nコード.txt";

	out = fopen(outname,"w");
	list = (CHORD*)calloc(num,sizeof(CHORD));
	do
	{
		count++;
		if(count>1000)
		{
			printf("%d個の %d -> %d は出来ませんでした\n",num,start->dia,end->dia);
			printf("nchord()の試行回数エラーです\n");
			exit(1);
			goto LABEL;
		}
		go=1;
		for(i=0;i<num;i++)
		{
			list[i].dia = 1 + rand()%7;
			if(list[i].dia==1||list[i].dia==4||list[i].dia==5){list[i].kind = CHORD_KIND_M;}
			else if(list[i].dia==7){list[i].kind = CHORD_KIND_dim;}
			else{list[i].kind = CHORD_KIND_m;}//今はメジャーでなければマイナー
		}list[0] = *start;list[num-1] = *end;
		for(i=0;i<(num-1);i++){if(ChordIsOk(&list[i],&list[i+1]) < 0)go=0;}
	}while(go==0);

	for(i=0;i<num;i++)
		{
			if(list[i].dia==1 || list[i].dia==4 || list[i].dia==5){list[i].kind = CHORD_KIND_M;}
			else if(list[i].dia==7){list[i].kind = CHORD_KIND_dim;}
			else{list[i].kind = CHORD_KIND_m;}//今はメジャーでなければマイナー
		}

	for(i=0;i<num;i++){fprintf(out,"%d,%d\n",list[i].dia,list[i].kind);}
LABEL:
	fclose(out);
	free(list);
	return 0;
}
*/
