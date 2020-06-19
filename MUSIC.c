#define MUSIC_C
#include "MUSIC.h"
#include <stdlib.h>

int IsChordTone(int ust,CHORD *chord)//ノンコードトーンで(-1)を返す関数
{//dia=7は問題
	//この関数はコードトーンで根音からの差を返す
	//つまり同音なら 0 も返すわけで,
	//どちらかといえばノンコードトーンで(-1)を返すという認識で

	int interval;//今は適当に3度を返しておく
	int myroot;//0-11で引数のコードの根音
	switch(chord->dia)
	{
	case 1:case 4:case 5:
		if(chord->kind!=CHORD_KIND_M){
			//printf("今の時点で[%d,%d]が回ってくるなんてあり得ないんだけどね by IsChordTone()\n",chord->dia,chord->kind);
		}break;}
	switch(chord->dia)
	{
	case 2:case 3:case 6:
		if(chord->kind!=CHORD_KIND_m){
			//printf("今の時点で[%d,%d]が回ってくるなんてあり得ないんだけどね by IsChordTone()\n",chord->dia,chord->kind);
		}break;}
	switch(chord->dia)
	{
	case 7:
		if(chord->kind!=CHORD_KIND_dim){
			//printf("今の時点で[%d,%d]が回ってくるなんてあり得ないんだけどね by IsChordTone()\n",chord->dia,chord->kind);
		}break;}
	if(chord->dia >0)
	{
		//printf("IsChordTone(%d,%d)\n",ust,chord->dia);
		switch(chord->dia)
		{
		case 1:myroot =  0;break;
		case 2:myroot =  2;break;
		case 3:myroot =  4;break;
		case 4:myroot =  5;break;
		case 5:myroot =  7;break;
		case 6:myroot =  9;break;
		case 7:myroot = 11;break;
		default:printf("異常なDIA = [%d]\n終了します\nby IsChordTone()\n",chord->dia);exit(1);
			myroot= -1;break;
		}//1-7モードなので0-11モードに切り替え
	}
	//else{0-11モード;myroot = chord->root;}
	
	/*
	printf("\nUST[%d]は",ust);
	switch(ust%12)//この表憶えとくと便利
	{
	case 0:printf("[C]\n");break;
	case 1:printf("[Des]\n");break;
	case 2:printf("[D]\n");break;
	case 3:printf("[Es]\n");break;
	case 4:printf("[E]\n");break;
	case 5:printf("[F]\n");break;
	case 6:printf("[Fis]\n");break;
	case 7:printf("[G]\n");break;
	case 8:printf("[As]\n");break;
	case 9:printf("[A]\n");break;
	case 10:printf("[B]\n");break;
	case 11:printf("[H]\n");break;
	default:printf("異常なUST = [%d]\n終了します\nby IsChordTone()\n",ust);exit(1);
	}
	*/
	interval = ((12+ust%12)-myroot) % 12;//myroot=11は問題
	
	//printf("U,R = [%d,%d]\n",ust,myroot);
	/*
	printf("INTERVAL = [%d] つまり => ",interval);
	switch(interval)
	{
	case 0:printf("完全1度\n");break;
	case 1:printf("短2度\n");break;
	case 2:printf("長2度\n");break;
	case 3:printf("短3度\n");break;
	case 4:printf("長3度\n");break;
	case 5:printf("完全4度\n");break;
	case 6:printf("増4度\n");break;
	case 7:printf("完全5度\n");break;
	case 8:printf("短6度\n");break;
	case 9:printf("長6度\n");break;
	case 10:printf("短7度\n");break;
	case 11:printf("長7度\n");break;
	default:printf("意味不明な INTERVAL = [%d]\n反吐が出るぜ!!\n",interval);exit(1);
		break;
	}
	*/

	//if(chord->dia==7)printf("interval = [%d]\n",interval);

	switch(chord->kind)
	{
	case CHORD_KIND_M:if(interval==0 || interval==4 || interval==7){return interval;}else{break;}
	case CHORD_KIND_m:if(interval==0 || interval==3 || interval==7){return interval;}else{break;}
	case CHORD_KIND_aug:if(interval==0 || interval==4 || interval==8){return interval;}else{break;}
	case CHORD_KIND_dim:if(interval==0 || interval==3 || interval==6){return interval;}else{break;}
	case CHORD_KIND_6:if(interval==0 || interval==4 || interval==7 || interval==9){return interval;}else{break;}
	case CHORD_KIND_m6:if(interval==0 || interval==3 || interval==7 || interval==9){return interval;}else{break;}
	case CHORD_KIND_maj7:if(interval==0 || interval==4 || interval==7 || interval==11){return interval;}else{break;}
	case CHORD_KIND_mmaj7:if(interval==0 || interval==3 || interval==7 || interval==11){return interval;}else{break;}
	case CHORD_KIND_7:if(interval==0 || interval==4 || interval==7 || interval==10){return interval;}else{break;}
	case CHORD_KIND_m7:if(interval==0 || interval==3 || interval==7 || interval==10){return interval;}else{break;}
	case CHORD_KIND_7aug:if(interval==0 || interval==4 || interval==8 || interval==10){return interval;}else{break;}
	case CHORD_KIND_7f5:if(interval==0 || interval==4 || interval==6 || interval==10){return interval;}else{break;}
	case CHORD_KIND_m7aug:if(interval==0 || interval==4 || interval==8 || interval==10){return interval;}else{break;}
	case CHORD_KIND_m7f5:if(interval==0 || interval==4 || interval==6 || interval==10){return interval;}else{break;}
	case CHORD_KIND_sus4:if(interval==0 || interval==5 || interval==7){return interval;}else{break;}
	case CHORD_KIND_7sus4:if(interval==0 || interval==5 || interval==7 || interval==10){return interval;}else{break;}
	default:
		printf("意味不明な CHORD_KIND = [%d]\n反吐が出るぜ!!\n",chord->kind);
		printf("終了します\n");exit(1);
		break;
	}
	return (-1);
}

int NearChordTone(int base,int key,int updown,CHORD *chord,int interval)//interval==1で根音を返す
{int x;
	//もし interval>0 ならリクエストに応じるべき
	//if(dia>0){1-7モード;}else{0-11モード;}
	int i,ust,pm;pm = (updown>0)?1:-1;ust = base;//printf("NearChordTone(開始)\n");
	for(i=0;i<24;i++)
	{
		x = IsChordTone(ust,chord);//printf("x=[%d] => " ,x);
		if(x==0 && interval==1){
			//printf("根音[%d]を返します by NearChordTone(%d,%d)\n",ust,chord->dia,interval);
			return ust;}
		else if(x==7 && interval==5){
			//printf("5音[%d]を返します by NearChordTone(%d,%d)\n",ust,chord->dia,interval);
			return ust;}
		else if(((x==3&&chord->kind==CHORD_KIND_m)||(x==4&&chord->kind==CHORD_KIND_M)) && interval==3)
		{
			//printf("5音[%d]を返します by NearChordTone(%d,%d)\n",ust,chord->dia,interval);
			return ust;}
		else if(x>=0 && interval<=0){
			//printf("何となく[%d]を返します by NearChordTone(%d,%d,%d)\n",ust,chord->dia,chord->kind,interval);
			return ust;}
		else{ust += pm;}
	}
	return (-1);
}