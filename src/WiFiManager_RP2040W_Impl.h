/****************************************************************************************************************************
  WiFiManager_RP2040W_Impl.h
  For RP2040W boards

  WiFiManager_RP2040W is a library for the SeeedStudiocWIO Terminal/Arduino platform

  Modified from 
  1) Tzapu        https://github.com/tzapu/WiFiManager
  2) Ken Taylor   https://github.com/kentaylor
  3) Khoi Hoang   https://github.com/khoih-prog/WiFiManager_RP2040W

  Built by Khoi Hoang https://github.com/khoih-prog/WiFiManager_RP2040W
  Licensed under MIT license
  Version: 1.0.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      20/08/2022 Initial Coding for RP2040W
 *****************************************************************************************************************************/

#pragma once

#ifndef WiFiManager_RP2040W_Impl_h
#define WiFiManager_RP2040W_Impl_h


//////////////////////////////////////////

RP2040W_WMParameter::RP2040W_WMParameter(const char *custom)
{
  _WMParam_data._id = NULL;
  _WMParam_data._placeholder = NULL;
  _WMParam_data._length = 0;
  _WMParam_data._value = NULL;
  _WMParam_data._labelPlacement = WFM_LABEL_BEFORE;

  _customHTML = custom;
}

//////////////////////////////////////////

RP2040W_WMParameter::RP2040W_WMParameter(const char *id, const char *placeholder, const char *defaultValue, const int& length, const char *custom, const int& labelPlacement)
{
  init(id, placeholder, defaultValue, length, custom, labelPlacement);
}

//////////////////////////////////////////

RP2040W_WMParameter::RP2040W_WMParameter(const WMParam_Data& WMParam_data)
{
  init(WMParam_data._id, WMParam_data._placeholder, WMParam_data._value, WMParam_data._length, "", WMParam_data._labelPlacement);
}                  
//////
//////////////////////////////////////////

void RP2040W_WMParameter::init(const char *id, const char *placeholder, const char *defaultValue, const int& length, const char *custom, const int& labelPlacement)
{
  _WMParam_data._id = id;
  _WMParam_data._placeholder = placeholder;
  _WMParam_data._length = length;
  _WMParam_data._labelPlacement = labelPlacement;

  _WMParam_data._value = new char[_WMParam_data._length + 1];

  if (_WMParam_data._value != NULL)
  {
    memset(_WMParam_data._value, 0, _WMParam_data._length + 1);

    if (defaultValue != NULL)
    {
      strncpy(_WMParam_data._value, defaultValue, _WMParam_data._length);
    }
  }
  
  _customHTML = custom;
}

//////////////////////////////////////////

RP2040W_WMParameter::~RP2040W_WMParameter()
{
  if (_WMParam_data._value != NULL)
  {
    delete[] _WMParam_data._value;
  }
}

//////////////////////////////////////////

// Using Struct to get/set whole data at once
void RP2040W_WMParameter::setWMParam_Data(const WMParam_Data& WMParam_data)
{
  LOGINFO(F("setWMParam_Data"));
  
  memcpy(&_WMParam_data, &WMParam_data, sizeof(_WMParam_data));
}

//////////////////////////////////////////

void RP2040W_WMParameter::getWMParam_Data(WMParam_Data &WMParam_data)
{
  LOGINFO(F("getWMParam_Data"));
  
  memcpy(&WMParam_data, &_WMParam_data, sizeof(WMParam_data));
}

//////////////////////////////////////////

const char* RP2040W_WMParameter::getValue()
{
  return _WMParam_data._value;
}

//////////////////////////////////////////

const char* RP2040W_WMParameter::getID()
{
  return _WMParam_data._id;
}

//////////////////////////////////////////

const char* RP2040W_WMParameter::getPlaceholder()
{
  return _WMParam_data._placeholder;
}

//////////////////////////////////////////

int RP2040W_WMParameter::getValueLength()
{
  return _WMParam_data._length;
}

//////////////////////////////////////////

int RP2040W_WMParameter::getLabelPlacement()
{
  return _WMParam_data._labelPlacement;
}

//////////////////////////////////////////

const char* RP2040W_WMParameter::getCustomHTML()
{
  return _customHTML;
}

//////////////////////////////////////////

/**
   [getParameters description]
   @access public
*/
RP2040W_WMParameter** WiFiManager_RP2040W::getParameters() 
{
  return _params;
}

/**
   [getParametersCount description]
   @access public
*/
int WiFiManager_RP2040W::getParametersCount() 
{
  return _paramsCount;
}

//////////////////////////////////////////

char* WiFiManager_RP2040W::getRFC952_hostname(const char* iHostname)
{
  memset(RFC952_hostname, 0, sizeof(RFC952_hostname));

  size_t len = (RFC952_HOSTNAME_MAXLEN < strlen(iHostname)) ? RFC952_HOSTNAME_MAXLEN : strlen(iHostname);

  size_t j = 0;

  for (size_t i = 0; i < len - 1; i++)
  {
    if (isalnum(iHostname[i]) || iHostname[i] == '-')
    {
      RFC952_hostname[j] = iHostname[i];
      j++;
    }
  }
  
  // no '-' as last char
  if (isalnum(iHostname[len - 1]) || (iHostname[len - 1] != '-'))
    RFC952_hostname[j] = iHostname[len - 1];

  return RFC952_hostname;
}

//////////////////////////////////////////

WiFiManager_RP2040W::WiFiManager_RP2040W(const char *iHostname)
{
#if USE_DYNAMIC_PARAMS
  _max_params = WIFI_MANAGER_MAX_PARAMS;
  _params = (RP2040W_WMParameter**)malloc(_max_params * sizeof(RP2040W_WMParameter*));
#endif

  //WiFi not yet started here, must call WiFi.mode(WIFI_STA) and modify function WiFiGenericClass::mode(wifi_mode_t m) !!!

  WiFi.mode(WIFI_STA);

  if (iHostname[0] == 0)
  {
    // Random hostname if not set
    String _hostname = "RP2040W-" + String(micros(), HEX);

    _hostname.toUpperCase();

    getRFC952_hostname(_hostname.c_str());

  }
  else
  {
    // Prepare and store the hostname only not NULL
    getRFC952_hostname(iHostname);
  }

  LOGWARN1(F("RFC925 Hostname ="), RFC952_hostname);

  setHostname();

  networkIndices = NULL;
}

//////////////////////////////////////////

WiFiManager_RP2040W::~WiFiManager_RP2040W()
{
#if USE_DYNAMIC_PARAMS
  if (_params != NULL)
  {
    LOGINFO(F("freeing allocated params!"));

    free(_params);
  }
#endif

  if (networkIndices)
  {
    free(networkIndices); //indices array no longer required so free memory
  }
}

//////////////////////////////////////////

