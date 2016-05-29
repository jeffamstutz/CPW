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

#define CATCH_CONFIG_MAIN
#include <tests/catch.hpp>
#include <CPW/parallel_for.h>

#include <algorithm>
#include <vector>

TEST_CASE("parallel_for")
{
  const size_t N_ELEMENTS = 1000000;

  const int bad_value  = 0;
  const int good_value = 1;

  std::vector<int> v(N_ELEMENTS);
  std::fill(v.begin(), v.end(), bad_value);

  CPW::parallel_for(N_ELEMENTS,[&](decltype(N_ELEMENTS) taskIndex) {
    v[taskIndex] = good_value;
  });

  auto found = std::find(v.begin(), v.end(), bad_value);

  REQUIRE(found == v.end());
}
