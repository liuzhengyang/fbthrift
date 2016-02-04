/*
 * Copyright 2016 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include <thrift/lib/cpp2/security/SecurityKillSwitchPoller.h>
#include <thrift/lib/cpp2/security/SecurityKillSwitch.h>
#include <folly/futures/Barrier.h>
#include <folly/Baton.h>
#include <folly/Singleton.h>

namespace apache {
namespace thrift {

using namespace testing;
using namespace std;

TEST(PollerTest, StartsCleanly) {
  folly::Baton<> b;
  int numCalls = 0;
  auto counter = [&]{
    ++numCalls;
    b.post();
    return true;
  };
  SecurityKillSwitchPoller poller(chrono::milliseconds(10000), counter);
  EXPECT_TRUE(b.timed_wait(chrono::seconds(5)));
  EXPECT_EQ(numCalls, 1);
}

TEST(PollerTest, PollsInSeparateThread) {
  auto current = this_thread::get_id();
  folly::Baton<> b;
  auto func = [&]{
    b.post();
    return current != this_thread::get_id();
  };
  SecurityKillSwitchPoller poller(chrono::milliseconds(10), func);
  EXPECT_TRUE(b.timed_wait(chrono::seconds(5)));
  EXPECT_TRUE(poller.isKillSwitchEnabled());
}

TEST(PollerTest, RunsRepeatedly) {
  folly::futures::Barrier b(5);
  auto countdown = [&]{
    b.wait();
    return true;
  };
  SecurityKillSwitchPoller poller(chrono::milliseconds(10), countdown);
  b.wait().get(chrono::seconds(5));
}

TEST(PollerTest, SingletonTest) {
  auto poller = folly::Singleton<SecurityKillSwitchPoller>::try_get();
  EXPECT_TRUE(poller);
}
}
}
