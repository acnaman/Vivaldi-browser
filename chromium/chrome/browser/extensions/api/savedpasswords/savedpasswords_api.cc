// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/extensions/api/savedpasswords/savedpasswords_api.h"

#include "base/prefs/pref_service.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/extensions/api/savedpasswords.h"
#include "content/public/browser/web_ui.h"
#include "chrome/common/pref_names.h"
#include "net/base/net_util.h"
#include "base/strings/string_number_conversions.h"

namespace extensions {
  namespace passwords = api::savedpasswords;
  using passwords::SavedPasswordItem;

  SavedpasswordsGetListFunction::SavedpasswordsGetListFunction()
  :password_manager_presenter_(this)
{
}

  bool SavedpasswordsGetListFunction::RunAsync() {
  AddRef();
  password_manager_presenter_.Initialize();
  password_manager_presenter_.UpdatePasswordLists();
  return true;
}

  SavedpasswordsGetListFunction::~SavedpasswordsGetListFunction() {
}

  Profile* SavedpasswordsGetListFunction::GetProfile() {
 return ChromeUIThreadExtensionFunction::GetProfile();
}

#if !defined(OS_ANDROID)
  gfx::NativeWindow SavedpasswordsGetListFunction::GetNativeWindow() const {
  return NULL;
}
#endif

  void SavedpasswordsGetListFunction::SetPasswordList(
  const ScopedVector<autofill::PasswordForm>& password_list,
  bool show_passwords) {

  std::vector<linked_ptr<SavedPasswordItem> > svd_pwd_entries;
  base::ListValue entries;
  languages_ = GetProfile()->GetPrefs()->GetString(prefs::kAcceptLanguages);

  for (size_t i = 0; i < password_list.size(); ++i) {
    linked_ptr<api::savedpasswords::SavedPasswordItem> new_node(GetSavedPasswordItem(password_list[i], i));
    svd_pwd_entries.push_back(new_node);
    }

  results_ = api::savedpasswords::GetList::Results::Create(svd_pwd_entries);
  SendAsyncResponse();
}

SavedPasswordItem* SavedpasswordsGetListFunction::GetSavedPasswordItem(autofill::PasswordForm *form, int id){
  SavedPasswordItem* notes_tree_node = new SavedPasswordItem();
  notes_tree_node->username =  base::UTF16ToUTF8(form->username_value);
  notes_tree_node->origin = base::UTF16ToUTF8(net::FormatUrl(form->origin, languages_));
  notes_tree_node->id = base::Int64ToString(id);

  return notes_tree_node;
}

void SavedpasswordsGetListFunction::SendAsyncResponse() {
  base::MessageLoop::current()->PostTask(
    FROM_HERE,
    base::Bind(&SavedpasswordsGetListFunction::SendResponseToCallback, this));
}

void SavedpasswordsGetListFunction::SendResponseToCallback() {
  SendResponse(true);
  Release();  // Balanced in RunAsync().
}


void SavedpasswordsGetListFunction::SetPasswordExceptionList(
  const ScopedVector<autofill::PasswordForm>& password_exception_list) {
}

void SavedpasswordsGetListFunction::ShowPassword(
  size_t index,
  const std::string& origin_url,
  const std::string& username,
  const base::string16& password_value) {
}


SavedpasswordsRemoveFunction::SavedpasswordsRemoveFunction():
password_manager_presenter_(this)
{
}


SavedpasswordsRemoveFunction::~SavedpasswordsRemoveFunction(){
}

bool SavedpasswordsRemoveFunction::RunAsync(){
  AddRef(); //Balanced in SendResponseToCallback
  password_manager_presenter_.Initialize();
  password_manager_presenter_.UpdatePasswordLists();

  scoped_ptr<passwords::Remove::Params> params(
    passwords::Remove::Params::Create(*args_));
    EXTENSION_FUNCTION_VALIDATE(params.get());
    base::StringToInt64(params->id, &idToRemove);
  return true;
}

Profile* SavedpasswordsRemoveFunction::GetProfile(){
  return ChromeUIThreadExtensionFunction::GetProfile();
}
void SavedpasswordsRemoveFunction::ShowPassword(size_t index,
      const std::string& origin_url,
      const std::string& username,
      const base::string16& password_value){
}

void SavedpasswordsRemoveFunction::SetPasswordList(
  const ScopedVector<autofill::PasswordForm>& password_list,
  bool show_passwords){

  password_manager_presenter_.RemoveSavedPassword(static_cast<size_t>(idToRemove));

  results_ = passwords::Remove::Results::Create();
  SendAsyncResponse();
}
void SavedpasswordsRemoveFunction::SetPasswordExceptionList(
  const ScopedVector<autofill::PasswordForm>& password_exception_list){
}

#if !defined(OS_ANDROID)
gfx::NativeWindow SavedpasswordsRemoveFunction::GetNativeWindow() const {
  return NULL;
}
#endif

void SavedpasswordsRemoveFunction::SendResponseToCallback() {
  SendResponse(true);
  Release();  // Balanced in RunAsync().
}

void SavedpasswordsRemoveFunction::SendAsyncResponse() {
  base::MessageLoop::current()->PostTask(
    FROM_HERE,
    base::Bind(&SavedpasswordsRemoveFunction::SendResponseToCallback, this));
}

}  // namespace extensions
