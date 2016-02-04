/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <thrift/lib/cpp2/protocol/BinaryProtocol.h>
#include <thrift/test/gen-cpp2/DebugProtoTest_types.h>

#include <math.h>

#include <folly/Benchmark.h>
#include <gflags/gflags.h>
#include <gtest/gtest.h>

using namespace thrift::test::debug;
using namespace thrift::test::debug::cpp2;
using namespace apache::thrift;
using namespace std;


// Globals so that the read test can read the write test data.
OneOfEach ooe;
unique_ptr<IOBuf> buf;

template <typename TBufferType_>
void runTestRead(int iters) {
  OneOfEach ooe2;
  TBufferType_ prot;
  for (int i = 0; i < iters; i ++) {
    prot.setInput(buf.get());
    ooe2.read(&prot);
  }
}

BENCHMARK(runTestRead_BinaryProtocolReader, iters) {
  runTestRead<BinaryProtocolReader>(iters);
}

template <typename TBufferType_>
void runTestWrite(int iters)
{
  TBufferType_ prot;
  size_t bufSize = ooe.serializedSizeZC(&prot);
  folly::IOBufQueue queue;

  for (int i = 0; i < iters; i ++) {
    queue.clear();
    prot.setOutput(&queue, bufSize);
    ooe.write(&prot);
  }

  buf = queue.move();
}

BENCHMARK(runTestWrite_BinaryProtocolWriter, iters) {
  runTestWrite<BinaryProtocolWriter>(iters);
}

TEST(protocol2, readwrite) {
  runTestWrite<BinaryProtocolWriter>(1);
  runTestRead<BinaryProtocolReader>(1);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  ooe.im_true   = true;
  ooe.im_false  = false;
  ooe.a_bite    = 0xd6;
  ooe.integer16 = 27000;
  ooe.integer32 = 1 << 24;
  ooe.integer64 = (uint64_t)6000 * 1000 * 1000;
  ooe.double_precision = M_PI;
  ooe.some_characters  = "JSON THIS! \"\1";
  ooe.zomg_unicode     = "\xd7\n\a\t";
  ooe.base64 = "\1\2\3\255";
  ooe.string_string_map["one"] = "two";
  ooe.string_string_hash_map["three"] = "four";
  ooe.float_precision = (float)12.345;
  ooe.rank_map[567419810] = (float)0.211184;
  ooe.rank_map[507959914] = (float)0.080382;

  auto ret = RUN_ALL_TESTS();
  if (!ret) {
    folly::runBenchmarksOnFlag();
  }

  return 0;
}
