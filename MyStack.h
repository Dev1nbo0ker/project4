#ifndef MYSTACK_H
#define MYSTACK_H

#include <vector>

// 自定义栈，禁止使用 std::stack
// 底层使用 vector 以满足非递归 DFS 的需求
template <typename T>
class MyStack {
public:
    void push(const T& value) {
        data_.push_back(value);
    }

    void pop() {
        data_.pop_back();
    }

    T& top() {
        return data_.back();
    }

    const T& top() const {
        return data_.back();
    }

    bool empty() const {
        return data_.empty();
    }

    size_t size() const {
        return data_.size();
    }

private:
    std::vector<T> data_;
};

#endif
