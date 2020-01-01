#ifndef MYSTL_TYPETRAITS_H_
#define MYSTL_TYPETRAITS_H_

// 本头文件主要实现 type_traits模板类，用于萃取型别的特性

namespace mystl {

// 两个不包含任何成员的类，用于辅助实现type_traits模板
// 由于文件末尾还有名为 true_type，false_type的类别名，故而这里带一个下划线
struct _true_type { };
struct _false_type { };

template<typename type>
struct type_traits {
    using has_trivial_default_constructor   = _false_type;
    using has_trivial_copy_constructtor     = _false_type;
    using has_trivial_assignment_operator   = _false_type;
    using has_trivial_destructor            = _false_type;
    using is_POD_type                       = _false_type;
};

// 针对各种算术整型的特化版本
template<>
struct type_traits<bool> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<char> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<unsigned char> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<signed char> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<wchar_t> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<short> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<unsigned short> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<int> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<unsigned int> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<long> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<unsigned long> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<long long> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<unsigned long long> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<float> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<double> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

template<>
struct type_traits<long double> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

// 针对指针的偏特化版本
template<typename type>
struct type_traits<type*> {
    using has_trivial_default_constructor   = _true_type;
    using has_trivial_copy_constructtor     = _true_type;
    using has_trivial_assignment_operator   = _true_type;
    using has_trivial_destructor            = _true_type;
    using is_POD_type                       = _true_type;
};

// 一个辅助实现 true_type 和 false_type 的类
template<typename T, T v>
struct intergral_constant {
    using value_type = T;
    using type       = intergral_constant<T, v>;

    static constexpr T value = v;
};

using true_type = intergral_constant<bool, true>;
using false_type = intergral_constant<bool, false>;

// 接受普通类型时继承 false_type ,此时其静态成员 value 为 false
template<typename T>
struct is_const: public false_type { };

// 偏特化使得接受 const 类型时继承 true_type，此时其静态成员 value 为 true
template<typename T>
struct is_const<const T>: public true_type { };

}
#endif // !MYSTL_TYPETRAITS_H_