//
// Created by xwd19 on 25-11-23.
//

export module numcpp:concepts;

import <concepts>;
import <type_traits>;

namespace numcpp
{
    template <class T>
    concept Integral = std::is_integral_v<T>;

    template <class T>
    concept FloatingPoint = std::is_floating_point_v<T>;

    // 约束表达式为Integral和FloatingPoint的析取式
    template <class T>
    concept Number = Integral<T> || FloatingPoint<T>;

    // 预先定义了IteratorMemberFunction这个类型，表示一个返回值为T::iterator，参数列表为空的成员函数
    template <class T>
    using IteratorMemberFunction = T::iterator(T::*)();
    /**
    * 一个 指向类 T 的成员函数的指针类型
    * 属于类 T（由 T::* 表示这是一个 T 的成员指针）；
    * 没有参数（括号 () 表示无参）；
    * 返回类型是 T::iterator（即 T 类型中定义的嵌套类型 iterator）。
    **/


    // 用IteratorMemberFunction<T>(&T::begin)从T中获取成员函数的函数指针，
    // 使用decltype获取类型，判断该类型是否为一个成员函数
    template <class T>
    concept HasBegin = std::is_member_function_pointer_v
    <
        decltype(
            IteratorMemberFunction<T>(&T::begin))
    >;

    // 使用IteratorMemberFunction<T>(&T::end)从T中获取成员函数的函数指针，
    // 使用decltype获取类型，判断该类型是否为一个成员函数。
    // 如果类型不包含end成员函数，或者end函数的函数签名不同，都会违反这个约束
    template <class T>
    concept HasEnd = std::is_member_function_pointer_v<
        decltype(
            IteratorMemberFunction<T>(&T::end)
        )
    >;

    template <class T>
    concept IsIterable = HasBegin<T> && HasEnd<T>;

    // 首先使用IsIterable<T>约束类型必须可遍历，
    // 再使用Number<typename T::value_type>约束类型的value_type，
    // 嵌套类型必须符合Number这个概念的约束，
    // 因此，约束表达式也是一个合取式
    template <class T>
    concept IsNumberIterable = IsIterable<T> && Number<typename T::value_type>;

    // 使用了requires表达式，采用std::common_type_t判断From和To是否有相同的类型，如果存在相同类型返回true，否则返回false
    template <class From, class To>
    concept AnyConvertible = requires
    {
        typename std::common_type_t<From, To>;
    };
}
