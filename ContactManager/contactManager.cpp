#include "contactManager.h"

int main(int argc, char *argv[])
{
    if (!manageInit())
    {
        printf("程序初始化失败,请重新尝试\r\n");
        system("pause");
        return 0;
    }

    menuView();
    manageExit();
    system("pause");

    return 0;
}

//该程序初始化
int manageInit()
{
    int ret = crudInit();
    srand(time(NULL));
    return ret;
}

//该程序退出
void manageExit()
{
    crudExit();
    printf("感谢您的使用\r\n");
}

// 输入整数
int inputInt()
{
    int num = 0;
    rewind(stdin);
    scanf("%d", &num);
    return num;
}

// 输入字符
int inputChar()
{
    char ch = '\0';
    rewind(stdin);
    ch = getchar();
    return ch;
}

// 输入字符串 返回length
void inputStr(char *data, int maxLen)
{
    rewind(stdin);
    fgets(data, maxLen, stdin); //输入数据

    int length = strlen(data);
    if (data[length - 1] == '\n')
    {
        data[length - 1] = 0;
    }
    
    return;
}

//菜单界面
void menuView()
{
    while (1)
    {
        system("cls");
        printf("  联系人管理系统  \r\n");
        printf("1.增加功能\r\n");
        printf("2.删除功能\r\n");
        printf("3.修改功能\r\n");
        printf("4.查询功能\r\n");
        printf("5.统计功能\r\n");
        printf("6.存储信息功能\r\n");
        printf("7.自动填充功能\r\n");
        printf("8.退出\r\n");
        printf("请选择: ");
        int choose = inputInt();
        switch (choose)
        {
            case 1:
            {
                addInput();
                break;
            }
            case 2:
            {
                deleteInput();
                break;
            }
            case 3:
            {
                updateInput();
                break;
            }
            case 4:
            {
                queryView();
                break;
            }
            case 5:
            {
                statisticalView();
                break;
            }
            case 6:
            {
                storeInfoView();
                break;
            }
            case 7:
            {
                //autoExpansion();
                break;
            }
            case 8:
            {
                return;
                break;
            }
            default:
            {
                printf("对不起，输入有误，请重新选择\r\n");
                break;
            }

        }
        system("pause");
    }
}


// 增加数据输入
void addInput()
{
    Type data = { 0 };

    printf("请输入需要添加的姓名：");
    inputStr(data.name, NAME_LENGTH + 1);
    printf("请输入对应的长号：");
    inputStr(data.phone, PHONE_LENGTH + 1);
    printf("请输入对应的短号：");
    data.shortPhone = inputInt();

    //获取存储信息
    int dataSize = calcDataSize(&data);
    int nodeIndex = getStorageInfo(dataSize);
    if (nodeIndex == INDEX_ERR)
    {
        printf("对不起,剩余空间不够\r\n");
        return;
    }

    if (addInputData(nodeIndex, &data))
    {
        printf("添加成功\r\n");
    }
    else
    {
        printf("添加失败\r\n");
    }

    return;
}

//确认核对检查
int checkConfirmInput(char confirm)
{
    if (confirm != 'y' && confirm != 'Y')
        return 0;
    return 1;
}


//删除输入
void deleteInput()
{
    printf("请输入要删除的编号:");
    int dataId = inputInt();

    Type data = { 0 };
    //获取存储信息
    if (!findDataById(dataId, &data))
    {
        printf("对不起,查找不到该数据\r\n");
        return;
    }
    printf("需删除数据如下: \r\n");
    printData(dataId, &data);

    printf("请确认是否删除(y or n)：");
    char confirm = inputChar();
    if (!checkConfirmInput(confirm))
    {//不删除
        printf("该数据未删除\r\n");
        return;
    }

    if (deleteDataBuf(dataId))
    {
        printf("删除成功\r\n");
    }
    else
    {
        printf("删除失败\r\n");
    }
}


