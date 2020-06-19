//パート無しのスコアファイル作成関数
#ifndef SCORE_H
#define SCORE_H
typedef struct{int tempo;}SCORE_HEADER;
typedef struct{int length,bh,ch,ah,rh,chord_root,chord_kind,passage_head,passage_number;}SCORE_LINE;//passage_head,passage_numberを入れるか吟味すること
#ifndef SCORE_C
extern int ScoreMaker_Movement(char *movementfn);//楽章(1曲)のスコアファイル作成関数
extern int score_line_sscanf(char *line,SCORE_LINE *dest);
#endif
#endif