#if USE_DYNAMIC_PARAMS
bool WiFiManager_RP2040W::addParameter(RP2040W_WMParameter *p)
#else
void WiFiManager_RP2040W::addParameter(RP2040W_WMParameter *p)
#endif
{
#if USE_DYNAMIC_PARAMS

  if (_paramsCount == _max_params)
  {
    // rezise the params array
    _max_params += WIFI_MANAGER_MAX_PARAMS;
    
    LOGINFO1(F("Increasing _max_params to:"), _max_params);
    
    RP2040W_WMParameter** new_params = (RP2040W_WMParameter**)realloc(_params, _max_params * sizeof(RP2040W_WMParameter*));

    if (new_params != NULL)
    {
      _params = new_params;
    }
    else
    {
      LOGINFO(F("ERROR: failed to realloc params, size not increased!"));
 
      return false;
    }
  }

  _params[_paramsCount] = p;
  _paramsCount++;
  
  LOGINFO1(F("Adding parameter"), p->getID());
  
  return true;

#else

  // Danger here. Better to use Tzapu way here
  if (_paramsCount < (WIFI_MANAGER_MAX_PARAMS))
  {
    _params[_paramsCount] = p;
    _paramsCount++;
     
    LOGINFO1(F("Adding parameter"), p->getID());
  }
  else
  {
    LOGINFO("Can't add parameter. Full");
  }

#endif
}

//////////////////////////////////////////

void WiFiManager_RP2040W::setupConfigPortal()
{
  stopConfigPortal = false; //Signal not to close config portal

  dnsServer.reset(new DNSServer());

  server.reset(new WiFiWebServer(HTTP_PORT_TO_USE));

  // optional soft ip config
  // Must be put here before dns server start to take care of the non-default ConfigPortal AP IP.
  // Check (https://github.com/khoih-prog/ESP_WiFiManager/issues/58)
  if (_WiFi_AP_IPconfig._ap_static_ip)
  {
    LOGWARN3(F("Custom AP IP/GW/Subnet = "), _WiFi_AP_IPconfig._ap_static_ip, _WiFi_AP_IPconfig._ap_static_gw, _WiFi_AP_IPconfig._ap_static_sn);
    
    WiFi.softAPConfig(_WiFi_AP_IPconfig._ap_static_ip, _WiFi_AP_IPconfig._ap_static_gw, _WiFi_AP_IPconfig._ap_static_sn);
  }

  /* Setup the DNS server redirecting all the domains to the apIP */
  if (dnsServer)
  {
    dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
    
    // DNSServer started with "*" domain name, all DNS requests will be passsed to WiFi.softAPIP()
    if (! dnsServer->start(DNS_PORT, "*", WiFi.softAPIP()))
    {
      // No socket available
      LOGERROR(F("Can't start DNS Server. No available socket"));
    }
  }
  else
  {
    // No space available
    LOGERROR(F("Can't initiate DNS Server. No enough space"));
  }

  _configPortalStart = millis();

  LOGWARN1(F("Configuring AP SSID ="), _apName);

  if (_apPassword != NULL)
  {
    if (strlen(_apPassword) < 8 || strlen(_apPassword) > 63)
    {
      // fail passphrase to short or long!
      LOGERROR(F("Invalid AccessPoint password. Ignoring"));
      
      _apPassword = NULL;
    }
    LOGWARN1(F("AP PWD ="), _apPassword);
  }
  
  static int channel;
  
  // Use random channel if  _WiFiAPChannel == 0
  if (_WiFiAPChannel == 0)
    channel = (_configPortalStart % MAX_WIFI_CHANNEL) + 1;
  else
    channel = _WiFiAPChannel;
  
  if (_apPassword != NULL)
  {   
    //WiFi.softAP(_apName, _apPassword);//password option
    // AP channel for RP2040W not working in arduino-pico core, v2.4.0
    WiFi.softAP(_apName, _apPassword, channel);
  }
  else
  {
    // Can't use channel here
    WiFi.softAP(_apName);
  }
  //////
   
  delay(500); // Without delay I've seen the IP address blank
  
  LOGWARN1(F("AP IP address ="), WiFi.softAPIP());

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server->on("/", std::bind(&WiFiManager_RP2040W::handleRoot, this));
  server->on("/wifi", std::bind(&WiFiManager_RP2040W::handleWifi, this));
  server->on("/wifisave", std::bind(&WiFiManager_RP2040W::handleWifiSave, this));
  server->on("/close", std::bind(&WiFiManager_RP2040W::handleServerClose, this));
  server->on("/i", std::bind(&WiFiManager_RP2040W::handleInfo, this));
  server->on("/r", std::bind(&WiFiManager_RP2040W::handleReset, this));
  server->on("/state", std::bind(&WiFiManager_RP2040W::handleState, this));
  server->on("/scan", std::bind(&WiFiManager_RP2040W::handleScan, this));
  server->onNotFound(std::bind(&WiFiManager_RP2040W::handleNotFound, this));
  server->begin(); // Web server start
  
  LOGWARN(F("HTTP server started"));
}

//////////////////////////////////////////

bool WiFiManager_RP2040W::autoConnect()
{
  String ssid = "RP2040W_DEADBEEF";

  return autoConnect(ssid.c_str(), NULL);
}

// To permit autoConnect() to use STA static IP or DHCP IP.
#ifndef AUTOCONNECT_NO_INVALIDATE
  #define AUTOCONNECT_NO_INVALIDATE true
#endif

//////////////////////////////////////////

bool WiFiManager_RP2040W::autoConnect(char const *apName, char const *apPassword)
{
  LOGINFO(F("\nAutoConnect using previously saved SSID/PW, but keep previous settings"));
  // Connect to previously saved SSID/PW, but keep previous settings
  connectWifi();
 
  unsigned long startedAt = millis();

  while (millis() - startedAt < 10000)
  {
    //delay(100);
    delay(200);

    if (WiFi.status() == WL_CONNECTED)
    {
      float waited = (millis() - startedAt);
       
      LOGWARN1(F("Connected after waiting (s) :"), waited / 1000);
      LOGWARN1(F("Local ip ="), WiFi.localIP());
      
      return true;
    }
  }

  return startConfigPortal(apName, apPassword);
}

//////////////////////////////////////////

bool  WiFiManager_RP2040W::startConfigPortal()
{
  String ssid = "RP2040W_DEADBEEF";
  ssid.toUpperCase();

  return startConfigPortal(ssid.c_str(), NULL);
}

//////////////////////////////////////////

