// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/sync/test/integration/sync_extension_installer.h"

#include "base/bind.h"
#include "base/location.h"
#include "base/single_thread_task_runner.h"
#include "base/thread_task_runner_handle.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/sync/test/integration/sync_datatype_helper.h"
#include "chrome/browser/sync/test/integration/sync_extension_helper.h"
#include "chrome/browser/sync/test/integration/sync_test.h"
#include "content/public/browser/notification_source.h"

SyncedExtensionInstaller::SyncedExtensionInstaller(Profile* profile)
    : profile_(profile),
      weak_ptr_factory_(this) {
  DoInstallSyncedExtensions();
  registrar_.Add(this,
                 extensions::NOTIFICATION_EXTENSION_UPDATING_STARTED,
                 content::Source<Profile>(profile_));
}

SyncedExtensionInstaller::~SyncedExtensionInstaller() {
}

void SyncedExtensionInstaller::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK_EQ(extensions::NOTIFICATION_EXTENSION_UPDATING_STARTED, type);

  // The extension system is trying to check for updates.  In the real world,
  // this would be where synced extensions are asynchronously downloaded from
  // the web store and installed.  In this test framework, we use this event as
  // a signal that it's time to asynchronously fake the installation of these
  // extensions.

  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE,
      base::Bind(&SyncedExtensionInstaller::DoInstallSyncedExtensions,
                 weak_ptr_factory_.GetWeakPtr()));
}

void SyncedExtensionInstaller::DoInstallSyncedExtensions() {
  // Do not try to install any extensions when running against real servers.
  // We can not assume that we have a clean slate of extensions installed per
  // profile before running the test cases.
  if (!sync_datatype_helper::test()->UsingExternalServers()) {
    SyncExtensionHelper::GetInstance()->
        InstallExtensionsPendingForSync(profile_);
  }
}
