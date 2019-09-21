#include "Serializer.h"
#include "SerializedNode.h"
uint32_t getLength(const Node *node);
uint8_t* Serializer::serialize(const Node *node, uint32_t *outLength) {
    // get lenth of the Nodes
    *outLength = getLength(node);

    const Node *p= node;
    std::unordered_map<uint32_t, SNode> *dataMap = new std::unordered_map<uint32_t, SNode>();
    for(uint32_t i =0; i< *outLength; i++)
    {
        SNode temp(p->name, p->dataCount);
        for(uint32_t j =0; j<temp.dataCount; j++)
        {
            temp.data[j] = node->data[j];
        }
        dataMap->insert(std::make_pair(i, temp));
        p = p->next;

    }
    //Signal if it is a loop.
    SNode temp("", 0);
    if(node->prev != nullptr)
    {
        temp.name="loop";
    }
    dataMap->insert(std::make_pair(*outLength, temp));
    return (uint8_t*) dataMap;
}

uint32_t getLength(const Node *node)
{
    uint32_t leng =0;
    const Node *head = node;
    const Node *p2 = node;
    bool flag = false;
    while(p2!=nullptr&&(p2!=head||!flag))
    {
        flag = true;
        leng++;
        p2 = p2->next;
    }
    return leng;
}