//修改输入
void updateInput()
{
    printf("请输入要更新的编号:");
    int dataId = inputInt();

    //获取存储信息
    Type data = { 0 };
    if (!findDataById(dataId, &data))
    {
        printf("对不起,查找不到该数据\r\n");
        return;
    }
    printf("需更新数据如下: \r\n");
    printData(dataId, &data);
    
    int oldLength = calcDataSize(&data); //旧数据长度

    printf("请输入需要更新的姓名：");
    inputStr(data.name, NAME_LENGTH + 1);
    printf("请输入更新的长号：");
    inputStr(data.phone, PHONE_LENGTH + 1);
    printf("请输入更新的短号：");
    data.shortPhone = inputInt();

    int newLength = calcDataSize(&data); //新数据长度

    if (newLength > oldLength) //需获取新的存储空间
    {
        //获取存储信息
        int nodeIndex = getStorageInfo(newLength);
        if (nodeIndex == INDEX_ERR)
        {
            printf("对不起,剩余空间不够\r\n");
            return;
        }
        
        //添加新数据
        if (!addInputData(nodeIndex, &data))
        {
            printf("对不起,更新添加数据失败\r\n");
            return;
        }

        //删除旧数据
        if (!deleteDataBuf(dataId))
        {
            printf("更新失败,旧数据删除失败\r\n");
            return;
        }
    }
    else //直接更新
    {
        if (!updateDataBuf(dataId, &data, newLength))
        {
            printf("更新数据失败\r\n");
        }
    }

    printf("更新成功\r\n");
    return;
}

//查询界面
void queryView()
{
    while (1)
    {
        system("cls");
        printf("  查询功能  \r\n");
        printf("1.查询所有数据\r\n");
        printf("2.根据姓名查找\r\n");
        printf("3.根据长号查找\r\n");
        printf("4.根据编号查找\r\n");
        printf("请选择: ");
        int choose = inputInt();
        switch (choose)
        {
        case 1:
        {
            showAllData();
            return;
        }
        case 2:
        {
            findByNameInput();
            return;
        }
        case 3:
        {
            findByPhoneInput();
            return;
        }
        case 4:
        {
            findByIdInput();
            return;
        }
        default:
        {
            printf("对不起，输入有误，请重新选择\r\n");
            system("pause");
            break;
        }
        }
    }
}

// 展示所有数据
void showAllData()
{
    int totalNum = getDataTotalNum();
    Type data = { 0 };
    int flag = 0;

    for (int i = 0; i < totalNum; ++i)
    {
        int dataId = i +1;
        if (findDataById(dataId, &data))
        {
            printData(dataId, &data);
            printf("\r\n");
            flag = 1;
        }
    }

    if (!flag)
    {
        printf("暂无数据\r\n");
    }
}

//名字查找 输入
void findByNameInput()
{
    char name[NAME_LENGTH + 1] = {0};
    printf("请输入需要查找的姓名(模糊)：");
    inputStr(name, NAME_LENGTH + 1);

    int totalNum = getDataTotalNum();
    int flag = 0;

    Type data = { 0 };

    for (int i = 0; i < totalNum; ++i)
    {
        int dataId = i + 1;
        if (checkDataByContent(dataId, name, &data, FIND_NAME))
        {
            printData(dataId, &data);
            printf("\r\n");
            flag = 1;
        }
    }

    if (!flag)
    {
        printf("暂无数据\r\n");
    }
}

//长号查找 输入
void findByPhoneInput()
{
    char phone[PHONE_LENGTH + 1] = { 0 };
    printf("请输入需要查找的长号(模糊)：");
    inputStr(phone, PHONE_LENGTH + 1);

    int totalNum = getDataTotalNum();
    int flag = 0;

    Type data = { 0 };

    for (int i = 0; i < totalNum; ++i)
    {
        int dataId = i + 1;
        if (checkDataByContent(dataId, phone, &data, FIND_PHONE))
        {
            printData(dataId, &data);
            printf("\r\n");
            flag = 1;
        }
    }

    if (!flag)
    {
        printf("暂无数据\r\n");
    }
}

