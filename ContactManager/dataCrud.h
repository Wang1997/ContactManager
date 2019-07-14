#pragma once
#include <string.h>
#include "store.h"

//������󳤶�
#define NAME_LENGTH 15
#define PHONE_LENGTH 11

struct telBook
{
    char name[NAME_LENGTH+1]; //����
    char phone[PHONE_LENGTH+1]; //����
    int shortPhone; //�̺�
};

//�洢��������
typedef struct telBook Type, *PType;

//���ݽڵ�
typedef struct node
{
    int index; //��Ӧ����Դ����
    int size;//��ռ�ܿռ�
    int memberLen[3]; //����Ӧ��Առ�ó���
}Node, *PNode;

// ����Դ���洢����
#define BUF_LENGTH 0x5000

#define INDEX_ERR -1


/*************************interface*****************************/

// ��ʼ��
int crudInit();

// �˳�����
int crudExit();

// ������������ռ�õĿռ�
int calcDataSize(PType pData);

// ��ȡһ��洢λ��
int getStorageInfo(int dataSize);

// �������
int addInputData(int nodeIndex, PType pData);

//ɾ������
int deleteDataBuf(int dataId);

//��������
int updateDataBuf(int dataId, PType pContent, int conSize);

//����Id��������
int findDataById(int dataId, PType pData);

//��ȡ�����ܹ�����
int getDataTotalNum();
