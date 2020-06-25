#include <stdio.h>
#include "PASSAGE.h"
#include "UST.h"

//要は chord_passage_compiler() って名前の関数が
//ベルトを出力すればなんでもいい手段は選ばない
//後で同じ名前の本番の関数が出来たらテスト用関数は削除する

int chord_passage_compiler(char *passagefn,char *outputfn)//テスト用関数
{int linekind,length,barh,ch,ah,rh,root,kind;
	char header[1280];
	FILE *mapfp;char mapfn[FILENAME_MAX];//コード装填済みのファイル

	//同じAメロでもジョイントは異なる可能性あり
	//よって1度手にかけた楽節でもベルトを作り直す必要がある

	static int t = 0;
	//バレットベルト(bullet belt)みたいな
	//ひとまず楽節の長い[length,root,kind]ファイルを作成

	FORMAT_PASSAGE p;FILE *passagefp;
	FILE *beltfp;char beltfn[FILENAME_MAX];
	
	passagefp = fopen(passagefn,"r");p = passage_fscanf(passagefp);fclose(passagefp);
	//ここより上は p が使えないので注意!!
	
	sprintf(mapfn,"MAP_%d_NEW.txt",p.masterfile);

	//printf("[%s]の処理 by chord_passage_compiler()\n",passagefn);

mapfp = fopen(mapfn,"r");
//printf("%d行目で停止\n",__LINE__);exit(1);
if(mapfp==NULL){printf("[%s]が見つかりませんM_[%d] by chord_passage_compiler(%s)\n",mapfn,p.masterfile,passagefn);
	printf("%d行目で停止\n",__LINE__);exit(1);return 0;}
	else{fscanf(mapfp,"%s",header);}//ヘッダー情報除去

//printf("%d行目で停止\n",__LINE__);exit(1);

	
	
	sprintf(beltfn,"コード[%d].txt",t++);

	beltfp = fopen(beltfn,"w");
	while(fscanf(mapfp,"%d,%d,%d,%d,%d,%d,%d,%d",&linekind,&length,&barh,&ch,&ah,&rh,&root,&kind) > 0)
	{
		//printf("%d,%d,%d,%d,%d,%d,%d,%d\n",linekind,length,barh,ch,ah,rh,root,kind);
		//printf("%d行目で停止\n",__LINE__);//exit(1);
		if(ch>0){fprintf(beltfp,"%d,%d,%d\n",ch*length,root,kind);}
	}
	fclose(beltfp);
fclose(mapfp);
//printf("%d行目で停止\n",__LINE__);exit(1);
	return 0;
}

int chord_passage_linker(char *movementfn)
{
	FILE *movementfp;
	static int t=0;char line[1280];

	FILE *pbeltfp;char pbeltfn[FILENAME_MAX];
	FILE *mbeltfp;char mbeltfn[FILENAME_MAX];
	
	char passagefn[FILENAME_MAX];//別に楽節ファイルを開くわけではないが後々のため
	
	
	

	sprintf(mbeltfn,"1曲分のコード進行.txt");
	mbeltfp = fopen(mbeltfn,"w");
	movementfp = fopen(movementfn,"r");
	while(fscanf(movementfp,"%s",passagefn)>0)
	{
		chord_passage_compiler(passagefn,NULL);//ここで作ったファイルを
		sprintf(pbeltfn,"コード[%d].txt",t++);
		pbeltfp = fopen(pbeltfn,"r");if(pbeltfp==NULL){printf("[%s]が見つかりません by chord_passage_linker()\n",pbeltfn);}
		while(fscanf(pbeltfp,"%s",line)>0){fprintf(mbeltfp,"%s\n",line);}//即付け足す
		fclose(pbeltfp);
		
	}
	fclose(movementfp);
	fclose(mbeltfp);

//////////////////////////////////////以上1楽章(1曲)分のベルトファイル作成完了!!
	
	ChordSplitter(mbeltfn);

	return 0;
}

int ChordSplitter(char *chordbeltfn)//三つ編み分解関数
{
	USTFILEINFO finf = {120,1};
	
	int length,root,kind;
	FILE *chordbeltfp;

	char *partname[3] = {"根音","3音","5音"};
	char prefn[3][FILENAME_MAX];
	FILE *prefp[3];

	int i;

	//printf("三つ編みの[%s]を分解します\n",chordbeltfn);//return 0;

	for(i=0;i<3;i++){sprintf(prefn[i],"%s.txt",partname[i]);}

	for(i=0;i<3;i++)
	{
		prefp[i] = fopen(prefn[i],"w");
		if(prefp[i]==NULL){printf("[%s]を作成出来ませんでした\n",prefn[i]);}
		else{printf("[%s]を作成しました\n",prefn[i]);}
	}
	for(i=0;i<3;i++)
	{
		chordbeltfp = fopen(chordbeltfn,"r");
		while(fscanf(chordbeltfp,"%d,%d,%d",&length,&root,&kind)>0)
		{
			fprintf(prefp[i],"%d,%d\n",length,testc(root,i));
		}
		fclose(chordbeltfp);
		
	}
	for(i=0;i<3;i++){fclose(prefp[i]);}
	
	for(i=0;i<3;i++){ust_file_out(partname[i],&finf);}
	
	return 0;
}


int testc(int root,int i)
{
	int x;
	x=root+2*i;
	
	switch(x%7)
	{
	case 0:x = 59;break;
	case 1:x = 60;break;//C
	case 2:x = 62;break;//D
	case 3:x = 64;break;//E
	case 4:x = 65;break;//F
	case 5:x = 67;break;//G
	case 6:x = 69;break;//A
	default:x = 0;break;
	}
	
	return x;
}
