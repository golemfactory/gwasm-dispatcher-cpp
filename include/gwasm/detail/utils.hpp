#ifndef GWASM_UTILS_HPP
#define GWASM_UTILS_HPP

#include <type_traits>
#include <utility>

namespace gwasm::detail {

// overloaded

template <typename... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

template <typename... Ts>
overloaded(Ts...)->overloaded<Ts...>;

// for_each_in_tuple

template <typename Tuple, typename F>
void
for_each_in_tuple(F&& f, Tuple&& tuple)
{
    std::apply([&](auto&&... i) { (f(std::forward<decltype(i)>(i)), ...); },
               std::forward<Tuple>(tuple));
}

// is_iterable

template <typename T, typename = void>
struct is_iterable : std::false_type
{};

template <typename T>
struct is_iterable<T,
                   std::void_t<decltype(std::begin(std::declval<T>())),
                               decltype(std::end(std::declval<T>())),
                               typename T::value_type>> : std::true_type
{};

template <typename T>
constexpr bool is_iterable_v = is_iterable<T>::value;

// is_like_tuple

template <typename T, typename = void>
struct is_like_tuple : std::false_type
{};

// TODO: how to check for std::get or std::tuple_element, when a std::tuple can
// be empty?
template <typename T>
struct is_like_tuple<T, std::void_t<std::tuple_size<T>>> : std::true_type
{};

template <typename T>
constexpr bool is_like_tuple_v = is_like_tuple<T>::value;

// is_applicable

// Tells if Tuple types match F arguments.
template <typename F, typename Tuple, typename = void>
struct is_applicable : std::false_type
{};

template <typename F, typename Tuple>
struct is_applicable<
    F,
    Tuple,
    std::void_t<decltype(std::apply(std::declval<F>(), std::declval<Tuple>()))>>
    : std::true_type
{};

template <typename F, typename Tuple>
constexpr bool is_applicable_v = is_applicable<F, Tuple>::value;

} // namespace gwasm::detail

#endif // GWASM_UTILS_HPP
