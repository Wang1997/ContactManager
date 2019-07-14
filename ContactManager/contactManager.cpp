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
        int choose;
        rewind(stdin);
        scanf("%d", &choose);
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
                //updateInput();
                break;
            }
            case 4:
            {
                //queryView();
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
    rewind(stdin);
    fgets(data, NAME_LENGTH + 1, stdin); //输入数据

    int nameLength = strlen(data);
    if (data[nameLength - 1] == '\n')
    {
        data[nameLength - 1] = 0;
    }
    else
    {
        nameLength++; //算上 \0
    }

    printf("请输入对应的联系方式：");
    rewind(stdin);
    fgets(data + nameLength, PHONE_LENGTH + 1, stdin); //输入数据
    int totalLength = nameLength + strlen(data + nameLength);
    if (data[totalLength - 1] == '\n')
    {
        data[totalLength - 1] = 0;
    }
    else
    {
        totalLength++; //算上 \0
    }

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
    int dataId = 0;
    rewind(stdin);
    scanf("%d", &dataId);

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
    rewind(stdin);
    char confirm;
    confirm = getchar();
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


//打印某一条数据
void printData(int dataId, PType pData)
{
    printf("[%d]\t", dataId);
    printf("%s\t",pData);
    printf("%s\r\n", pData + strlen(pData) + 1);
}



