#define MOVEMENT_C
#include "MOVEMENT.h"
#include <stdio.h>
#include <stdlib.h>

int MovementMaker(MOVEMENT_INFO *movementinfo)
{
	int i;
	char *table[] = {"1,1,前奏","1,2,Aメロ","1,3,Bメロ","1,4,サビ","1,5,間奏","2,2,Aメロ","2,3,Bメロ","2,4,サビ","1,6,後奏"};
	
	FILE *fp=NULL;char fn[FILENAME_MAX] = "デフォルト.txt";
	MOVEMENT_INFO x;//ここで採用するパラメータ
	
	extern int test_HandP();//後々押しつぶされるジャッジマン的テスト関数
	test_HandP();

	x = *movementinfo;
	//まずは引数で未指定のパラメータをここで埋める
	if(180<x.tempo || x.tempo<100)
	{
		//printf("異常なTEMPO = [%d] => 120 に再設定\n",x.tempo);
		x.tempo = 120;
	}x.key = 0;//今はハ長調で
	
	//printf("今は何もしていないけど MovementMaker() 起動\n");
	//printf("TEMPO = [%d] ってことで\n",x.tempo);
sprintf(fn,"MOVEMENT_1.txt");//これでもいける
	fp = fopen(fn,"w");
	fprintf(fp,"1,%d,%d,[行数,回数,ID(固有の値),文字列]\n",x.tempo,x.key);
	for(i=0;i<9;i++){fprintf(fp,"2,%d,%s\n",i+1,table[i]);}
	fclose(fp);
	//printf("終了します by MovementMaker()\n");exit(1);
	PassagesMaker(fn);


	return 0;
}
