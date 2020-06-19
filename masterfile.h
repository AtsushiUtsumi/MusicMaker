#ifndef MASTERFILEH
#define MASTERFILEH
//親のマイナンバー,長さ,フレーズ,中身,コード,中身,アクセント => f[6] とする(構造体にしてそのまんまの順で書き込むべき)
//ヘッダー情報は2行(1フォーマット,2数字)
#define MASTERFORMATNUMBER 8
typedef struct
{
	int parent,length,chord_border_painted,chord_border_file
		,rythm_border_painted,rythm_border_file
		,accent_border_painted,accent_border_file
		,linenumber,passage,position;
	int NofBar,count,size;
	int barpropaty,joint;//小節の頭は>0でプロパティーファイルナンバー,JOINT>0で楽節最後のマスターファイルなので長さは自由これで楽節間を滑らかにつなぐ

	int barhead,file_bar;
	int file_chord,file_rhythm;
	int auftaktID;//先頭のアウフタクトを記入(子供に感染させるかは自由)
	int Unit,CoreID;

	int *ChildID;
}MASTERFORMAT;

typedef struct{int c_head,c_file,have_more_children;}CHILDREN_SIDE;
//NofBar,count,size;//追加

#ifndef MASTERFILEC
extern int fill_children_side(int masterfilenumber,CHILDREN_SIDE *dest,int n);
extern FILE *masteropen(int masterfilenumber);
extern MASTERFORMAT master_fscanf(FILE *masterfp);
extern int master_fprintf(FILE *masterfp,MASTERFORMAT *source);
extern int writer_kaiki(int mynumber);//マスターファイルの回帰呼出作成
#endif
#endif