//Id查找 输入
void findByIdInput()
{
    printf("请输入要查找的编号:");
    int dataId = inputInt();

    //获取存储信息
    Type data = { 0 };
    if (!findDataById(dataId, &data))
    {
        printf("对不起,查找不到该数据\r\n");
        return;
    }
    printf("查找数据如下: \r\n");
    printData(dataId, &data);
}

//统计界面
void statisticalView()
{
    
    int totalNum = getDataTotalNum(); //获取数量

    if (totalNum <= 0)
    {
        printf("对不起, 暂无数据可统计\r\n");
        return;
    }
    
    //存放姓名 数组指针 char [NAME_LENGTH + 1]
    char (*names)[NAME_LENGTH + 1] = NULL;
    names = (char(*)[NAME_LENGTH + 1])malloc(totalNum * (NAME_LENGTH+1));
    //存放出现次数
    int *counts = NULL;
    counts = (int*)malloc(totalNum * sizeof(int));
    if (names == NULL || counts == NULL)
    {
        printf("对不起,数据统计分配内存有误\r\n");
        return;
    }
    
    memset(names,0, totalNum * (NAME_LENGTH + 1));
    memset(counts, 0, totalNum * sizeof(int));

    int statisticalNum = statisticalData(names, counts);
    if (statisticalNum <= 0)
    {
        printf("对不起,数据统计失败\r\n");
        goto FREE_END;
    }

    system("cls");
    printf("统计数据如下: \r\n");
    printf("共有%d条联系人信息\r\n", statisticalNum);
    for (int i = 0; i < statisticalNum; ++i)
    {
        printf("姓名: %s 出现了%d次,比例为%.2lf%%\r\n", names[i],counts[i],
            counts[i] * 100.0 / totalNum);
    }

FREE_END:
    //释放内存
    if (names != NULL)
    {
        free(names);
        names = NULL;
    }
    if (counts != NULL)
    {
        free(counts);
        counts = NULL;
    }
    return;
}

//存储信息界面
void storeInfoView()
{
    while (1)
    {
        system("cls");
        printf("  存储信息  \r\n");
        printf("1.存储资源分布情况\r\n");
        printf("2.碎片整理\r\n");
        printf("请选择: ");
        int choose;
        scanf("%d", &choose);
        switch (choose)
        {
            case 1:
            {
                showStorageResource();
                return;
            }
            case 2:
            {
                if (defragment())
                {
                    printf("碎片整理成功\r\n");
                }
                else
                {
                    printf("碎片整理失败\r\n");
                }
                return;
            }
            default:
            {
                printf("对不起，输入有误，请重新选择\r\n");
                system("pause");
                break;
            }
        }
    }
}

//展示存储资源图
void showStorageResource()
{
    int totalNum = getDataTotalNum(); //获取数量
    int totalSize = 0;

    if (totalNum > 0)
    {
        IndexInfo indexInfo = {0};
        int lastEndIndex = 0;
        for (int i = 0; i < totalNum; ++i)
        {
            getIndexInfo(i, &indexInfo);
            for (int j = indexInfo.index - 1; j >= lastEndIndex; --j)
            {
                printf("□"); //空闲
                if (++totalSize % 10 == 0)
                    printf("\r\n");
            }
            for (int j = 0; j < indexInfo.size; ++j)
            {
                printf("■"); //占有
                if (++totalSize % 10 == 0)
                    printf("\r\n");
            }
            lastEndIndex = indexInfo.index + indexInfo.size;
        }
    }

    printf("\r\n");
    printf("总共空间大小:0x%x,剩余0x%x未分配\r\n", BUF_LENGTH, BUF_LENGTH - totalSize);
}


//打印某一条数据
void printData(int dataId, PType pData)
{
    printf("编号: %d\r\n", dataId);
    printf("姓名: %s\r\n",pData->name);
    printf("长号: %s\r\n", pData->phone);
    printf("短号: %d\r\n", pData->shortPhone);
}



