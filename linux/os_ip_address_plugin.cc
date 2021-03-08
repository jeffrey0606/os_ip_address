#include "include/os_ip_address/os_ip_address_plugin.h"
#include "../windows/include/netif/gmlc/netif/NetIF.hpp"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <sstream>

#include <cstring>

using namespace std;

#define OS_IP_ADDRESS_PLUGIN(obj)                                     \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), os_ip_address_plugin_get_type(), \
                              OsIpAddressPlugin))

struct _OsIpAddressPlugin
{
  GObject parent_instance;
};

G_DEFINE_TYPE(OsIpAddressPlugin, os_ip_address_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void os_ip_address_plugin_handle_method_call(
    OsIpAddressPlugin *self,
    FlMethodCall *method_call)
{
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar *method = fl_method_call_get_name(method_call);

  if (strcmp(method, "getPlatformVersion") == 0)
  {
    struct utsname uname_data = {};
    uname(&uname_data);
    g_autofree gchar *version = g_strdup_printf("Linux %s", uname_data.version);
    g_autoptr(FlValue) result = fl_value_new_string(version);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  }
  else if (strcmp(method, "getIpAddress") == 0)
  {
    auto addrs_ipv4 = gmlc::netif::getInterfaceAddressesV4();
    //auto ip = addrs_ipv4[addrs_ipv4.size() - 1];
    //const char * ip_ptr = ip.c_str();
    std::string ip_ptr = "";
    auto ips = "";
    for (auto ip = addrs_ipv4.begin(); ip != addrs_ipv4.end(); ++ip) {
      ip_ptr.append(* ip).append("-");
    }
    ip_ptr.pop_back();

    ips = ip_ptr.c_str();
    
    g_autoptr(FlValue) result = fl_value_new_string(ips);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  }
  else
  {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void os_ip_address_plugin_dispose(GObject *object)
{
  G_OBJECT_CLASS(os_ip_address_plugin_parent_class)->dispose(object);
}

static void os_ip_address_plugin_class_init(OsIpAddressPluginClass *klass)
{
  G_OBJECT_CLASS(klass)->dispose = os_ip_address_plugin_dispose;
}

static void os_ip_address_plugin_init(OsIpAddressPlugin *self) {}

static void method_call_cb(FlMethodChannel *channel, FlMethodCall *method_call,
                           gpointer user_data)
{
  OsIpAddressPlugin *plugin = OS_IP_ADDRESS_PLUGIN(user_data);
  os_ip_address_plugin_handle_method_call(plugin, method_call);
}

void os_ip_address_plugin_register_with_registrar(FlPluginRegistrar *registrar)
{
  OsIpAddressPlugin *plugin = OS_IP_ADDRESS_PLUGIN(
      g_object_new(os_ip_address_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "os_ip_address",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}
