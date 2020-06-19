#ifndef UST_H
#define UST_H
typedef struct{int Tempo,Tracks;}USTFILEINFO;
typedef struct{int counter,length,note;char lylic[5];}USTNOTEINFO;

#ifndef UST_C
extern FILE *ust_fopen(char *filename,USTFILEINFO *x);
extern int ust_fprintf(FILE *fp,USTNOTEINFO *x);
extern int ust_fclose(FILE *fp);
extern int ust_file_out(const char *partname,USTFILEINFO *finf);
#endif

#endif
