#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "Structures.h"

struct SNode
{
    SNode(std::string n, int dCount):name(n),dataCount(dCount),data(dCount){}
    std::string name;

    uint8_t dataCount;
    std::vector<Data> data;
};

