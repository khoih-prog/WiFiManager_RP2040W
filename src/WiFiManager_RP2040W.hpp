/****************************************************************************************************************************
  WiFiManager_RP2040W.hpp
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

#ifndef WiFiManager_RP2040W_hpp
#define WiFiManager_RP2040W_hpp

#if ( defined(ARDUINO_RASPBERRY_PI_PICO_W) )
  #define USE_WIFI_NINA         false
	#define USE_WIFI101           false
	#define USE_WIFI_CUSTOM       false
#else
  #error For RASPBERRY_PI_PICO_W using arduino-pico core only! Please check your Tools->Board setting. 
#endif

#ifndef WIFI_MANAGER_RP2040W_VERSION
  #define WIFI_MANAGER_RP2040W_VERSION             "WiFiManager_RP2040W v1.0.0"
  
  #define WIFI_MANAGER_RP2040W_VERSION_MAJOR       1
  #define WIFI_MANAGER_RP2040W_VERSION_MINOR       0
  #define WIFI_MANAGER_RP2040W_VERSION_PATCH       0

  #define WIFI_MANAGER_RP2040W_VERSION_INT         1000000
#endif

///////////////////////////////////////////////////////////////////

#include "WiFiManager_RP2040W_Debug.h"

#if ( defined(HTTP_PORT) && (HTTP_PORT < 65536) && (HTTP_PORT > 0) )
  #if (_WIFIMGR_LOGLEVEL_ > 3)
    #warning Using custom HTTP_PORT
  #endif
    
  #define HTTP_PORT_TO_USE     HTTP_PORT
#else
  #if (_WIFIMGR_LOGLEVEL_ > 3)
    #warning Using default HTTP_PORT = 80
  #endif
  
  #define HTTP_PORT_TO_USE     80
#endif

#include <WiFi.h>
#include <WiFiWebServer.h>

#include <DNSServer.h>
#include <memory>
#undef min
#undef max
#include <algorithm>

///////////////////////////////////////////////////////////////////

typedef struct
{
  IPAddress _ap_static_ip;
  IPAddress _ap_static_gw;
  IPAddress _ap_static_sn;

}  WiFi_AP_IPConfig;

// Thanks to @Amorphous for the feature and code
// (https://community.blynk.cc/t/esp-wifimanager-for-esp32-and-esp8266/42257/13)
// To enable to configure from sketch
#if !defined(USE_CONFIGURABLE_DNS)
  #define USE_CONFIGURABLE_DNS        false
#endif

///////////////////////////////////////////////////////////////////

typedef struct
{
  IPAddress _sta_static_ip;
  IPAddress _sta_static_gw;
  IPAddress _sta_static_sn;
  IPAddress _sta_static_dns1;
}  WiFi_STA_IPConfig;

///////////////////////////////////////////////////////////////////

#define WFM_LABEL_BEFORE 1
#define WFM_LABEL_AFTER 2
#define WFM_NO_LABEL 0

///////////////////////////////////////////////////////////////////

/** Handle CORS in pages */
// Default false for using only whenever necessary to avoid security issue when using CORS (Cross-Origin Resource Sharing)
#ifndef USING_CORS_FEATURE
  // Contributed by AlesSt (https://github.com/AlesSt) to solve AJAX CORS protection problem of API redirects on client side
  // See more in https://github.com/khoih-prog/WiFiManager_RP2040W/issues/27 and https://en.wikipedia.org/wiki/Cross-origin_resource_sharing
  #define USING_CORS_FEATURE     false
#endif

///////////////////////////////////////////////////////////////////

const char WM_HTTP_200[] PROGMEM = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
const char WM_HTTP_HEAD_START[] PROGMEM = "<!DOCTYPE html><html lang='en'><head><meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/><title>{v}</title>";

