# ArdUnground

![Travis ci](https://travis-ci.org/storca/ArdUnground.svg?branch=master)

ArdUnground is an arduino library that update weather station data to WeatherUnderground or another rapidfire server

## Prerequisites

You need to have the Ethernet.h library installed (native arduino library)
See https://www.arduino.cc/en/Guide/Libraries for further information


## Reference and projects used for this library
* [Weather-Station.ino](https://github.com/danfein/Ethernet-Weather/blob/master/Weather-Station.ino) - Wunderground upload via arduino
* [Wundergound PWS Protocol](http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol) - Documentation for Wunderground PWS protocol
* [UrlEncode function](https://github.com/zenmanenergy/ESP8266-Arduino-Examples/blob/master/helloWorld_urlencoded/urlencode.ino) - An arduino function to escape URLs

## Installation

Download the project as a zip file and extract the content in your arduino/libraries folder as usual
See https://www.arduino.cc/en/Guide/Libraries for further information

## Usage

First include the two libraries as follow :
```C++
#include <Ethernet.h>
#include <ArdUnground.h>
```

Declare a 'WeatherStation' object :

```Arduino
WeatherStation station("your station id", "your station password", "the station name");
```
/!\ The communications are made via http which means that your password may be intercepted /!\
Use a unique password for you weather station

Declare an 'EthernetClient' object:
```Arduino
EthernetClient client;
```
Set your 'client' object as usual

### Functions available

Set the temperature to send in Farenheight :
```Arduino
WeatherStation.setTempF(float tempF);
```

Set the humidity to send in % :
```Arduino
WeatherStation.setHumidity(float humidity);
```

More functions to come here, this is just a draft of the library

Post the PWS data to WeatherUnderground :
Where 'timestamp' is optional, it's "now" by default : the timestamp "now" will be interpreted as the current time (UTC) by the rapidfire server. See http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol_
```Arduino
WeatherStation.post(EthernetClient client, String timestamp = "now");
```

### Full example
[WeatherStation.ino](https://github.com/storca/ArdUnground/blob/master/examples/WeatherStation/WeatherStation.ino)
```Arduino
//Local directory
#include <ArdUnground.h>
//Native library
#include <Ethernet.h>

//Reference for Ethernet library : https://www.arduino.cc/en/Tutorial/WebClient
//Mac address for the ethernet client
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//If the dhcp server fails
IPAddress ip(192, 168, 0, 177);

EthernetClient client;

WeatherStation station("id", "password", "Arduino Custom station");

void setup()
{
  Serial.begin(9600);
  
  if (!Ethernet.begin(mac))
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }

  // give the Ethernet shield a second to initialize:
  Serial.println("connecting...");
  delay(1000);
  
	station.set("weatherstation.wunderground.com", "GET /weatherstation/updateweatherstation.php?");
  /*
   Not necessary, these values are set by default in ArdUnderground.h
   */
}

void loop()
{
	float temp = 67.65444; //In °F
  float humidity = 40.65; //In %
  /*
   * See http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol#GET_parameters
   * For further information about unities
   */

   station.setTempF(temp);
   station.setHumidity(humidity);
   /*
    * Set the temperature and humidity before posting it
    */

    station.post(client);
    /*
     * That's one way to do it
     * The second argument has a default value of "now"
     * When the timestamp has a value of "now" ; the server will produce a timestamp by itself
     * http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol#GET_parameters
     */
    station.post(client, "2001-01-01 10:32:35");
    /*
     * Another way to do it
     * Note : the timestamp will be url escaped later, no need to do it here
     */
}
```
