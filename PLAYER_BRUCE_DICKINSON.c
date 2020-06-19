#include "PartsFunction.h"
#include "PARTS_COMMON.h"

static const char *playername = "BRUCE_DICKINSON";
static const char *partname = "PLAYER_BRUCE_DICKINSON";
static int NOP;//起動時での他のパート数を記録しておく
typedef struct
{
	int wave,pre,note,nanika;int LyricsID;

}X;//個々のパートで使用

struct
{
	int LineNumber;//全体で何行か
	SCORE_LINE *s;
	PART **OP;int NofOP;
	X *x;
}lines={-1};//このパートで使うであろうラインを動的に確保する

int TEST_N_Lyrics(int n,int LyricsID)//N文字の歌詞を
{
	char *table[] ={"た","い","や","き"};
	int i;
	FILE *fp;char fn[260];
	char *DefaultFn = "歌詞.txt";
	sprintf(fn,"%s",DefaultFn);
	fp = fopen(fn,"w");
	for(i=0;i<n;i++){fprintf(fp,"%s\n",table[i%(sizeof(table)/sizeof(char*))]);}
	fclose(fp);
	return 0;
}

static int SameChildSameX(int n,CHILDREN_SIDE *cside,X *x)
{
	int i,j,k;
	for(i=0;i<n;i++)//同じファイルナンバーならコピー中身を全てコピー
	{
		if(cside[i].c_head>0)//1st ヘッドキャッチ
		{
			for(j=i;j<n;j++)
			{
				if(cside[j].c_head>0 && cside[j].c_file==cside[i].c_file)//2nd ヘッドキャッチ
				{
					for(k=0;j+k<n;k++)
					{
						if(cside[i+k].c_file==cside[j+k].c_file)//何か単位ならアレンジ書き換え自由
						{
							x[j+k] = x[i+k];//今は全てコピー
						}else{break;}
					}
				}
			}
		}
	}//コピー完了
	return 0;}

static int Maker_BRUCE_DICKINSON_Master(int MasterNumber,SCORE_LINE *s,X *x,int n,int PassageID,int NofS,int NofOP,PART **OP)
{
	extern char *TestScaleFn(int);
	extern int DIAtoMIDI(int);
	
	MASTERFORMAT m;FILE *wavefp;CHILDREN_SIDE *cside;int i,j;CHORD nowchord;int lastpre,lastnote=-1;int maku;
	
	LoadMasterInfo(&m,MasterNumber);
	//printf("CORE[%d](%d小節) が中心 by Maker_BRUCE_DICKINSON_Master(%d)",m.CoreID,m.Unit,MasterNumber);
	//printf("<= コイツ自身は[%d]小節\n",m.NofBar);
	if(PassageID==1 || PassageID==5 ||PassageID==6){for(i=0;i<n;i++){x[i].note = 0;}return 0;}
	cside = CreateCside(n,MasterNumber);
//////初期設定完了
	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++以下WAVE PRE NOTE 決定



	for(i=0;i<n;i++)
	{
		if(i==0 || (s[i].bh>0&&rand()%2==0)){x[i].nanika = 1+rand()%3;}//適当
	}
	wavefp = WaveFileOpen(n,MasterNumber);
	for(j=0;j<n;j++)
	{
		if(cside[j].have_more_children>0 && rand()%3==0){/*子供に頼む*/Maker_BRUCE_DICKINSON_Master(cside[j].c_file,&s[j],&x[j],cside[j].c_head,PassageID,NofS,NofOP,&OP[j]);}
		else{/*ここで決める*/
			for(i=j;i<n;i++)
			{
				if(cside[i].c_head>0 || x[i].nanika>0){rewind(wavefp);x[i].pre = 12 + 50+rand()%6;lastpre = x[i].pre;}
				if(s[i].rh>0){fscanf(wavefp,"%d",&x[i].wave);x[i].pre = lastpre+x[i].wave*(1+rand()%2);}
			}}
	}fclose(wavefp);

	for(maku=2,i=n-1;i>=0;i--)//最後は伸ばし
	{
		nowchord.dia = s[i].chord_root;nowchord.kind= s[i].chord_kind;
		if(x[i].pre>0 && maku>0)
		{
			if(maku==2){x[i].note = NearChordTone(x[i].pre,0,1,&nowchord,1);}
			else{x[i].note = NearChordTone(x[i].pre,0,1,&nowchord,5);}
			maku=0;
		}
		if(cside[i].c_head>0)maku=1;
	}

	for(i=0;i<n;i++){nowchord.dia = s[i].chord_root;nowchord.kind= s[i].chord_kind;
	if(x[i].pre>0)
	{
		x[i].note = ScaleNote(
			(lastnote>0) ? lastnote : NearChordTone(x[i].pre,0,1,&nowchord,0)
			,x[i].wave
			,DIAtoMIDI(s[i].chord_root)
			,TestScaleFn(DIAtoMIDI(s[i].chord_root))
			);
		lastnote = x[i].note;
		x[i].note = (s[i].ah>0) ? NearChordTone(x[i].pre,0,1,&nowchord,0):NearWhite(x[i].pre,1,1);
	}else{x[i].note = -1;}}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++以上WAVE PRE NOTE 決定
	SameChildSameX(n,cside,x);free(cside);UnloadMasterInfo(&m);
	return 0;
}

