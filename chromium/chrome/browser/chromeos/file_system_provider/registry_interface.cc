// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/chromeos/file_system_provider/registry_interface.h"

namespace chromeos {
namespace file_system_provider {

RegistryInterface::~RegistryInterface() {
}

RegistryInterface::RestoredFileSystem::RestoredFileSystem() {
}

RegistryInterface::RestoredFileSystem::~RestoredFileSystem() {
}

}  // namespace file_system_provider
}  // namespace chromeos
