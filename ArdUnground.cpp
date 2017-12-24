#include <Arduino.h>
#include <ArdUnground.h>

#include <Ethernet.h>

WeatherStation::WeatherStation(const char* stationId, const char* stationPassword, const char* stationName)
{
	this->_stationId = stationId;
	this->_stationPassword = stationPassword;
	this->_stationName = stationName;
}

void WeatherStation::setTempF(float tempF)
{
	this->_tempF = tempF;
}

void WeatherStation::setHumidity(float humidity)
{
	this->_humidity = humidity;
}

void WeatherStation::set(char* server, char* webpage)
{
	this->_server = server;
	this->_webpage = webpage;
}
bool WeatherStation::post(EthernetClient client, String timestamp = "now")
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
}

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