// ========================================================================== //
// The MIT License (MIT)                                                      //
//                                                                            //
// Copyright (c) 2016-2017 Jefferson Amstutz                                  //
//                                                                            //
// Permission is hereby granted, free of charge, to any person obtaining a    //
// copy of this software and associated documentation files (the "Software"), //
// to deal in the Software without restriction, including without limitation  //
// the rights to use, copy, modify, merge, publish, distribute, sublicense,   //
// and/or sell copies of the Software, and to permit persons to whom the      //
// Software is furnished to do so, subject to the following conditions:       //
//                                                                            //
// The above copyright notice and this permission notice shall be included in //
// in all copies or substantial portions of the Software.                     //
//                                                                            //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    //
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    //
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        //
// DEALINGS IN THE SOFTWARE.                                                  //
// ========================================================================== //

#pragma once

#include <cstdlib>
#include <type_traits>

namespace CPW {

//NOTE(jda) - This checks at compile time if T implements the method
//            'void T::operator()'.
template <typename T>
struct has_operator_method
{
  template <class, class> class checker;

  template <typename C>
  static std::true_type test(checker<C, decltype(&C::operator())> *);

  template <typename C>
  static std::false_type test(...);

  using type = decltype(test<T>(nullptr));
  static const bool value = std::is_same<std::true_type, type>::value;
};

//NOTE(jda) - This checks at compile time if T implements the method
//            'void T::operator(P taskIndex)', where P is an integral type
//            (must be short, int, uint, or size_t) at compile-time. To be used
//            inside a static_assert().
template <typename T>
struct has_operator_method_with_integral_param
{
  template <typename P>
  using t_param    = void(T::*)(P) const;
  using byte_t     = unsigned char;
  using operator_t = decltype(&T::operator());

  using param_is_byte     = std::is_same<t_param<byte_t>  , operator_t>;
  using param_is_short    = std::is_same<t_param<short>   , operator_t>;
  using param_is_int      = std::is_same<t_param<int>     , operator_t>;
  using param_is_unsigned = std::is_same<t_param<unsigned>, operator_t>;
  using param_is_long     = std::is_same<t_param<long>    , operator_t>;
  using param_is_size_t   = std::is_same<t_param<size_t>  , operator_t>;

  static const bool value = has_operator_method<T>::value &&
    (param_is_byte::value     ||
     param_is_short::value    ||
     param_is_int::value      ||
     param_is_unsigned::value ||
     param_is_long::value     ||
     param_is_size_t::value);
};

}//namespace CPW