bool  WiFiManager_RP2040W::startConfigPortal(char const *apName, char const *apPassword)
{
  //setup AP
  int connRes = WiFi.waitForConnectResult();

  LOGINFO("WiFi.waitForConnectResult Done");

  if (connRes == WL_CONNECTED)
  {
    LOGINFO("SET AP_STA");
    
    //WiFi.mode(WIFI_AP_STA); //Dual mode not OK for RP2040W
    WiFi.mode(WIFI_AP);
  }
  else
  {
    LOGINFO("SET AP");

    WiFi.mode(WIFI_AP); // Dual mode becomes flaky if not connected to a WiFi network.
  }

  _apName = apName;
  _apPassword = apPassword;

  //notify we entered AP mode
  if (_apcallback != NULL)
  {
    LOGINFO("_apcallback");
    
    _apcallback(this);
  }

  connect = false;

  setupConfigPortal();

  bool TimedOut = true;

  LOGINFO("startConfigPortal : Enter loop");

  while (_configPortalTimeout == 0 || millis() < _configPortalStart + _configPortalTimeout)
  {
    //DNS
    dnsServer->processNextRequest();
    //HTTP
    server->handleClient();
  
    if (connect)
    {
      TimedOut = false;
      delay(2000);

      LOGERROR(F("Connecting to new AP"));

      // using user-provided  _ssid, _pass in place of system-stored ssid and pass
      if (connectWifi(_ssid, _pass) != WL_CONNECTED)
      {  
        LOGERROR(F("Failed to connect"));
    
        WiFi.mode(WIFI_AP); // Dual mode becomes flaky if not connected to a WiFi network.
      }
      else
      {
        //notify that configuration has changed and any optional parameters should be saved
        if (_savecallback != NULL)
        {
          //todo: check if any custom parameters actually exist, and check if they really changed maybe
          _savecallback();
        }
        break;
      }

      if (_shouldBreakAfterConfig)
      {
        //flag set to exit after config after trying to connect
        //notify that configuration has changed and any optional parameters should be saved
        if (_savecallback != NULL)
        {
          //todo: check if any custom parameters actually exist, and check if they really changed maybe
          _savecallback();
        }
        
        break;
      }
    }

    if (stopConfigPortal)
    {
      LOGERROR("Stop ConfigPortal");  	//KH
     
      stopConfigPortal = false;
      break;
    }
    
    yield();
  }

  WiFi.mode(WIFI_STA);
  
  if (TimedOut)
  {
    setHostname();

    // To fix static IP when CP not entered or timed-out
    setWifiStaticIP();
    
    // KH mod for RP2040W
    WiFi.begin(_ssid.c_str(), _pass.c_str());
    //////
    
    int connRes = waitForConnectResult();

    LOGERROR1("Timed out connection result:", getStatus(connRes));
  }

  server->stop();
  server.reset();
  dnsServer->stop();
  dnsServer.reset();

  return  WiFi.status() == WL_CONNECTED;
}

//////////////////////////////////////////

void WiFiManager_RP2040W::setWifiStaticIP()
{ 
  // check if we've got static_ip settings, if we do, use those.
  if (_WiFi_STA_IPconfig._sta_static_ip)
  {
    WiFi.config(_WiFi_STA_IPconfig._sta_static_ip, _WiFi_STA_IPconfig._sta_static_gw, _WiFi_STA_IPconfig._sta_static_sn);
    
    LOGWARN1(F("Custom STA IP/GW/Subnet : "), WiFi.localIP());
  }
}

//////////////////////////////////////////

int WiFiManager_RP2040W::reconnectWifi()
{
  int connectResult;
  
  // using user-provided  _ssid, _pass in place of system-stored ssid and pass
  if ( ( connectResult = connectWifi(_ssid, _pass) ) != WL_CONNECTED)
  {  
    LOGERROR1(F("Failed to connect to"), _ssid);
    
    if ( ( connectResult = connectWifi(_ssid1, _pass1) ) != WL_CONNECTED)
    {  
      LOGERROR1(F("Failed to connect to"), _ssid1);

    }
    else
      LOGERROR1(F("Connected to"), _ssid1);
  }
  else
      LOGERROR1(F("Connected to"), _ssid);
  
  return connectResult;
}

//////////////////////////////////////////

int WiFiManager_RP2040W::connectWifi(const String& ssid, const String& pass)
{
  // Add option if didn't input/update SSID/PW => Use the previous saved Credentials.
  // But update the Static/DHCP options if changed.
  if ( (ssid != "") || ( (ssid == "") && (WiFi_SSID() != "") ) )
  {   
    //fix for auto connect racing issue. Move up from v1.1.0 to avoid resetSettings()
    if (WiFi.status() == WL_CONNECTED)
    {
      LOGWARN(F("Already connected. Bailing out."));
      return WL_CONNECTED;
    }
  
    if (ssid != "")
      resetSettings();

    setWifiStaticIP();

    //WiFi.mode(WIFI_AP_STA); //Dual mode not OK for RP2040W
    WiFi.mode(WIFI_AP);

    setHostname();
    
    if (ssid != "")
    {
      // Start Wifi with new values.
      LOGWARN(F("Connect to new WiFi using new IP parameters"));
      
      WiFi.begin(ssid.c_str(), pass.c_str());
    }
    else
    {
      // Start Wifi with old values.
      LOGWARN(F("Connect to previous WiFi using new IP parameters"));
      
      // KH, TODO
      //WiFi.begin();
    }
  }
  else if (WiFi_SSID() == "")
  {
    LOGWARN(F("No saved credentials"));
  }

  int connRes = waitForConnectResult();
  LOGWARN1("Connection result: ", getStatus(connRes));

  return connRes;
}

//////////////////////////////////////////

uint8_t WiFiManager_RP2040W::waitForConnectResult()
{
  if (_connectTimeout == 0)
  {
    unsigned long startedAt = millis();
    
    // In ESP8266, WiFi.waitForConnectResult() @return wl_status_t (0-255) or -1 on timeout !!!
    // In ESP32, WiFi.waitForConnectResult() @return wl_status_t (0-255)
    // So, using int for connRes to be safe
    //int connRes = WiFi.waitForConnectResult();
    WiFi.waitForConnectResult();
    
    float waited = (millis() - startedAt);

    LOGWARN1(F("Connected after waiting (s) :"), waited / 1000);
    LOGWARN1(F("Local ip ="), WiFi.localIP());

    // Fix bug from v1.1.0+, connRes is sometimes not correct.
    //return connRes;
    return WiFi.status();
  }
  else
  {
    LOGERROR(F("Waiting WiFi connection with time out"));
    unsigned long start = millis();
    bool keepConnecting = true;
    uint8_t status;

    while (keepConnecting)
    {
      status = WiFi.status();
      if (millis() > start + _connectTimeout)
      {
        keepConnecting = false;
        LOGERROR(F("Connection timed out"));
      }

      if (status == WL_CONNECTED || status == WL_CONNECT_FAILED)
      {
        keepConnecting = false;
      }
      delay(100);
    }
    return status;
  }
}

//////////////////////////////////////////

