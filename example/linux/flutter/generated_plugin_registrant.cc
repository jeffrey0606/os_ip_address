//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <os_ip_address/os_ip_address_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) os_ip_address_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "OsIpAddressPlugin");
  os_ip_address_plugin_register_with_registrar(os_ip_address_registrar);
}
