#include "eventBus.h"

void EventBus::publish(const Event& event) {
    m_eventQueue.push_back(event);
}

bool EventBus::poll(Event& event) {
    if (m_eventQueue.empty()) {
        return false;
    }

    event = m_eventQueue.front();
    m_eventQueue.pop_front();
    return true;
}
