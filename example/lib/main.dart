import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:os_ip_address/os_ip_address.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = 'Unknown';
  String _ipAddress = 'Unknown';

  @override
  void initState() {
    super.initState();
    initPlatformState();
    getIpAddress();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    String platformVersion;
    // Platform messages may fail, so we use a try/catch PlatformException.
    try {
      platformVersion = await OsIpAddress.platformVersion;
    } on PlatformException {
      platformVersion = 'Failed to get platform version.';
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _platformVersion = platformVersion;
    });
  }

  Future<void> getIpAddress() async {
    String ipAddress;
    try {
      final temp = await OsIpAddress.ipAddress;
      temp.removeWhere((ip) => ip == "127.0.0.1" || ip == "0.0.0.0");
      if(temp.length > 1) {
        ipAddress = temp[1];
      } else {
        ipAddress = temp[0];
      }
      
    } on PlatformException{
      ipAddress = "failed to get ip address";
    }

    if (!mounted) return;

    setState(() {
      _ipAddress = ipAddress;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Get Linux Ip Address Plugin'),
        ),
        body: Center(
          child: Text('ip address : $_ipAddress\n'),
        ),
      ),
    );
  }
}
