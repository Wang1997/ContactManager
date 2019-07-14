#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "dataCrud.h"

#define NAME_LENGTH 10
#define PHONE_LENGTH 11

//该程序初始化
int manageInit();

//该程序退出
void manageExit();

// 菜单界面
void menuView();

// 增加数据输入
void addInput();

//删除输入
void deleteInput();

//确认核对检查
int checkConfirmInput(char confirm);

//打印某一条数据
void printData(int dataId,PType pData);
