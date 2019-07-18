#pragma once
#include <stdio.h>

//数据源
enum SRC{DATA,NODE};

#define SRC_NUM 2

#define READ_MODE "rb"
#define WRITE_MODE "wb"
#define READ_WRITE_MODE "rb+"

#define SUCCESS 1
#define FAIL 0

//初始化
int storeInit();
//退出
int storeExit();

//打开
int storeOpen(SRC src, const char *mode);

//关闭
int storeClose(SRC src);

// 读取数据 返回实际读取值
int storeRead(SRC src,int size,void *buffer);

// 写入数据 返回实际写入值
int storeWrite(SRC src, int size, const void *buffer);

//设置读写位置
int storeSeek(SRC src,long offset);

