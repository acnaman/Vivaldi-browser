// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/extensions/updater/chromeos_extension_cache_delegate.h"

#include <stddef.h>

#include "chrome/browser/chromeos/settings/cros_settings.h"
#include "chromeos/settings/cros_settings_names.h"

namespace extensions {
namespace {

const char kLocalCacheDir[] = "/var/cache/external_cache";

}  // namespace

ChromeOSExtensionCacheDelegate::ChromeOSExtensionCacheDelegate()
    : cache_dir_(kLocalCacheDir) {
}

ChromeOSExtensionCacheDelegate::ChromeOSExtensionCacheDelegate(
    const base::FilePath& cache_dir)
    : cache_dir_(cache_dir) {
}

const base::FilePath& ChromeOSExtensionCacheDelegate::GetCacheDir() const {
  return cache_dir_;
}

size_t ChromeOSExtensionCacheDelegate::GetMaximumCacheSize() const {
  size_t max_size = ExtensionCacheDelegate::GetMaximumCacheSize();
  int policy_size = 0;
  if (chromeos::CrosSettings::Get()->GetInteger(chromeos::kExtensionCacheSize,
                                                &policy_size) &&
      policy_size >= static_cast<int>(GetMinimumCacheSize())) {
    max_size = policy_size;
  }
  return max_size;
}

}  // namespace extensions
