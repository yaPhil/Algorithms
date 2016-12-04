#include <iostream>
#include <vector>
#include <string>

class TNode {
public:
    TNode(TNode* par, char toPar) {
        son.assign(28, nullptr);
        go.assign(28, nullptr);
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
    long long numberTerm;
    bool isNumberCounted;
    char charToParent;                          // символ, ведущий к родителю
    bool isLeaf;                                // флаг, является ли вершина терминалом
    std::vector<long long> leafPatternNumber;              // номера строк, за которые отвечает терминал
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
    if(v->go[c - 'a'] == nullptr) {                 // если переход по символу c ещё не вычислен
        if(v->son[c - 'a'] != nullptr) {
            v->go[c - 'a'] = v->son[c - 'a'];
        } else if(v == root) {
            v->go[c - 'a'] = root;
        } else {
            v->go[c - 'a'] = getLink(getSuffixLink(v), c);
        }
    }
    return v->go[c - 'a'];
}

void addString(const std::string &s, long long patternNumber) {
    TNode* cur = root;
    for(auto i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (cur->son[c - 'a'] == nullptr) {
            cur->son[c - 'a'] = new TNode(cur, c);
            /* здесь также нужно обнулить указатели на переходы и сыновей */
        }
        cur = cur->son[c - 'a'];
    }
    cur->isLeaf = true;
    cur->leafPatternNumber.push_back(patternNumber);
}

TNode* getUp(TNode* v) {
    if(v == nullptr) {
        return nullptr;
    }
    if(v->up == nullptr) {
        if (v == root) {
            v->up = nullptr;
        } else {
            if(getSuffixLink(v)->isLeaf) {
                v->up = getSuffixLink(v);
            } else {
                v->up = getUp(getSuffixLink(v));
            }
        }
    }
    return v->up;
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

std::vector<std::vector<long long>> gr;
std::vector<long long> color;
long long ans = 0;
void dfs(long long borNode, TNode* automatNode) {
    color[borNode] = 1;
    ans += getTermCounter(automatNode);
    for(long long i = 0; i < 28; ++i) {
        if(gr[borNode][i] != 0 && color[gr[borNode][i]] == 0) {
            dfs(gr[borNode][i], getLink(automatNode, (char)(i + 'a')));
        }
    }
    color[borNode] = 2;
}
int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    long long n;
    std::cin >> n;
    gr.resize((unsigned long) (n + 1));
    for(long long i = 1; i <= n; ++i) {
        gr[i].resize(28);
    }
    for(long long i = 1; i <= n; ++i) {
        long long k;
        std::cin >> k;
        for(long long j = 0; j < k; ++j) {
            long long to;
            char c;
            std::cin >> to >> c;
            gr[i][c - 'a'] = to;
        }
    }
    root = new TNode(nullptr,'0');
    long long m;
    std::cin >> m;
    for(long long i = 0; i < m; ++i) {
        std::string str;
        std::cin >> str;
        addString(str, i);
    }
    color.assign((unsigned long) (n + 2), 0);
    dfs(1, root);
    std::cout << ans;
    return 0;
}