static int Maker_BRUCE_DICKINSON_Passage(int passageID,SCORE_LINE *s,X *x,int n)//横並びファイル作成すべき(音高のみ)
{
	FILE *mysoundfp;char mysoundfn[260];
	FILE *scrapfp;char scrapfn[260];
	
	FORMAT_PASSAGE p;
	
	int outnote;
	PART **OP;//OtherPartsの略(ポインタテーブル)
	int NofOP;
	int i,j;CHORD nowchord;int ID;
	
	
	//printf("Maker_BRUCE_DICKINSON_Passage(%d)開始\n",passageID);
	sprintf(scrapfn,"SCRAP_%s_%d.txt",playername,passageID);
	scrapfp = fopen(scrapfn,"r");
	if(scrapfp!=NULL)
	{
		//printf("既存のファイル[%s]をロードします\n",scrapfn);
		for(i=0;i<n;i++){fscanf(scrapfp,"%d,%d,%d",&x[i].wave,&x[i].pre,&x[i].note);}
		fclose(scrapfp);
		return 0;
	}
	LoadPassageInfo(&p,passageID);

	//balance(p.masterfile,350);
	NofOP = NOP;
	
	sprintf(mysoundfn,"mysound_%s_%d.txt",partname,passageID);mysoundfp = fopen(mysoundfn,"w");

OP = CreatePart(n,NofOP,1,passageID);
	Maker_BRUCE_DICKINSON_Master(p.masterfile,s,x,n,passageID,1,NofOP,OP);
DestroyPart(OP,NofOP);



	for(i=0,outnote= 0;i<n;i++)
	{
		for(j=0;j<1;j++)
		{
			if(x[i].note>0){outnote = x[i].note;}
			fprintf(mysoundfp,"%d",outnote);
			if(j==0){fprintf(mysoundfp,"\n");}else{fprintf(mysoundfp,",");}
		}
	}
	fclose(mysoundfp);
	AddSession_Passage(1,passageID,mysoundfn);

	scrapfp = fopen(scrapfn,"w");
	for(i=0;i<n;i++){fprintf(scrapfp,"%d,%d,%d\n",x[i].wave,x[i].pre,x[i].note);}
	fclose(scrapfp);
	return 0;
}

int Maker_BRUCE_DICKINSON_Movement(char *scorefn)
{
	char trackname[FILENAME_MAX];int NofPL;char test[1000];int ID;X *x;USTFILEINFO finf = {240,1};FILE *tfp=NULL;char tfn[260];int linekind;char line[1000];SCORE_LINE *s;int *parts;int m=0,n,i,j;//パートの行列
	FILE *scorefp=NULL;
	
//printf("Maker_BRUCE_DICKINSON_Movement(%s)開始\n",scorefn);
	NOP = NumberOfParts(NULL,1);

	scorefp = fopen(scorefn,"r");while(fscanf(scorefp,"%d,%s",&linekind,line)>0){if(linekind==LINEKIND_BODY)m++;}fclose(scorefp);scorefp=NULL;
	x = (X*)calloc(m,sizeof(X));
	s = (SCORE_LINE*)calloc(m,sizeof(SCORE_LINE));
//////////////////////////////////////////////////スコアファイル読み込み開始
	scorefp = fopen(scorefn,"r");fscanf(scorefp,"%d,%s",&linekind,line);for(i=0;i<m;i++){fscanf(scorefp,"%d,%s",&linekind,line);score_line_sscanf(line,&s[i]);}fclose(scorefp);scorefp=NULL;
//////////////////////////////////////////////////スコアファイル読み込み完了

//////////////////////////////////メモリ上の操作はここから
	for(i=0;i<m;i++)
	{
		if(s[i].passage_head==1)
		{
			for(j=i,NofPL=1;j!=m-1 && s[j+1].passage_head==0;j++,NofPL++);
			Maker_BRUCE_DICKINSON_Passage(s[i].passage_number,&s[i],&x[i],NofPL);
		}
	}//exit(1);
//////////////////////////////////メモリ上の操作はここまで

//////////////////////////////////////////////////以下出力
	sprintf(tfn,"%s.txt",playername);
	sprintf(trackname,"%s",playername);
	tfp = fopen(tfn,"w");for(i=0;i<m;i++){fprintf(tfp,"%d,%d\n",s[i].length,x[i].note);}fclose(tfp);
	   free(x);free(s);
	   
	   ust_file_out(trackname,&finf);//printf("Maker_BRUCE_DICKINSON_Movement()終了\n");
	   
	   //MidiFileOut(trackname);

	   return 0;}