//Convenient for debugging but wasteful of program space.
//Remove if short of space
const char* WiFiManager_RP2040W::getStatus(const int& status)
{
  switch (status)
  {
    case WL_IDLE_STATUS:
      return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL:
      return "WL_NO_SSID_AVAIL";
    case WL_CONNECTED:
      return "WL_CONNECTED";
    case WL_CONNECT_FAILED:
      return "WL_CONNECT_FAILED";
    case WL_DISCONNECTED:
      return "WL_DISCONNECTED";
    default:
      return "UNKNOWN";
  }
}

//////////////////////////////////////////

String WiFiManager_RP2040W::getConfigPortalSSID()
{
  return _apName;
}

//////////////////////////////////////////

String WiFiManager_RP2040W::getConfigPortalPW()
{
  return _apPassword;
}

//////////////////////////////////////////

void WiFiManager_RP2040W::resetSettings()
{
  LOGINFO(F("Previous settings invalidated"));
  
  WiFi.disconnect();

  delay(200);
  return;
}

//////////////////////////////////////////

void WiFiManager_RP2040W::setTimeout(const unsigned long& seconds)
{
  setConfigPortalTimeout(seconds);
}

//////////////////////////////////////////

void WiFiManager_RP2040W::setConfigPortalTimeout(const unsigned long& seconds)
{
  _configPortalTimeout = seconds * 1000;
}

//////////////////////////////////////////

void WiFiManager_RP2040W::setConnectTimeout(const unsigned long& seconds)
{
  _connectTimeout = seconds * 1000;
}

//////////////////////////////////////////

void WiFiManager_RP2040W::setDebugOutput(bool debug)
{
  _debug = debug;
}

//////////////////////////////////////////

// KH, new from v1.0.10 to enable dynamic/random channel
int WiFiManager_RP2040W::setConfigPortalChannel(const int& channel)
{
  // If channel < MIN_WIFI_CHANNEL - 1 or channel > MAX_WIFI_CHANNEL => channel = 1
  // If channel == 0 => will use random channel from MIN_WIFI_CHANNEL to MAX_WIFI_CHANNEL
  // If (MIN_WIFI_CHANNEL <= channel <= MAX_WIFI_CHANNEL) => use it
  if ( (channel < MIN_WIFI_CHANNEL - 1) || (channel > MAX_WIFI_CHANNEL) )
    _WiFiAPChannel = 1;
  else if ( (channel >= MIN_WIFI_CHANNEL - 1) && (channel <= MAX_WIFI_CHANNEL) )
    _WiFiAPChannel = channel;

  return _WiFiAPChannel;
}

//////////////////////////////////////////

void WiFiManager_RP2040W::setAPStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn)
{
  LOGINFO(F("setAPStaticIPConfig"));
  _WiFi_AP_IPconfig._ap_static_ip = ip;
  _WiFi_AP_IPconfig._ap_static_gw = gw;
  _WiFi_AP_IPconfig._ap_static_sn = sn;
}

//////////////////////////////////////////

void WiFiManager_RP2040W::setAPStaticIPConfig(const WiFi_AP_IPConfig&  WM_AP_IPconfig)
{
  LOGINFO(F("setAPStaticIPConfig"));
  
  memcpy((void*) &_WiFi_AP_IPconfig, &WM_AP_IPconfig, sizeof(_WiFi_AP_IPconfig));
}

//////////////////////////////////////////

void WiFiManager_RP2040W::getAPStaticIPConfig(WiFi_AP_IPConfig  &WM_AP_IPconfig)
{
  LOGINFO(F("getAPStaticIPConfig"));
  
  memcpy((void*) &WM_AP_IPconfig, &_WiFi_AP_IPconfig, sizeof(WM_AP_IPconfig));
}
//////
//////////////////////////////////////////

void WiFiManager_RP2040W::setSTAStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn)
{
  LOGINFO(F("setSTAStaticIPConfig"));
  _WiFi_STA_IPconfig._sta_static_ip = ip;
  _WiFi_STA_IPconfig._sta_static_gw = gw;
  _WiFi_STA_IPconfig._sta_static_sn = sn;
}

//////////////////////////////////////////


void WiFiManager_RP2040W::setSTAStaticIPConfig(const WiFi_STA_IPConfig& WM_STA_IPconfig)
{
  LOGINFO(F("setSTAStaticIPConfig"));
  
  memcpy((void*) &_WiFi_STA_IPconfig, &WM_STA_IPconfig, sizeof(_WiFi_STA_IPconfig));
}

//////////////////////////////////////////

void WiFiManager_RP2040W::getSTAStaticIPConfig(WiFi_STA_IPConfig &WM_STA_IPconfig)
{
  LOGINFO(F("getSTAStaticIPConfig"));
  
  memcpy((void*) &WM_STA_IPconfig, &_WiFi_STA_IPconfig, sizeof(WM_STA_IPconfig));
}
//////
//////////////////////////////////////////

#if USE_CONFIGURABLE_DNS
void WiFiManager_RP2040W::setSTAStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn, 
                                           const IPAddress& dns_address_1)
{
  LOGINFO(F("setSTAStaticIPConfig for USE_CONFIGURABLE_DNS"));
  _WiFi_STA_IPconfig._sta_static_ip = ip;
  _WiFi_STA_IPconfig._sta_static_gw = gw;
  _WiFi_STA_IPconfig._sta_static_sn = sn;
  _WiFi_STA_IPconfig._sta_static_dns1 = dns_address_1; //***** Added argument *****
}
#endif

//////////////////////////////////////////

void WiFiManager_RP2040W::setMinimumSignalQuality(const int& quality)
{
  _minimumQuality = quality;
}

//////////////////////////////////////////

void WiFiManager_RP2040W::setBreakAfterConfig(bool shouldBreak)
{
  _shouldBreakAfterConfig = shouldBreak;
}

//////////////////////////////////////////

void WiFiManager_RP2040W::reportStatus(String &page)
{
  page += PSTR(WM_HTTP_SCRIPT_NTP_MSG);

  if (WiFi_SSID() != "")
  {
    page += F("Configured to connect to access point <b>");
    page += WiFi_SSID();

    if (WiFi.status() == WL_CONNECTED)
    {
      page += F(" and currently connected</b> on IP <a href=\"http://");
      page += WiFi.localIP().toString();
      page += F("/\">");
      page += WiFi.localIP().toString();
      page += F("</a>");
    }
    else
    {
      page += F(" but not currently connected</b> to network.");
    }
  }
  else
  {
    page += F("No network currently configured.");
  }
}

//////////////////////////////////////////

