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

// ��������
int inputInt()
{
    int num = 0;
    rewind(stdin);
    scanf("%d", &num);
    return num;
}

// �����ַ�
int inputChar()
{
    char ch = '\0';
    rewind(stdin);
    ch = getchar();
    return ch;
}

// �����ַ��� ����length
int inputFirstStr(char *data, int maxLen)
{
    rewind(stdin);
    fgets(data, maxLen, stdin); //��������

    int length = strlen(data);
    if (data[length - 1] == '\n')
    {
        --length;
        data[length] = 0;
    }
    
    return length;
}

// �����ַ��� ����size
int inputSecondStr(char *data, int maxLen)
{
    int firLength = strlen(data) + 1;

    rewind(stdin);
    fgets(data + firLength, maxLen, stdin); //��������

    int secLength = firLength + strlen(data + firLength); //���ϵ�һ����
    if (data[secLength - 1] == '\n')
    {
        data[secLength - 1] = 0;
    }
    else
    {
        secLength++; //���� '\0'
    }

    return secLength;
}

//��ȡ���ݳ���
int getDataLength(char *data)
{
    int firLength = strlen(data) + 1;
    int secLength = firLength + strlen(data + firLength); //���ϵ�һ����
    return secLength + 1;
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
    inputFirstStr(data, NAME_LENGTH + 1);
    printf("�������Ӧ����ϵ��ʽ��");
    int totalLength = inputSecondStr(data, PHONE_LENGTH + 1);

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
    int dataId = inputInt();

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
    char confirm = inputChar();
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


//�޸�����
void updateInput()
{
    printf("������Ҫ���µ�Id:");
    int dataId = inputInt();

    //��ȡ�洢��Ϣ
    Type data[NAME_LENGTH + PHONE_LENGTH + 2] = { 0 };
    if (!findDataById(dataId, data))
    {
        printf("�Բ���,���Ҳ���������\r\n");
        return;
    }
    printf("�������������: \r\n");
    printData(dataId, data);
    
    int oldLength = getDataLength(data); //�����ݳ���

    printf("��������Ҫ���µ�������");
    inputFirstStr(data, NAME_LENGTH + 1);
    printf("��������µ���ϵ��ʽ��");
    int newLength = inputSecondStr(data, PHONE_LENGTH + 1);

    if (newLength > oldLength) //���ȡ�µĴ洢�ռ�
    {
        //��ȡ�洢��Ϣ
        int nodeIndex = getStorageInfo(newLength);
        if (nodeIndex == INDEX_ERR)
        {
            printf("�Բ���,ʣ��ռ䲻��\r\n");
            return;
        }
        
        //���������
        if (!addInputData(nodeIndex, data))
        {
            printf("�Բ���,�����������ʧ��\r\n");
            return;
        }

        //ɾ��������
        if (!deleteDataBuf(dataId))
        {
            printf("����ʧ��,������ɾ��ʧ��\r\n");
            return;
        }
    }
    else //ֱ�Ӹ���
    {
        if (!updateDataBuf(dataId, data, newLength))
        {
            printf("��������ʧ��\r\n");
        }
    }

    printf("���³ɹ�\r\n");
    return;
}

//��ѯ����
void queryView()
{
    while (1)
    {
        system("cls");
        printf("  ��ѯ����  \r\n");
        printf("1.��ѯ��������\r\n");
        printf("2.�������ݲ���\r\n");
        printf("3.����Id����\r\n");
        printf("��ѡ��: ");
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
            printf("�Բ�����������������ѡ��\r\n");
            system("pause");
            break;
        }
        }
    }
}

// չʾ��������
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
        printf("��������\r\n");
    }
}

//���ݲ��� ����
void findByContentInput()
{

}

//Id���� ����
void findByIdInput()
{
    printf("������Ҫ���ҵ�Id:");
    int dataId = inputInt();

    //��ȡ�洢��Ϣ
    Type data[NAME_LENGTH + PHONE_LENGTH + 2] = { 0 };
    if (!findDataById(dataId, data))
    {
        printf("�Բ���,���Ҳ���������\r\n");
        return;
    }
    printf("������������: \r\n");
    printData(dataId, data);
}


//��ӡĳһ������
void printData(int dataId, PType pData)
{
    printf("[%d]\t", dataId);
    printf("%s\t",pData);
    printf("%s\r\n", pData + strlen(pData) + 1);
}



