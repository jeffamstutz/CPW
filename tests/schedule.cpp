// ========================================================================== //
// The MIT License (MIT)                                                      //
//                                                                            //
// Copyright (c) 2016 Jefferson Amstutz                                       //
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <tests/doctest.h>
#include <CPW/schedule.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>

TEST_CASE("async")
{
  std::atomic<int> val{0};
  std::mutex mutex;
  std::condition_variable condition;

  auto *val_p  = &val;
  auto *cond_p = &condition;

  CPW::schedule([=](){
    *val_p = 1;
    cond_p->notify_all();
  });

  std::unique_lock<std::mutex> lock(mutex);
  auto now = std::chrono::system_clock::now();
  condition.wait_until(lock,
                       now + std::chrono::seconds(3),
                       [&](){ return val.load() == 1; });

  // NOTE(jda) - can't directly check std::atomic value due to compile error...
  int check_val = val;
  REQUIRE(check_val == 1);
}
