#include "Deserializer.h"
#include "SerializedNode.h"
Node* Deserializer::deserialize(const uint8_t* data, uint32_t dataLength) {
    const std::unordered_map<uint32_t, SNode> *dataMap = (const std::unordered_map<uint32_t, SNode> *) data;
    if(dataLength==0) return nullptr;
    Node *head = new Node();
    head->prev = nullptr;
    head->next = nullptr;
    for( uint32_t i = 1; i< dataLength; i++)
    {
        Node *temp = new Node();
        temp->next = head->next;
        head->next= temp;
        temp->prev = head;
        if(temp->next!=nullptr)
                temp->next->prev=temp;
    }
    uint32_t index = 0;
    for(Node *p = head; p!=nullptr; p = p->next)
    {
        SNode temp(dataMap->at(index));
        p->dataCount = temp.dataCount;
        p->name = temp.name;
        p->data = new Data[p->dataCount];
        for(uint8_t j=0; j< p->dataCount; j++)
            p->data[j] = temp.data[j];
        index ++;
    }
    SNode temp(dataMap->at(dataLength));
    if(temp.name=="loop")
    {
        Node *tail;
        for(tail = head; tail->next!=nullptr; tail = tail->next)
            ;
        tail ->next = head;
        head->prev = tail;

    }
    return head;
}
