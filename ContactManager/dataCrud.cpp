#include "dataCrud.h"

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
        seek(NODE, getStoreIndex(i), SEEK_SET);
        read(NODE, sizeof(Node), 1, &node);
        //拷贝数据
        seek(NODE, getStoreIndex(i+1), SEEK_SET);
        write(NODE, sizeof(Node), 1, &node);
    }

    //插入节点
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    write(NODE, sizeof(Node), 1, pInsertNode);

    //更新节点数
    nodeNum++;
    seek(NODE, 0, SEEK_SET);
    write(NODE, sizeof(int), 1, &nodeNum);

    return SUCCESS;
}

// 检测索引数据有效性
static int checkIndex(int nodeIndex)
{
    int nodeNum = 0;
    seek(NODE, 0, SEEK_SET);
    read(NODE, sizeof(int), 1, &nodeNum); //读取节点长度

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
static int writeBufByNode(PNode pNode, PType pData)
{
    seek(BUF, pNode->index, SEEK_SET);
    write(BUF, sizeof(char), pNode->memberLen[0], pData->name);
    write(BUF, sizeof(char), pNode->memberLen[1], pData->phone);
    write(BUF, sizeof(char), pNode->memberLen[2], &pData->shortPhone);

    return SUCCESS;
}

// 根据节点读数据
static int readBufByNode(PNode pNode, PType pData)
{
    seek(BUF, pNode->index, SEEK_SET);
    read(BUF, sizeof(char), pNode->memberLen[0], pData->name);
    read(BUF, sizeof(char), pNode->memberLen[1], pData->phone);
    read(BUF, sizeof(char), pNode->memberLen[2], &pData->shortPhone);
    
    return SUCCESS;
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

    if (!open(NODE, READ_WRITE_MODE))
    {
        return INDEX_ERR; //文件打开失败
    }
    
    seek(NODE, 0, SEEK_SET);
    read(NODE,sizeof(int),1,&nodeNum); //读取节点长度
    Node node = { 0 };
    int dataIndex = 0;
    int findIndex = 0;
    
    for(; findIndex < nodeNum;++findIndex)
    {
        read(NODE,sizeof(Node),1,&node);
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

    close(NODE); //关闭文件
    
    return findIndex;
}

// 添加数据
int addInputData(int nodeIndex, PType pData)
{
    int retMsg = SUCCESS;

    //打开节点文件
    if (!open(NODE, READ_WRITE_MODE))
    {
        retMsg = FAIL;
        goto addEnd;
    }

    if (!checkIndex(nodeIndex)) //检查索引
    {
        retMsg = FAIL;
        goto addEnd;
    }

    //读取节点数据
    Node node = { 0 };
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    read(NODE, sizeof(Node), 1, &node);

    //更新数据节点
    updateNodeByData(&node, pData);

    //打开数据文件
    if (!open(BUF, READ_WRITE_MODE))
    {
        retMsg = FAIL;
        goto addEnd;
    }
    
    //写入数据
    writeBufByNode(&node, pData);
    
    //写入节点
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    write(NODE, sizeof(Node), 1, &node);

addEnd:
    close(NODE);
    close(BUF);
    
    return retMsg;
}

//删除数据
int deleteDataBuf(int dataId)
{
    int nodeIndex = dataId - 1;

    //打开节点文件
    if (!open(NODE, READ_WRITE_MODE))
    {
        return FAIL;
    }

    int nodeNum = 0;
    seek(NODE, 0, SEEK_SET);
    read(NODE, sizeof(int), 1, &nodeNum); //读取节点长度

    if (nodeIndex < 0 || nodeIndex >= nodeNum) //检查索引
    {
        close(NODE);
        return FAIL;
    }

    //删除节点数据
    Node node = { 0 };
    for (int i = nodeIndex + 1; i < nodeNum; ++i)
    {
        //读数据
        seek(NODE, getStoreIndex(i), SEEK_SET);
        read(NODE, sizeof(Node), 1, &node);
        //拷贝数据
        seek(NODE, getStoreIndex(i - 1), SEEK_SET);
        write(NODE, sizeof(Node), 1, &node);
    }

    //更新节点数
    nodeNum--;
    seek(NODE, 0, SEEK_SET);
    write(NODE, sizeof(int), 1, &nodeNum);

    close(NODE);
    return SUCCESS;
}

//更新数据
int updateDataBuf(int dataId, PType pContent, int conSize)
{
    int retMsg = SUCCESS;
    int nodeIndex = dataId - 1;

    //打开节点文件
    if (!open(NODE, READ_WRITE_MODE))
    {
        retMsg = FAIL;
        goto updateEnd;
    }

    if (!checkIndex(nodeIndex)) //检查索引
    {
        retMsg = FAIL;
        goto updateEnd;
    }

    //读取节点数据
    Node node = { 0 };
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    read(NODE, sizeof(Node), 1, &node);

    if (conSize > node.size) //长度太长
    {
        retMsg = FAIL;
        goto updateEnd;
    }

    //更新节点数据
    node.size = conSize;
    updateNodeByData(&node, pContent);
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    write(NODE, sizeof(Node), 1, &node);

    //打开数据文件
    if (!open(BUF, READ_WRITE_MODE))
    {
        retMsg = FAIL;
        goto updateEnd;
    }

    //更新源数据
    writeBufByNode(&node,pContent);

updateEnd:
    close(NODE);
    close(BUF);

    return retMsg;
}


//根据Id查找内容
int findDataById(int dataId, PType pData)
{
    int retMsg = SUCCESS;
    int nodeIndex = dataId - 1;
    
    //打开节点文件
    if (!open(NODE, READ_MODE))
    {
        retMsg = FAIL;
        goto findIdEnd;
    }

    if (!checkIndex(nodeIndex)) //检查索引
    {
        retMsg = FAIL;
        goto findIdEnd;
    }

    //读取节点数据
    Node node = { 0 };
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    read(NODE, sizeof(Node), 1, &node);

    //打开数据文件
    if (!open(BUF, READ_MODE))
    {
        retMsg = FAIL;
        goto findIdEnd;
    }

    //读数据
    readBufByNode(&node,pData);

findIdEnd:
    close(NODE);
    close(BUF);

    return retMsg;
}

//获取数据总共个数
int getDataTotalNum()
{
    //打开节点文件
    if (!open(NODE, READ_MODE))
    {
        return 0;
    }

    int nodeNum = 0;
    seek(NODE, 0, SEEK_SET);
    read(NODE, sizeof(int), 1, &nodeNum); //读取节点长度

    close(NODE);
    return nodeNum;
}
