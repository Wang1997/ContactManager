#pragma once
#include <stdio.h>

//数据源
enum SRC{BUF,NODE};

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
int open(SRC src, const char *mode);

//关闭
int close(SRC src);

// 读取数据 返回实际读取值
int read(SRC src,int size,int count,void *buffer);

// 写入数据 返回实际写入值
int write(SRC src, int size, int count, const void *buffer);

//设置读写位置
int seek(SRC src,long offset,int origin);

