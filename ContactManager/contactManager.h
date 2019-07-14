#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "dataCrud.h"

//�ó����ʼ��
int manageInit();

//�ó����˳�
void manageExit();

// ��������
int inputInt();

// �����ַ�
int inputChar();

// �����ַ���
void inputStr(char *data, int maxLen);

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
