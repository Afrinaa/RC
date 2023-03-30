#define BLYNK_TEMPLATE_ID "TMPLoaa9Beao"
#define BLYNK_TEMPLATE_NAME "RK cradle"
#define BLYNK_AUTH_TOKEN "r7WxaXywVjP0weZN7mh6JcsYiGcTeoOn"

// Your WiFi Credentials.
// Set password to "" for open networks.
char ssid[] = "BUBT_Computer_Lab-520";
char pass[] = "bubt1234";

// define the GPIO connected with Sensors & LEDs
#define SERVO1_PIN    2 //D4
#define SERVO2_PIN    4 //D2
#define RAIN_SENSOR   5  //D1
#define GREEN_LED     14 //D5
#define RED_LED       13 //D7
#define WIFI_LED      16 //D0

//#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Servo.h>
Servo servo;
int RAIN_SENSOR_Value = 0;
bool isconnected = false;
char auth[] = BLYNK_AUTH_TOKEN;

Servo servo1,servo2;
int angleValue = 90; // start angle in the middle
int stepValue = 5; // angle increment per loop
int minAngle = 0; // minimum angle
int maxAngle = 180; // maximum angle
int delayTime = 20; // delay time between steps (in milliseconds)

#define VPIN_BUTTON_2    V2
#define VPIN_SERVO_ANGLE V0
#define VPIN_SWING_COUNT V1
#define VPIN_POWER_BTN   V3

BlynkTimer timer;

void checkBlynkStatus() { // called every 2 seconds by SimpleTimer
  getSensorData();
  isconnected = Blynk.connected();
  if (isconnected == true) {
    digitalWrite(WIFI_LED, LOW);
    sendSensorData();
    Serial.println("Blynk Connected");
  }
  else{
    digitalWrite(WIFI_LED, HIGH);
    Serial.println("Blynk Not Connected");
  }
}

void getSensorData()
{
 
  RAIN_SENSOR_Value = digitalRead(RAIN_SENSOR);
   if (RAIN_SENSOR_Value == 0 ){
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  }
  else{
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
}

void sendSensorData()
{  
   if (RAIN_SENSOR_Value == 0 )
  {
    Blynk.logEvent("rain", "Water Detected!");
    Blynk.virtualWrite(VPIN_BUTTON_2, "Water Detected!");
  }
  else if (RAIN_SENSOR_Value == 1 )
  {
    Blynk.virtualWrite(VPIN_BUTTON_2, "No Water Detected.");
  }
}

void setup()
{
  Serial.begin(9600);
 
  servo1.attach(2);
  servo2.attach(4);
 
  pinMode(RAIN_SENSOR, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);
  pinMode(SERVO1_PIN, OUTPUT);
  pinMode(SERVO2_PIN, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(WIFI_LED, HIGH);

  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(1000);
}
int i, pos, noOfSwing, swingAngle,totalAngle, deductedAngle, powerButton; //globally declared
BLYNK_WRITE(V0) //taking angle input (0-90)
{
 
  swingAngle=param.asInt();
  Serial.println(swingAngle);
  Blynk.syncVirtual(V0);
 
}
BLYNK_WRITE(V1) //taking number of swing
{
 
 noOfSwing= param.asInt();
 Serial.println(noOfSwing);
 Blynk.syncVirtual(V1);
 
}
BLYNK_WRITE(V3) //taking power status
{
  powerButton= param.asInt();
  Serial.println(powerButton);
  if(powerButton==1){
    swing();
   }
  Blynk.syncVirtual(V3);

}
void swing()
{
  int halfAngle=swingAngle/2;
  totalAngle=angleValue+halfAngle; //servo middle point, 90 theke 90+swingAngle porjonto jabe
  deductedAngle=angleValue-halfAngle; //jehetu mirroring hoitese tai 90-swingAngle porjonto jabe
  for(i=1;i<=noOfSwing;i++) {
         servo1.write(totalAngle);
         servo2.write(deductedAngle);
         delay(500);
         servo1.write(deductedAngle);
         servo2.write(totalAngle);
         delay(500);
  }
  //kaj seshe middle point a ene rakhar jonno
  servo1.write(angleValue);
  servo2.write(angleValue);
  //blynk a sob value 0 kore deyar jonno
  Blynk.virtualWrite(V0, 0);
  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V3, 0);
}

void loop()
{
  Blynk.run();
  timer.run();
}