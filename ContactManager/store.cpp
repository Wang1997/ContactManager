#include "store.h"

// 文件指针
static FILE *stream[SRC_NUM] = {NULL};
// 对应文件路径
static char *filePath[] = {"MyData.bin","node.bin"};

//初始化
int storeInit()
{
    //判断buf文件
    int ret = storeOpen(DATA, READ_MODE);
    if (ret == FAIL)
    {
        if (!storeOpen(DATA, WRITE_MODE))
        {
            return FAIL;
        }
    }
    storeClose(DATA);
    //判断node文件
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


//退出 
int storeExit()
{ //关闭文件操作
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

//打开
int storeOpen(SRC src, const char *mode)
{
    if (stream[src] != NULL)
    {
        return FAIL; //已经打开
    }

    stream[src] = fopen(filePath[src], mode);

    if (stream[src] == NULL)
    {
        return FAIL;
    }

    return SUCCESS;
}

//关闭
int storeClose(SRC src)
{
    if (stream[src] == NULL)
    {
        return SUCCESS; //已经关闭
    }

    if (fclose(stream[src]))
    {
        return FAIL; //关闭失败
    }

    stream[src] = NULL;
    return SUCCESS;
}

// 读取数据 返回实际读取值
int storeRead(SRC src, int size, void *buffer)
{
    if (stream[src] == NULL)
    {
        return 0; 
    }

    return fread(buffer, size , 1, stream[src]);
}

// 写入数据 返回实际写入值
int storeWrite(SRC src, int size, const void *buffer)
{
    if (stream[src] == NULL)
    {
        return 0;
    }

    return fwrite(buffer, size, 1, stream[src]);
}

//设置读写位置
int storeSeek(SRC src, long offset)
{
    if (stream[src] == NULL)
    {
        return FAIL;
    }
    return !fseek(stream[src],offset, SEEK_SET);
}