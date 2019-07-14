#pragma once
#include "store.h"

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

//����Id��������
int findDataById(int dataId, PType pData);
