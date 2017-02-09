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

#include <CPW/TaskingTypeTraits.h>
#include <CPW/schedule.inl>

namespace CPW {

  // NOTE(jda) - This abstraction takes a lambda which should take captured
  //             variables by *value* to ensure no captured references race
  //             with the task itself.

  // NOTE(jda) - No priority is associated with this call, but could be added
  //             later with a hint enum, using a default value for the priority
  //             to not require specifying it.
  template<typename TASK_T>
  inline void schedule(TASK_T&& fcn)
  {
    static_assert(has_operator_method<TASK_T>::value,
                  "CPW::schedule() requires the implementation of method "
                  "'void TASK_T::operator()'.");

    schedule_impl(std::forward<TASK_T>(fcn));
  }

} // namespace CPW
