#pragma once

#include <cstdint>
#include <string>

struct Data {
    uint32_t id;

    float x;
    float y;
};

struct Node {
    Node();
    ~Node();

    Node *prev;
    Node *next;

    std::string name;

    uint8_t dataCount;
    Data *data;
};
