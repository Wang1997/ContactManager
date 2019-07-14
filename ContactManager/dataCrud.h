#pragma once
#include "store.h"

//存储数据类型
typedef char Type, *PType;

//数据节点
typedef struct node
{
    int index; //对应数据源索引
    int length;//所占空间长度
}Node, *PNode;

// 数据源最大存储长度
#define BUF_LENGTH 0x5000

#define INDEX_ERR -1


/*************************interface*****************************/

// 初始化
int crudInit();

// 退出程序
int crudExit();

// 获取一块存储位置
int getStorageInfo(int dataLength);

// 添加数据
int addInputData(int nodeIndex, PType pData);

//删除数据
int deleteDataBuf(int dataId);

//根据Id查找内容
int findDataById(int dataId, PType pData);
