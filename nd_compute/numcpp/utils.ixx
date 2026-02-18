//
// Created by xwd19 on 2026/2/16.
//

export module numcpp:utils;

import :types;

namespace numcpp
{
    using std::size_t;

    export size_t calcShapeSize(const Shape& shape) {
        if (!shape.size()) {
            return 0;
        }

        size_t size = 1;
        for (size_t axis : shape) {
            size *= axis;
        }

        return size;
    }
}
