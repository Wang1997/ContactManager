#include "dataCrud.h"

//数据节点 对内结构体
typedef struct node
{
    int index; //对应数据源索引
    int size;//所占总空间
    int memberLen[3]; //所对应成员占用长度
}Node, *PNode;

/*************************static function*****************************/

// 获得节点存储对应位置
static int getStoreIndex(int nodeIndex)
{
    return sizeof(int) + sizeof(Node) * nodeIndex;
}

static int addNewNode(PNode pInsertNode, int nodeIndex,int nodeNum)
{
    if (pInsertNode->index + pInsertNode->size > BUF_LENGTH)
    { //数据空间不够
        return FAIL;
    }
    
    Node node = { 0 };
    for (int i = nodeNum; i > nodeIndex; --i)
    {
        //读数据
        storeSeek(NODE, getStoreIndex(i-1));
        storeRead(NODE, sizeof(Node), &node);
        //拷贝数据
        storeSeek(NODE, getStoreIndex(i));
        storeWrite(NODE, sizeof(Node), &node);
    }

    //插入节点
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeWrite(NODE, sizeof(Node), pInsertNode);

    //更新节点数
    nodeNum++;
    storeSeek(NODE, 0);
    storeWrite(NODE, sizeof(int), &nodeNum);

    return SUCCESS;
}

// 检测索引数据有效性
static int checkIndex(int nodeIndex)
{
    int nodeNum = 0;
    storeSeek(NODE, 0);
    storeRead(NODE, sizeof(int), &nodeNum); //读取节点长度

    if (nodeIndex < 0 || nodeIndex >= nodeNum)
    {
        return FAIL;
    }

    return SUCCESS;
}

// 根据数据更新节点
static int updateNodeByData(PNode pNode, PType pData)
{
    pNode->memberLen[0] = strlen(pData->name) + 1;
    pNode->memberLen[1] = strlen(pData->phone) + 1;
    pNode->memberLen[2] = sizeof(pData->shortPhone);
    
    return SUCCESS;
}

// 根据节点写数据
static int writeDataByNode(PNode pNode, PType pData)
{
    storeSeek(DATA, pNode->index);
    storeWrite(DATA, pNode->memberLen[0], pData->name);
    storeWrite(DATA, pNode->memberLen[1], pData->phone);
    storeWrite(DATA, pNode->memberLen[2], &pData->shortPhone);

    return SUCCESS;
}

// 根据节点读数据
static int readDataByNode(PNode pNode, PType pData)
{
    storeSeek(DATA, pNode->index);
    storeRead(DATA, pNode->memberLen[0], pData->name);
    storeRead(DATA, pNode->memberLen[1], pData->phone);
    storeRead(DATA, pNode->memberLen[2], &pData->shortPhone);
    
    return SUCCESS;
}

//数据匹配
static int dataCompare(const char* src, const char* content)
{
    int srcLen = strlen(src);
    int conLen = strlen(content);

    if (conLen > srcLen)
    {
        return 0;
    }

    for (int i = 0; i < srcLen; ++i)
    {
        int index = i; //重复进行匹配
        int findFlag = 1;
        for (int j = 0; j < conLen; ++j)
        { 
            if (src[index] != content[j])
            {
                findFlag = 0;
                break;
            }
            index++;
        }
        if (findFlag)
        { //匹配成功
            return 1;
        }
    }

    return 0;
}

// 统计姓名
void statistical(char(*names)[NAME_LENGTH + 1], int *couns ,
                                            int *statisticalNum , char *name)
{
    int findFlag = 0;
    for (int i = 0; i < *statisticalNum; ++i)
    {
        if (strcmp(names[i], name) == 0) //匹配成功
        {
            couns[i]++; //对应的数量++
            findFlag = 1;
            break;
        }
    }

    if (!findFlag) //未找到
    {
        //将此姓名添加到末尾
        strcpy(names[*statisticalNum], name);
        couns[*statisticalNum] = 1;
        (*statisticalNum)++; //计数++
    }
}

/*************************interface*****************************/

// 初始化
int crudInit()
{
    return storeInit();
}

// 退出程序
int crudExit()
{
    return storeExit();
}

// 计算数据所需占用的空间
int calcDataSize(PType pData)
{
    int size = 0;
    size += strlen(pData->name) + 1; //加上\0
    size += strlen(pData->phone) + 1; //加上\0
    size += sizeof(pData->shortPhone);
    return size;
}

