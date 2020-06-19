#ifndef ACCENT_BORDERH
#define ACCENT_BORDERH
//フォーマット構造体とメンバー数の定義
//#define FORMATNUMBER 1
//typedef struct{int parent,length,chord_border_painted,chord_border_file,rythm_border_painted,rythm_border_file,accent_border_painted,accent_border_file;}FORMAT;

#ifndef ACCENT_BORDERC
extern int count_accent_border(int masternumber);//マスターナンバーを引き数にしてボーダーの個数カウントする回帰呼出関数
extern int kaiki_accent_border(int masternumber);//中身を作る関数
//extern FORMAT fscanf_accent_border(int number);//ファイルから1行読んで構造体を返す関数
//extern int fprintf_accent_border(int number);//構造体を読んでファイルに1行出力する関数
#endif

#endif