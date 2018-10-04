#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h> 

const int MQ_PIN=A0; 
int RL_VALUE=5;                                     //define the load resistance on the board, in kilo ohms
float RO_CLEAN_AIR_FACTOR=3.70;                     //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                                    //which is derived from the chart in datasheet

int CALIBARAION_SAMPLE_TIMES=50;                    //define how many samples you are going to take in the calibration phase
int CALIBRATION_SAMPLE_INTERVAL=500;                //define the time interal(in milisecond) between each samples in the
                                                    //cablibration phase
int READ_SAMPLE_INTERVAL=50;                        //define how many samples you are going to take in normal operation
int READ_SAMPLE_TIMES=5;                            //define the time interal(in milisecond) between each samples in 
                                                    //normal operation

#define         GAS_CO2             0   
#define         GAS_CO              1   
#define         GAS_NH4             2    




float           CO2Curve[3]  =  {1,2.4,-1.229};   //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent"
                                                    //to the original curve. 
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.21), point2: (lg10000, -0.59) 
float           COCurve[3]  =  {1,2.9,-1.076};    //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent" 
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15) 
float           NH4Curve[3] ={1,2.74,-1.476};    //two points are taken from the curve. 
                                                    //with these two points, a line is formed which is "approximately equivalent" 
                                                    //to the original curve.
                                                    //data format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000,  -0.22)                                                     
float           Ro           =  10;                 //Ro is initialized to 10 kilo ohms
 
                                                    
const char* ssid = "Password";
const char* pwd = "12345678";

int sensor;
float temperature, refvoltage, temp;

//int ledPin = 14;                //connected to pin D5 in nodemcu 1.0 esp-12e module
WiFiServer server(80);           // open port 80 for server connection

void setup() 
{
  Serial.begin(115200); //initialise the serial communication
  delay(10);

  //defining the pins, i/p and o/p
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);
  digitalWrite(LED_BUILTIN, LOW); 
        
  //connecting to wifi
  Serial.println();
  Serial.println();
  Serial.println("Welcome to SCETATHON-2018");
  Serial.println("AIR CONTROL PROJECT");
  Serial.println("Connecting to wifi......... ");  
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
    Serial.println("Server started.....");

    //get the ip address and print it
    Serial.print("This is your ip address: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");  
}

void loop()
{

  long iPPM_CO2 = 0;
  long iPPM_CO = 0;
  long iPPM_NH4 = 0;
    
  iPPM_CO2 = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_CO2);
  iPPM_CO = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_CO);
  iPPM_NH4 = MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_NH4);

  Serial.print("CO2: ");
  Serial.print(iPPM_CO2);
  Serial.print("\t");
  Serial.println("PPM");
  Serial.print('\n');

  Serial.print("CO: ");
  Serial.print(iPPM_CO);
  Serial.print("\t");
  Serial.println("PPM");
  Serial.print('\n');

  Serial.print("NH4: ");
  Serial.print(iPPM_NH4);
  Serial.print("\t");
  Serial.println("PPM");
  Serial.println("----------------------------");
 

  
  if(WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
        String url = "http://192.168.43.29/write_data.php?co2="+String(iPPM_CO2)+"&&co="+String(iPPM_CO)+"&&nh4="+String(iPPM_NH4);
        digitalWrite(LED_BUILTIN,HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN,LOW);    
        http.begin(url);
  int httpcode=http.GET();
    /*if(httpcode>0)
    {
      Serial.print(httpcode);
    }*/
    http.end();
  }
  delay(100);
}


float MQResistanceCalculation(int raw_adc)
{
  return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}



float MQCalibration(int mq_pin)
{
  int i;
  float val=0;

  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {            //take multiple samples
    val += MQResistanceCalculation(analogRead(MQ_PIN));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/CALIBARAION_SAMPLE_TIMES;                   //calculate the average value
  val = val/RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro                                        
  return val;                                                      //according to the chart in the datasheet 

}


float MQRead(int mq_pin)
{
  int i;
  float rs=0;
 
  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }
 
  rs = rs/READ_SAMPLE_TIMES;
 
  return rs;  
}



long MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{
  if ( gas_id == GAS_CO2 ) {
     return MQGetPercentage(rs_ro_ratio,CO2Curve);
  } else if ( gas_id == GAS_CO ) {
     return MQGetPercentage(rs_ro_ratio,COCurve);
  } else if ( gas_id == GAS_NH4 ) {
     return MQGetPercentage(rs_ro_ratio,NH4Curve);
  }    
 
  return 0;
}



long  MQGetPercentage(float rs_ro_ratio, float *pcurve)
{
  return (pow(10,( ((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}
