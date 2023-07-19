//    Copyright 2022, farm-ng inc.
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

// Copyright (c) farm-ng, inc. All rights reserved.

#include "farm_ng/core/prototools/event_service.h"

#include <gtest/gtest.h>

using namespace farm_ng;

core::proto::EventSubscription makeSubscription(
    std::string topic,
    std::string consumer_path,
    int every_n = 0,
    int k_messages = 0,
    bool discard_payload = false) {
  core::proto::EventSubscription subscription;
  subscription.mutable_consumer_uri()->set_authority(getAuthority());
  subscription.mutable_consumer_uri()->set_path(consumer_path);
  subscription.mutable_consumer_uri()->set_scheme("protobuf");
  subscription.set_topic(topic);
  subscription.set_every_n(every_n);
  subscription.set_k_messages(k_messages);
  subscription.set_discard_payload(discard_payload);
  return subscription;
}

TEST(event_service_client, connect) {  // NOLINT
  Context ctx;

  core::proto::EventSubscriptions subscriptions;
  subscriptions.add_subscriptions()->CopyFrom(
      makeSubscription("source/foo/a", "dest/foo/a"));

  subscriptions.add_subscriptions()->CopyFrom(
      makeSubscription("source/foo/b", "dest/foo/b"));

  auto client1 = EventServiceClient::create(
      ctx, "events1", "localhost:95076", subscriptions);
  auto client2 = EventServiceClient::create(
      ctx, "events2", "localhost:95076", subscriptions);

  ctx.run();
}
