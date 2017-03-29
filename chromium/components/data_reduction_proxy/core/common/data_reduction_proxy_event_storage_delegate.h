// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_DATA_REDUCTION_PROXY_CORE_COMMON_DATA_REDUCTION_PROXY_EVENT_STORAGE_DELEGATE_H_
#define COMPONENTS_DATA_REDUCTION_PROXY_CORE_COMMON_DATA_REDUCTION_PROXY_EVENT_STORAGE_DELEGATE_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class Value;
}

namespace data_reduction_proxy {

// Defines an interface for storing Data Reduction Proxy events.
class DataReductionProxyEventStorageDelegate {
 public:
  enum SecureProxyCheckState {
    CHECK_UNKNOWN,
    CHECK_PENDING,
    CHECK_SUCCESS,
    CHECK_FAILED,
  };

  // Stores a DATA_REDUCTION_PROXY event with no parameters.
  virtual void AddEvent(scoped_ptr<base::Value> event) = 0;

  // Stores a DATA_REDUCTION_PROXY_ENABLED event.
  virtual void AddEnabledEvent(scoped_ptr<base::Value> event, bool enabled) = 0;

  // Stores a DATA_REDUCTION_PROXY_BYPASS_REQUESTED event.
  virtual void AddAndSetLastBypassEvent(scoped_ptr<base::Value> event,
                                        int64 expiration_ticks) = 0;

  // Stores a DATA_REDUCTION_PROXY_CANARY_REQUEST event.
  virtual void AddEventAndSecureProxyCheckState(
      scoped_ptr<base::Value> event,
      SecureProxyCheckState state) = 0;
};

}  // namespace data_reduction_proxy

#endif  // COMPONENTS_DATA_REDUCTION_PROXY_CORE_COMMON_DATA_REDUCTION_PROXY_EVENT_STORAGE_DELEGATE_H_
