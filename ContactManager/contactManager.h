#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "dataCrud.h"

#define NAME_LENGTH 10
#define PHONE_LENGTH 11

//�ó����ʼ��
int manageInit();

//�ó����˳�
void manageExit();

// �˵�����
void menuView();

// ������������
void addInput();

//ɾ������
void deleteInput();

//ȷ�Ϻ˶Լ��
int checkConfirmInput(char confirm);

//��ӡĳһ������
void printData(int dataId,PType pData);
