#include "store.h"

// 文件指针
static FILE *stream[SRC_NUM] = {NULL};
// 对应文件路径
static char *filePath[] = {"buf.bin","node.bin"};

//初始化
int storeInit()
{
    //判断buf文件
    int ret = open(BUF, READ_MODE);
    if (ret == FAIL)
    {
        if (!open(BUF, WRITE_MODE))
        {
            return FAIL;
        }
    }
    close(BUF);
    //判断node文件
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


//退出 
int storeExit()
{ //关闭文件操作
    for (int i = 0; i < SRC_NUM; ++i)
    {
        if (stream[i] != NULL)
        {
            fclose(stream[i]);
        }
    }
    return SUCCESS;
}

//打开
int open(SRC src, const char *mode)
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
int close(SRC src)
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
int read(SRC src, int size, int count, void *buffer)
{
    if (stream[src] == NULL)
    {
        return 0; //关闭
    }

    return fread(buffer,size,count, stream[src]);
}

// 写入数据 返回实际写入值
int write(SRC src, int size, int count, const void *buffer)
{
    if (stream[src] == NULL)
    {
        return 0; //关闭
    }

    return fwrite(buffer,size,count, stream[src]);
}

//设置读写位置
int seek(SRC src, long offset, int origin)
{
    if (stream[src] == NULL)
    {
        return FAIL; //关闭
    }
    return !fseek(stream[src],offset,origin);
}