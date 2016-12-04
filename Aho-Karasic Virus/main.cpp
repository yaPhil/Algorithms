#include <iostream>
#include <vector>
#include <string>
#include <map>

class TNode {
public:
    TNode(TNode* par, char toPar) {
        son.assign(30, nullptr);
        go.assign(30, nullptr);
        parent = par;
        suffLink = nullptr;
        up = nullptr;
        charToParent = toPar;
        isLeaf = false;
        numberTerm = 0;
        isNumberCounted = false;
    }
    std::vector<TNode*> son;                                // массив сыновей
    std::vector<TNode*> go;                                 // массив переходов (запоминаем переходы в ленивой рекурсии), используемый для вычисления суффиксных ссылок
    TNode* parent;                                // вершина родитель
    TNode* suffLink;                              // суффиксная ссылка (вычисляем в ленивой рекурсии)
    TNode* up;                                    // сжатая суффиксная ссылка
    bool isNumberCounted;
    long long numberTerm;
    char charToParent;                          // символ, ведущий к родителю
    bool isLeaf;                                // флаг, является ли вершина терминалом
    std::vector<int> leafPatternNumber;              // номера строк, за которые отвечает терминал
};

TNode* root;

TNode* getLink(TNode* v, char c);

TNode* getSuffixLink(TNode* v) {
    if(v == nullptr) {
        return nullptr;
    }
    if(v->suffLink == nullptr) {
        if(v == root || v->parent == root) {
            v->suffLink = root;
        } else {
            v->suffLink = getLink(getSuffixLink(v->parent), v->charToParent);
        }
    }
    return v->suffLink;
}

TNode* getLink(TNode* v, char c) {
    if(v->go[c - '0'] == nullptr) {                 // если переход по символу c ещё не вычислен
        if(v->son[c - '0'] != nullptr) {
            v->go[c - '0'] = v->son[c - '0'];
        } else if(v == root) {
            v->go[c - '0'] = root;
        } else {
            v->go[c - '0'] = getLink(getSuffixLink(v), c);
        }
    }
    return v->go[c - '0'];
}

void addString(const std::string &s, int patternNumber) {
    TNode* cur = root;
    for(auto i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (cur->son[c - '0'] == nullptr) {
            cur->son[c - '0'] = new TNode(cur, c);
            /* здесь также нужно обнулить указатели на переходы и сыновей */
        }
        cur = cur->son[c - '0'];
    }
    cur->isLeaf = true;
    cur->leafPatternNumber.push_back(patternNumber);
}

long long getTermCounter(TNode* v) {
    if(v == nullptr) {
        return 0;
    }
    if(!v->isNumberCounted) {
        v->isNumberCounted = true;
        if(v == root) {
            v->numberTerm = 0;
        } else {
            if(v->isLeaf) {
                v->numberTerm += v->leafPatternNumber.size();
            }
            v->numberTerm += getTermCounter(getSuffixLink(v));
        }
    }
    return v->numberTerm;
}

std::map<TNode*, int> color;

bool dfs(TNode* v) {
    color[v] = 1;
    for(int i = 0; i < 2; ++i) {
        TNode* next = getLink(v, (char)(i + '0'));
        if(color.count(next) == 0 && getTermCounter(next) == 0) {
            if(dfs(next)) {
                return true;
            }
        } else if(getTermCounter(next) == 0 && color[next] == 1) {
            return true;
        }
    }
    color[v] = 2;
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    int n;
    std::cin >> n;
    root = new TNode(nullptr, '0');
    for(int i = 0; i < n; ++i) {
        std::string str;
        std::cin >> str;
        addString(str, i);
    }
    if(dfs(root)) {
        std::cout << "TAK" << std::endl;
    } else {
        std::cout << "NIE" << std::endl;
    }
    return 0;
}