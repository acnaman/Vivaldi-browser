// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_MOJO_MOJO_APPLICATION_HOST_H_
#define CONTENT_BROWSER_MOJO_MOJO_APPLICATION_HOST_H_

#include "base/memory/scoped_ptr.h"
#include "base/process/process_handle.h"
#include "content/common/application_setup.mojom.h"
#include "content/common/mojo/channel_init.h"
#include "content/common/mojo/service_registry_impl.h"
#include "third_party/mojo/src/mojo/edk/embedder/scoped_platform_handle.h"

#if defined(OS_ANDROID)
#include "content/browser/mojo/service_registry_android.h"
#endif

namespace IPC {
class Sender;
}

namespace content {

// MojoApplicationHost represents the code needed on the browser side to setup
// a child process as a Mojo application via Chrome IPC. The child process
// should use MojoApplication to handle messages generated by an instance of
// MojoApplicationHost. MojoApplicationHost makes the ServiceRegistry interface
// available so that child-provided services can be invoked.
class CONTENT_EXPORT MojoApplicationHost {
 public:
  MojoApplicationHost();
  ~MojoApplicationHost();

  // Two-phase initialization:
  //  1- Init makes service_registry() available synchronously.
  //  2- Activate establishes the actual connection to the peer process.
  bool Init();
  void Activate(IPC::Sender* sender, base::ProcessHandle process_handle);

  void WillDestroySoon();

  // Shuts down the Mojo channel. Must be called from the IO thread.
  void ShutdownOnIOThread();

  ServiceRegistry* service_registry() { return &service_registry_; }

#if defined(OS_ANDROID)
  ServiceRegistryAndroid* service_registry_android() {
    return service_registry_android_.get();
  }
#endif

  void OverrideIOTaskRunnerForTest(
      scoped_refptr<base::TaskRunner> io_task_runner);

 private:
  ChannelInit channel_init_;
  mojo::embedder::ScopedPlatformHandle client_handle_;

  bool did_activate_;

  scoped_ptr<ApplicationSetup> application_setup_;
  ServiceRegistryImpl service_registry_;

  scoped_refptr<base::TaskRunner> io_task_runner_override_;

#if defined(OS_ANDROID)
  scoped_ptr<ServiceRegistryAndroid> service_registry_android_;
#endif

  DISALLOW_COPY_AND_ASSIGN(MojoApplicationHost);
};

}  // namespace content

#endif  // CONTENT_BROWSER_MOJO_MOJO_APPLICATION_HOST_H_
