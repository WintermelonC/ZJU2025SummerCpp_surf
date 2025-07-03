#pragma once

#include <functional>
#include <vector>

template <typename T>
class Observable {
public:
    void subscribe(std::function<void(const T&)> callback) {
        // 将回调函数添加到订阅者列表中
        subscribers.push_back(callback);
    }

    void notify(const T& data) {
        // 遍历所有订阅者并调用他们的回调函数
        for (const auto& subscriber : subscribers) {
            subscriber(data);
        }
    }
    
private:
    // 存储回调函数
    std::vector<std::function<void(const T&)>> subscribers;
};