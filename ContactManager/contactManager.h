#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "dataCrud.h"

// ��Դͼһ��չʾ����
#define RESOURCE 30

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

//�������� ����
void findByNameInput();

//���Ų��� ����
void findByPhoneInput();

//Id���� ����
void findByIdInput();

//ͳ�ƽ���
void statisticalView();

//�洢��Ϣ����
void storeInfoView();

//չʾ�洢��Դͼ
void showStorageResource();

//��ӡĳһ������
void printData(int dataId,PType pData);