// KH, update from v1.0.10
const char WM_HTTP_STYLE[] PROGMEM = "<style>div{padding:2px;font-size:1em;}body,textarea,input,select{background: 0;border-radius: 0;font: 16px sans-serif;margin: 0}textarea,input,select{outline: 0;font-size: 14px;border: 1px solid #ccc;padding: 8px;width: 90%}.btn a{text-decoration: none}.container{margin: auto;width: 90%}@media(min-width:1200px){.container{margin: auto;width: 30%}}@media(min-width:768px) and (max-width:1200px){.container{margin: auto;width: 50%}}.btn,h2{font-size: 2em}h1{font-size: 3em}.btn{background: #0ae;border-radius: 4px;border: 0;color: #fff;cursor: pointer;display: inline-block;margin: 2px 0;padding: 10px 14px 11px;width: 100%}.btn:hover{background: #09d}.btn:active,.btn:focus{background: #08b}label>*{display: inline}form>*{display: block;margin-bottom: 10px}textarea:focus,input:focus,select:focus{border-color: #5ab}.msg{background: #def;border-left: 5px solid #59d;padding: 1.5em}.q{float: right;width: 64px;text-align: right}.l{background: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==') no-repeat left center;background-size: 1em}input[type='checkbox']{float: left;width: 20px}.table td{padding:.5em;text-align:left}.table tbody>:nth-child(2n-1){background:#ddd}fieldset{border-radius:0.5rem;margin:0px;}</style>";
//////

// KH, update from v1.1.0
const char WM_HTTP_SCRIPT[] PROGMEM = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();document.getElementById('s1').value=l.innerText||l.textContent;document.getElementById('p1').focus();document.getElementById('timezone').value=timezone.name();}</script>";

///////////////////////////////////////////////////////////////////

// From v1.0.9 to permit disable or configure NTP from sketch
#ifndef USE_WIFIMANAGER_RP2040W_NTP
  // From v1.0.6 to enable NTP config
  #define USE_WIFIMANAGER_RP2040W_NTP     true
#endif

#if USE_WIFIMANAGER_RP2040W_NTP

#include "utils/TZ.h"

const char WM_HTTP_SCRIPT_NTP_MSG[] PROGMEM = "<p>Your Timezone is : <b><label id='timezone' name='timezone'></b><script>document.getElementById('timezone').innerHTML=timezone.name();document.getElementById('timezone').value=timezone.name();</script></p>";
const char WM_HTTP_SCRIPT_NTP_HIDDEN[] PROGMEM = "<p><input type='hidden' id='timezone' name='timezone'><script>document.getElementById('timezone').innerHTML=timezone.name();document.getElementById('timezone').value=timezone.name();</script></p>";

#if !(USE_CLOUDFLARE_NTP)
  #undef USE_CLOUDFLARE_NTP
  #define USE_CLOUDFLARE_NTP      true
  
  #if (_WIFIMGR_LOGLEVEL_ > 3)
  	#warning Forcing USE_CLOUDFLARE_NTP for RP2040W as low memory can cause blank page
  #endif
#endif

// To permit disable or configure NTP from sketch
#ifndef USE_CLOUDFLARE_NTP
  #define USE_CLOUDFLARE_NTP          false
#endif

