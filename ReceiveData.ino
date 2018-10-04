//include libraries
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

//Access point credentials
const char* ssid = "Password";
const char* pwd = "12345678";

int sensor;
float temperature, refvoltage, temp;

int ledPin = 13; //connected to pin 7 in nodemcu 1.0 esp-12e module
WiFiServer server(80);  // open port 80 for server connection
void setup()
{
  Serial.begin(9600); //initialise the serial communication
  delay(10);

  //defining the pins, i/p and o/p
  pinMode(ledPin, OUTPUT);
  pinMode(A0, INPUT);
  digitalWrite(ledPin, LOW);

  refvoltage = 2.25;   //reference voltage for temperature calculation

  //connecting to wifi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to wifi ");
  Serial.println(ssid);

  WiFi.begin(ssid, pwd);

  //attempt to connect to wifi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("."); //progress with dots until the wifi is connected
  }
  Serial.println("");

  //while connected print this
  Serial.println("Wifi connected");

  //starting the server
  server.begin();
  Serial.println("Server started.");

  //get the ip address and print it
  Serial.print("This is your ip address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop()
{

  sensor = analogRead(A0);
  // temperature = (refvoltage * sensor * 100) / 1023; //converting degree to celcius
  /*if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    String url = "http://192.168.43.29/write_data.php?value=" + String(sensor);
    Serial.println(url);
    http.begin(url);
    int httpcode = http.GET();
    if (httpcode > 0)
    {
      Serial.print(httpcode);
    }
    http.end();
  }*/
  delay(5000);
  if (WiFi.status() == WL_CONNECTED)
  {
    String lastmessage = "";

    //if(WiFi.status()==WL_CONNECTED)



    HTTPClient http;
//    String url1 = "http://192.168.43.29/write_data.php?value=" + String(sensor);
//    Serial.println(url1);
//    http.begin(url1);

    String url = "http://192.168.43.29/get_data.php";

    http.begin(url);

    //  http.addHeader("Content-Type","text/plain");

    int httpCode = http.GET();

    String data = http.getString();

    //lastmessage = getlastline(data); 
    Serial.print(data);
    Serial.print('\n');
    if(data.toInt()>400)
    {
      Serial.println("1");
    }
    else
    {
      Serial.println("0");
      
    }
    
    http.end();
//lastmessage avto nathi
  }
  

}
