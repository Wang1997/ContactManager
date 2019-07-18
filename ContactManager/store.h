#pragma once
#include <stdio.h>

//����Դ
enum SRC{DATA,NODE};

#define SRC_NUM 2

#define READ_MODE "rb"
#define WRITE_MODE "wb"
#define READ_WRITE_MODE "rb+"

#define SUCCESS 1
#define FAIL 0

//��ʼ��
int storeInit();
//�˳�
int storeExit();

//��
int storeOpen(SRC src, const char *mode);

//�ر�
int storeClose(SRC src);

// ��ȡ���� ����ʵ�ʶ�ȡֵ
int storeRead(SRC src,int size,void *buffer);

// д������ ����ʵ��д��ֵ
int storeWrite(SRC src, int size, const void *buffer);

//���ö�дλ��
int storeSeek(SRC src,long offset);

