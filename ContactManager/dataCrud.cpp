#include "dataCrud.h"

/*************************static function*****************************/

// ��ýڵ�洢��Ӧλ��
static int getStoreIndex(int nodeIndex)
{
    return sizeof(int) + sizeof(Node) * nodeIndex;
}

static int addNewNode(PNode pInsertNode, int nodeIndex,int nodeNum)
{
    if (pInsertNode->index + pInsertNode->size > BUF_LENGTH)
    { //���ݿռ䲻��
        return FAIL;
    }
    
    Node node = { 0 };
    for (int i = nodeNum; i > nodeIndex; --i)
    {
        //������
        seek(NODE, getStoreIndex(i), SEEK_SET);
        read(NODE, sizeof(Node), 1, &node);
        //��������
        seek(NODE, getStoreIndex(i+1), SEEK_SET);
        write(NODE, sizeof(Node), 1, &node);
    }

    //����ڵ�
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    write(NODE, sizeof(Node), 1, pInsertNode);

    //���½ڵ���
    nodeNum++;
    seek(NODE, 0, SEEK_SET);
    write(NODE, sizeof(int), 1, &nodeNum);

    return SUCCESS;
}

// �������������Ч��
static int checkIndex(int nodeIndex)
{
    int nodeNum = 0;
    seek(NODE, 0, SEEK_SET);
    read(NODE, sizeof(int), 1, &nodeNum); //��ȡ�ڵ㳤��

    if (nodeIndex < 0 || nodeIndex >= nodeNum)
    {
        return FAIL;
    }

    return SUCCESS;
}

// �������ݸ��½ڵ�
static int updateNodeByData(PNode pNode, PType pData)
{
    pNode->memberLen[0] = strlen(pData->name) + 1;
    pNode->memberLen[1] = strlen(pData->phone) + 1;
    pNode->memberLen[2] = sizeof(pData->shortPhone);
    
    return SUCCESS;
}

// ���ݽڵ�д����
static int writeBufByNode(PNode pNode, PType pData)
{
    seek(BUF, pNode->index, SEEK_SET);
    write(BUF, sizeof(char), pNode->memberLen[0], pData->name);
    write(BUF, sizeof(char), pNode->memberLen[1], pData->phone);
    write(BUF, sizeof(char), pNode->memberLen[2], &pData->shortPhone);

    return SUCCESS;
}

// ���ݽڵ������
static int readBufByNode(PNode pNode, PType pData)
{
    seek(BUF, pNode->index, SEEK_SET);
    read(BUF, sizeof(char), pNode->memberLen[0], pData->name);
    read(BUF, sizeof(char), pNode->memberLen[1], pData->phone);
    read(BUF, sizeof(char), pNode->memberLen[2], &pData->shortPhone);
    
    return SUCCESS;
}

/*************************interface*****************************/

// ��ʼ��
int crudInit()
{
    return storeInit();
}

// �˳�����
int crudExit()
{
    return storeExit();
}

// ������������ռ�õĿռ�
int calcDataSize(PType pData)
{
    int size = 0;
    size += strlen(pData->name) + 1; //����\0
    size += strlen(pData->phone) + 1; //����\0
    size += sizeof(pData->shortPhone);
    return size;
}

// ��ȡһ��洢λ��
int getStorageInfo(int dataSize)
{

    int nodeNum = 0;

    if (!open(NODE, READ_WRITE_MODE))
    {
        return INDEX_ERR; //�ļ���ʧ��
    }
    
    seek(NODE, 0, SEEK_SET);
    read(NODE,sizeof(int),1,&nodeNum); //��ȡ�ڵ㳤��
    Node node = { 0 };
    int dataIndex = 0;
    int findIndex = 0;
    
    for(; findIndex < nodeNum;++findIndex)
    {
        read(NODE,sizeof(Node),1,&node);
        //Ѱ�Һ�������
        if (node.index - dataIndex >= dataSize)
        {
            break;
        }
        dataIndex = node.index + node.size;
    }
    
    node.index = dataIndex;
    node.size = dataSize;

    //�����½ڵ�
    if (!addNewNode(&node,findIndex, nodeNum))
    {
        findIndex = INDEX_ERR; //����
    }

    close(NODE); //�ر��ļ�
    
    return findIndex;
}

