#include "vector.hpp"

template <typename TK, typename TV>
class Skiplist{
    private:
    struct Node{
        Vector<Node*> l;
        int h;
        TK key;
        TV value;
    };

    unsigned long long getH(){
        int ret = 1;
        while(ret < maxH && rand(0, 1) < p) ret++;
        return ret;
    }

    unsigned long long maxH;
    float p;
    Node* head;

    public:
    Skiplist(float _p, unsigned long long _maxH){
        maxH = _maxH;
        p = _p;
        head->l.resize(maxH);
        head->h = maxH;
    }
    
    Node* find(TK x, Vector<Node*> & path){
        node *cur = head;
        for(int i = maxH - 1; i >= 0; i--){
            while(cur->l[i] != nullptr && cur->l[i]->key < x){
                cur = cur->l[i];
            }
            path[i] = cur;
        }
        return cur->l[0];
    }

    void insert(TK k, TV v){
        Vector<Node*> path(maxH);
        Node* nxt = find(k, path);
        Node* cur = (Node*)malloc(sizeof(Node));
        cur->h = getH();
        cur->l.resize(cur->h);
    }
};