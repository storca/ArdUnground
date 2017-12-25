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
  
	station.set("rtupdate.wunderground.com", "GET /weatherstation/updateweatherstation.php?");
  /*
   Not necessary, these values are set by default in ArdUnderground.h
   There is another server for WunderGround : weatherstation.wunderground.com
   I don't know if it's a test server or anything else :p
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
