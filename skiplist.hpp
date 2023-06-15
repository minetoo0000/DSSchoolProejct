#include "vector.hpp"

template <typename TK, typename TV>
struct Node{
    Vector<Node*> l;
    int h;
    TK key;
    TV value;
};

