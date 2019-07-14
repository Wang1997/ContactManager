#include "store.h"

// �ļ�ָ��
static FILE *stream[SRC_NUM] = {NULL};
// ��Ӧ�ļ�·��
static char *filePath[] = {"buf.bin","node.bin"};

//��ʼ��
int storeInit()
{
    //�ж�buf�ļ�
    int ret = open(BUF, READ_MODE);
    if (ret == FAIL)
    {
        if (!open(BUF, WRITE_MODE))
        {
            return FAIL;
        }
    }
    close(BUF);
    //�ж�node�ļ�
    ret = open(NODE, READ_MODE);
    if (ret == FAIL)
    {
        if (!open(NODE, WRITE_MODE))
        {
            return FAIL;
        }
        write(NODE,sizeof(int),1, &ret);
    }
    close(NODE);
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
        }
    }
    return SUCCESS;
}

//��
int open(SRC src, const char *mode)
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
int close(SRC src)
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
int read(SRC src, int size, int count, void *buffer)
{
    if (stream[src] == NULL)
    {
        return 0; //�ر�
    }

    return fread(buffer,size,count, stream[src]);
}

// д������ ����ʵ��д��ֵ
int write(SRC src, int size, int count, const void *buffer)
{
    if (stream[src] == NULL)
    {
        return 0; //�ر�
    }

    return fwrite(buffer,size,count, stream[src]);
}

//���ö�дλ��
int seek(SRC src, long offset, int origin)
{
    if (stream[src] == NULL)
    {
        return FAIL; //�ر�
    }
    return !fseek(stream[src],offset,origin);
}