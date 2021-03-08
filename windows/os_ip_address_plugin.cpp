#include "include/os_ip_address/os_ip_address_plugin.h"
#include "include/netif/gmlc/netif/NetIF.hpp"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>

namespace {

class OsIpAddressPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  OsIpAddressPlugin();

  virtual ~OsIpAddressPlugin();

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

// static
void OsIpAddressPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "os_ip_address",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<OsIpAddressPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

OsIpAddressPlugin::OsIpAddressPlugin() {}

OsIpAddressPlugin::~OsIpAddressPlugin() {}

void OsIpAddressPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    std::ostringstream version_stream;
    version_stream << "Windows ";
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
    result->Success(flutter::EncodableValue(version_stream.str()));
  } else if (method_call.method_name().compare("getIpAddress") == 0) {
    auto addrs_ipv4 = gmlc::netif::getInterfaceAddressesV4();
    std::string ip_ptr = "";
    auto ips = "";
    for (auto ip = addrs_ipv4.begin(); ip != addrs_ipv4.end(); ++ip) {
      ip_ptr.append(* ip).append("-");
    }
    ip_ptr.pop_back();

    ips = ip_ptr.c_str();

    
    result->Success(flutter::EncodableValue(ips));
    //result->Success(flutter::EncodableValue(ip_ptr));
  } else {
    result->NotImplemented();
  }
}

}  // namespace

void OsIpAddressPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  OsIpAddressPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
