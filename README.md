# ArdUnground

![Travis ci](https://travis-ci.org/storca/ArdUnground.svg?branch=master)

ArdUnground is an arduino library that update weather station data to WeatherUnderground or another rapidfire server

## Prerequisites

You need to have the Ethernet.h library installed (native arduino library)
See https://www.arduino.cc/en/Guide/Libraries for further information

## Installation

Download the project as a zip file and extract the content in your arduino/libraries folder as usual
See https://www.arduino.cc/en/Guide/Libraries for further information

## Usage

First include the two libraries as follow :
```
#include <Ethernet.h>
#include <ArdUnground.h>
```

Declare a 'WeatherStation' object :

```
WeatherStation station("your station id", "your station password", "the station name");
```
/!\ The communications are made via http which means that your password may be intercepted /!\
Use a unique password for you weather station

Declare an 'EthernetClient' object:
```
EthernetClient client;
```
Set your 'client' object as usual

### Functions available

Set the temperature to send in Farenheight :
```
WeatherStation.setTempF(float tempF);
```

Set the humidity to send in % :
```
WeatherStation.setHumidity(float humidity);
```

More functions to come here, this is just a draft of the library

Post the PWS data to WeatherUnderground :
Where 'timestamp' is optional, it's "now" by default : the timestamp "now" will be interpreted as the current time (UTC) by the rapidfire server. See http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol_
```
WeatherStation.post(EthernetClient client, String timestamp = "now");
```

## Reference and projects used for this library
* [Weather-Station.ino](https://github.com/danfein/Ethernet-Weather/blob/master/Weather-Station.ino) - Wunderground upload via arduino
* [Wundergound PWS Protocol](http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol) - Documentation for Wunderground PWS protocol
* [UrlEncode function](https://github.com/zenmanenergy/ESP8266-Arduino-Examples/blob/master/helloWorld_urlencoded/urlencode.ino) - An arduino function to escape URLs