// 获取一块存储位置
int getStorageInfo(int dataSize)
{

    int nodeNum = 0;

    if (!storeOpen(NODE, READ_WRITE_MODE))
    {
        return INDEX_ERR; //文件打开失败
    }
    
    storeSeek(NODE, 0);
    storeRead(NODE, sizeof(int), &nodeNum); //读取节点长度
    Node node = { 0 };
    int dataIndex = 0;
    int findIndex = 0;
    
    for(; findIndex < nodeNum;++findIndex)
    {
        storeRead(NODE, sizeof(Node), &node);
        //寻找合适条件
        if (node.index - dataIndex >= dataSize)
        {
            break;
        }
        dataIndex = node.index + node.size;
    }
    
    node.index = dataIndex;
    node.size = dataSize;

    //开辟新节点
    if (!addNewNode(&node,findIndex, nodeNum))
    {
        findIndex = INDEX_ERR; //错误
    }

    storeClose(NODE); //关闭文件
    
    return findIndex;
}

// 添加数据
int addInputData(int nodeIndex, PType pData)
{
    int retMsg = FAIL;

    //打开节点文件
    if (!storeOpen(NODE, READ_WRITE_MODE))
    {
        goto addEnd;
    }

    if (!checkIndex(nodeIndex)) //检查索引
    {
        goto addEnd;
    }

    //读取节点数据
    Node node = { 0 };
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeRead(NODE, sizeof(Node), &node);

    //更新数据节点
    updateNodeByData(&node, pData);

    //打开数据文件
    if (!storeOpen(DATA, READ_WRITE_MODE))
    {
        goto addEnd;
    }
    
    //写入数据
    writeDataByNode(&node, pData);
    
    //写入节点
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeWrite(NODE, sizeof(Node), &node);
    
    retMsg = SUCCESS;

addEnd:
    storeClose(DATA);
    storeClose(NODE);
    
    return retMsg;
}

//删除数据
int deleteDataBuf(int dataId)
{
    int nodeIndex = dataId - 1;

    //打开节点文件
    if (!storeOpen(NODE, READ_WRITE_MODE))
    {
        return FAIL;
    }

    int nodeNum = 0;
    storeSeek(NODE, 0);
    storeRead(NODE, sizeof(int), &nodeNum); //读取节点长度

    if (nodeIndex < 0 || nodeIndex >= nodeNum) //检查索引
    {
        storeClose(NODE);
        return FAIL;
    }

    //删除节点数据
    Node node = { 0 };
    for (int i = nodeIndex + 1; i < nodeNum; ++i)
    {
        //读数据
        storeSeek(NODE, getStoreIndex(i));
        storeRead(NODE, sizeof(Node), &node);
        //拷贝数据
        storeSeek(NODE, getStoreIndex(i - 1));
        storeWrite(NODE, sizeof(Node), &node);
    }

    //更新节点数
    nodeNum--;
    storeSeek(NODE, 0);
    storeWrite(NODE, sizeof(int), &nodeNum);

    storeClose(NODE);
    return SUCCESS;
}

//更新数据
int updateDataBuf(int dataId, PType pContent, int conSize)
{
    int retMsg = FAIL;
    int nodeIndex = dataId - 1;

    //打开节点文件
    if (!storeOpen(NODE, READ_WRITE_MODE))
    {
        goto updateEnd;
    }

    if (!checkIndex(nodeIndex)) //检查索引
    {
        goto updateEnd;
    }

    //读取节点数据
    Node node = { 0 };
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeRead(NODE, sizeof(Node), &node);

    if (conSize > node.size) //长度太长
    {
        goto updateEnd;
    }

    //更新节点数据
    node.size = conSize;
    updateNodeByData(&node, pContent);
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeWrite(NODE, sizeof(Node), &node);

    //打开数据文件
    if (!storeOpen(DATA, READ_WRITE_MODE))
    {
        goto updateEnd;
    }

    //更新源数据
    writeDataByNode(&node,pContent);

    retMsg = SUCCESS;

updateEnd:
    storeClose(DATA);
    storeClose(NODE);

    return retMsg;
}


//根据Id查找内容
int findDataById(int dataId, PType pData)
{
    int retMsg = FAIL;
    int nodeIndex = dataId - 1;
    
    //打开节点文件
    if (!storeOpen(NODE, READ_MODE))
    {
        goto findIdEnd;
    }

    if (!checkIndex(nodeIndex)) //检查索引
    {
        goto findIdEnd;
    }

    //读取节点数据
    Node node = { 0 };
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeRead(NODE, sizeof(Node), &node);

    //打开数据文件
    if (!storeOpen(DATA, READ_MODE))
    {
        goto findIdEnd;
    }

    //读数据
    readDataByNode(&node,pData);

    retMsg = SUCCESS;

findIdEnd:
    storeClose(DATA);
    storeClose(NODE);

    return retMsg;
}

