#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "dataCrud.h"

// 资源图一行展示数量
#define RESOURCE 30

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

//姓名查找 输入
void findByNameInput();

//长号查找 输入
void findByPhoneInput();

//Id查找 输入
void findByIdInput();

//统计界面
void statisticalView();

//存储信息界面
void storeInfoView();

//展示存储资源图
void showStorageResource();

//打印某一条数据
void printData(int dataId,PType pData);
