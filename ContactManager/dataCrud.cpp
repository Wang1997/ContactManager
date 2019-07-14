#include "dataCrud.h"

/*************************static function*****************************/

// 获得节点存储对应位置
static int getStoreIndex(int nodeIndex)
{
    return sizeof(int) + sizeof(Node) * nodeIndex;
}

static int addNewNode(PNode pInsertNode, int nodeIndex,int nodeNum)
{
    if (pInsertNode->index + pInsertNode->length > BUF_LENGTH)
    { //数据空间不够
        return FAIL;
    }
    
    Node node = { 0,0 };
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

// 获取一块存储位置
int getStorageInfo(int dataLength)
{

    int nodeNum = 0;

    if (!open(NODE, READ_WRITE_MODE))
    {
        return INDEX_ERR; //文件打开失败
    }
    
    seek(NODE, 0, SEEK_SET);
    read(NODE,sizeof(int),1,&nodeNum); //读取节点长度
    Node node = {0,0};
    int dataIndex = 0;
    int findIndex = 0;
    
    for(; findIndex < nodeNum;++findIndex)
    {
        read(NODE,sizeof(Node),1,&node);
        //寻找合适条件
        if (node.index - dataIndex >= dataLength)
        {
            break;
        }
        dataIndex = node.index + node.length;
    }
    
    node.index = dataIndex;
    node.length = dataLength;

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
    if (!open(NODE, READ_MODE))
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
    Node node = { 0,0 };
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    read(NODE, sizeof(Node), 1, &node);

    //打开数据文件
    if (!open(BUF, READ_WRITE_MODE))
    {
        retMsg = FAIL;
        goto addEnd;
    }
    
    //写入数据
    seek(BUF, node.index * sizeof(Type), SEEK_SET);
    write(BUF, sizeof(Type), node.length, pData);

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
    Node node = { 0,0 };
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
    Node node = { 0,0 };
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    read(NODE, sizeof(Node), 1, &node);

    //打开数据文件
    if (!open(BUF, READ_MODE))
    {
        retMsg = FAIL;
        goto findIdEnd;
    }

    //读数据
    seek(BUF, node.index * sizeof(Type), SEEK_SET);
    read(BUF, sizeof(Type), node.length, pData);

findIdEnd:
    close(NODE);
    close(BUF);

    return retMsg;
}
