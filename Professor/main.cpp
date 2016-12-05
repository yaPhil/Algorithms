#include <iostream>
#include <vector>
#include <string>

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

std::vector<int> zFunction(std::string &str) {
    std::vector<int> zFunction(str.size());
    int left = 0;
    int right = 0;
    zFunction[0] = (int) str.size();
    for (size_t i = 1; i < str.size(); ++i) {
        zFunction[i] = std::max(0, std::min(right - (int)i, zFunction[(int)i - left]));
        while (i + zFunction[i] < str.size() && str[zFunction[i]] == str[i + zFunction[i]]) {
            ++zFunction[i];
        }
        if (i + zFunction[i] >= right) {
            left = (int) i;
            right = (int) (i + zFunction[i]);
        }
    }
    return zFunction;
}

int ans;
void countAns(TNode* node) {
    if(node == nullptr) {
        return;
    }
    if(node != root) {
        ++ans;
    }
    for(int i = 0; i < 30; ++i) {
        countAns(node->step[i]);
    }
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::string str;
    std::cin >> str;
    root = new TNode();
    for(int i = 1; i < str.size(); ++i) {
        std::string s1, s2, sum;
        s1 = str.substr(0, (unsigned long) i);
        s2 = str.substr((unsigned long) i, str.size());
        sum = s2 + "#" + s1;
        std::vector<int> zed = zFunction(sum);
        int max = -2000000000;
        for(int j = (int) s2.size(); j <= str.size(); ++j) {
            if(zed[j] > max) {
                max = zed[j];
            }
        }
        addToBor(root, s2.substr(0, (unsigned long) max), 0);
    }
    countAns(root);
    std::cout << ans << std::endl;
    return 0;
}