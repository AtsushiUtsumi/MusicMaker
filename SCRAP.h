//スコアを読んで各パートの共通事項
//どんなスクラップファイルを作るかは各パート専用の関数にお任せ
#ifndef SCRAP_H
#define SCRAP_H
typedef struct{int note;}SCRAP_LINE;
#ifndef SCRAP_C
extern int cat_scrap(char *scorefn,char *scrapfn);//スコアにパートスクラップを引っ付けるだけのシケた関数

extern int Maker_Oboe(char *scorefn);

#endif
#endif