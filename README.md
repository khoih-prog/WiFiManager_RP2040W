# WiFiManager_RP2040W

[![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiManager_RP2040W.svg?)](https://www.ardu-badge.com/WiFiManager_RP2040W)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/WiFiManager_RP2040W.svg)](https://github.com/khoih-prog/WiFiManager_RP2040W/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/WiFiManager_RP2040W/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/WiFiManager_RP2040W.svg)](http://github.com/khoih-prog/WiFiManager_RP2040W/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents

* [Why do we need this WiFiManager_RP2040W library](#why-do-we-need-this-WiFiManager_RP2040W-library)
  * [Features](#features)
  * [Important Notes](#Important-Notes)
  
  
* [Changelog](changelog.md) 
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [How It Works](#how-it-works) 
* [HOWTO Basic configurations](#howto-basic-configurations)
  * [1. Using default for every configurable parameter](#1-using-default-for-every-configurable-parameter)
  * [2. Using CORS (Cross-Origin Resource Sharing) feature](#2-using-cors-cross-origin-resource-sharing-feature)
  * [3. Using MultiWiFi auto(Re)connect feature](#3-using-multiwifi-autoreconnect-feature)
* [HOWTO Open Config Portal](#howto-open-config-portal)
* [Examples](#examples)
  * [ 1. ConfigOnDoubleReset](examples/ConfigOnDoubleReset)
  * [ 2. ConfigOnStartup](examples/ConfigOnStartup)
* [So, how it works?](#so-how-it-works)
* [Documentation](#documentation)
  * [Password protect the configuration Access Point](#password-protect-the-configuration-access-point)
  * [Callbacks](#callbacks)
    * [Save settings](#save-settings)
  * [ConfigPortal Timeout](#configportal-timeout) 
  * [Custom Parameters](#custom-parameters)
  * [Custom IP Configuration](#custom-ip-configuration) 
    * [Custom Access Point IP Configuration (currently not working)](#custom-access-point-ip-configuration-currently-not-working)
    * [Custom Station (client) Static IP Configuration (currently not working)](#custom-station-client-static-ip-configuration-currently-not-working) 
  * [Custom HTML, CSS, Javascript](#custom-html-css-javascript)
  * [Filter Networks](#filter-networks) 
* [Example ConfigOnDoubleReset](#example-ConfigOnDoubleReset)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ConfigOnDoubleReset](#1-ConfigOnDoubleReset)
    * [DRD detected, Config Portal will open](#drd-detected-config-portal-will-open)
    * [Start normally with correct config data](#Start-normally-with-correct-config-data)
  * [2. ConfigOnStartup](#2-ConfigOnStartup)
    * [No stored Credentials, Config Portal will open without timeout](#no-stored-credentials-config-portal-will-open-without-timeout)
    * [Restarted, Config Portal will open with 120s timeout](#restarted-config-portal-will-open-with-120s-timeout)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License and credits](#license-and-credits)
* [Copyright](#copyright)

---
---

### Why do we need this [WiFiManager_RP2040W library](https://github.com/khoih-prog/WiFiManager_RP2040W)

#### Features

This library is based on, modified, bug-fixed and improved from:

1. [`Tzapu WiFiManager`](https://github.com/tzapu/WiFiManager)
2. [`Ken Taylor WiFiManager`](https://github.com/kentaylor/WiFiManager)
3. [`Khoi Hoang's ESP_WiFiManager`](https://github.com/khoih-prog/ESP_WiFiManager)

to provide support to `RASPBERRY_PI_PICO_W with CYW43439 WiFi`.

This is a WiFi Connection manager with fallback web ConfigPortal. It's using a web ConfigPortal, served from the `RASPBERRY_PI_PICO_W`, and operating as an access point.

#### Important Notes

1. To be used with [`arduino-pico core v2.4.1+`](https://github.com/earlephilhower/arduino-pico) because of dependency on `DNSServer` library.

---
---

## Prerequisites

1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
2. [`Earle Philhower's arduino-pico core v2.4.1+`](https://github.com/earlephilhower/arduino-pico) for **RASPBERRY_PI_PICO_W with CYW43439 WiFi**, etc. [![GitHub release](https://img.shields.io/github/release/earlephilhower/arduino-pico.svg)](https://github.com/earlephilhower/arduino-pico/releases/latest)
3. [`Functional-Vlpp library v1.0.2+`](https://github.com/khoih-prog/functional-vlpp) to use server's lambda function. To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/Functional-Vlpp.svg?)](https://www.ardu-badge.com/Functional-Vlpp)
4. [`WiFiWebServer library v1.9.3+`](https://github.com/khoih-prog/WiFiWebServer). To install. check [![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiWebServer.svg?)](https://www.ardu-badge.com/WiFiWebServer)
5. [`DoubleResetDetector_Generic v1.8.1+`](https://github.com/khoih-prog/DoubleResetDetector_Generic). To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/DoubleResetDetector_Generic.svg?)](https://www.ardu-badge.com/DoubleResetDetector_Generic)
6. [`WiFiMulti_Generic library v1.2.2+`](https://github.com/khoih-prog/WiFiMulti_Generic) to use WiFiMulti function. To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiMulti_Generic.svg?)](https://www.ardu-badge.com/WiFiMulti_Generic).

---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for `WiFiManager_RP2040W`, then select / install the latest version. You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/WiFiManager_RP2040W.svg?)](https://www.ardu-badge.com/WiFiManager_RP2040W) for more detailed instructions.

### Manual Install

1. Navigate to [WiFiManager_RP2040W](https://github.com/khoih-prog/WiFiManager_RP2040W) page.
2. Download the latest release `WiFiManager_RP2040W-main.zip`.
3. Extract the zip file to `WiFiManager_RP2040W-main` directory 
4. Copy the whole `WiFiManager_RP2040W-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**WiFiManager_RP2040W** library](https://registry.platformio.org/libraries/khoih-prog/WiFiManager_RP2040W) by using [**Library Manager**](https://registry.platformio.org/libraries/khoih-prog/WiFiManager_RP2040W/installation). Search for **WiFiManager_RP2040W** in [Platform.io **Author's Libraries**](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---


### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include this `.hpp` file

```
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "WiFiManager_RP2040W.hpp"     //https://github.com/khoih-prog/WiFiManager_RP2040W
```

in many files. But be sure to use the following `.h` file **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "WiFiManager_RP2040W.h"           //https://github.com/khoih-prog/WiFiManager_RP2040W
```

---
---

## How It Works

- The [ConfigOnDoubleReset](examples/ConfigOnDoubleReset) example shows how it works and should be used as the basis for a sketch that uses this library.

- Using any WiFi enabled device with a browser (computer, phone, tablet) connect to the newly created Access Point (AP) using configurable SSID and Password (specified in sketch)

```cpp
// SSID and PW for Config Portal
String ssid = "RP2040W_ConfigPortal";
const char* password = "RP2040W_Pass";
```

then connect WebBrowser to configurable ConfigPortal IP address, default is **192.168.42.1**

- Choose one of the access points scanned, enter password, click ***Save***.
- RP2040W will restart, then try to connect to the WiFi network using STA-only mode.

---
---

### HOWTO Basic configurations

#### 1. Using default for every configurable parameter

- Include in your sketch

```cpp
// Use from 0 to 4. Higher number, more debugging messages and memory usage.
#define _WIFIMGR_LOGLEVEL_    4

#include <WiFi.h>

#include <WiFiMulti_Generic.h>
WiFiMulti_Generic wifiMulti;

#include <LittleFS.h>

#define  CONFIG_FILENAME        ("/wifi_cred.dat")

#define DRD_FLAG_DATA_SIZE      4

#define DRD_GENERIC_DEBUG       true  //false

#include <DoubleResetDetector_Generic.h>      //https://github.com/khoih-prog/DoubleResetDetector_Generic

// Number of seconds after reset during which a
// subseqent reset will be considered a double reset.
#define DRD_TIMEOUT             10

// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS             0

//DoubleResetDetector_Generic drd(DRD_TIMEOUT, DRD_ADDRESS);
DoubleResetDetector_Generic* drd;//////

// SSID and PW for Config Portal
String ssid = "RP2040W_ConfigPortal";
const char* password = "RP2040W_Pass";

// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

#define LED_ON      HIGH
#define LED_OFF     LOW

#define MIN_AP_PASSWORD_SIZE    8

#define SSID_MAX_LEN            32
//From v1.0.10, WPA2 passwords can be up to 63 characters long.
#define PASS_MAX_LEN            64

typedef struct
{
  char wifi_ssid[SSID_MAX_LEN];
  char wifi_pw  [PASS_MAX_LEN];
}  WiFi_Credentials;

typedef struct
{
  String wifi_ssid;
  String wifi_pw;
}  WiFi_Credentials_String;

#define NUM_WIFI_CREDENTIALS      2

typedef struct
{
  WiFi_Credentials  WiFi_Creds [NUM_WIFI_CREDENTIALS];
} WM_Config;

WM_Config         WM_config;

//////

// Indicates whether ESP has WiFi credentials saved from previous session, or double reset detected
bool initialConfig = false;
//////

// Use false if you don't like to display Available Pages in Information Page of Config Portal
// Comment out or use true to display Available Pages in Information Page of Config Portal
// Must be placed before #include <WiFiManager_RP2040W.h>
#define USE_AVAILABLE_PAGES     false

// New in v1.0.11
#define USING_CORS_FEATURE          true

///////////////////////////////////////////////////

// Use DHCP
#warning Using DHCP IP
IPAddress stationIP   = IPAddress(0, 0, 0, 0);
IPAddress gatewayIP   = IPAddress(192, 168, 42, 1);
IPAddress netMask     = IPAddress(255, 255, 255, 0);

#define USE_CONFIGURABLE_DNS      false   //true

IPAddress dns1IP      = gatewayIP;

///////////////////////////////////////////////////

#define USE_CUSTOM_AP_IP          false

IPAddress APStaticIP  = IPAddress(192, 168, 42, 1);
IPAddress APStaticGW  = IPAddress(192, 168, 42, 1);
IPAddress APStaticSN  = IPAddress(255, 255, 255, 0);

#include <WiFiManager_RP2040W.h>              //https://github.com/khoih-prog/WiFiManager_RP2040W

```
---

#### 2. Using CORS (Cross-Origin Resource Sharing) feature

1. To use CORS feature with **default** CORS Header "*". Some WebBrowsers won't accept this allowing-all "*" CORS Header.

```cpp
// Default false for using only whenever necessary to avoid security issue
#define USING_CORS_FEATURE     true
```

2. To use CORS feature with specific CORS Header "Your Access-Control-Allow-Origin". **To be modified** according to your specific Allowed-Origin.

```cpp
// Default false for using only whenever necessary to avoid security issue
#define USING_CORS_FEATURE     true

...

#if USING_CORS_FEATURE
  RP2040W_WiFiManager.setCORSHeader("Your Access-Control-Allow-Origin");
#endif
```

3. Not use CORS feature (default)

```cpp
// Default false for using only whenever necessary to avoid security issue
#define USING_CORS_FEATURE     false
```

---

#### 3. Using MultiWiFi auto(Re)connect feature

1. In loop()

```cpp
void check_WiFi(void)
{
  if ( (WiFi.status() != WL_CONNECTED) )
  {
    Serial.println("\nWiFi lost. Call connectMultiWiFi in loop");
    connectMultiWiFi();
  }
}

void check_status(void)
{
  static ulong checkwifi_timeout    = 0;

  static ulong current_millis;

#define WIFICHECK_INTERVAL    1000L

  current_millis = millis();
  
  // Check WiFi every WIFICHECK_INTERVAL (1) seconds.
  if ((current_millis > checkwifi_timeout) || (checkwifi_timeout == 0))
  {
    check_WiFi();
    checkwifi_timeout = current_millis + WIFICHECK_INTERVAL;
  }
}

void loop()
{
  // put your main code here, to run repeatedly
  check_status();
}
```

---
---

### HOWTO Open Config Portal

- When you want to open a config portal, just add

```cpp
WiFiManager_RP2040W RP2040W_WiFiManager;
```

then later call

```cpp
RP2040W_WiFiManager.startConfigPortal()
```

While in AP mode, connect to it using its `SSID` ("RP2040W_ConfigPortal") / `Password` ("RP2040W_Pass"), then open a browser to the AP IP, default `192.168.42.1`, configure wifi then save. The WiFi connection information will be saved in non volatile memory. It will then reboot and autoconnect.


Once WiFi network information is saved in the `RP2040W`, it will try to autoconnect to WiFi every time it is started, without requiring any function calls in the sketch.

---
---

### Examples

 1. [ConfigOnDoubleReset](examples/ConfigOnDoubleReset)
 2. [ConfigOnStartup](examples/ConfigOnStartup)

---
---

## So, how it works?

In `ConfigPortal Mode`, it starts an access point called `RP2040W_ConfigPortal`. Connect to it using the `configurable password` you can define in the code. For example, `RP2040W_Pass` (see examples):

```cpp
// SSID and PW for Config Portal
String ssid = "RP2040W_ConfigPortal";
const char* password = "RP2040W_Pass";
```
After you connected, please, go to `http://192.168.42.1`, you'll see this `Main` page:

<p align="center">
    <img src="https://github.com/khoih-prog/WiFiManager_RP2040W/blob/main/Images/Main.png">
</p>

Select `Information` to enter the Info page where the board info will be shown (short page)

<p align="center">
    <img src="https://github.com/khoih-prog/WiFiManager_RP2040W/blob/main/Images/Info_Short.png">
</p>

Select `Configuration` to enter this page where you can select an AP and specify its WiFi Credentials

<p align="center">
    <img src="https://github.com/khoih-prog/WiFiManager_RP2040W/blob/main/Images/Configuration.png">
</p>

Enter your credentials, then click ***Save***. The WiFi Credentials will be saved and the board reboots to connect to the selected WiFi AP.

<p align="center">
    <img src="https://github.com/khoih-prog/WiFiManager_RP2040W/blob/main/Images/Saved.png">
</p>

If you're already connected to a listed WiFi AP and don't want to change anything, just select ***Exit Portal*** from the `Main` page to reboot the board and connect to the previously-stored AP. The WiFi Credentials are still intact.

---
---

## Documentation

#### Password protect the configuration Access Point

You can password protect the ConfigPortal AP.  Simply add an SSID as the first parameter and the password as a second parameter to `startConfigPortal`. See the above examples.
A short password seems to have unpredictable results so use one that's around 8 characters or more in length.
The guidelines are that a wifi password must consist of 8 to 63 ASCII-encoded characters in the range of 32 to 126 (decimal)

```cpp
RP2040W_WiFiManager.startConfigPortal( SSID , password )
```

#### Callbacks

##### Save settings

This gets called when custom parameters have been set **AND** a connection has been established. Use it to set a flag, so when all the configuration finishes, you can save the extra parameters somewhere.


```cpp
RP2040W_WiFiManager.setSaveConfigCallback(saveConfigCallback);
```
saveConfigCallback declaration and example

```cpp
//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () 
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
```

#### ConfigPortal Timeout

If you need to set a timeout so the `RP2040W` doesn't hang waiting to be configured for ever. 

```cpp
#define CONFIG_PORTAL_TIMEOUT_SEC       120

RP2040W_WiFiManager.setConfigPortalTimeout(CONFIG_PORTAL_TIMEOUT_SEC);
```

which will wait 2 minutes (120 seconds). When the time passes, the startConfigPortal function will return and continue the sketch, unless you're accessing the Config Portal. 

In this case, the `startConfigPortal` function will stay until you save config data or exit the Config Portal.


See  [ConfigOnDoubleReset](examples/ConfigOnDoubleReset) example for a more complex version.

---
---

#### Custom Parameters

Many applications need configuration parameters like `MQTT host and port`, [Blynk](http://www.blynk.cc) or [emoncms](http://emoncms.org) tokens, etc. While it is possible to use `WiFiManager_RP2040W` to collect additional parameters it is better to read these parameters from a web service once `WiFiManager_RP2040W` has been used to connect to the internet.

To capture other parameters with `WiFiManager_RP2040W` is a lot more involved than all the other features and requires adding custom HTML to your form. If you want to do it with `WiFiManager_RP2040W` see the example [ConfigOnDoubleReset](https://github.com/khoih-prog/WiFiManager_RP2040W/tree/main/examples/ConfigOnDoubleReset)

#### Custom IP Configuration

You can set a custom IP for both AP (access point, config mode) and STA (station mode, client mode, normal project state)

##### Custom Access Point IP Configuration (currently not working)

This will set your captive portal to a specific IP should you need/want such a feature. Add the following snippet before `startConfigPortal()`

```cpp
//set custom ip for portal
RP2040W_WiFiManager.setAPStaticIPConfig(IPAddress(192,168,42,1), IPAddress(192,168,42,1), IPAddress(255,255,255,0));
```

##### Custom Station (client) Static IP Configuration

This will use the specified IP configuration instead of using DHCP in station mode.
```cpp
RP2040W_WiFiManager.setSTAStaticIPConfig(IPAddress(192,168,2,180), IPAddress(192,168,2,1), IPAddress(255,255,255,0));
```

#### Custom HTML, CSS, Javascript

There are various ways in which you can inject custom HTML, CSS or Javascript into the ConfigPortal.

The options are:
- inject custom head element
You can use this to any html bit to the head of the ConfigPortal. If you add a `<style>` element, bare in mind it overwrites the included css, not replaces.

```cpp
RP2040W_WiFiManager.setCustomHeadElement("<style>html{filter: invert(100%); -webkit-filter: invert(100%);}</style>");
```

- inject a custom bit of html in the configuration form

```cpp
WIO_WMParameter custom_text("<p>This is just a text paragraph</p>");
RP2040W_WiFiManager.addParameter(&custom_text);
```

- inject a custom bit of html in a configuration form element
Just add the bit you want added as the last parameter to the custom parameter constructor.

```cpp
WIO_WMParameter custom_mqtt_server("server", "mqtt server", "iot.eclipse", 40, " readonly");
```

#### Filter Networks

You can filter networks based on signal quality and show/hide duplicate networks.

- If you would like to filter low signal quality networks you can tell WiFiManager to not show networks below an arbitrary quality %;

```cpp
RP2040W_WiFiManager.setMinimumSignalQuality(10);
```
will not show networks under 10% signal quality. If you omit the parameter it defaults to 8%;

- You can also remove or show duplicate networks (default is remove).
Use this function to show (or hide) all networks.

```cpp
RP2040W_WiFiManager.setRemoveDuplicateAPs(false);
```
---
---

### Example [ConfigOnDoubleReset](examples/ConfigOnDoubleReset)


https://github.com/khoih-prog/WiFiManager_RP2040W/blob/ad8a0071e17ad7f30195899db88d0b3294c1a5cd/examples/ConfigOnDoubleReset/ConfigOnDoubleReset.ino#L16-L561


---
---

## Debug Terminal Output Samples

### 1. ConfigOnDoubleReset

This is terminal debug output when running [ConfigOnDoubleReset](examples/ConfigOnDoubleReset) on **RASPBERRY_PI_PICO_W**. 

Config Portal (CP) was requested to input and save WiFi Credentials. The boards then connected to WiFi successfully. Then DRD was detected, CP was again requested to update WiFi Credentials.

#### DRD detected, Config Portal will open

```
Starting ConfigOnDoubleReset with DoubleResetDetect on RASPBERRY_PI_PICO_W
WiFiManager_RP2040W v1.0.0
[WM] RFC925 Hostname = RP2040W-179A4E
[WM] Set CORS Header to :  Your Access-Control-Allow-Origin
LittleFS Flag read = 0xd0d01234
Flag read = 0xd0d01234
doubleResetDetected
Saving to DRD file : 0xd0d04321
Saving DRD file OK
LittleFS Flag read = 0xd0d04321
ClearFlag write = 0xd0d04321
[WM] Open Config Portal without Timeout: Double Reset Detected
[WM] LoadWiFiCfgFile 
[WM] OK
[WM] * Add SSID =  HueNet1 , PW =  12345678
[WM] * Add SSID =  HueNet2 , PW =  12345678
[WM] Open Config Portal without Timeout: No stored Credentials.
[WM] Starting configuration portal.
[WM] WiFi.waitForConnectResult Done
[WM] SET AP
[WM] Configuring AP SSID = RP2040W_ConfigPortal
[WM] AP PWD = RP2040W_Pass
[WM] AP IP address = 192.168.42.1
[WM] HTTP server started
[WM] startConfigPortal : Enter loop
[WM] Handle root
[WM] captivePortal: hostHeader =  192.168.42.1
[WM] Handle WiFi
[WM] Scanning Network
[WM] scanWifiNetworks: Done, Scanned Networks n = 13
[WM] Sorting
[WM] Removing Dup
[WM] DUP AP: 
[WM] Index = 0
[WM] SSID = HueNet
[WM] RSSI = -32
[WM] Index = 1
[WM] SSID = HueNetTek
[WM] RSSI = -46
[WM] Index = 2
[WM] SSID = HueNet1
[WM] RSSI = -48
[WM] Index = 3
[WM] SSID = HueNet2
[WM] RSSI = -58
[WM] Index = 4
[WM] SSID = SmartRG-02a2
[WM] RSSI = -84
[WM] Index = 5
[WM] SSID = Guest5655
[WM] RSSI = -84
[WM] Index = 6
[WM] SSID = 
[WM] RSSI = -86
[WM] Index = 8
[WM] SSID = ESP151CD5
[WM] RSSI = -92
[WM] Index = 9
[WM] SSID = Rogers 786
[WM] RSSI = -93
[WM] Index = 10
[WM] SSID = Rogers5651
[WM] RSSI = -93
[WM] Index = 11
[WM] SSID = FishBowl
[WM] RSSI = -93
[WM] Index = 12
[WM] SSID = FishTank
[WM] RSSI = -95
[WM] Static IP = (IP unset)
[WM] Sent config page
[WM] WiFi save
[WM] TZ name = America/Toronto
[WM] Sent wifi save page
[WM] Connecting to new AP
[WM] Already connected. Bailing out.
[WM] WiFi connected...yeey :)
[WM] * Add SSID =  HueNet1 , PW =  12345678
[WM] * Add SSID =  HueNet2 , PW =  12345678
[WM] SaveWiFiCfgFile 
[WM] OK
After waiting 0.00 secs more in setup(), connection result is connected
[WM] freeing allocated params!
HH
```

#### Start normally with correct config data

```
Starting ConfigOnDoubleReset with DoubleResetDetect on RASPBERRY_PI_PICO_W
WiFiManager_RP2040W v1.0.0
[WM] RFC925 Hostname = RP2040W-11BD0A
[WM] Set CORS Header to :  Your Access-Control-Allow-Origin
LittleFS Flag read = 0xd0d04321
Flag read = 0xd0d04321
No doubleResetDetected
Saving DOUBLERESETDETECTOR_FLAG to DRD file : 0xd0d01234
Saving DRD file OK
SetFlag write = 0xd0d01234
[WM] No Double Reset Detected
[WM] LoadWiFiCfgFile 
[WM] OK
[WM] * Add SSID =  HueNet1 , PW =  12345678
[WM] * Add SSID =  HueNet2 , PW =  12345678
[WM] ConnectWiFi in setup
[WM] ConnectMultiWiFi with :
[WM] * Additional SSID =  HueNet1 , PW =  12345678
[WM] * Additional SSID =  HueNet2 , PW =  12345678
[WM] Connecting MultiWifi...
[WM] WiFi connected after time:  2
[WM] SSID: HueNet1 ,RSSI= 0
[WM] IP address: 192.168.2.180

[WM] freeing allocated params!
Stop doubleResetDetecting
Saving to DRD file : 0xd0d04321
Saving DRD file OK
LittleFS Flag read = 0xd0d04321
ClearFlag write = 0xd0d04321
HHH
```

---

### 2. ConfigOnStartup

This is terminal debug output when running [ConfigOnStartup](examples/ConfigOnStartup) on  **RASPBERRY_PI_PICO_W**.

This example will open a configuration portal for CONFIG_PORTAL_TIMEOUT_SEC seconds when first powered up if the boards has stored WiFi Credentials.

Otherwise, it'll stay indefinitely in ConfigPortal until getting WiFi Credentials and connecting to WiFi

#### No stored Credentials, Config Portal will open without timeout

```cpp
Starting ConfigOnStartup on RASPBERRY_PI_PICO_W
WiFiManager_RP2040W v1.0.0
[WM] RFC925 Hostname = RP2040W-221895
[WM] Set CORS Header to :  Your Access-Control-Allow-Origin
[WM] LoadWiFiCfgFile 
[WM] failed
[WM] Stored: SSID =  , Pass = 
[WM] Opening configuration portal.
[WM] Open Config Portal without Timeout: No stored Credentials.
[WM] Starting configuration portal.
[WM] WiFi.waitForConnectResult Done
[WM] SET AP
[WM] Configuring AP SSID = RP2040W_ConfigPortal
[WM] AP PWD = RP2040W_Pass
[WM] AP IP address = 192.168.42.1
[WM] HTTP server started
[WM] startConfigPortal : Enter loop
```

#### Restarted, Config Portal will open with 120s timeout

```
Starting ConfigOnStartup on RASPBERRY_PI_PICO_W
WiFiManager_RP2040W v1.0.0
[WM] RFC925 Hostname = RP2040W-148FE7
[WM] Set CORS Header to :  Your Access-Control-Allow-Origin
[WM] LoadWiFiCfgFile 
[WM] OK
[WM] * Add SSID =  HueNet1 , PW =  12345678
[WM] * Add SSID =  HueNet2 , PW =  12345678
[WM] Stored: SSID = HueNet2 , Pass = 12345678
[WM] Opening configuration portal.
[WM] * Add SSID =  HueNet2 , PW =  12345678
[WM] Got stored Credentials. Timeout for Config Portal = 120
[WM] Starting configuration portal.
[WM] WiFi.waitForConnectResult Done
[WM] SET AP
[WM] Configuring AP SSID = RP2040W_ConfigPortal
[WM] AP PWD = RP2040W_Pass
[WM] AP IP address = 192.168.42.1
[WM] HTTP server started
[WM] startConfigPortal : Enter loop
[WM] Connected after waiting (s) : 3.26
[WM] Local ip = (IP unset)
[WM] Timed out connection result: WL_CONNECT_FAILED
[WM] Not connected to WiFi but continuing anyway.
[WM] LoadWiFiCfgFile 
[WM] OK
[WM] * Add SSID =  HueNet1 , PW =  12345678
[WM] * Add SSID =  HueNet2 , PW =  12345678
[WM] ConnectMultiWiFi in setup
[WM] ConnectMultiWiFi with :
[WM] * LittleFS-stored Router_SSID =  HueNet2 , Router_Pass =  12345678
[WM] * Additional SSID =  HueNet1 , PW =  12345678
[WM] * Additional SSID =  HueNet2 , PW =  12345678
[WM] Connecting MultiWifi...
[WM] WiFi connected after time:  2
[WM] SSID: HueNet1 ,RSSI= 0
[WM] IP address: 192.168.2.180
After waiting 14.00 secs more in setup(), connection result is connected
Local IP = 192.168.2.180
[WM] freeing allocated params!
H
```

---
---

### Debug

Debug is enabled by default on Serial. To disable, add before `startConfigPortal()`

```cpp
RP2040W_WiFiManager.setDebugOutput(false);
```

You can also change the debugging level from 0 to 4

```cpp
// Use from 0 to 4. Higher number, more debugging messages and memory usage.
#define _WIFIMGR_LOGLEVEL_    3
```
---
---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the `arduino-pico` core for Arduino.

Sometimes, the library will only work if you update the `arduino-pico` core to the latest version because I am using newly added functions.


---

### Issues ###

Submit issues to: [WiFiManager_RP2040W issues](https://github.com/khoih-prog/WiFiManager_RP2040W/issues)

---
---


### TO DO

1. Bug Searching and Killing


### DONE

 1. Add support to **RP2040W** built-in CYW43439 WiFi using [`arduino-pico core`](https://github.com/earlephilhower/arduino-pico)

 
---
---

### Contributions and Thanks

1. Based on and modified from [Tzapu](https://github.com/tzapu/WiFiManager) and [KenTaylor's version]( https://github.com/kentaylor/WiFiManager)


<table>
  <tr>
    <td align="center"><a href="https://github.com/Tzapu"><img src="https://github.com/Tzapu.png" width="100px;" alt="Tzapu"/><br /><sub><b>⭐️ Tzapu</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/kentaylor"><img src="https://github.com/kentaylor.png" width="100px;" alt="kentaylor"/><br /><sub><b>⭐️ Ken Taylor</b></sub></a><br /></td>
  </tr> 
</table>

---

### Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License and credits ###

- The library is licensed under [MIT](https://github.com/khoih-prog/WiFiManager_RP2040W/blob/main/LICENSE)

---

## Copyright

Copyright 2022- Khoi Hoang


