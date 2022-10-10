#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <i2cdetect.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SCL --> D1 ; SDA --> D2 to make the OLED screen work with esp8266 ; check pin layout on esp8266 board
#define AP_IP "172.0.0.1"
#define BUTTON 5

int totalClients ;

// NF-MYSELF : always specify width, height, and add reference to Wire ( &Wire)
Adafruit_SSD1306 display(128, 64, &Wire, -1); // adafruit object init : give -1 as a parameter bcz we dont have any pins that is the reset pin

const byte DNS_PORT = 53 ;
IPAddress apIP(172,0,0,1);
DNSServer dnsServer ;
ESP8266WebServer webServer(80);

String html = ""
              "<!DOCTYPE html><html><head>"
              "<title>Starbucks Wifi</title></head>"
              "<body style='background-image: url('https://image.shutterstock.com/image-photo/penang-malaysia-april-13-2018-260nw-1072863746.jpg') ;background-repeat: no-repeat;background-size: cover;'>"
              "<div id=\"form\" style='width: 500px; height: 720px;background-color: white;text-align: center;position: absolute;left: 35%;top: 10%;font-family: Arial, Helvetica, sans-serif;'>"
            "<div><img src=\"starbucks_logo.png\" alt=\"startbucks-logo\" style=\"position: relative;margin-top: 15px;width: 25%;\">"
            "<p style=\"color: green; font-weight : bold; font-size : 18px\"> Wi-Fi + Coffee = &#x1F49A</p></div>"
            "<div><p style=\"line-height:1.6 ;\">Welcome to Starbucks <br> Log on to our network once, and this device <br>will automatically connect at participating <br>Starbucks stores everywhere you go.</p></div>"
            "<div><input type=\"text\" placeholder=\"First name\" style=\"margin-bottom: 10px;width: 70%;padding: 12px 20px;margin: 8px 0;display: inline-block;border: 1px solid #ccc;border-radius: 4px;box-sizing: border-box;\"> <br>"
            "<input type=\"text\" placeholder=\"Last name\" style=\"margin-bottom: 10px;width: 70%;padding: 12px 20px;margin: 8px 0;display: inline-block;border: 1px solid #ccc;border-radius: 4px;box-sizing: border-box;\"> <br>"
            "<input type=\"text\" placeholder=\"Email\" style=\"margin-bottom: 10px;width: 70%;padding: 12px 20px;margin: 8px 0;display: inline-block;border: 1px solid #ccc;border-radius: 4px;box-sizing: border-box;\"> <br></div>"
            "<div><p>By accepting, I agree to receive emails about <br>"
            "Starbuck news, promotions and offers</p>"
            "<button style=\"width: 300px;height: 80px;background-color: #04AA6D; /* Green */color: white;font-size: 23px;cursor: pointer;border-bottom-left-radius: 40px ;border-top-left-radius: 40px;border-bottom-right-radius: 40px ;border-top-right-radius: 40px;\">"
            "Accept & Connect</button>"
            "<p><br> Wi-Fi powered by our friends at Google</p>"
            "</div></div></body></html>";




void displayInt(int tclients){
  // Clear the buffer.
	display.clearDisplay();

	// Display Text
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,28);
  display.print("Total number of Clients : ");
	display.println(tclients);
	display.display();
	delay(2000);
	display.clearDisplay();  
}

void displayAPinfo(){

  String macAP = WiFi.softAPmacAddress();
  // Clear the buffer.
	display.clearDisplay();

	// Display Text
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,15);
  display.println("Mac Address : ");
  display.println(macAP);
  display.print("AP ip : ");
  display.println(AP_IP);
	display.display();
	delay(2000);
	display.clearDisplay();
}


void setup(){
  Serial.begin(115200);

  pinMode(BUTTON, INPUT_PULLUP);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
  WiFi.softAP("ESP8266 captive portal");

  dnsServer.start(DNS_PORT, "*", apIP);

  webServer.onNotFound([](){
    webServer.send(200, "text/html", html);
  });

  webServer.begin();  
  Serial.println("ESP access point has started...");

  // Dispalying data on the OLED screen
  // initialize with the I2C addr 0x3C
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
}

void loop(){
  dnsServer.processNextRequest();
  webServer.handleClient();
  delay(500);
  totalClients = WiFi.softAPgetStationNum(); // will return the total number of clients
  displayInt(totalClients);
  //displayAPinfo();
  
  
  
}