/** Handle root or redirect to captive portal */
void WiFiManager_RP2040W::handleRoot()
{
  LOGDEBUG(F("Handle root"));

  // Disable _configPortalTimeout when someone accessing Portal to give some time to config
  _configPortalTimeout = 0;		//KH

  if (captivePortal())
  {
    // If caprive portal redirect instead of displaying the error page.
    return;
  }

  server->sendHeader(PSTR(WM_HTTP_CACHE_CONTROL), PSTR(WM_HTTP_NO_STORE));

#if USING_CORS_FEATURE
  // For configure CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
  server->sendHeader(PSTR(WM_HTTP_CORS), _CORS_Header);
#endif
    
  server->sendHeader(PSTR(WM_HTTP_PRAGMA), PSTR(WM_HTTP_NO_CACHE));
  server->sendHeader(PSTR(WM_HTTP_EXPIRES), "-1");

  String page = PSTR(WM_HTTP_HEAD_START);
  
  page.replace("{v}", "Options");
  page += PSTR(WM_HTTP_SCRIPT);
  page += PSTR(WM_HTTP_SCRIPT_NTP);
  page += PSTR(WM_HTTP_STYLE);
  page += _customHeadElement;
  page += PSTR(WM_HTTP_HEAD_END);
  page += "<h2>";
  //page += _apName;
  page += "RP2040W";

  if (WiFi_SSID() != "")
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      page += " on ";
      page += WiFi_SSID();
    }
    else
    {
      page += " <s>on ";
      page += WiFi_SSID();
      page += "</s>";
    }
  }

  page += "</h2>";
  page += PSTR(WM_HTTP_PORTAL_OPTIONS);
  page += F("<div class=\"msg\">");
  reportStatus(page);
  page += F("</div>");
  page += PSTR(WM_HTTP_END);

  server->send(200, "text/html", page);

}

//////////////////////////////////////////

/** Wifi config page handler */
void WiFiManager_RP2040W::handleWifi()
{
  LOGDEBUG(F("Handle WiFi"));

  // Disable _configPortalTimeout when someone accessing Portal to give some time to config
  _configPortalTimeout = 0;		//KH
  
  server->sendHeader(PSTR(WM_HTTP_CACHE_CONTROL), PSTR(WM_HTTP_NO_STORE));

#if USING_CORS_FEATURE
  // For configure CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
  server->sendHeader(PSTR(WM_HTTP_CORS), _CORS_Header);
#endif
    
  server->sendHeader(PSTR(WM_HTTP_PRAGMA), PSTR(WM_HTTP_NO_CACHE));
  server->sendHeader(PSTR(WM_HTTP_EXPIRES), "-1");
  
  String page = PSTR(WM_HTTP_HEAD_START);
  
  page.replace("{v}", "Config RP2040W");
  page += PSTR(WM_HTTP_SCRIPT);
  page += PSTR(WM_HTTP_SCRIPT_NTP);
  page += PSTR(WM_HTTP_STYLE);
  page += _customHeadElement;
  page += PSTR(WM_HTTP_HEAD_END);
  page += F("<h2>Configuration</h2>");

  //  KH, New, v1.0.6+
  numberOfNetworks = scanWifiNetworks(&networkIndices);

  //Print list of WiFi networks that were found in earlier scan
  if (numberOfNetworks == 0)
  {
    page += F("No network found. Refresh to scan again.");
  }
  else
  {
    // From v1.0.10
    page += PSTR(WM_FLDSET_START);
    //////
    
    //display networks in page
    for (int i = 0; i < numberOfNetworks; i++)
    {
      if (networkIndices[i] == -1)
        continue; // skip dups and those that are below the required quality
      
      LOGDEBUG1(F("Index ="), i);
      LOGDEBUG1(F("SSID ="), WiFi.SSID(networkIndices[i]));
      LOGDEBUG1(F("RSSI ="), WiFi.RSSI(networkIndices[i]));

      int quality = getRSSIasQuality(WiFi.RSSI(networkIndices[i]));

      String item = PSTR(WM_HTTP_ITEM);
      String rssiQ;
      rssiQ += quality;
      item.replace("{v}", WiFi.SSID(networkIndices[i]));
      item.replace("{r}", rssiQ);

      if (WiFi.encryptionType(networkIndices[i]) != ENC_TYPE_NONE)
      {
        item.replace("{i}", "l");
      }
      else
      {
        item.replace("{i}", "");
      }

      //LOGDEBUG(item);
      page += item;
      delay(0);
    }
    
    // From v1.0.10
    page += PSTR(WM_FLDSET_END);
    //////

    page += "<br/>";
  }

  page += PSTR(WM_HTTP_FORM_START);
  char parLength[2];
  
  page += PSTR(WM_FLDSET_START);
  
  // add the extra parameters to the form
  for (int i = 0; i < _paramsCount; i++)
  {
    if (_params[i] == NULL)
    {
      break;
    }
    
    String pitem;
    switch (_params[i]->getLabelPlacement())
    {
      case WFM_LABEL_BEFORE:
        pitem = PSTR(WM_HTTP_FORM_LABEL_BEFORE);
        break;
      case WFM_LABEL_AFTER:
        pitem = PSTR(WM_HTTP_FORM_LABEL_AFTER);
        break;
      default:
        // WFM_NO_LABEL
        pitem = PSTR(WM_HTTP_FORM_PARAM);
        break;
    }

    if (_params[i]->getID() != NULL)
    {
      pitem.replace("{i}", _params[i]->getID());
      pitem.replace("{n}", _params[i]->getID());
      pitem.replace("{p}", _params[i]->getPlaceholder());
      snprintf(parLength, 2, "%d", _params[i]->getValueLength());
      pitem.replace("{l}", parLength);
      pitem.replace("{v}", _params[i]->getValue());
      pitem.replace("{c}", _params[i]->getCustomHTML());
    }
    else
    {
      pitem = _params[i]->getCustomHTML();
    }

    page += pitem;
  }
  
  // From v1.0.10
  if (_paramsCount > 0)
  {
    page += PSTR(WM_FLDSET_END);
  }
  //////

  if (_params[0] != NULL)
  {
    page += "<br/>";
  }

  LOGDEBUG1(F("Static IP ="), _WiFi_STA_IPconfig._sta_static_ip.toString());
  
  // KH, Comment out in v1.0.9 to permit changing from DHCP to static IP, or vice versa
  // and add staticIP label in CP
  
  // From v1.0.10 to permit disable/enable StaticIP configuration in Config Portal from sketch. Valid only if DHCP is used.
  // You'll loose the feature of dynamically changing from DHCP to static IP, or vice versa
  // You have to explicitly specify false to disable the feature.

#if !USE_STATIC_IP_CONFIG_IN_CP
  if (_WiFi_STA_IPconfig._sta_static_ip)
#endif  
  {
    page += PSTR(WM_FLDSET_START);
    //////
    
    String item = PSTR(WM_HTTP_FORM_LABEL);
    item += PSTR(WM_HTTP_FORM_PARAM);
    item.replace("{i}", "ip");
    item.replace("{n}", "ip");
    item.replace("{p}", "Static IP");
    item.replace("{l}", "15");
    item.replace("{v}", _WiFi_STA_IPconfig._sta_static_ip.toString());

    page += item;

    item = PSTR(WM_HTTP_FORM_LABEL);
    item += PSTR(WM_HTTP_FORM_PARAM);
    item.replace("{i}", "gw");
    item.replace("{n}", "gw");
    item.replace("{p}", "Gateway IP");
    item.replace("{l}", "15");
    item.replace("{v}", _WiFi_STA_IPconfig._sta_static_gw.toString());

    page += item;

    item = PSTR(WM_HTTP_FORM_LABEL);
    item += PSTR(WM_HTTP_FORM_PARAM);
    item.replace("{i}", "sn");
    item.replace("{n}", "sn");
    item.replace("{p}", "Subnet");
    item.replace("{l}", "15");
    item.replace("{v}", _WiFi_STA_IPconfig._sta_static_sn.toString());

  #if USE_CONFIGURABLE_DNS
    //***** Added for DNS address options *****
    page += item;

    item = PSTR(WM_HTTP_FORM_LABEL);
    item += PSTR(WM_HTTP_FORM_PARAM);
    item.replace("{i}", "dns1");
    item.replace("{n}", "dns1");
    item.replace("{p}", "DNS1 IP");
    item.replace("{l}", "15");
    item.replace("{v}", _WiFi_STA_IPconfig._sta_static_dns1.toString());
    //***** End added for DNS address options *****
  #endif

    page += item;
    
    // From v1.0.10
    page += PSTR(WM_FLDSET_END);
    //////

    page += "<br/>";
  }
  
  page += PSTR(WM_HTTP_SCRIPT_NTP_HIDDEN);

  page += PSTR(WM_HTTP_FORM_END);

  page += PSTR(WM_HTTP_END);

  server->send(200, "text/html", page);

  LOGDEBUG(F("Sent config page"));
}

