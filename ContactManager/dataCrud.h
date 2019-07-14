#pragma once
#include "store.h"

//������󳤶�
#define NAME_LENGTH 15
#define PHONE_LENGTH 11

struct telBook
{
    char name[NAME_LENGTH+1]; //����
    char phone[PHONE_LENGTH+1]; //�绰
    
};

//�洢��������
typedef char Type, *PType;

//���ݽڵ�
typedef struct node
{
    int index; //��Ӧ����Դ����
    int length;//��ռ�ռ䳤��
}Node, *PNode;

// ����Դ���洢����
#define BUF_LENGTH 0x5000

#define INDEX_ERR -1




/*************************interface*****************************/

// ��ʼ��
int crudInit();

// �˳�����
int crudExit();

// ��ȡһ��洢λ��
int getStorageInfo(int dataLength);

// �������
int addInputData(int nodeIndex, PType pData);

//ɾ������
int deleteDataBuf(int dataId);

//��������
int updateDataBuf(int dataId, PType pContent, int conLen);

//����Id��������
int findDataById(int dataId, PType pData);

//��ȡ�����ܹ�����
int getDataTotalNum();
