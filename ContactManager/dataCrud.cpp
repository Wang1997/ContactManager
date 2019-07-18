#include "dataCrud.h"

//���ݽڵ� ���ڽṹ��
typedef struct node
{
    int index; //��Ӧ����Դ����
    int size;//��ռ�ܿռ�
    int memberLen[3]; //����Ӧ��Առ�ó���
}Node, *PNode;

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
        storeSeek(NODE, getStoreIndex(i-1));
        storeRead(NODE, sizeof(Node), &node);
        //��������
        storeSeek(NODE, getStoreIndex(i));
        storeWrite(NODE, sizeof(Node), &node);
    }

    //����ڵ�
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeWrite(NODE, sizeof(Node), pInsertNode);

    //���½ڵ���
    nodeNum++;
    storeSeek(NODE, 0);
    storeWrite(NODE, sizeof(int), &nodeNum);

    return SUCCESS;
}

// �������������Ч��
static int checkIndex(int nodeIndex)
{
    int nodeNum = 0;
    storeSeek(NODE, 0);
    storeRead(NODE, sizeof(int), &nodeNum); //��ȡ�ڵ㳤��

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
static int writeDataByNode(PNode pNode, PType pData)
{
    storeSeek(DATA, pNode->index);
    storeWrite(DATA, pNode->memberLen[0], pData->name);
    storeWrite(DATA, pNode->memberLen[1], pData->phone);
    storeWrite(DATA, pNode->memberLen[2], &pData->shortPhone);

    return SUCCESS;
}

// ���ݽڵ������
static int readDataByNode(PNode pNode, PType pData)
{
    storeSeek(DATA, pNode->index);
    storeRead(DATA, pNode->memberLen[0], pData->name);
    storeRead(DATA, pNode->memberLen[1], pData->phone);
    storeRead(DATA, pNode->memberLen[2], &pData->shortPhone);
    
    return SUCCESS;
}

//����ƥ��
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
        int index = i; //�ظ�����ƥ��
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
        { //ƥ��ɹ�
            return 1;
        }
    }

    return 0;
}