//匹配数据
int checkDataByContent(int dataId, char *content, PType pData, int type)
{
    int retMsg = FAIL;
    int nodeIndex = dataId - 1;

    //打开节点文件
    if (!storeOpen(NODE, READ_MODE))
    {
        goto checkNameEnd;
    }

    if (!checkIndex(nodeIndex)) //检查索引
    {
        goto checkNameEnd;
    }

    //读取节点数据
    Node node = { 0 };
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeRead(NODE, sizeof(Node), &node);

    //打开数据文件
    if (!storeOpen(DATA, READ_MODE))
    {
        goto checkNameEnd;
    }

    //读数据
    readDataByNode(&node, pData);
    
    switch (type)
    {
        case FIND_NAME :
        {
            if (dataCompare(pData->name, content))
            {
                retMsg = SUCCESS; //查找成功
            }
            break;
        }
        case FIND_PHONE:
        {
            if (dataCompare(pData->phone, content))
            {
                retMsg = SUCCESS; //查找成功
            }
            break;
        }
        default:
        {
            break;
        } 
    }
    

checkNameEnd:
    storeClose(DATA);
    storeClose(NODE);

    return retMsg;
}

int statisticalData(char (*names)[NAME_LENGTH + 1], int *counts)
{
    int statisticalNum = 0;

    //打开节点文件
    if (!storeOpen(NODE, READ_MODE))
    {
        goto statisticalEnd;
    }

    //打开数据文件
    if (!storeOpen(DATA, READ_MODE))
    {
        goto statisticalEnd;
    }

    int nodeNum = 0;
    storeSeek(NODE, 0);
    storeRead(NODE, sizeof(int), &nodeNum); //读取节点长度

    
    Node node = { 0 };
    Type data = { 0 };
    for (int i = 0; i < nodeNum; ++i)
    {
        //读取节点数据
        storeSeek(NODE, getStoreIndex(i));
        storeRead(NODE, sizeof(Node), &node);
        //读取对应数据
        readDataByNode(&node, &data);
        statistical(names,counts,&statisticalNum,data.name); //统计
    }
    
statisticalEnd:
    storeClose(DATA);
    storeClose(NODE);

    return statisticalNum;
}

//获取数据总共个数
int getDataTotalNum()
{
    //打开节点文件
    if (!storeOpen(NODE, READ_MODE))
    {
        return 0;
    }

    int nodeNum = 0;
    storeSeek(NODE, 0);
    storeRead(NODE, sizeof(int), &nodeNum); //读取节点长度

    storeClose(NODE);
    return nodeNum;
}


//获取索引结构体数据
int getIndexInfo(int nodeIndex,PIndexInfo pIndexInfo)
{
    int retMsg = FAIL;

    //打开节点文件
    if (!storeOpen(NODE, READ_MODE))
    {
        goto getIndexEnd;
    }

    if (!checkIndex(nodeIndex)) //检查索引
    {
        goto getIndexEnd;
    }

    //读取节点数据
    Node node = { 0 };
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeRead(NODE, sizeof(Node), &node);

    pIndexInfo->index = node.index;
    pIndexInfo->size = node.size;

    retMsg = SUCCESS;

getIndexEnd:
    storeClose(NODE);
    
    return retMsg;
}


//碎片整理
int defragment()
{
    int retMsg = FAIL;

    //打开节点文件
    if (!storeOpen(NODE, READ_WRITE_MODE))
    {
        goto defragmentEnd;
    }

    //打开数据文件
    if (!storeOpen(DATA, READ_WRITE_MODE))
    {
        goto defragmentEnd;
    }

    int nodeNum = 0;
    storeSeek(NODE, 0);
    storeRead(NODE, sizeof(int), &nodeNum); //读取节点长度
    
    int lastIndex = 0;
    Type data = {0};
    for (int i = 0; i < nodeNum; ++i)
    {
        //读取节点数据
        Node node = { 0 };
        storeSeek(NODE, getStoreIndex(i));
        storeRead(NODE, sizeof(Node), &node);
        //读取数据源
        readDataByNode(&node, &data);

        //需要进行碎片整理
        if (node.index != lastIndex)
        {
            node.index = lastIndex;
            //更新节点
            storeSeek(NODE, getStoreIndex(i));
            storeWrite(NODE, sizeof(Node), &node);
            //更新源数据
            writeDataByNode(&node, &data);
        }

        lastIndex = node.index + node.size; //上个数据
    }

    retMsg = SUCCESS;

defragmentEnd:
    storeClose(DATA);
    storeClose(NODE);

    return retMsg;
}