#if USE_CLOUDFLARE_NTP
const char WM_HTTP_SCRIPT_NTP[] PROGMEM = "<script src='https://cdnjs.cloudflare.com/ajax/libs/jstimezonedetect/1.0.7/jstz.min.js'></script><script>var timezone=jstz.determine();console.log('Your CloudFlare timezone is:' + timezone.name());document.getElementById('timezone').innerHTML = timezone.name();</script>";
#else
const char WM_HTTP_SCRIPT_NTP[] PROGMEM = "<script>(function(e){var t=function(){'use strict';var e='s',n=function(e){var t=-e.getTimezoneOffset();return t!==null?t:0},r=function(e,t,n){var r=new Date;return e!==undefined&&r.setFullYear(e),r.setDate(n),r.setMonth(t),r},i=function(e){return n(r(e,0,2))},s=function(e){return n(r(e,5,2))},o=function(e){var t=e.getMonth()>7?s(e.getFullYear()):i(e.getFullYear()),r=n(e);return t-r!==0},u=function(){var t=i(),n=s(),r=i()-s();return r<0?t+',1':r>0?n+',1,'+e:t+',0'},a=function(){var e=u();return new t.TimeZone(t.olson.timezones[e])},f=function(e){var t=new Date(2010,6,15,1,0,0,0),n={'America/Denver':new Date(2011,2,13,3,0,0,0),'America/Mazatlan':new Date(2011,3,3,3,0,0,0),'America/Chicago':new Date(2011,2,13,3,0,0,0),'America/Mexico_City':new Date(2011,3,3,3,0,0,0),'America/Asuncion':new Date(2012,9,7,3,0,0,0),'America/Santiago':new Date(2012,9,3,3,0,0,0),'America/Campo_Grande':new Date(2012,9,21,5,0,0,0),'America/Montevideo':new Date(2011,9,2,3,0,0,0),'America/Sao_Paulo':new Date(2011,9,16,5,0,0,0),'America/Los_Angeles':new Date(2011,2,13,8,0,0,0),'America/Santa_Isabel':new Date(2011,3,5,8,0,0,0),'America/Havana':new Date(2012,2,10,2,0,0,0),'America/New_York':new Date(2012,2,10,7,0,0,0),'Asia/Beirut':new Date(2011,2,27,1,0,0,0),'Europe/Helsinki':new Date(2011,2,27,4,0,0,0),'Europe/Istanbul':new Date(2011,2,28,5,0,0,0),'Asia/Damascus':new Date(2011,3,1,2,0,0,0),'Asia/Jerusalem':new Date(2011,3,1,6,0,0,0),'Asia/Gaza':new Date(2009,2,28,0,30,0,0),'Africa/Cairo':new Date(2009,3,25,0,30,0,0),'Pacific/Auckland':new Date(2011,8,26,7,0,0,0),'Pacific/Fiji':new Date(2010,11,29,23,0,0,0),'America/Halifax':new Date(2011,2,13,6,0,0,0),'America/Goose_Bay':new Date(2011,2,13,2,1,0,0),'America/Miquelon':new Date(2011,2,13,5,0,0,0),'America/Godthab':new Date(2011,2,27,1,0,0,0),'Europe/Moscow':t,'Asia/Yekaterinburg':t,'Asia/Omsk':t,'Asia/Krasnoyarsk':t,'Asia/Irkutsk':t,'Asia/Yakutsk':t,'Asia/Vladivostok':t,'Asia/Kamchatka':t,'Europe/Minsk':t,'Australia/Perth':new Date(2008,10,1,1,0,0,0)};return n[e]};return{determine:a,date_is_dst:o,dst_start_for:f}}();t.TimeZone=function(e){'use strict';var n={'America/Denver':['America/Denver','America/Mazatlan'],'America/Chicago':['America/Chicago','America/Mexico_City'],'America/Santiago':['America/Santiago','America/Asuncion','America/Campo_Grande'],'America/Montevideo':['America/Montevideo','America/Sao_Paulo'],'Asia/Beirut':['Asia/Beirut','Europe/Helsinki','Europe/Istanbul','Asia/Damascus','Asia/Jerusalem','Asia/Gaza'],'Pacific/Auckland':['Pacific/Auckland','Pacific/Fiji'],'America/Los_Angeles':['America/Los_Angeles','America/Santa_Isabel'],'America/New_York':['America/Havana','America/New_York'],'America/Halifax':['America/Goose_Bay','America/Halifax'],'America/Godthab':['America/Miquelon','America/Godthab'],'Asia/Dubai':['Europe/Moscow'],'Asia/Dhaka':['Asia/Yekaterinburg'],'Asia/Jakarta':['Asia/Omsk'],'Asia/Shanghai':['Asia/Krasnoyarsk','Australia/Perth'],'Asia/Tokyo':['Asia/Irkutsk'],'Australia/Brisbane':['Asia/Yakutsk'],'Pacific/Noumea':['Asia/Vladivostok'],'Pacific/Tarawa':['Asia/Kamchatka'],'Africa/Johannesburg':['Asia/Gaza','Africa/Cairo'],'Asia/Baghdad':['Europe/Minsk']},r=e,i=function(){var e=n[r],i=e.length,s=0,o=e[0];for(;s<i;s+=1){o=e[s];if(t.date_is_dst(t.dst_start_for(o))){r=o;return}}},s=function(){return typeof n[r]!='undefined'};return s()&&i(),{name:function(){return r}}},t.olson={},t.olson.timezones={'-720,0':'Etc/GMT+12','-660,0':'Pacific/Pago_Pago','-600,1':'America/Adak','-600,0':'Pacific/Honolulu','-570,0':'Pacific/Marquesas','-540,0':'Pacific/Gambier','-540,1':'America/Anchorage','-480,1':'America/Los_Angeles','-480,0':'Pacific/Pitcairn','-420,0':'America/Phoenix','-420,1':'America/Denver','-360,0':'America/Guatemala','-360,1':'America/Chicago','-360,1,s':'Pacific/Easter','-300,0':'America/Bogota','-300,1':'America/New_York','-270,0':'America/Caracas','-240,1':'America/Halifax','-240,0':'America/Santo_Domingo','-240,1,s':'America/Santiago','-210,1':'America/St_Johns','-180,1':'America/Godthab','-180,0':'America/Argentina/Buenos_Aires','-180,1,s':'America/Montevideo','-120,0':'Etc/GMT+2','-120,1':'Etc/GMT+2','-60,1':'Atlantic/Azores','-60,0':'Atlantic/Cape_Verde','0,0':'Etc/UTC','0,1':'Europe/London','60,1':'Europe/Berlin','60,0':'Africa/Lagos','60,1,s':'Africa/Windhoek','120,1':'Asia/Beirut','120,0':'Africa/Johannesburg','180,0':'Asia/Baghdad','180,1':'Europe/Moscow','210,1':'Asia/Tehran','240,0':'Asia/Dubai','240,1':'Asia/Baku','270,0':'Asia/Kabul','300,1':'Asia/Yekaterinburg','300,0':'Asia/Karachi','330,0':'Asia/Kolkata','345,0':'Asia/Kathmandu','360,0':'Asia/Dhaka','360,1':'Asia/Omsk','390,0':'Asia/Rangoon','420,1':'Asia/Krasnoyarsk','420,0':'Asia/Jakarta','480,0':'Asia/Shanghai','480,1':'Asia/Irkutsk','525,0':'Australia/Eucla','525,1,s':'Australia/Eucla','540,1':'Asia/Yakutsk','540,0':'Asia/Tokyo','570,0':'Australia/Darwin','570,1,s':'Australia/Adelaide','600,0':'Australia/Brisbane','600,1':'Asia/Vladivostok','600,1,s':'Australia/Sydney','630,1,s':'Australia/Lord_Howe','660,1':'Asia/Kamchatka','660,0':'Pacific/Noumea','690,0':'Pacific/Norfolk','720,1,s':'Pacific/Auckland','720,0':'Pacific/Tarawa','765,1,s':'Pacific/Chatham','780,0':'Pacific/Tongatapu','780,1,s':'Pacific/Apia','840,0':'Pacific/Kiritimati'},typeof exports!='undefined'?exports.jstz=t:e.jstz=t})(this);</script><script>var timezone=jstz.determine();console.log('Your Timezone is:' + timezone.name());document.getElementById('timezone').innerHTML = timezone.name();</script>";
#endif

