
import 'dart:async';

import 'package:flutter/services.dart';

class OsIpAddress {
  static const MethodChannel _channel =
      const MethodChannel('os_ip_address');

  static Future<String> get platformVersion async {
    final String version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }

  static Future<List<String>> get ipAddress async {
    final String ipAddress = await _channel.invokeMethod('getIpAddress');
    return ipAddress.split("-");
  }
}
