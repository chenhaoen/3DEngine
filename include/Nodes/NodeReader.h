#pragma once

#include <string_view>

class Node;

class NodeReader
{
public:
    static Node *read(const std::string_view &);
};