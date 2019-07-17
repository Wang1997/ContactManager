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
void inputStr(char *data, int maxLen)
{
    rewind(stdin);
    fgets(data, maxLen, stdin); //��������

    int length = strlen(data);
    if (data[length - 1] == '\n')
    {
        data[length - 1] = 0;
    }
    
    return;
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
    Type data = { 0 };

    printf("��������Ҫ��ӵ�������");
    inputStr(data.name, NAME_LENGTH + 1);
    printf("�������Ӧ�ĳ��ţ�");
    inputStr(data.phone, PHONE_LENGTH + 1);
    printf("�������Ӧ�Ķ̺ţ�");
    data.shortPhone = inputInt();

    //��ȡ�洢��Ϣ
    int dataSize = calcDataSize(&data);
    int nodeIndex = getStorageInfo(dataSize);
    if (nodeIndex == INDEX_ERR)
    {
        printf("�Բ���,ʣ��ռ䲻��\r\n");
        return;
    }

    if (addInputData(nodeIndex, &data))
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
    printf("������Ҫɾ���ı��:");
    int dataId = inputInt();

    Type data = { 0 };
    //��ȡ�洢��Ϣ
    if (!findDataById(dataId, &data))
    {
        printf("�Բ���,���Ҳ���������\r\n");
        return;
    }
    printf("��ɾ����������: \r\n");
    printData(dataId, &data);

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
    printf("������Ҫ���µı��:");
    int dataId = inputInt();

    //��ȡ�洢��Ϣ
    Type data = { 0 };
    if (!findDataById(dataId, &data))
    {
        printf("�Բ���,���Ҳ���������\r\n");
        return;
    }
    printf("�������������: \r\n");
    printData(dataId, &data);
    
    int oldLength = calcDataSize(&data); //�����ݳ���

    printf("��������Ҫ���µ�������");
    inputStr(data.name, NAME_LENGTH + 1);
    printf("��������µĳ��ţ�");
    inputStr(data.phone, PHONE_LENGTH + 1);
    printf("��������µĶ̺ţ�");
    data.shortPhone = inputInt();

    int newLength = calcDataSize(&data); //�����ݳ���

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
        if (!addInputData(nodeIndex, &data))
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
        if (!updateDataBuf(dataId, &data, newLength))
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
        printf("2.������������\r\n");
        printf("3.���ݳ��Ų���\r\n");
        printf("4.���ݱ�Ų���\r\n");
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
        printf("��������\r\n");
    }
}

//���ֲ��� ����
void findByNameInput()
{
    char name[NAME_LENGTH + 1] = {0};
    printf("��������Ҫ���ҵ�����(ģ��)��");
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
        printf("��������\r\n");
    }
}

//���Ų��� ����
void findByPhoneInput()
{
    char phone[PHONE_LENGTH + 1] = { 0 };
    printf("��������Ҫ���ҵĳ���(ģ��)��");
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
        printf("��������\r\n");
    }
}

//Id���� ����
void findByIdInput()
{
    printf("������Ҫ���ҵı��:");
    int dataId = inputInt();

    //��ȡ�洢��Ϣ
    Type data = { 0 };
    if (!findDataById(dataId, &data))
    {
        printf("�Բ���,���Ҳ���������\r\n");
        return;
    }
    printf("������������: \r\n");
    printData(dataId, &data);
}

//ͳ�ƽ���
void statisticalView()
{
    
    int totalNum = getDataTotalNum(); //��ȡ����

    if (totalNum <= 0)
    {
        printf("�Բ���, �������ݿ�ͳ��\r\n");
        return;
    }
    
    //������� ����ָ�� char [NAME_LENGTH + 1]
    char (*names)[NAME_LENGTH + 1] = NULL;
    names = (char(*)[NAME_LENGTH + 1])malloc(totalNum * (NAME_LENGTH+1));
    //��ų��ִ���
    int *counts = NULL;
    counts = (int*)malloc(totalNum * sizeof(int));
    if (names == NULL || counts == NULL)
    {
        printf("�Բ���,����ͳ�Ʒ����ڴ�����\r\n");
        return;
    }
    
    memset(names,0, totalNum * (NAME_LENGTH + 1));
    memset(counts, 0, totalNum * sizeof(int));

    int statisticalNum = statisticalData(names, counts);
    if (statisticalNum <= 0)
    {
        printf("�Բ���,����ͳ��ʧ��\r\n");
        goto FREE_END;
    }

    system("cls");
    printf("ͳ����������: \r\n");
    printf("����%d����ϵ����Ϣ\r\n", statisticalNum);
    for (int i = 0; i < statisticalNum; ++i)
    {
        printf("����: %s ������%d��,����Ϊ%.2lf%%\r\n", names[i],counts[i],
            counts[i] * 100.0 / totalNum);
    }

FREE_END:
    //�ͷ��ڴ�
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

//�洢��Ϣ����
void storeInfoView()
{
    while (1)
    {
        system("cls");
        printf("  �洢��Ϣ  \r\n");
        printf("1.�洢��Դ�ֲ����\r\n");
        printf("2.��Ƭ����\r\n");
        printf("��ѡ��: ");
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
                    printf("��Ƭ����ɹ�\r\n");
                }
                else
                {
                    printf("��Ƭ����ʧ��\r\n");
                }
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

//չʾ�洢��Դͼ
void showStorageResource()
{
    int totalNum = getDataTotalNum(); //��ȡ����
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
                printf("��"); //����
                if (++totalSize % 10 == 0)
                    printf("\r\n");
            }
            for (int j = 0; j < indexInfo.size; ++j)
            {
                printf("��"); //ռ��
                if (++totalSize % 10 == 0)
                    printf("\r\n");
            }
            lastEndIndex = indexInfo.index + indexInfo.size;
        }
    }

    printf("\r\n");
    printf("�ܹ��ռ��С:0x%x,ʣ��0x%xδ����\r\n", BUF_LENGTH, BUF_LENGTH - totalSize);
}


//��ӡĳһ������
void printData(int dataId, PType pData)
{
    printf("���: %d\r\n", dataId);
    printf("����: %s\r\n",pData->name);
    printf("����: %s\r\n", pData->phone);
    printf("�̺�: %d\r\n", pData->shortPhone);
}