#else
  const char WM_HTTP_SCRIPT_NTP_MSG[]     PROGMEM   = "";
  const char WM_HTTP_SCRIPT_NTP_HIDDEN[]  PROGMEM   = "";
  const char WM_HTTP_SCRIPT_NTP[]         PROGMEM   = "";
#endif

// KH, update from v1.0.10
const char WM_HTTP_HEAD_END[] PROGMEM = "</head><body><div class='container'><div style='text-align:left;display:inline-block;min-width:260px;'>";

const char WM_FLDSET_START[]  PROGMEM = "<fieldset>";
const char WM_FLDSET_END[]    PROGMEM = "</fieldset>";
//////

const char WM_HTTP_PORTAL_OPTIONS[] PROGMEM = "<form action='/wifi' method='get'><button class='btn'>Configuration</button></form><br/><form action='/i' method='get'><button class='btn'>Information</button></form><br/><form action='/close' method='get'><button class='btn'>Exit Portal</button></form><br/>";
const char WM_HTTP_ITEM[] PROGMEM = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char JSON_ITEM[] PROGMEM    = "{\"SSID\":\"{v}\", \"Encryption\":{i}, \"Quality\":\"{r}\"}";

// KH, update from v1.1.0
const char WM_HTTP_FORM_START[] PROGMEM = "<form method='get' action='wifisave'><fieldset><div><label>SSID</label><input id='s' name='s' length=32 placeholder='SSID'><div></div></div><div><label>Password</label><input id='p' name='p' length=64 placeholder='password'><div></div></div><div><label>SSID1</label><input id='s1' name='s1' length=32 placeholder='SSID1'><div></div></div><div><label>Password</label><input id='p1' name='p1' length=64 placeholder='password1'><div></div></div></fieldset>";
//////

