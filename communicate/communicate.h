#ifndef communicate_h
#define communicate_h

#include "../type/type.h"
#include "../list/list_expand.h"

typedef struct message{
    SID *p_sid;
    char titile[16];
    char content[0];
}MSG;

typedef struct file_head{
    char head_test[18];
    unsigned long long data_num;
}F_HEAD;

typedef struct data_file{
    FILE *fp;
    F_HEAD *pf_head;
    List *pf_stdlst;
}D_FILE;

typedef struct standard_data_blocks{
    unsigned int type;
    char *sid;
    _Bool if_data;
    unsigned int blocks_num;
    char *buff;
}STD_BLOCKS;

typedef struct standard_data_connection{
    char *f_sid;
    char *s_sid;
}STD_CTN;

typedef struct standard_data_head{
    unsigned long long data_blk_num;
    unsigned long long data_ctn_num;
}STD_HEAD;

typedef struct standard_data{
    SID *s_id;
    unsigned int type;
    _Bool lock;
    List *pd_blocklst;
    List *pd_ctnlst;
}STD_DATA;

D_FILE *initDataFileForWrite(char *route);
D_FILE *initDataFileForRead(char *route);
STD_BLOCKS *initStandardDBlocks(SID *p_sid, unsigned long long data_size);
STD_CTN *initStandardDConnection(SID *f_sid, SID *s_sid);
STD_DATA *initStandardData(unsigned int type);

int dataForStandardDBlock(STD_BLOCKS *p_stdb,void *data);
int dataFileAddStandardData(D_FILE *p_dfile, STD_DATA *p_std);
int standardDataAddBlock(STD_DATA *p_std, SID *p_sid ,void *data, unsigned long long data_size);
int standardDataAddConnection(STD_DATA *p_std, SID *f_sid, SID *s_sid);
int dataFileWriteIn(D_FILE *p_dfile);
int dataFileReadOut(D_FILE *p_dfile);

int releaseSTDBlocks(STD_BLOCKS *p_stdb);
int releaseStandardData(STD_DATA *p_std);
int releaseSTDConnection(STD_CTN *p_stdc);
int releaseDFile(D_FILE *p_file);

List *_doStandardDataInfoWrite(unsigned int type, void *value, List *er_list);
List *_doStandardDataWrite(unsigned int type, void *value, List *er_list);
List *_doStandardDConnectionWrite(unsigned int type, void *value, List *er_list);
List *_doStandardDBlockWrite(unsigned int type, void *value, List *er_list);

STD_DATA *listToSTD(List *);
STD_DATA *stackToSTD(Stack *);
STD_DATA *treeToSTD(Tree *);

#endif /* communicate_h */
