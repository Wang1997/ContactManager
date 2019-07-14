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
int inputFirstStr(char *data, int maxLen)
{
    rewind(stdin);
    fgets(data, maxLen, stdin); //输入数据

    int length = strlen(data);
    if (data[length - 1] == '\n')
    {
        --length;
        data[length] = 0;
    }
    
    return length;
}

// 输入字符串 返回size
int inputSecondStr(char *data, int maxLen)
{
    int firLength = strlen(data) + 1;

    rewind(stdin);
    fgets(data + firLength, maxLen, stdin); //输入数据

    int secLength = firLength + strlen(data + firLength); //加上第一部分
    if (data[secLength - 1] == '\n')
    {
        data[secLength - 1] = 0;
    }
    else
    {
        secLength++; //算上 '\0'
    }

    return secLength;
}

//获取数据长度
int getDataLength(char *data)
{
    int firLength = strlen(data) + 1;
    int secLength = firLength + strlen(data + firLength); //加上第一部分
    return secLength + 1;
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
                //statisticalWordsView();
                break;
            }
            case 6:
            {
                //storeInfoView();
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
    int inputNum = 0;
    Type data[NAME_LENGTH + PHONE_LENGTH + 2] = { 0 };

    printf("请输入需要添加的姓名：");
    inputFirstStr(data, NAME_LENGTH + 1);
    printf("请输入对应的联系方式：");
    int totalLength = inputSecondStr(data, PHONE_LENGTH + 1);

    //获取存储信息
    int nodeIndex = getStorageInfo(totalLength);
    if (nodeIndex == INDEX_ERR)
    {
        printf("对不起,剩余空间不够\r\n");
        return;
    }

    if (addInputData(nodeIndex, data))
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
    printf("请输入要删除的Id:");
    int dataId = inputInt();

    Type data[NAME_LENGTH + PHONE_LENGTH + 2] = { 0 };
    //获取存储信息
    if (!findDataById(dataId, data))
    {
        printf("对不起,查找不到该数据\r\n");
        return;
    }
    printf("需删除数据如下: \r\n");
    printData(dataId, data);

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
    printf("请输入要更新的Id:");
    int dataId = inputInt();

    //获取存储信息
    Type data[NAME_LENGTH + PHONE_LENGTH + 2] = { 0 };
    if (!findDataById(dataId, data))
    {
        printf("对不起,查找不到该数据\r\n");
        return;
    }
    printf("需更新数据如下: \r\n");
    printData(dataId, data);
    
    int oldLength = getDataLength(data); //旧数据长度

    printf("请输入需要更新的姓名：");
    inputFirstStr(data, NAME_LENGTH + 1);
    printf("请输入更新的联系方式：");
    int newLength = inputSecondStr(data, PHONE_LENGTH + 1);

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
        if (!addInputData(nodeIndex, data))
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
        if (!updateDataBuf(dataId, data, newLength))
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
        printf("2.根据内容查找\r\n");
        printf("3.根据Id查找\r\n");
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
            findByContentInput();
            return;
        }
        case 3:
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
    Type data[NAME_LENGTH + PHONE_LENGTH + 2] = { 0 };
    int flag = 0;

    for (int i = 0; i < totalNum; ++i)
    {
        int dataId = i +1;
        if (findDataById(dataId, data))
        {
            printData(dataId, data);
            flag = 1;
        }
    }

    if (!flag)
    {
        printf("暂无数据\r\n");
    }
}

//内容查找 输入
void findByContentInput()
{

}

//Id查找 输入
void findByIdInput()
{
    printf("请输入要查找的Id:");
    int dataId = inputInt();

    //获取存储信息
    Type data[NAME_LENGTH + PHONE_LENGTH + 2] = { 0 };
    if (!findDataById(dataId, data))
    {
        printf("对不起,查找不到该数据\r\n");
        return;
    }
    printf("查找数据如下: \r\n");
    printData(dataId, data);
}


//打印某一条数据
void printData(int dataId, PType pData)
{
    printf("[%d]\t", dataId);
    printf("%s\t",pData);
    printf("%s\r\n", pData + strlen(pData) + 1);
}



