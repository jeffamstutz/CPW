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

#ifdef CPW_TASKING_TBB
#  include <tbb/task.h>
#elif defined(CPW_TASKING_CILK)
#  include <cilk/cilk.h>
#elif defined(CPW_TASKING_INTERNAL)
#  include "TaskSys.h"
#endif

namespace CPW {

template<typename TASK_T>
inline void schedule_impl(TASK_T&& fcn)
{
#ifdef CPW_TASKING_TBB
  struct LocalTBBTask : public tbb::task
  {
    TASK_T func;
    tbb::task* execute() override { func(); return nullptr; }
    LocalTBBTask(TASK_T&& f) : func(std::forward<TASK_T>(f)) {}
  };

  auto *tbb_node =
      new(tbb::task::allocate_root())LocalTBBTask(std::forward<TASK_T>(fcn));
  tbb::task::enqueue(*tbb_node);
#elif defined(CPW_TASKING_CILK)
  cilk_spawn fcn();
#elif defined(CPW_TASKING_INTERNAL)
  struct LocalTask : public Task {
    TASK_T t;
    LocalTask(TASK_T&& fcn)
      : Task("LocalTask"), t(std::forward<TASK_T>(fcn)) {}
    void run(size_t) override { t(); }
  };

  Ref<LocalTask> task = new LocalTask(std::forward<TASK_T>(fcn));
  task->schedule(1, Task::FRONT_OF_QUEUE);
#else// OpenMP or Debug --> synchronous!
  fcn();
#endif
}

} // namespace CPW