//////////////////////////////////////////

/** Handle the WLAN save form and redirect to WLAN config page again */
void WiFiManager_RP2040W::handleWifiSave()
{
  LOGDEBUG(F("WiFi save"));

  //SAVE/connect here
  _ssid = server->arg("s").c_str();
  _pass = server->arg("p").c_str();

  _ssid1 = server->arg("s1").c_str();
  _pass1 = server->arg("p1").c_str();
  
  ///////////////////////
  
#if USING_CORS_FEATURE
  // For configure CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
  server->sendHeader(PSTR(WM_HTTP_CORS), _CORS_Header);
#endif  

#if USE_WIFIMANAGER_RP2040W_NTP 
  
  if (server->arg("timezone") != "")
  { 
    _timezoneName = server->arg("timezone");
    LOGDEBUG1(F("TZ name ="), _timezoneName);
  }
  else
  {
    LOGDEBUG(F("No TZ arg"));
  }

#endif
  ///////////////////////
  
  //parameters
  for (int i = 0; i < _paramsCount; i++)
  {
    if (_params[i] == NULL)
    {
      break;
    }

    //read parameter
    String value = server->arg(_params[i]->getID()).c_str();
    
    //store it in array
    
    value.toCharArray(_params[i]->_WMParam_data._value, _params[i]->_WMParam_data._length);
    //////
    
    LOGDEBUG2(F("Parameter and value :"), _params[i]->getID(), value);
  }

  if (server->arg("ip") != "")
  {
    String ip = server->arg("ip");
    optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_ip, ip.c_str());
  }

  if (server->arg("gw") != "")
  {
    String gw = server->arg("gw");
    optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_gw, gw.c_str());
  }

  if (server->arg("sn") != "")
  {
    String sn = server->arg("sn");
    optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_sn, sn.c_str());
  }

#if USE_CONFIGURABLE_DNS
  //*****  Added for DNS Options *****
  if (server->arg("dns1") != "")
  {
    String dns1 = server->arg("dns1");
    optionalIPFromString(&_WiFi_STA_IPconfig._sta_static_dns1, dns1.c_str());
  }
  //*****  End added for DNS Options *****
#endif

  String page = PSTR(WM_HTTP_HEAD_START);
  
  page.replace("{v}", "Credentials Saved");
  page += PSTR(WM_HTTP_SCRIPT);
  page += PSTR(WM_HTTP_STYLE);
  page += _customHeadElement;
  page += PSTR(WM_HTTP_HEAD_END);
  page += PSTR(WM_HTTP_SAVED);
  page.replace("{v}", _apName);
  page.replace("{x}", _ssid);
  
  // KH, update from v1.1.0
  page.replace("{x1}", _ssid1);
  //////
  
  page += PSTR(WM_HTTP_END);

  server->send(200, "text/html", page);

  LOGDEBUG(F("Sent wifi save page"));

  connect = true; //signal ready to connect/reset

  // Restore when Press Save WiFi
  _configPortalTimeout = DEFAULT_PORTAL_TIMEOUT;
}

//////////////////////////////////////////

/** Handle shut down the server page */
void WiFiManager_RP2040W::handleServerClose()
{
  LOGDEBUG(F("Server Close"));
  
  server->sendHeader(PSTR(WM_HTTP_CACHE_CONTROL), PSTR(WM_HTTP_NO_STORE));

#if USING_CORS_FEATURE
  // For configure CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
  server->sendHeader(PSTR(WM_HTTP_CORS), _CORS_Header);
#endif
    
  server->sendHeader(PSTR(WM_HTTP_PRAGMA), PSTR(WM_HTTP_NO_CACHE));
  server->sendHeader(PSTR(WM_HTTP_EXPIRES), "-1");
  
  String page = PSTR(WM_HTTP_HEAD_START);
  
  page.replace("{v}", "Close Server");
  page += PSTR(WM_HTTP_SCRIPT);
  page += PSTR(WM_HTTP_STYLE);
  page += _customHeadElement;
  page += PSTR(WM_HTTP_HEAD_END);
  page += F("<div class=\"msg\">");
  page += F("My network is <b>");
  page += WiFi_SSID();
  page += F("</b><br>");
  page += F("IP address is <b>");
  page += WiFi.localIP().toString();
  page += F("</b><br><br>");
  page += F("Portal closed...<br><br>");
  
  //page += F("Push button on device to restart configuration server!");
  
  page += PSTR(WM_HTTP_END);
  
  server->send(200, "text/html", page);
  
  stopConfigPortal = true; //signal ready to shutdown config portal
  
  LOGDEBUG(F("Sent server close page"));

  // Restore when Press Save WiFi
  _configPortalTimeout = DEFAULT_PORTAL_TIMEOUT;
}

//////////////////////////////////////////

