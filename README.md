# ArdUnground

ArdUnground is an arduino library that update weather station data to WeatherUnderground or another rapidfire server

### Prerequisites

You need to have the Ethernet.h library installed (native arduino library)
See https://www.arduino.cc/en/Guide/Libraries for further information

### Installation

Download the project as a zip file and extract the content in your arduino/libraries folder as usual
See https://www.arduino.cc/en/Guide/Libraries for further information

### Usage

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

... More to come
