//
// Created by xwd19 on 2026/2/22.
//

export module asyncpp.core:concepts;

import <type_traits>;

namespace asyncpp::core
{
    export template <typename T>
    concept Invocable = std::is_invocable_v<T>; // 判定 T 是否是可调用的
}
