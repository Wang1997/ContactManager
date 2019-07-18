#include "store.h"

// �ļ�ָ��
static FILE *stream[SRC_NUM] = {NULL};
// ��Ӧ�ļ�·��
static char *filePath[] = {"MyData.bin","node.bin"};

//��ʼ��
int storeInit()
{
    //�ж�buf�ļ�
    int ret = storeOpen(DATA, READ_MODE);
    if (ret == FAIL)
    {
        if (!storeOpen(DATA, WRITE_MODE))
        {
            return FAIL;
        }
    }
    storeClose(DATA);
    //�ж�node�ļ�
    ret = storeOpen(NODE, READ_MODE);
    if (ret == FAIL)
    {
        if (!storeOpen(NODE, WRITE_MODE))
        {
            return FAIL;
        }
        storeWrite(NODE, sizeof(int), &ret);
    }
    storeClose(NODE);
    return SUCCESS;
}


//�˳� 
int storeExit()
{ //�ر��ļ�����
    for (int i = 0; i < SRC_NUM; ++i)
    {
        if (stream[i] != NULL)
        {
            fclose(stream[i]);
            stream[i] = NULL;
        }
    }
    return SUCCESS;
}

//��
int storeOpen(SRC src, const char *mode)
{
    if (stream[src] != NULL)
    {
        return FAIL; //�Ѿ���
    }

    stream[src] = fopen(filePath[src], mode);

    if (stream[src] == NULL)
    {
        return FAIL;
    }

    return SUCCESS;
}

//�ر�
int storeClose(SRC src)
{
    if (stream[src] == NULL)
    {
        return SUCCESS; //�Ѿ��ر�
    }

    if (fclose(stream[src]))
    {
        return FAIL; //�ر�ʧ��
    }

    stream[src] = NULL;
    return SUCCESS;
}

// ��ȡ���� ����ʵ�ʶ�ȡֵ
int storeRead(SRC src, int size, void *buffer)
{
    if (stream[src] == NULL)
    {
        return 0; 
    }

    return fread(buffer, size , 1, stream[src]);
}

// д������ ����ʵ��д��ֵ
int storeWrite(SRC src, int size, const void *buffer)
{
    if (stream[src] == NULL)
    {
        return 0;
    }

    return fwrite(buffer, size, 1, stream[src]);
}

//���ö�дλ��
int storeSeek(SRC src, long offset)
{
    if (stream[src] == NULL)
    {
        return FAIL;
    }
    return !fseek(stream[src],offset, SEEK_SET);
}