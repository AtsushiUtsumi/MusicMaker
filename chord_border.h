#ifndef CHORD_BORDERH
#define CHORD_BORDERH
#include "music.h"
//フォーマット構造体とメンバー数の定義
//#define FORMATNUMBER 1
typedef struct{int length;CHORD chord;}FORMAT_CHORD_BORDER;

#ifndef CHORD_BORDERC
extern FILE *new_chord_border_open(int masterfilenumber);
extern FILE *chord_border_open(int number);
extern int count_chord_border(int masternumber);//マスターナンバーを引き数にしてボーダーの個数カウントする回帰呼出関数
extern int kaiki_chord_border(int masternumber);//中身を作る関数
extern FORMAT_CHORD_BORDER fscanf_chord_border(FILE *fp);//ファイルから1行読んで構造体を返す関数
extern int fprintf_chord_border(FILE *fp,FORMAT_CHORD_BORDER *format);//構造体を読んでファイルに1行出力する関数
#endif

#endif
