#include <iostream>
#include <string>
#include <algorithm>

class TNode {
public:
    TNode() {
        isTerm = false;
        step.resize(30, nullptr);
        size = 0;
    }
    bool isTerm;
    int size;
    std::vector<TNode*> step;
};

TNode* root;

void addToBor(TNode* node, const std::string &str, int pos) {
    if(pos == str.size()) {
        node->isTerm = true;
        node->size = 1;
        return;
    }
    if(node->step[str[pos] - 'a'] != nullptr) {
        addToBor(node->step[str[pos] - 'a'], str, pos + 1);
        node->size++;
    } else {
        node->step[str[pos] - 'a'] = new TNode();
        addToBor(node->step[str[pos] - 'a'], str, pos + 1);
        node->size++;
    }
}

bool search(TNode* node, std::string &ans, int wanna, int &current) {
    if(node->isTerm) {
        ++current;
    }
    if(current == wanna) {
        return true;
    }
    for(int i = 0; i < 30; ++i) {
        if(node->step[i] != nullptr) {
            if(node->step[i]->size < wanna - current) {
                current += node->step[i]->size;
            } else {
                if (search(node->step[i], ans, wanna, current)) {
                    ans.push_back((char) (i + 'a'));
                    return true;
                }
            }
        }
    }
    return false;
}

int strToInt(std::string &str) {
    int a = 0;
    for(size_t i = 0; i < str.size(); ++i) {
        a *= 10;
        a += str[i] - '0';
    }
    return a;
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    root = new TNode();
    int n;
    std::cin >> n;
    for(size_t t = 0; t < n; ++t) {
        std::string str;
        std::cin >> str;
        if(str[0] >= '0' && str[0] <= '9') {
            long k = strToInt(str);
            int cur = 0;
            std::string ret = "";
            search(root, ret, k, cur);
            for(int i = ret.size() - 1; i >= 0; --i) {
                std::cout << ret[i];
            }
            std::cout<<std::endl;
        } else {
            addToBor(root, str, 0);
        }
    }
    return 0;
}