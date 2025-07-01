#pragma once

#include <deque>
#include "event.h"

class EventBus {
public:
    void publish(const Event& event);
    bool poll(Event& event);

private:
    std::deque<Event> m_eventQueue;  // 事件队列
};
