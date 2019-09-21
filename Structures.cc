#include "Structures.h"

Node::Node()
    : prev(nullptr),
      next(nullptr),
      name(),
      dataCount(0),
      data(nullptr) {
}

Node::~Node() {
    if (data != nullptr) {
        delete data;
        data = nullptr;
        dataCount = 0;
    }
}
