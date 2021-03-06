#ifndef ARDUNGROUND_H
#define ARDUNGROUND_H

#include <Ethernet.h>

#define UNSET_VAR -460

class WeatherStation
{

	private:
	const char*	_stationId;
	const char* _stationPassword;
	const char* _stationName;

	char* _server = "rtupdate.wunderground.com";
	/*
	RapidFire server for WeatherUnderground network
	More details here : http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol#RapidFire_Updates
	*/
	char* _webpage = "GET /weatherstation/updateweatherstation.php?"; //Webpage to update weather stations

	/*
	Default weather variables
	*/
	float _tempF = UNSET_VAR;
	float _humidity = UNSET_VAR;
	int _dewptf = UNSET_VAR;

	//Wind
	int _winddir = UNSET_VAR;
	int _windspeedmph = UNSET_VAR;
	int _windgustdir = UNSET_VAR;
	int _windgustmph = UNSET_VAR;


	String urlencode(String str);
	/*
	A function that convert strings to escaped urls
	Function stolen here : https://github.com/zenmanenergy/ESP8266-Arduino-Examples/blob/master/helloWorld_urlencoded/urlencode.ino
	*/

	public:

	WeatherStation(const char* stationId, const char* stationPassword, const char* stationName);
	/*
	Set the details of the weather station
	*/

	void set(char* server, char* webpage);
	/*
	Set the server settings for weather update
	It's not necessary to call this function
	*/

	void setTempF(float tempF);
	void setHumidity(float humidity);
	void setDewptf(int dewptf);
	//Wind
	bool setWinddir(int windir);
	bool setWindspeedmph(int windspeedmph);
	bool setWindgustdir(int windgustdir);
	bool setWindgustmph(int windgustmph);

	bool post(EthernetClient client, String timestamp = "now");
	/*
	Post the actual weather details on the server
	*/
};

#endif
