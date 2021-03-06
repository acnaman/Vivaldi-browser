// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/keyed_service/core/simple_factory_key.h"

SimpleFactoryKey::SimpleFactoryKey(const base::FilePath& path,
                                   SimpleFactoryKey* original_key)
    : path_(path), original_key_(original_key) {}

SimpleFactoryKey::~SimpleFactoryKey() = default;