/** Handle the info page */
void WiFiManager_RP2040W::handleInfo()
{
  LOGDEBUG(F("Info"));

  // Disable _configPortalTimeout when someone accessing Portal to give some time to config
  _configPortalTimeout = 0;		//KH

  server->sendHeader(PSTR(WM_HTTP_CACHE_CONTROL), PSTR(WM_HTTP_NO_STORE));

#if USING_CORS_FEATURE
  // For configure CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
  server->sendHeader(PSTR(WM_HTTP_CORS), _CORS_Header);
#endif
    
  server->sendHeader(PSTR(WM_HTTP_PRAGMA), PSTR(WM_HTTP_NO_CACHE));
  server->sendHeader(PSTR(WM_HTTP_EXPIRES), "-1");
  
  String page = PSTR(WM_HTTP_HEAD_START);
  
  page.replace("{v}", "Info");
  page += PSTR(WM_HTTP_SCRIPT);
  page += PSTR(WM_HTTP_SCRIPT_NTP);
  page += PSTR(WM_HTTP_STYLE);
  page += _customHeadElement;
  page += PSTR(WM_HTTP_HEAD_END);
  
  page += F("<h2>WiFi Information</h2>");
  
  reportStatus(page);
  
  page += PSTR(WM_FLDSET_START);
  
  page += F("<h3>Device Data</h3>");
  page += F("<table class=\"table\">");
  page += F("<thead><tr><th>Name</th><th>Value</th></tr></thead><tbody><tr><td>Chip ID</td><td>");

  //page += String("DEADBEEF");		//ESP.getChipId();
  page += F("DEADBEEF");					//ESP.getChipId();

  page += F("</td></tr>");
  page += F("<tr><td>Flash Chip ID</td><td>");

  //page += String(ESP.getFlashChipId(), HEX);		//ESP.getFlashChipId();
  page += F("BEEFDEAD");					//ESP.getChipId();

  page += F("</td></tr>");
  
  // TODO
  page += F("TODO");

  page += F(" bytes</td></tr>");
  page += F("<tr><td>Access Point IP</td><td>");
  page += WiFi.softAPIP().toString();
  page += F("</td></tr>");
  page += F("<tr><td>Access Point MAC</td><td>");
  page += WiFi.softAPmacAddress();
  page += F("</td></tr>");

  page += F("<tr><td>SSID</td><td>");
  page += WiFi_SSID();
  page += F("</td></tr>");

  page += F("<tr><td>Station IP</td><td>");
  page += WiFi.localIP().toString();
  page += F("</td></tr>");

  page += PSTR(WM_FLDSET_END);
  
#if USE_AVAILABLE_PAGES  
  page += PSTR(WM_FLDSET_START);
  
  page += PSTR(WM_HTTP_AVAILABLE_PAGES);
  
  page += PSTR(WM_FLDSET_END);
#endif

  page += F("<p/>More information about WiFiManager_RP2040W at");
  page += F("<p/><a href=\"https://github.com/khoih-prog/WiFiManager_RP2040W\">https://github.com/khoih-prog/WiFiManager_RP2040W</a>");
  page += PSTR(WM_HTTP_END);

  server->send(200, "text/html", page);

  LOGDEBUG(F("Sent info page"));
}

//////////////////////////////////////////

/** Handle the state page */
void WiFiManager_RP2040W::handleState()
{
  LOGDEBUG(F("State - json"));
  
  server->sendHeader(PSTR(WM_HTTP_CACHE_CONTROL), PSTR(WM_HTTP_NO_STORE));

#if USING_CORS_FEATURE
  // For configure CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
  server->sendHeader(PSTR(WM_HTTP_CORS), _CORS_Header);
#endif
    
  server->sendHeader(PSTR(WM_HTTP_PRAGMA), PSTR(WM_HTTP_NO_CACHE));
  server->sendHeader(PSTR(WM_HTTP_EXPIRES), "-1");
  
  String page = F("{\"Soft_AP_IP\":\"");
  
  page += WiFi.softAPIP().toString();
  page += F("\",\"Soft_AP_MAC\":\"");
  page += WiFi.softAPmacAddress();
  page += F("\",\"Station_IP\":\"");
  page += WiFi.localIP().toString();
   
  page += F("\",");

  page += F("\"SSID\":\"");
  page += WiFi_SSID();
  page += F("\"}");
  
  server->send(200, "application/json", page);
  
  LOGDEBUG(F("Sent state page in json format"));
}

//////////////////////////////////////////

/** Handle the scan page */
void WiFiManager_RP2040W::handleScan()
{
  LOGDEBUG(F("Scan"));

  // Disable _configPortalTimeout when someone accessing Portal to give some time to config
  _configPortalTimeout = 0;		//KH

  LOGDEBUG(F("State-Json"));
  
  server->sendHeader(PSTR(WM_HTTP_CACHE_CONTROL), PSTR(WM_HTTP_NO_STORE));

#if USING_CORS_FEATURE
  // For configure CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
  server->sendHeader(PSTR(WM_HTTP_CORS), _CORS_Header);
#endif
    
  server->sendHeader(PSTR(WM_HTTP_PRAGMA), PSTR(WM_HTTP_NO_CACHE));
  server->sendHeader(PSTR(WM_HTTP_EXPIRES), "-1");

  int n;
  int *indices;

  //Space for indices array allocated on heap in scanWifiNetworks
  //and should be freed when indices no longer required.

  n = scanWifiNetworks(&indices);
  LOGDEBUG(F("In handleScan, scanWifiNetworks done"));
  String page = F("{\"Access_Points\":[");

  //display networks in page
  for (int i = 0; i < n; i++)
  {
    if (indices[i] == -1)
      continue; // skip duplicates and those that are below the required quality

    if (i != 0)
      page += F(", ");

    LOGDEBUG1(F("Index ="), i);
    LOGDEBUG1(F("SSID ="), WiFi.SSID(indices[i]));
    LOGDEBUG1(F("RSSI ="), WiFi.RSSI(indices[i]));

    int quality = getRSSIasQuality(WiFi.RSSI(indices[i]));
    String item = PSTR(JSON_ITEM);
    String rssiQ;
    
    rssiQ += quality;
    item.replace("{v}", WiFi.SSID(indices[i]));
    item.replace("{r}", rssiQ);

    if (WiFi.encryptionType(indices[i]) != ENC_TYPE_NONE)
    {
      item.replace("{i}", "true");
    }
    else
    {
      item.replace("{i}", "false");
    }
    //LOGDEBUG(item);
    page += item;
    delay(0);
  }

  if (indices)
  {
    free(indices); //indices array no longer required so free memory
  }

  page += F("]}");
  
  server->send(200, "application/json", page);
  
  LOGDEBUG(F("Sent WiFiScan Data in Json format"));
}

//////////////////////////////////////////

