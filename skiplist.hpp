#include "vector.hpp"
#include <random>
#include <cstring>

struct Key{
    char* key;
    bool operator<(Key other){
        int ret = strcmp(key, other.key);
        if(ret < 0) return 1;
        return 0;
    }

    void operator=(char* c){
        key = c;
    }

    Key(){
        key = "\0";
    }

    Key(char* c){
        key = c;
    }
};

ostream& operator<<(ostream& os, const Key& cur)
{
    os << cur.key;
    return os;
}

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

    public:
    Skiplist(float _p, unsigned long _maxH){
        maxH = _maxH;
        p = _p;
        head->l.resize(maxH);
        head->l.fill(nullptr);
        head->h = maxH;
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
        cur->l.fill(nullptr);
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