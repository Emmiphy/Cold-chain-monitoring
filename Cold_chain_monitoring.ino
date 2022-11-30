#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>



#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>

#define WIFI_SSID "Smartiphy company"
#define WIFI_PASSWORD "57859_764"

#define SMTP_server "smtp.gmail.com"
#define SMTP_Port 465


#define sender_email "iotdatainfo@gmail.com"
#define sender_password "@Emma080250"

#define Recipient_email "emmiphy@gmail.com"


SMTPSession smtp;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x3F, 16 column and 2 rows
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  dht.begin();     // initialize the sensor
  lcd.init();      // initialize the lcd
  lcd.backlight(); // open the backlight 

  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print("Smartiphy ");        // print message at (0, 0)
  lcd.setCursor(0, 1);         // move cursor to   (2, 1)
  lcd.print("Cold-Chain Monitoring"); // print message at (2, 1)
  delay (3000);
}

void loop()
{
  delay(2000); // wait a few seconds between measurements

  float humi  = dht.readHumidity();    // read humidity
  float tempC = dht.readTemperature(); // read temperature

  lcd.clear();
  // check if any reads failed
  if (isnan(humi) || isnan(tempC)) {
    lcd.setCursor(0, 0);
    lcd.print("waiting for data...");
  } else {
    lcd.setCursor(0, 0);  // start to print at the first row
    lcd.print("Temp: ");
    lcd.print(tempC);     // print the temperature
    lcd.print((char)223); // print ° character
    lcd.print("C");

    lcd.setCursor(0, 1);  // start to print at the second row
    lcd.print("Humi: ");
    lcd.print(humi);      // print the humidity
    lcd.print("%");


      Serial.begin(9600);
  Serial.println();
  Serial.print("Connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  }


  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  smtp.debug(1);


  ESP_Mail_Session session;


  session.server.host_name = SMTP_server ;
  session.server.port = SMTP_Port;
  session.login.email = sender_email;
  session.login.password = sender_password;
  session.login.user_domain = "";

  /* Declare the message class */
  SMTP_Message message;


  message.sender.name = "ESP8266";
  message.sender.email = sender_email;
  message.subject = "Cold-Chain Monitoring";
  message.addRecipient("Microcontrollerslab",Recipient_email);

   //Send HTML message
  String htmlMsg = "<style> table, th, td { border:1px solid black; }</style><body><h2>Cold chain monitoring</h2><table style=\"width:100%\"><tr><th>Location</th><th>Temperature:".tempC". Humidity: ".humi".</th><th>Country</th></tr><tr><td>65</td><td id='date-time'><script> var dt = new Date(); document.getElementById('date-time').innerHTML=dt;</script></td><td>Country</td></tr><tr><td></td><td></td><td></td></tr></table><p>Smartiphy Company 2022</p></body></html>";
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit; 



  if (!smtp.connect(&session))
    return;


  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
delay(30000); 
}