// KH, add from v1.0.10
const char WM_HTTP_FORM_LABEL_BEFORE[] PROGMEM = "<div><label for='{i}'>{p}</label><input id='{i}' name='{n}' length={l} placeholder='{p}' value='{v}' {c}><div></div></div>";
const char WM_HTTP_FORM_LABEL_AFTER[] PROGMEM = "<div><input id='{i}' name='{n}' length={l} placeholder='{p}' value='{v}' {c}><label for='{i}'>{p}</label><div></div></div>";
//////

const char WM_HTTP_FORM_LABEL[] PROGMEM = "<label for='{i}'>{p}</label>";
const char WM_HTTP_FORM_PARAM[] PROGMEM = "<input id='{i}' name='{n}' length={l} placeholder='{p}' value='{v}' {c}>";

const char WM_HTTP_FORM_END[] PROGMEM = "<button class='btn' type='submit'>Save</button></form>";

// KH, update from v1.1.0
const char WM_HTTP_SAVED[] PROGMEM = "<div class='msg'><b>Credentials Saved</b><br>Try connecting RP2040W to the {x}/{x1} network. Wait around 10 seconds then check <a href='/'>if it's OK.</a> <p/>The {v} AP will run on the same WiFi channel of the {x}/{x1} AP. You may have to manually reconnect to the {v} AP.</div>";
//////

const char WM_HTTP_END[] PROGMEM = "</div></body></html>";

//KH, from v1.1.0
const char WM_HTTP_HEAD_CL[]         PROGMEM = "Content-Length";
const char WM_HTTP_HEAD_CT[]         PROGMEM = "text/html";
const char WM_HTTP_HEAD_CT2[]        PROGMEM = "text/plain";

//KH Add repeatedly used const
const char WM_HTTP_CACHE_CONTROL[]   PROGMEM = "Cache-Control";
const char WM_HTTP_NO_STORE[]        PROGMEM = "no-cache, no-store, must-revalidate";
const char WM_HTTP_PRAGMA[]          PROGMEM = "Pragma";
const char WM_HTTP_NO_CACHE[]        PROGMEM = "no-cache";
const char WM_HTTP_EXPIRES[]         PROGMEM = "Expires";
const char WM_HTTP_CORS[]            PROGMEM = "Access-Control-Allow-Origin";
const char WM_HTTP_CORS_ALLOW_ALL[]  PROGMEM = "*";

#if USE_AVAILABLE_PAGES
const char WM_HTTP_AVAILABLE_PAGES[] PROGMEM = "<h3>Available Pages</h3><table class='table'><thead><tr><th>Page</th><th>Function</th></tr></thead><tbody><tr><td><a href='/'>/</a></td><td>Menu page.</td></tr><tr><td><a href='/wifi'>/wifi</a></td><td>Show WiFi scan results and enter WiFi configuration.</td></tr><tr><td><a href='/wifisave'>/wifisave</a></td><td>Save WiFi configuration information and configure device. Needs variables supplied.</td></tr><tr><td><a href='/close'>/close</a></td><td>Close the configuration server and configuration WiFi network.</td></tr><tr><td><a href='/i'>/i</a></td><td>This page.</td></tr><tr><td><a href='/r'>/r</a></td><td>Delete WiFi configuration and reboot. ESP device will not reconnect to a network until new WiFi configuration data is entered.</td></tr><tr><td><a href='/state'>/state</a></td><td>Current device state in JSON format. Interface for programmatic WiFi configuration.</td></tr><tr><td><a href='/scan'>/scan</a></td><td>Run a WiFi scan and return results in JSON format. Interface for programmatic WiFi configuration.</td></tr></table>";
#else
const char WM_HTTP_AVAILABLE_PAGES[] PROGMEM = "";
#endif

