// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/font_fallback.h"

#include <string>
#include <vector>

namespace gfx {

std::vector<std::string> GetFallbackFontFamilies(
    const std::string& font_family) {
  return std::vector<std::string>();
}

}  // namespace gfx
