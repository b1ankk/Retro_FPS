#pragma once
#include <memory>

namespace game
{
    template<typename T>
    using GameObjRef = const std::shared_ptr<T>&;

}