//KH
#define WIFI_MANAGER_MAX_PARAMS 20

///////////////////////////////////////////////////////////////////

typedef struct
{
  const char *_id;
  const char *_placeholder;
  char       *_value;
  int         _length;
  int         _labelPlacement;

}  WMParam_Data;

///////////////////////////////////////////////////////////////////

class RP2040W_WMParameter 
{
  public:
    RP2040W_WMParameter(const char *custom);
    RP2040W_WMParameter(const char *id, const char *placeholder, const char *defaultValue, const int& length, 
                    const char *custom = "", const int& labelPlacement = WFM_LABEL_BEFORE);
              
    RP2040W_WMParameter(const WMParam_Data& WMParam_data);                   
                   
    ~RP2040W_WMParameter();
    
    void setWMParam_Data(const WMParam_Data& WMParam_data);
    void getWMParam_Data(WMParam_Data& WMParam_data);
    //////

    const char *getID();
    const char *getValue();
    const char *getPlaceholder();
    int         getValueLength();
    int         getLabelPlacement();
    const char *getCustomHTML();
    
  private:
  
    WMParam_Data _WMParam_data;
    
    const char *_customHTML;

    void init(const char *id, const char *placeholder, const char *defaultValue, const int& length, 
              const char *custom, const int& labelPlacement);

    friend class WiFiManager_RP2040W;
};

///////////////////////////////////////////////////////////////////

#define USE_DYNAMIC_PARAMS				true
#define DEFAULT_PORTAL_TIMEOUT  	60000L

// From v1.0.10 to permit disable/enable StaticIP configuration in Config Portal from sketch. Valid only if DHCP is used.
// You have to explicitly specify false to disable the feature.
#ifndef USE_STATIC_IP_CONFIG_IN_CP
  #define USE_STATIC_IP_CONFIG_IN_CP          true
#endif

///////////////////////////////////////////////////////////////////

class WiFiManager_RP2040W
{
  public:

    WiFiManager_RP2040W(const char *iHostname = "");

    ~WiFiManager_RP2040W();

    // Update feature from v1.0.11. Can use with STA staticIP now
    bool          autoConnect();
    bool          autoConnect(char const *apName, char const *apPassword = NULL);
    //////

    //if you want to start the config portal
    bool          startConfigPortal();
    bool          startConfigPortal(char const *apName, char const *apPassword = NULL);

    // get the AP name of the config portal, so it can be used in the callback
    String        getConfigPortalSSID();
    // get the AP password of the config portal, so it can be used in the callback
    String        getConfigPortalPW();

    void          resetSettings();

    //sets timeout before WiFiWebServer loop ends and exits even if there has been no setup.
    //usefully for devices that failed to connect at some point and got stuck in a WiFiWebServer loop
    //in seconds setConfigPortalTimeout is a new name for setTimeout
    void          setConfigPortalTimeout(const unsigned long& seconds);
    void          setTimeout(const unsigned long& seconds);

    //sets timeout for which to attempt connecting, usefull if you get a lot of failed connects
    void          setConnectTimeout(const unsigned long& seconds);


    void          setDebugOutput(bool debug);
    //defaults to not showing anything under 8% signal quality if called
    void          setMinimumSignalQuality(const int& quality = 8);
    
    // KH, new from v1.0.10 to enable dynamic/random channel
    int           setConfigPortalChannel(const int& channel = 1);
    //////
    
    //sets a custom ip /gateway /subnet configuration
    void          setAPStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn);
    
    void          setAPStaticIPConfig(const WiFi_AP_IPConfig& WM_AP_IPconfig);
    void          getAPStaticIPConfig(WiFi_AP_IPConfig& WM_AP_IPconfig);
    //////
    
    //sets config for a static IP
    void          setSTAStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn);
    
    void          setSTAStaticIPConfig(const WiFi_STA_IPConfig&  WM_STA_IPconfig);
    void          getSTAStaticIPConfig(WiFi_STA_IPConfig& WM_STA_IPconfig);
    //////
    
