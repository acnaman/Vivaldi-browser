// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

GEN_INCLUDE(['options_browsertest_base.js']);

/**
 * TestFixture for language options WebUI testing.
 * @extends {testing.Test}
 * @constructor
 */
function LanguageOptionsWebUITest() {}

LanguageOptionsWebUITest.prototype = {
  __proto__: OptionsBrowsertestBase.prototype,

  /** @override */
  browsePreload: 'chrome://settings-frame/languages',
};

// Test opening language options has correct location.
// TODO(vivaldi) Reenable for Vivaldi
GEN('#if defined(OS_MACOSX)');
GEN('#define MAYBE_testOpenLanguageOptions ' +
    'DISABLED_testOpenLanguageOptions');
GEN('#else');
GEN('#define MAYBE_testOpenLanguageOptions ' +
    'testOpenLanguageOptions');
GEN('#endif  // defined(OS_MACOSX)');
TEST_F('LanguageOptionsWebUITest', 'MAYBE_testOpenLanguageOptions', function () {
  assertEquals(this.browsePreload, document.location.href);
});

GEN('#if defined(OS_WIN) || defined(OS_CHROMEOS)');
// Test reselecting the same language as the current UI locale. This should show
// a "Chrome is displayed in this language" message rather than a restart banner
// or a [ Display Chrome in this language ] button.
TEST_F('LanguageOptionsWebUITest', 'reselectUILocale', function() {
  var currentLang = loadTimeData.getString('currentUiLanguageCode');
  $('language-options-list').selectLanguageByCode(currentLang);
  LanguageOptions.uiLanguageSaved(currentLang);

  expectTrue($('language-options-ui-language-button').hidden);
  expectFalse($('language-options-ui-language-message').hidden);
  expectTrue($('language-options-ui-notification-bar').hidden);
});
GEN('#endif');  // defined(OS_WIN) || defined(OS_CHROMEOS)