// �������
int addInputData(int nodeIndex, PType pData)
{
    int retMsg = SUCCESS;

    //�򿪽ڵ��ļ�
    if (!open(NODE, READ_WRITE_MODE))
    {
        retMsg = FAIL;
        goto addEnd;
    }

    if (!checkIndex(nodeIndex)) //�������
    {
        retMsg = FAIL;
        goto addEnd;
    }

    //��ȡ�ڵ�����
    Node node = { 0 };
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    read(NODE, sizeof(Node), 1, &node);

    //�������ݽڵ�
    updateNodeByData(&node, pData);

    //�������ļ�
    if (!open(BUF, READ_WRITE_MODE))
    {
        retMsg = FAIL;
        goto addEnd;
    }
    
    //д������
    writeBufByNode(&node, pData);
    
    //д��ڵ�
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    write(NODE, sizeof(Node), 1, &node);

addEnd:
    close(NODE);
    close(BUF);
    
    return retMsg;
}

//ɾ������
int deleteDataBuf(int dataId)
{
    int nodeIndex = dataId - 1;

    //�򿪽ڵ��ļ�
    if (!open(NODE, READ_WRITE_MODE))
    {
        return FAIL;
    }

    int nodeNum = 0;
    seek(NODE, 0, SEEK_SET);
    read(NODE, sizeof(int), 1, &nodeNum); //��ȡ�ڵ㳤��

    if (nodeIndex < 0 || nodeIndex >= nodeNum) //�������
    {
        close(NODE);
        return FAIL;
    }

    //ɾ���ڵ�����
    Node node = { 0 };
    for (int i = nodeIndex + 1; i < nodeNum; ++i)
    {
        //������
        seek(NODE, getStoreIndex(i), SEEK_SET);
        read(NODE, sizeof(Node), 1, &node);
        //��������
        seek(NODE, getStoreIndex(i - 1), SEEK_SET);
        write(NODE, sizeof(Node), 1, &node);
    }

    //���½ڵ���
    nodeNum--;
    seek(NODE, 0, SEEK_SET);
    write(NODE, sizeof(int), 1, &nodeNum);

    close(NODE);
    return SUCCESS;
}

//��������
int updateDataBuf(int dataId, PType pContent, int conSize)
{
    int retMsg = SUCCESS;
    int nodeIndex = dataId - 1;

    //�򿪽ڵ��ļ�
    if (!open(NODE, READ_WRITE_MODE))
    {
        retMsg = FAIL;
        goto updateEnd;
    }

    if (!checkIndex(nodeIndex)) //�������
    {
        retMsg = FAIL;
        goto updateEnd;
    }

    //��ȡ�ڵ�����
    Node node = { 0 };
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    read(NODE, sizeof(Node), 1, &node);

    if (conSize > node.size) //����̫��
    {
        retMsg = FAIL;
        goto updateEnd;
    }

    //���½ڵ�����
    node.size = conSize;
    updateNodeByData(&node, pContent);
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    write(NODE, sizeof(Node), 1, &node);

    //�������ļ�
    if (!open(BUF, READ_WRITE_MODE))
    {
        retMsg = FAIL;
        goto updateEnd;
    }

    //����Դ����
    writeBufByNode(&node,pContent);

updateEnd:
    close(NODE);
    close(BUF);

    return retMsg;
}


//����Id��������
int findDataById(int dataId, PType pData)
{
    int retMsg = SUCCESS;
    int nodeIndex = dataId - 1;
    
    //�򿪽ڵ��ļ�
    if (!open(NODE, READ_MODE))
    {
        retMsg = FAIL;
        goto findIdEnd;
    }

    if (!checkIndex(nodeIndex)) //�������
    {
        retMsg = FAIL;
        goto findIdEnd;
    }

    //��ȡ�ڵ�����
    Node node = { 0 };
    seek(NODE, getStoreIndex(nodeIndex), SEEK_SET);
    read(NODE, sizeof(Node), 1, &node);

    //�������ļ�
    if (!open(BUF, READ_MODE))
    {
        retMsg = FAIL;
        goto findIdEnd;
    }

    //������
    readBufByNode(&node,pData);

findIdEnd:
    close(NODE);
    close(BUF);

    return retMsg;
}

//��ȡ�����ܹ�����
int getDataTotalNum()
{
    //�򿪽ڵ��ļ�
    if (!open(NODE, READ_MODE))
    {
        return 0;
    }

    int nodeNum = 0;
    seek(NODE, 0, SEEK_SET);
    read(NODE, sizeof(int), 1, &nodeNum); //��ȡ�ڵ㳤��

    close(NODE);
    return nodeNum;
}
