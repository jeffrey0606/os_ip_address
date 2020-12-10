import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:os_ip_address/os_ip_address.dart';

void main() {
  const MethodChannel channel = MethodChannel('os_ip_address');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await OsIpAddress.platformVersion, '42');
  });
}