#if USE_CONFIGURABLE_DNS
    void          setSTAStaticIPConfig(const IPAddress& ip, const IPAddress& gw, const IPAddress& sn,
                                       const IPAddress& dns_address_1, const IPAddress& dns_address_2);
#endif

    //called when AP mode and config portal is started
    void          setAPCallback(void(*func)(WiFiManager_RP2040W*));
    //called when settings have been changed and connection was successful
    void          setSaveConfigCallback(void(*func)());

#if USE_DYNAMIC_PARAMS
    //adds a custom parameter
    bool 				addParameter(RP2040W_WMParameter *p);
#else
    //adds a custom parameter
    void 				addParameter(RP2040W_WMParameter *p);
#endif

    //if this is set, it will exit after config, even if connection is unsucessful.
    void          setBreakAfterConfig(bool shouldBreak);
    //if this is set, try WPS setup when starting (this will delay config portal for up to 2 mins)
    //TODO
    //if this is set, customise style
    void          setCustomHeadElement(const char* element);
    //if this is true, remove duplicated Access Points - defaut true
    void          setRemoveDuplicateAPs(bool removeDuplicates);
    //Scan for WiFiNetworks in range and sort by signal strength
    //space for indices array allocated on the heap and should be freed when no longer required
    int           scanWifiNetworks(int **indicesptr);

    // return SSID of router in STA mode got from config portal. NULL if no user's input //KH
    String				getSSID() 
    {
      return _ssid;
    }

    // return password of router in STA mode got from config portal. NULL if no user's input //KH
    String				getPW() 
    {
      return _pass;
    }
    
    // return SSID of router in STA mode got from config portal. NULL if no user's input //KH
    String				getSSID1() 
    {
      return _ssid1;
    }

    // return password of router in STA mode got from config portal. NULL if no user's input //KH
    String				getPW1() 
    {
      return _pass1;
    }
    
    #define MAX_WIFI_CREDENTIALS        2
    
    String				getSSID(const uint8_t& index) 
    {
      if (index == 0)
        return _ssid;
      else if (index == 1)
        return _ssid1;
      else     
        return String("");
    }
    
    String				getPW(const uint8_t& index) 
    {
      if (index == 0)
        return _pass;
      else if (index == 1)
        return _pass1;
      else     
        return String("");
    }
    //////
    
    // For configure CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
#if USING_CORS_FEATURE
    void setCORSHeader(const char* CORSHeaders)
    {     
      _CORS_Header = CORSHeaders;

      LOGWARN1(F("Set CORS Header to : "), _CORS_Header);
    }
    
    const char* getCORSHeader()
    {
      return _CORS_Header;
    }
#endif   
    
    //returns the list of Parameters
    RP2040W_WMParameter** getParameters();
    // returns the Parameters Count
    int           getParametersCount();

    const char*   getStatus(const int& status);

    String WiFi_SSID()
    {
      return WiFi.SSID();
    }

    void setHostname()
    {
      if (RFC952_hostname[0] != 0)
      {   
        WiFi.setHostname(RFC952_hostname);  
      }
    }

#if USE_WIFIMANAGER_RP2040W_NTP
    
    String getTimezoneName() 
    {  
      return _timezoneName;
    }

    void setTimezoneName(const String& inTimezoneName) 
    {  
      _timezoneName = inTimezoneName;
    }
    //////
    
    //See: https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
    // EST5EDT,M3.2.0,M11.1.0 (for America/New_York)
    // EST5EDT is the name of the time zone
    // EST is the abbreviation used when DST is off
    // 6 hours is the time difference from GMT
    // EDT is the abbreviation used when DST is on
    // ,M3 is the third month
    // .2 is the second occurrence of the day in the month
    // .0 is Sunday
    // ,M11 is the eleventh month
    // .1 is the first occurrence of the day in the month
    // .0 is Sunday   
    
    const char * getTZ(const char * timezoneName)
    {               
      //const char TZ_NAME[][TIMEZONE_MAX_LEN]
      for (uint16_t index = 0; index < sizeof(TZ_NAME) / TIMEZONE_MAX_LEN; index++)
      {
        if ( !strncmp(timezoneName, (TZ_NAME[index]), strlen((TZ_NAME[index])) ) )
        {
          yield();
          return (ESP_TZ_NAME[index]);            
        }    
      }
      
      return "";      
    }
    
    
    const char * getTZ(const String& timezoneName)
    {
      return getTZ(timezoneName.c_str());      
    }
    
    
