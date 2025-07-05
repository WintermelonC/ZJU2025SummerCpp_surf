#include "../playerViewModel.h"

std::function<void()> PlayerViewModel::getMouseRightClickCommand() {
    return [this]() {
        usePower();
    };
}