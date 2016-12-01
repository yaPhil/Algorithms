#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>


class FiniteState
{
public:
    FiniteState()
    {
    }

    FiniteState(char c, FiniteState* destination)
    {
        this->transfer_.push_back({c, destination});
    }
    void addStep(char c, FiniteState* destination)
    {
        this->transfer_.push_back({c, destination});
    }

    const std::vector<std::pair<char, FiniteState *>> &getTransfer() const {
        return transfer_;
    }

private:
    std::vector<std::pair<char, FiniteState*>> transfer_;
};

class FiniteAutomat
{
public:
    FiniteAutomat(char c)
    {
        finish_.push_back(new FiniteState());
        start_ = new FiniteState(c, finish_[0]);
    }
    FiniteAutomat(FiniteAutomat *first, FiniteAutomat *second)
    {
        for (int i = 0; i < first->finish_.size(); ++i)
        {
            this->finish_.push_back(first->finish_[i]);
        }
        for (int i = 0; i < second->finish_.size(); ++i) {
            this->finish_.push_back(second->finish_[i]);
        }
        start_ = new FiniteState();
        start_->addStep('1', first->start_);
        start_->addStep('1', second->start_);
    }
    FiniteAutomat(FiniteAutomat *repeater)
    {
        start_ = new FiniteState('1', repeater->start_);
        finish_.push_back(start_);
        for (int i = 0; i < repeater->finish_.size(); ++i) {
            repeater->finish_[i]->addStep('1', start_);
        }
    }

    void attach(FiniteAutomat *attachment)
    {
        for(size_t i = 0; i < this->finish_.size(); ++i)
        {
            this->finish_[i]->addStep('1', attachment->start_);
        }
        this->finish_.clear();
        for(size_t i = 0; i < attachment->finish_.size(); ++i)
        {
            this->finish_.push_back(attachment->finish_[i]);
        }
    }

    FiniteState *getStart() const {
        return start_;
    }

    const std::vector<FiniteState *> &getFinish() const {
        return finish_;
    }

private:
    FiniteState *start_;
    std::vector<FiniteState*> finish_;
};

bool dfs( FiniteState* const v, const std::vector<FiniteState*> &end, std::map<FiniteState*, int> counter, std::string remainder)
{
    if(counter.count(v) == 0)
    {
        counter.insert({v, remainder.size()});
    }
    else
    {
        counter[v] = (int)remainder.size();
    }
    if(remainder.size() == 0)
    {
        for (size_t i = 0; i < end.size(); ++i) {
            if (end[i] == v) {
                return true;
            }
        }
    }
    auto steps = v->getTransfer();
    for(size_t i = 0; i < steps.size(); ++i)
    {
        if(remainder.size() > 0)
        {
            if(steps[i].first == remainder[0])
            {
                if(counter.count(steps[i].second) == 0 || counter[steps[i].second] > remainder.size() - 1)
                {
                    if (dfs(steps[i].second, end, counter, remainder.substr(1, remainder.size() - 1))) {
                        return true;
                    }
                }
            }
            else
            {
                if(steps[i].first == '1')
                {
                    if(counter.count(steps[i].second) == 0 || counter[steps[i].second] > remainder.size())
                    {
                        if (dfs(steps[i].second, end, counter, remainder)) {
                            return true;
                        }
                    }
                }
            }
        }
        else
        {
            if(steps[i].first == '1')
            {
                if(counter.count(steps[i].second) == 0 || counter[steps[i].second] > 0)
                {
                    if (dfs(steps[i].second, end, counter, "")) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    std::string regularExpression, searchingFragment;
    std::cin >> regularExpression >> searchingFragment;
    std::vector<FiniteAutomat*> stack;
    std::vector<std::string> builtExpression;
    for(size_t i = 0; i < regularExpression.size(); ++i)
    {
        if(std::isspace(regularExpression[i]) != 0)
        {
            continue;
        }
        if(regularExpression[i] >= 'a' && regularExpression[i] <= 'c' || regularExpression[i] == '1')
        {
            stack.push_back(new FiniteAutomat(regularExpression[i]));
            std::string tmp = "";
            tmp += regularExpression[i];
            builtExpression.push_back(tmp);
        }
        else
        {
            if(regularExpression[i] == '.')
            {
                if(stack.size() < 2)
                {
                    std::cout << "Invalid expression" << std::endl;
                    return 0;
                }
                FiniteAutomat *second = stack[stack.size() - 1];
                FiniteAutomat *first = stack[stack.size() - 2];
                stack.pop_back();
                stack.pop_back();
                first->attach(second);
                stack.push_back(first);

                std::string two = builtExpression.back();
                builtExpression.pop_back();
                std::string one = builtExpression.back();
                builtExpression.pop_back();
                std::string tmp = "";
                tmp += '(';
                tmp += one;
                tmp += ')';
                tmp += '.'; tmp += '('; tmp += two; tmp += ')';
                builtExpression.push_back(tmp);
            }
            else
            {
                if(regularExpression[i] == '+')
                {
                    if(stack.size() < 2)
                    {
                        std::cout << "Invalid expression" << std::endl;
                        return 0;
                    }
                    FiniteAutomat *second = stack[stack.size() - 1];
                    FiniteAutomat *first = stack[stack.size() - 2];
                    stack.pop_back();
                    stack.pop_back();
                    stack.push_back(new FiniteAutomat(first, second));

                    std::string two = builtExpression.back();
                    builtExpression.pop_back();
                    std::string one = builtExpression.back();
                    builtExpression.pop_back();
                    std::string tmp = "";
                    tmp += '(';
                    tmp += one;
                    tmp += ')';
                    tmp += '+'; tmp += '('; tmp += two; tmp += ')';
                    builtExpression.push_back(tmp);
                }
                else
                {
                    if(regularExpression[i] == '*')
                    {
                        if(stack.size() < 1)
                        {
                            std::cout << "Invalid expression" << std::endl;
                            return 0;
                        }
                        FiniteAutomat *repeater = stack[stack.size() - 1];
                        stack.pop_back();
                        stack.push_back(new FiniteAutomat(repeater));

                        std::string one = builtExpression.back();
                        builtExpression.pop_back();
                        std::string tmp = "";
                        tmp += '(';
                        tmp += one;
                        tmp += ')';
                        tmp += '*';
                        builtExpression.push_back(tmp);
                    }
                    else
                    {
                        std::cout << "Invalid expression" << std::endl;
                        return 0;
                    }
                }
            }
        }
    }
    if(stack.size() != 1)
    {
        std::cout<< "Invalid expression" << std::endl;
        return 0;
    }
    FiniteAutomat *finiteStateAutomat = stack.front();
    stack.pop_back();
    FiniteState* start = finiteStateAutomat->getStart();
    auto finish = finiteStateAutomat->getFinish();
    searchingFragment.erase(std::remove(searchingFragment.begin(), searchingFragment.end(), '1'), searchingFragment.end());
    std::map<FiniteState*, int> antiCircle;
    for(size_t i = 0; i < searchingFragment.size(); ++i)
    {
        antiCircle.clear();
        if(dfs(start, finish, antiCircle, searchingFragment.substr(i, searchingFragment.size() - i)))
        {
            std::cout << searchingFragment.size() - i << std::endl;
            return 0;
        }
    }
    std::cout << 0 << std::endl;
    std::cout << builtExpression[0] << std::endl;
    return 0;
}