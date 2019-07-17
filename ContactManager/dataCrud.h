#pragma once
#include <string.h>
#include "store.h"

//数据最大长度
#define NAME_LENGTH 15
#define PHONE_LENGTH 11

struct telBook
{
    char name[NAME_LENGTH+1]; //名字
    char phone[PHONE_LENGTH+1]; //长号
    int shortPhone; //短号
};

//存储数据类型
typedef struct telBook Type, *PType;

typedef struct indexInfo
{
    int index; //开始下标
    int size;//所占空间
}IndexInfo,*PIndexInfo;

// 数据源最大存储长度
#define BUF_LENGTH 0x5000

#define INDEX_ERR -1

#define FIND_NAME 1
#define FIND_PHONE 2


/*************************interface*****************************/

// 初始化
int crudInit();

// 退出程序
int crudExit();

// 计算数据所需占用的空间
int calcDataSize(PType pData);

// 获取一块存储位置
int getStorageInfo(int dataSize);

// 添加数据
int addInputData(int nodeIndex, PType pData);

//删除数据
int deleteDataBuf(int dataId);

//更新数据
int updateDataBuf(int dataId, PType pContent, int conSize);

//根据Id查找内容
int findDataById(int dataId, PType pData);

//匹配数据
int checkDataByContent(int dataId, char *content,PType pData,int type);

//统计数据
int statisticalData(char (*names)[NAME_LENGTH+1],int *counts);

//获取数据总共个数
int getDataTotalNum();

//获取索引结构体数据
int getIndexInfo(int nodeIndex,PIndexInfo pIndexInfo);

//碎片整理
int defragment();
