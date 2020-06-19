//UST関連のまずは作るべき関数達
//int USTto();//USTからへ
//int toUST();//からUSTへ
#ifndef CONVERT_H
#define CONVERT_H
#ifndef CONVERT_C
extern int USTtoPIA();//USTからピアノへ(0to無限)
extern int PIAtoUST();//ピアノ(0to無限)からUSTへ
extern int USTto();//USTからダイアトニック(1to7)へ
extern int toUST();//ダイアトニック(1to7)からUSTへ
extern int USTto();//USTから音階(1to11)へ
extern int toUST();//音階(1to11)からUSTへ
#endif
#endif
