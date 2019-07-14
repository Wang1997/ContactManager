#include "contactManager.h"

int main(int argc, char *argv[])
{
    if (!manageInit())
    {
        printf("�����ʼ��ʧ��,�����³���\r\n");
        system("pause");
        return 0;
    }

    menuView();
    manageExit();
    system("pause");

    return 0;
}

//�ó����ʼ��
int manageInit()
{
    int ret = crudInit();
    srand(time(NULL));
    return ret;
}

//�ó����˳�
void manageExit()
{
    crudExit();
    printf("��л����ʹ��\r\n");
}

//�˵�����
void menuView()
{
    while (1)
    {
        system("cls");
        printf("  ��ϵ�˹���ϵͳ  \r\n");
        printf("1.���ӹ���\r\n");
        printf("2.ɾ������\r\n");
        printf("3.�޸Ĺ���\r\n");
        printf("4.��ѯ����\r\n");
        printf("5.ͳ�ƹ���\r\n");
        printf("6.�洢��Ϣ����\r\n");
        printf("7.�Զ���书��\r\n");
        printf("8.�˳�\r\n");
        printf("��ѡ��: ");
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
                printf("�Բ�����������������ѡ��\r\n");
                break;
            }

        }
        system("pause");
    }
}


// ������������
void addInput()
{
    int inputNum = 0;
    Type data[NAME_LENGTH + PHONE_LENGTH + 2] = { 0 };

    printf("��������Ҫ��ӵ�������");
    rewind(stdin);
    fgets(data, NAME_LENGTH + 1, stdin); //��������

    int nameLength = strlen(data);
    if (data[nameLength - 1] == '\n')
    {
        data[nameLength - 1] = 0;
    }
    else
    {
        nameLength++; //���� \0
    }

    printf("�������Ӧ����ϵ��ʽ��");
    rewind(stdin);
    fgets(data + nameLength, PHONE_LENGTH + 1, stdin); //��������
    int totalLength = nameLength + strlen(data + nameLength);
    if (data[totalLength - 1] == '\n')
    {
        data[totalLength - 1] = 0;
    }
    else
    {
        totalLength++; //���� \0
    }

    //��ȡ�洢��Ϣ
    int nodeIndex = getStorageInfo(totalLength);
    if (nodeIndex == INDEX_ERR)
    {
        printf("�Բ���,ʣ��ռ䲻��\r\n");
        return;
    }

    if (addInputData(nodeIndex, data))
    {
        printf("��ӳɹ�\r\n");
    }
    else
    {
        printf("���ʧ��\r\n");
    }

    return;
}

//ȷ�Ϻ˶Լ��
int checkConfirmInput(char confirm)
{
    if (confirm != 'y' && confirm != 'Y')
        return 0;
    return 1;
}


//ɾ������
void deleteInput()
{
    printf("������Ҫɾ����Id:");
    int dataId = 0;
    rewind(stdin);
    scanf("%d", &dataId);

    Type data[NAME_LENGTH + PHONE_LENGTH + 2] = { 0 };
    //��ȡ�洢��Ϣ
    if (!findDataById(dataId, data))
    {
        printf("�Բ���,���Ҳ���������\r\n");
        return;
    }
    printf("��ɾ����������: \r\n");
    printData(dataId, data);

    printf("��ȷ���Ƿ�ɾ��(y or n)��");
    rewind(stdin);
    char confirm;
    confirm = getchar();
    if (!checkConfirmInput(confirm))
    {//��ɾ��
        printf("������δɾ��\r\n");
        return;
    }

    if (deleteDataBuf(dataId))
    {
        printf("ɾ���ɹ�\r\n");
    }
    else
    {
        printf("ɾ��ʧ��\r\n");
    }
}


//��ӡĳһ������
void printData(int dataId, PType pData)
{
    printf("[%d]\t", dataId);
    printf("%s\t",pData);
    printf("%s\r\n", pData + strlen(pData) + 1);
}



