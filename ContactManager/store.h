#pragma once
#include <stdio.h>

//����Դ
enum SRC{BUF,NODE};

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
int open(SRC src, const char *mode);

//�ر�
int close(SRC src);

// ��ȡ���� ����ʵ�ʶ�ȡֵ
int read(SRC src,int size,int count,void *buffer);

// д������ ����ʵ��д��ֵ
int write(SRC src, int size, int count, const void *buffer);

//���ö�дλ��
int seek(SRC src,long offset,int origin);

