#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "dataCrud.h"



//�ó����ʼ��
int manageInit();

//�ó����˳�
void manageExit();

// ��������
int inputInt();

// �����ַ�
int inputChar();

// �����ַ��� ����length
int inputFirstStr(char *data, int maxLen);

// �����ַ��� ����size
int inputSecondStr(char *data, int maxLen);

//��ȡ���ݳ���
int getDataLength(char *data);

// �˵�����
void menuView();

// ������������
void addInput();

//ɾ������
void deleteInput();

//�޸�����
void updateInput();

//ȷ�Ϻ˶Լ��
int checkConfirmInput(char confirm);

//��ѯ����
void queryView();

// չʾ��������
void showAllData();

//���ݲ��� ����
void findByContentInput();

//Id���� ����
void findByIdInput();

//��ӡĳһ������
void printData(int dataId,PType pData);