// ͳ������
void statistical(char(*names)[NAME_LENGTH + 1], int *couns ,
                                            int *statisticalNum , char *name)
{
    int findFlag = 0;
    for (int i = 0; i < *statisticalNum; ++i)
    {
        if (strcmp(names[i], name) == 0) //ƥ��ɹ�
        {
            couns[i]++; //��Ӧ������++
            findFlag = 1;
            break;
        }
    }

    if (!findFlag) //δ�ҵ�
    {
        //����������ӵ�ĩβ
        strcpy(names[*statisticalNum], name);
        couns[*statisticalNum] = 1;
        (*statisticalNum)++; //����++
    }
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

    if (!storeOpen(NODE, READ_WRITE_MODE))
    {
        return INDEX_ERR; //�ļ���ʧ��
    }
    
    storeSeek(NODE, 0);
    storeRead(NODE, sizeof(int), &nodeNum); //��ȡ�ڵ㳤��
    Node node = { 0 };
    int dataIndex = 0;
    int findIndex = 0;
    
    for(; findIndex < nodeNum;++findIndex)
    {
        storeRead(NODE, sizeof(Node), &node);
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

    storeClose(NODE); //�ر��ļ�
    
    return findIndex;
}

// �������
int addInputData(int nodeIndex, PType pData)
{
    int retMsg = FAIL;

    //�򿪽ڵ��ļ�
    if (!storeOpen(NODE, READ_WRITE_MODE))
    {
        goto addEnd;
    }

    if (!checkIndex(nodeIndex)) //�������
    {
        goto addEnd;
    }

    //��ȡ�ڵ�����
    Node node = { 0 };
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeRead(NODE, sizeof(Node), &node);

    //�������ݽڵ�
    updateNodeByData(&node, pData);

    //�������ļ�
    if (!storeOpen(DATA, READ_WRITE_MODE))
    {
        goto addEnd;
    }
    
    //д������
    writeDataByNode(&node, pData);
    
    //д��ڵ�
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeWrite(NODE, sizeof(Node), &node);
    
    retMsg = SUCCESS;

addEnd:
    storeClose(DATA);
    storeClose(NODE);
    
    return retMsg;
}

//ɾ������
int deleteDataBuf(int dataId)
{
    int nodeIndex = dataId - 1;

    //�򿪽ڵ��ļ�
    if (!storeOpen(NODE, READ_WRITE_MODE))
    {
        return FAIL;
    }

    int nodeNum = 0;
    storeSeek(NODE, 0);
    storeRead(NODE, sizeof(int), &nodeNum); //��ȡ�ڵ㳤��

    if (nodeIndex < 0 || nodeIndex >= nodeNum) //�������
    {
        storeClose(NODE);
        return FAIL;
    }

    //ɾ���ڵ�����
    Node node = { 0 };
    for (int i = nodeIndex + 1; i < nodeNum; ++i)
    {
        //������
        storeSeek(NODE, getStoreIndex(i));
        storeRead(NODE, sizeof(Node), &node);
        //��������
        storeSeek(NODE, getStoreIndex(i - 1));
        storeWrite(NODE, sizeof(Node), &node);
    }

    //���½ڵ���
    nodeNum--;
    storeSeek(NODE, 0);
    storeWrite(NODE, sizeof(int), &nodeNum);

    storeClose(NODE);
    return SUCCESS;
}

//��������
int updateDataBuf(int dataId, PType pContent, int conSize)
{
    int retMsg = FAIL;
    int nodeIndex = dataId - 1;

    //�򿪽ڵ��ļ�
    if (!storeOpen(NODE, READ_WRITE_MODE))
    {
        goto updateEnd;
    }

    if (!checkIndex(nodeIndex)) //�������
    {
        goto updateEnd;
    }

    //��ȡ�ڵ�����
    Node node = { 0 };
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeRead(NODE, sizeof(Node), &node);

    if (conSize > node.size) //����̫��
    {
        goto updateEnd;
    }

    //���½ڵ�����
    node.size = conSize;
    updateNodeByData(&node, pContent);
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeWrite(NODE, sizeof(Node), &node);

    //�������ļ�
    if (!storeOpen(DATA, READ_WRITE_MODE))
    {
        goto updateEnd;
    }

    //����Դ����
    writeDataByNode(&node,pContent);

    retMsg = SUCCESS;

updateEnd:
    storeClose(DATA);
    storeClose(NODE);

    return retMsg;
}


//����Id��������
int findDataById(int dataId, PType pData)
{
    int retMsg = FAIL;
    int nodeIndex = dataId - 1;
    
    //�򿪽ڵ��ļ�
    if (!storeOpen(NODE, READ_MODE))
    {
        goto findIdEnd;
    }

    if (!checkIndex(nodeIndex)) //�������
    {
        goto findIdEnd;
    }

    //��ȡ�ڵ�����
    Node node = { 0 };
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeRead(NODE, sizeof(Node), &node);

    //�������ļ�
    if (!storeOpen(DATA, READ_MODE))
    {
        goto findIdEnd;
    }

    //������
    readDataByNode(&node,pData);

    retMsg = SUCCESS;

findIdEnd:
    storeClose(DATA);
    storeClose(NODE);

    return retMsg;
}

//ƥ������
int checkDataByContent(int dataId, char *content, PType pData, int type)
{
    int retMsg = FAIL;
    int nodeIndex = dataId - 1;

    //�򿪽ڵ��ļ�
    if (!storeOpen(NODE, READ_MODE))
    {
        goto checkNameEnd;
    }

    if (!checkIndex(nodeIndex)) //�������
    {
        goto checkNameEnd;
    }

    //��ȡ�ڵ�����
    Node node = { 0 };
    storeSeek(NODE, getStoreIndex(nodeIndex));
    storeRead(NODE, sizeof(Node), &node);

    //�������ļ�
    if (!storeOpen(DATA, READ_MODE))
    {
        goto checkNameEnd;
    }

    //������
    readDataByNode(&node, pData);
    
    switch (type)
    {
        case FIND_NAME :
        {
            if (dataCompare(pData->name, content))
            {
                retMsg = SUCCESS; //���ҳɹ�
            }
            break;
        }
        case FIND_PHONE:
        {
            if (dataCompare(pData->phone, content))
            {
                retMsg = SUCCESS; //���ҳɹ�
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

    //�򿪽ڵ��ļ�
    if (!storeOpen(NODE, READ_MODE))
    {
        goto statisticalEnd;
    }

    //�������ļ�
    if (!storeOpen(DATA, READ_MODE))
    {
        goto statisticalEnd;
    }

    int nodeNum = 0;
    storeSeek(NODE, 0);
    storeRead(NODE, sizeof(int), &nodeNum); //��ȡ�ڵ㳤��

    
    Node node = { 0 };
    Type data = { 0 };
    for (int i = 0; i < nodeNum; ++i)
    {
        //��ȡ�ڵ�����
        storeSeek(NODE, getStoreIndex(i));
        storeRead(NODE, sizeof(Node), &node);
        //��ȡ��Ӧ����
        readDataByNode(&node, &data);
        statistical(names,counts,&statisticalNum,data.name); //ͳ��
    }
    
statisticalEnd:
    storeClose(DATA);
    storeClose(NODE);

    return statisticalNum;
}

//��ȡ�����ܹ�����
int getDataTotalNum()
{
    //�򿪽ڵ��ļ�
    if (!storeOpen(NODE, READ_MODE))
    {
        return 0;
    }

    int nodeNum = 0;
    storeSeek(NODE, 0);
    storeRead(NODE, sizeof(int), &nodeNum); //��ȡ�ڵ㳤��

    storeClose(NODE);
    return nodeNum;
}


//��ȡ�����ṹ������
int getIndexInfo(int nodeIndex,PIndexInfo pIndexInfo)
{
    int retMsg = FAIL;

    //�򿪽ڵ��ļ�
    if (!storeOpen(NODE, READ_MODE))
    {
        goto getIndexEnd;
    }

    if (!checkIndex(nodeIndex)) //�������
    {
        goto getIndexEnd;
    }

    //��ȡ�ڵ�����
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


//��Ƭ����
int defragment()
{
    int retMsg = FAIL;

    //�򿪽ڵ��ļ�
    if (!storeOpen(NODE, READ_WRITE_MODE))
    {
        goto defragmentEnd;
    }

    //�������ļ�
    if (!storeOpen(DATA, READ_WRITE_MODE))
    {
        goto defragmentEnd;
    }

    int nodeNum = 0;
    storeSeek(NODE, 0);
    storeRead(NODE, sizeof(int), &nodeNum); //��ȡ�ڵ㳤��
    
    int lastIndex = 0;
    Type data = {0};
    for (int i = 0; i < nodeNum; ++i)
    {
        //��ȡ�ڵ�����
        Node node = { 0 };
        storeSeek(NODE, getStoreIndex(i));
        storeRead(NODE, sizeof(Node), &node);
        //��ȡ����Դ
        readDataByNode(&node, &data);

        //��Ҫ������Ƭ����
        if (node.index != lastIndex)
        {
            node.index = lastIndex;
            //���½ڵ�
            storeSeek(NODE, getStoreIndex(i));
            storeWrite(NODE, sizeof(Node), &node);
            //����Դ����
            writeDataByNode(&node, &data);
        }

        lastIndex = node.index + node.size; //�ϸ�����
    }

    retMsg = SUCCESS;

defragmentEnd:
    storeClose(DATA);
    storeClose(NODE);

    return retMsg;
}
