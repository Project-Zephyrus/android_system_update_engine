//
// Copyright (C) 2018 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "update_engine/dlcservice_chromeos.h"

#include <dlcservice/proto_bindings/dlcservice.pb.h>
// NOLINTNEXTLINE(build/include_alpha) "dbus-proxies.h" needs "dlcservice.pb.h"
#include <dlcservice/dbus-proxies.h>

#include "update_engine/dbus_connection.h"

using dlcservice::DlcModuleList;
using std::string;
using std::vector;

namespace chromeos_update_engine {

std::unique_ptr<DlcServiceInterface> CreateDlcService() {
  return std::make_unique<DlcServiceChromeOS>();
}

bool DlcServiceChromeOS::GetInstalled(vector<string>* dlc_module_ids) {
  if (!dlc_module_ids)
    return false;
  dlc_module_ids->clear();

  org::chromium::DlcServiceInterfaceProxy dlcservice_proxy(
      DBusConnection::Get()->GetDBus());

  dlcservice::DlcModuleList dlc_module_list;
  if (!dlcservice_proxy.GetInstalled(&dlc_module_list, nullptr)) {
    LOG(ERROR) << "dlcservice does not return installed DLC module list.";
    return false;
  }
  for (const auto& dlc_module_info : dlc_module_list.dlc_module_infos()) {
    dlc_module_ids->emplace_back(dlc_module_info.dlc_id());
  }
  return true;
}

}  // namespace chromeos_update_engine