#endif    

  private:
    std::unique_ptr<DNSServer>        dnsServer;

    //KH, for RP2040W
    std::unique_ptr<WiFiWebServer>        server;

#define RFC952_HOSTNAME_MAXLEN      24

    char RFC952_hostname[RFC952_HOSTNAME_MAXLEN + 1];

    char* getRFC952_hostname(const char* iHostname);

    void          setupConfigPortal();

    const char*   _apName = "no-net";
    const char*   _apPassword = NULL;
    
    String        _ssid   = "";
    String        _pass   = "";
    
    String        _ssid1  = "";
    String        _pass1  = "";
    //////
    
#if USE_WIFIMANAGER_RP2040W_NTP
    // Timezone info
    String        _timezoneName         = "";
#endif

    unsigned long _configPortalTimeout  = 0;

    unsigned long _connectTimeout       = 0;
    unsigned long _configPortalStart    = 0;

    int           numberOfNetworks;
    int           *networkIndices;
    
    // KH, new from v1.0.10 to enable dynamic/random channel
    // default to channel 1
    #define MIN_WIFI_CHANNEL      1
    #define MAX_WIFI_CHANNEL      11    // Channel 12,13 is flaky, because of bad number 13 ;-)

    int _WiFiAPChannel = 1;
    //////

    WiFi_AP_IPConfig  _WiFi_AP_IPconfig;
    
    WiFi_STA_IPConfig _WiFi_STA_IPconfig = { IPAddress(0, 0, 0, 0), IPAddress(192, 168, 42, 1), IPAddress(255, 255, 255, 0),
                                             IPAddress(192, 168, 42, 1) };
    //////
    
    int           _paramsCount            = 0;
    int           _minimumQuality         = -1;
    bool          _removeDuplicateAPs     = true;
    bool          _shouldBreakAfterConfig = false;
    bool          _tryWPS                 = false;

    const char*   _customHeadElement      = "";

    int           status                  = WL_IDLE_STATUS;
    
    // For configure CORS Header, default to WM_HTTP_CORS_ALLOW_ALL = "*"
#if USING_CORS_FEATURE
    const char*     _CORS_Header          = WM_HTTP_CORS_ALLOW_ALL;   //"*";
#endif   
    //////
    
    void          setWifiStaticIP();
    int           reconnectWifi();
    int           connectWifi(const String& ssid = "", const String& pass = "");
    
    uint8_t       waitForConnectResult();

    void          handleRoot();
    void          handleWifi();
    void          handleWifiSave();
    void          handleServerClose();
    void          handleInfo();
    void          handleState();
    void          handleScan();
    void          handleReset();
    void          handleNotFound();
    bool          captivePortal();
    
    void          reportStatus(String& page);

    // DNS server
    const byte    DNS_PORT = 53;

    //helpers
    int           getRSSIasQuality(const int& RSSI);
    bool          isIp(const String& str);
    String        toStringIp(const IPAddress& ip);

    bool          connect;
    bool          stopConfigPortal = false;
    
    bool          _debug = false;     //true;

    void(*_apcallback)  (WiFiManager_RP2040W*)  = NULL;
    void(*_savecallback)()              = NULL;

#if USE_DYNAMIC_PARAMS
    int                    _max_params;
    RP2040W_WMParameter** _params;
#else
    RP2040W_WMParameter* _params[WIFI_MANAGER_MAX_PARAMS];
#endif

    template <typename Generic>
    void          DEBUG_WM(Generic text);

    template <class T>
    auto optionalIPFromString(T *obj, const char *s) -> decltype(obj->fromString(s)) 
    {
      return  obj->fromString(s);
    }
    
    auto optionalIPFromString(...) -> bool 
    {
      LOGINFO("NO fromString METHOD ON IPAddress, you need ESP8266 core 2.1.0 or newer for Custom IP configuration to work.");
      return false;
    }
};

#endif    // WiFiManager_RP2040W_h