/** Handle the reset page */
void WiFiManager_RP2040W::handleReset()
{
  LOGDEBUG(F("Reset"));
  
  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");
  
  String page = PSTR(WM_HTTP_HEAD_START);
  
  page.replace("{v}", "WiFi Information");
  page += PSTR(WM_HTTP_SCRIPT);
  page += PSTR(WM_HTTP_STYLE);
  page += _customHeadElement;
  page += PSTR(WM_HTTP_HEAD_END);
  page += F("Resetting");
  page += PSTR(WM_HTTP_END);
  
  server->send(200, "text/html", page);

  LOGDEBUG(F("Sent reset page"));
  delay(5000);
  
  // Temporary fix for issue of not clearing WiFi SSID/PW from flash of ESP32
  // See https://github.com/khoih-prog/WiFiManager_RP2040W/issues/25 and https://github.com/espressif/arduino-esp32/issues/400
  resetSettings();
  //////

	// Restart for RP2040W, use either one is OK
	NVIC_SystemReset();
	//rp2040.reboot();

  delay(2000);
}

//////////////////////////////////////////

void WiFiManager_RP2040W::handleNotFound()
{
  if (captivePortal())
  {
    // If caprive portal redirect instead of displaying the error page.
    return;
  }

  String message = "File Not Found\n\n";
  
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";

  for (uint8_t i = 0; i < server->args(); i++)
  {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }

  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");
  
  server->send(404, "text/plain", message);
}

//////////////////////////////////////////

/**
   HTTPD redirector
   Redirect to captive portal if we got a request for another domain.
   Return true in that case so the page handler do not try to handle the request again.
*/
bool WiFiManager_RP2040W::captivePortal()
{
  LOGDEBUG1(F("captivePortal: hostHeader = "), server->hostHeader());
  
  if (!isIp(server->hostHeader()))
  {
    LOGINFO1(F("Request redirected to captive portal : "), server->client().localIP());
    
    server->sendHeader(F("Location"), (String)F("http://") + toStringIp(server->client().localIP()), true);
    server->send(302, PSTR(WM_HTTP_HEAD_CT2), ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server->client().stop(); // Stop is needed because we sent no content length
    
    return true;
  }
  return false;
}

//////////////////////////////////////////

//start up config portal callback
void WiFiManager_RP2040W::setAPCallback(void(*func)(WiFiManager_RP2040W* myWiFiManager))
{
  _apcallback = func;
}

//////////////////////////////////////////

//start up save config callback
void WiFiManager_RP2040W::setSaveConfigCallback(void(*func)())
{
  _savecallback = func;
}

//////////////////////////////////////////

//sets a custom element to add to head, like a new style tag
void WiFiManager_RP2040W::setCustomHeadElement(const char* element) {
  _customHeadElement = element;
}

//if this is true, remove duplicated Access Points - defaut true
void WiFiManager_RP2040W::setRemoveDuplicateAPs(bool removeDuplicates)
{
  _removeDuplicateAPs = removeDuplicates;
}

//////////////////////////////////////////

//Scan for WiFiNetworks in range and sort by signal strength
//space for indices array allocated on the heap and should be freed when no longer required
int WiFiManager_RP2040W::scanWifiNetworks(int **indicesptr)
{
  LOGDEBUG(F("Scanning Network"));

  int n = WiFi.scanNetworks();

  LOGDEBUG1(F("scanWifiNetworks: Done, Scanned Networks n ="), n); 

  //KH, Terrible bug here. WiFi.scanNetworks() returns n < 0 => malloc( negative == very big ) => crash!!!
  //In .../esp32/libraries/WiFi/src/WiFiType.h
  //#define WIFI_SCAN_RUNNING   (-1)
  //#define WIFI_SCAN_FAILED    (-2)
  //if (n == 0)
  if (n <= 0)
  {
    LOGDEBUG(F("No network found"));
    return (0);
  }
  else
  {
    // Allocate space off the heap for indices array.
    // This space should be freed when no longer required.
    int* indices = (int *)malloc(n * sizeof(int));

    if (indices == NULL)
    {
      LOGDEBUG(F("ERROR: Out of memory"));
      *indicesptr = NULL;
      return (0);
    }

    *indicesptr = indices;
   
    //sort networks
    for (int i = 0; i < n; i++)
    {
      indices[i] = i;
    }

    LOGDEBUG(F("Sorting"));

    // RSSI SORT
    // old sort
    for (int i = 0; i < n; i++)
    {
      for (int j = i + 1; j < n; j++)
      {
        if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i]))
        {
          std::swap(indices[i], indices[j]);
        }
      }
    }

    LOGDEBUG(F("Removing Dup"));

    // remove duplicates ( must be RSSI sorted )
    if (_removeDuplicateAPs)
    {
      String cssid;
      for (int i = 0; i < n; i++)
      {
        if (indices[i] == -1)
          continue;

        cssid = WiFi.SSID(indices[i]);
        for (int j = i + 1; j < n; j++)
        {
          if (cssid == WiFi.SSID(indices[j]))
          {
            LOGDEBUG1("DUP AP:", WiFi.SSID(indices[j]));
            indices[j] = -1; // set dup aps to index -1
          }
        }
      }
    }

    for (int i = 0; i < n; i++)
    {
      if (indices[i] == -1)
        continue; // skip dups

      int quality = getRSSIasQuality(WiFi.RSSI(indices[i]));

      if (!(_minimumQuality == -1 || _minimumQuality < quality))
      {
        indices[i] = -1;
        LOGDEBUG(F("Skipping low quality"));
      }
    }

#if (DEBUG_WIFIMGR > 2)
    for (int i = 0; i < n; i++)
    {
      if (indices[i] == -1)
        continue; // skip dups
      else
        Serial.println(WiFi.SSID(indices[i]));
    }
#endif

    return (n);
  }
}

//////////////////////////////////////////

int WiFiManager_RP2040W::getRSSIasQuality(const int& RSSI)
{
  int quality = 0;

  if (RSSI <= -100)
  {
    quality = 0;
  }
  else if (RSSI >= -50)
  {
    quality = 100;
  }
  else
  {
    quality = 2 * (RSSI + 100);
  }

  return quality;
}

//////////////////////////////////////////

/** Is this an IP? */
bool WiFiManager_RP2040W::isIp(const String& str)
{
  for (unsigned int i = 0; i < str.length(); i++)
  {
    int c = str.charAt(i);

    if (c != '.' && c != ':' && (c < '0' || c > '9'))
    {
      return false;
    }
  }
  return true;
}

//////////////////////////////////////////

/** IP to String? */
String WiFiManager_RP2040W::toStringIp(const IPAddress& ip)
{
  String res = "";
  for (int i = 0; i < 3; i++)
  {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }

  res += String(((ip >> 8 * 3)) & 0xFF);

  return res;
}

//////////////////////////////////////////

#endif    //WiFiManager_RP2040W_Impl_h
