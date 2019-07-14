#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "dataCrud.h"

//该程序初始化
int manageInit();

//该程序退出
void manageExit();

// 输入整数
int inputInt();

// 输入字符
int inputChar();

// 输入字符串
void inputStr(char *data, int maxLen);

// 菜单界面
void menuView();

// 增加数据输入
void addInput();

//删除输入
void deleteInput();

//修改输入
void updateInput();

//确认核对检查
int checkConfirmInput(char confirm);

//查询界面
void queryView();

// 展示所有数据
void showAllData();

//内容查找 输入
void findByContentInput();

//Id查找 输入
void findByIdInput();

//打印某一条数据
void printData(int dataId,PType pData);
