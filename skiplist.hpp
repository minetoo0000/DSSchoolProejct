#include "vector.hpp"
#include <random>

template <typename TK, typename TV>
class Skiplist{
    private:
    struct Node;
    struct Node{
        Vector<Node*> l;
        int h;
        TK key;
        TV value;
    };

    unsigned long long maxH;
    float p;
    Node* head = new Node();

    float rand(){
        random_device rd;
        mt19937 mt(rd());
        uniform_real_distribution<float> dist(0, 1);
        return dist(mt);
    }

    unsigned long long getH(){
        int ret = 1;
        while(ret < maxH && rand() < p) ret++;
        return ret;
    }

    public:
    Skiplist(float _p, unsigned long _maxH){
        maxH = _maxH;
        p = _p;
        head->l.resize(maxH);
        head->h = maxH;
    }
    
    Node* find(TK x, Vector<Node*> & path){
        Node *cur = head;
        for(int i = maxH - 1; i >= 0; i--){
            while(cur->l[i] != nullptr && cur->l[i]->key < x){
                cur = cur->l[i];
            }
            path[i] = cur;
        }
        return cur->l[0];
    }

    Node* find(TK x){
        Node *cur = head;
        for(int i = maxH - 1; i >= 0; i--){
            while(cur->l[i] != nullptr && cur->l[i]->key < x){
                cur = cur->l[i];
            }
        }
        return cur->l[0];
    }

    void insert(TK k, TV v){
        Vector<Node*> path(maxH);
        Node* nxt = find(k, path);
        Node* cur = new Node();
        cur->key = k;
        cur->value = v;
        cur->h = getH();
        cur->l.resize(cur->h);
        for(int i = 0; i < cur->h; i++){
            cur->l[i] = path[i]->l[i];
            path[i]->l[i] = cur;
        }
    }

    void remove(TK k){
        Vector<Node*> path(maxH);
        Node* target = find(k, path);
        for(int i = 0; i < target->h; i++){
            path[i]->l[i] = target->l[i];
        }
        delete(target);
    }

    void print(){
        Node* cur = head;
        while(cur->l[0]){
            cur = cur->l[0];
            cout << "key : " << cur->key << " value : " << cur->value << '\n';
        }
    }
};