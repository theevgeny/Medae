#pragma once

#include <type_traits>

namespace Medae::Utils {

template<typename T, template<typename...> class Tmpl> struct is_instantiation_of : std::false_type
{
};

template<template<typename...> class Tmpl, typename... Args>
struct is_instantiation_of<Tmpl<Args...>, Tmpl> : std::true_type
{
};

template<typename T, template<typename...> class Tmpl>
inline constexpr bool is_instantiation_of_v = is_instantiation_of<T, Tmpl>::value;

} // namespace Medae::Utils