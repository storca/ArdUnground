#include <Arduino.h>
#include <ArdUnground.h>

#include <Ethernet.h>

/**
 * New WeatherStation object
 * @param stationId       Your station ID
 * @param stationPassword Your station password
 * @param stationName     The station name
 */
WeatherStation::WeatherStation(const char* stationId, const char* stationPassword, const char* stationName)
{
	this->_stationId = stationId;
	this->_stationPassword = stationPassword;
	this->_stationName = stationName;
}
/**
 * Set the temperature in farennheight to send
 * @param tempF temperature in °f
 */
void WeatherStation::setTempF(float tempF)
{
	this->_tempF = tempF;
}
/**
 * Set the humidity to send in %
 * @param humidity humidity
 */
void WeatherStation::setHumidity(float humidity)
{
	if(humidity > 100.0 || humidity < 0.0)
	{
		this->_humidity = NULL;
	}
	else
	{
		this->_humidity = humidity;
	}
}
/**
 * Set dew point in °F
 * @param dewptf Dew point in °F
 */
void WeatherStation::setDewptf(int dewptf)
{
	this->_dewptf = dewptf;
}
/**
 * Set wind direction
 * @param  winddir Wind direction
 * @return         true if success
 */
bool WeatherStation::setWinddir(int winddir)
{
	if(winddir > 360 || winddir < 0)
	{
		this->_winddir = NULL;
		return false;
	}
	else
	{
		this->_winddir = winddir;
		return true;
	}
}
/**
 * Set the wind speed in miles per hour
 * @param  windspeedmph Speed of the wind
 * @return              true is success
 */
bool WeatherStation::setWindspeedmph(int windspeedmph)
{
	if(windspeedmph < 0)
	{
		this->_windspeedmph = NULL;
		return false;
	}
	else
	{
		this->_windspeedmph = windspeedmph;
		return true;
	}
}

/**
 * Set wind direction
 * @param  winddir Wind direction
 * @return         true if success
 */
bool WeatherStation::setWindgustdir(int winddir)
{
	if(windgustdir > 360 || windgustdir < 0)
	{
		this->_windgustdir = NULL;
		return false;
	}
	else
	{
		this->_windgustdir = windgustdir;
		return true;
	}
}
/**
 * Set the wind speed in miles per hour
 * @param  windspeedmph Speed of the wind
 * @return              true is success
 */
bool WeatherStation::setWindgustmph(int windspeedmph)
{
	if(windgustmph < 0)
	{
		this->_windgustmph = NULL;
		return false;
	}
	else
	{
		this->_windgustmph = windgustmph;
		return true;
	}
}
/**
 * Set the server info
 * @param server  RapidFire server
 * @param webpage HTTP request
 */
void WeatherStation::set(char* server, char* webpage)
{
	this->_server = server;
	this->_webpage = webpage;
}
/**
 * Post the previous set data to the server
 * @param  client    Ethernet Client
 * @param  timestamp Timestamp
 * @return           true if success
 */
bool WeatherStation::post(EthernetClient client, String timestamp)
{
	//Based on https://github.com/danfein/Ethernet-Weather/blob/master/Weather-Station.ino

	//According to WeatherUnderground PWS protocol http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol

	if (client.connect(this->_server, 80)) //HTTP connection -> Passwords are transfered in plain text
	{
		//Critical Data
		client.print(this->_webpage);
		client.print("ID=");
		client.print(this->_stationId);
		client.print("&PASSWORD=");
		client.print(this->_stationPassword);
		client.print("&dateutc=");
		//client.print(timestamp); //Only accept "now" because of url escapement

		client.print(WeatherStation::urlencode(timestamp));
		/*
		Timestamps are in Mysql Format.
		See http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol#GET_parameters for further information
		*/

		//Weather data
		if (this->_humidity != NULL)
		{
			client.print("&humidity=");
			client.print(this->_humidity);
		}
		if (this->_tempF != NULL)
		{
			client.print("&tempf=");
			client.print(this->_tempF);
		}
		if(this->_dewptf != NULL)
		{
			client.print("&dewptf=");
			client.print(this->_dewptf);
		}
		if(this->_winddir != NULL)
		{
			client.print("&winddir=");
			client.print(this->_winddir);
		}
		if(this->_windgustdir != NULL)
		{
			client.print("&windgustdir=");
			client.print(this->_windgustdir);
		}
		if(this->_windgustmph != NULL)
		{
			client.print("&windgustmph=");
			client.print(this->_windgustmph);
		}
		if(this->_windspeedmph != NULL)
		{
			client.print("&windspeedmph=");
			client.print(this->_windspeedmph);
		}

		//Station data
		client.print("&softwaretype=");
		//client.print(this->_stationName); //Must be url escaped
		client.print(WeatherStation::urlencode(this->_stationName)); //Is url escaped now :p
		client.print("&action=updateraw"); //We specify that we update data of a weather station
		client.print("&realtime=1&rtfreq=2.5"); //Those options are necessary according to PWS doc

		//Last data
		client.print(" HTTP/1.0\r\n");
		client.print("Accept: text/html\r\n");
		client.print("Host: ");
		client.print(this->_server);
		client.print("\r\n\r\n");

		//Send it ALL
		client.println();
	}
	else
	{
		return false;
	}
	return true;
}
/**
 * Encode a string to an url
 * @param  str String to encode
 * @return     Encoded url
 */
String WeatherStation::urlencode(String str)
//Function stolen here : https://github.com/zenmanenergy/ESP8266-Arduino-Examples/blob/master/helloWorld_urlencoded/urlencode.ino
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;

}
