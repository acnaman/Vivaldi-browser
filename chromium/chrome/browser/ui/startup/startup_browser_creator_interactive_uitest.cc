// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <vector>

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/message_loop/message_loop.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/prefs/session_startup_pref.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/sessions/session_restore.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/startup/startup_browser_creator.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

typedef InProcessBrowserTest StartupBrowserCreatorTest;

// Chrome OS doesn't support multiprofile.
// And BrowserWindow::IsActive() always returns false in tests on MAC.
// And this test is useless without that functionality.
#if !defined(OS_CHROMEOS) && !defined(OS_MACOSX)
// TODO reenable test for Vivaldi
IN_PROC_BROWSER_TEST_F(StartupBrowserCreatorTest, DISABLED_LastUsedProfileActivated) {
  ProfileManager* profile_manager = g_browser_process->profile_manager();

  // Create 4 profiles, they will be scheduled for destruction when the last
  // browser window they are associated to will be closed.
  Profile* profile_1 = profile_manager->GetProfile(
      profile_manager->user_data_dir().Append(FILE_PATH_LITERAL(
          "New Profile 1")));
  ASSERT_TRUE(profile_1);
  Profile* profile_2 = profile_manager->GetProfile(
      profile_manager->user_data_dir().Append(FILE_PATH_LITERAL(
          "New Profile 2")));
  ASSERT_TRUE(profile_2);
  Profile* profile_3 = profile_manager->GetProfile(
      profile_manager->user_data_dir().Append(FILE_PATH_LITERAL(
          "New Profile 3")));
  ASSERT_TRUE(profile_3);
  Profile* profile_4 = profile_manager->GetProfile(
      profile_manager->user_data_dir().Append(FILE_PATH_LITERAL(
          "New Profile 4")));
  ASSERT_TRUE(profile_4);

  SessionStartupPref pref_urls(SessionStartupPref::URLS);
  pref_urls.urls.push_back(ui_test_utils::GetTestUrl(
      base::FilePath(base::FilePath::kCurrentDirectory),
      base::FilePath(FILE_PATH_LITERAL("title1.html"))));
  SessionStartupPref::SetStartupPref(profile_1, pref_urls);
  SessionStartupPref::SetStartupPref(profile_2, pref_urls);
  SessionStartupPref::SetStartupPref(profile_3, pref_urls);
  SessionStartupPref::SetStartupPref(profile_4, pref_urls);

  // Do a simple non-process-startup browser launch.
  base::CommandLine dummy(base::CommandLine::NO_PROGRAM);

  StartupBrowserCreator browser_creator;
  std::vector<Profile*> last_opened_profiles;
  last_opened_profiles.push_back(profile_1);
  last_opened_profiles.push_back(profile_2);
  last_opened_profiles.push_back(profile_3);
  last_opened_profiles.push_back(profile_4);
  browser_creator.Start(dummy, profile_manager->user_data_dir(), profile_2,
                        last_opened_profiles);

  while (!browser_creator.ActivatedProfile())
    base::MessageLoop::current()->RunUntilIdle();

  Browser* new_browser = NULL;

  // The last used profile (the profile_2 in this case) must be active.
  ASSERT_EQ(1u, chrome::GetBrowserCount(profile_2,
                                        browser()->host_desktop_type()));
  new_browser = FindBrowserWithProfile(profile_2,
                                       browser()->host_desktop_type());
  ASSERT_TRUE(new_browser);
  EXPECT_TRUE(new_browser->window()->IsActive());

  // All other profiles browser should not be active.
  ASSERT_EQ(1u, chrome::GetBrowserCount(profile_1,
                                        browser()->host_desktop_type()));
  new_browser = FindBrowserWithProfile(profile_1,
                                       browser()->host_desktop_type());
  ASSERT_TRUE(new_browser);
  EXPECT_FALSE(new_browser->window()->IsActive());

  ASSERT_EQ(1u, chrome::GetBrowserCount(profile_3,
                                        browser()->host_desktop_type()));
  new_browser = FindBrowserWithProfile(profile_3,
                                       browser()->host_desktop_type());
  ASSERT_TRUE(new_browser);
  EXPECT_FALSE(new_browser->window()->IsActive());

  ASSERT_EQ(1u, chrome::GetBrowserCount(profile_4,
                                        browser()->host_desktop_type()));
  new_browser = FindBrowserWithProfile(profile_4,
                                       browser()->host_desktop_type());
  ASSERT_TRUE(new_browser);
  EXPECT_FALSE(new_browser->window()->IsActive());
}
#endif  // !OS_MACOSX && !OS_CHROMEOS
