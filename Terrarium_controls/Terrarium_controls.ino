#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHTesp.h"

#define DHTpin 14
DHTesp dht;

const int LIGHTS = 5;
const int MOISTURE_SENSOR_PIN = A0;

ESP8266WebServer server(80); // create a web server on port 80

int convertToPercent(int value) {
  if (value < 0) {
    return 0;
  }
  if (value > 950) {
    return 100;
  }
  return (value / 9.5);
}

void handleRoot() {
  int humidity = floor(dht.getHumidity());
  int temperature = floor(dht.getTemperature());
  int moisture = convertToPercent(analogRead(MOISTURE_SENSOR_PIN)); 

  String html = "<html><head><link rel='stylesheet' href='/style.css' type='text/css'><meta name='viewport' content='height=896, width=414'> <title>Terraium</title></head><body> <div class='app'> <h1>TERARRIUM</h1> <h4>By Karim & Neha</h4> <div class='monitor'> <div class='single-progress-bar'> <label for='water'><img src='https://i.ibb.co/DMLWWKC/Group-10.png' class='data-icon'> "+String(moisture)+"%"+"</label> <progress id='water' value='"+String(moisture)+"' max='100'></progress> </div><div class='single-progress-bar'> <label for='humidity'><img src='https://i.ibb.co/ZcLjTfs/Vector-1.png' class='data-icon'>"+String(humidity)+"%"+"</label> <progress id='humidity' value='"+String(humidity)+"' max='100'></progress> </div><div class='single-progress-bar'> <label for='temp'><img src='https://i.ibb.co/GMR8Wmb/Vector-1.png' class='temp-icon'>"+String(temperature)+"C"+"</label> <progress id='temp' value='"+String(temperature)+"' max='50'></progress> </div></div><div class='controls'> <a href='/lighton'> <div class='button' id='float-left'> <img src='https://i.ibb.co/J2wdcYm/sun-alt-svgrepo-com-1.png'> <p>ON</p></div></a> <a href='/lightoff'> <div class='button' id='float-right'> <img src='https://i.ibb.co/TkPYhvH/moon-stars-svgrepo-com-1.png'> <p>OFF</p></div></a> </div></div></body></html>";
  server.send(200, "text/html", html);
}

void handleCSS(){
  String css = "@import url('https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap');body{font-family: 'Roboto', sans-serif; color: white; margin:0px; background-image: url('https://i.ibb.co/PDBdY2r/Frame-2.png'); background-size: cover;}.app{margin: auto; height: 896px; width: 414px; text-align: center; padding: 30px 0px 15px 0px;}h1{font-size: 50px; font-weight: 550;}h4{font-size: 20px; font-weight: 200; margin: -15px 0px 50px 0px;}.monitor{width: 340px; margin: auto; background: rgb(255,255,255,0.13); border-radius: 15px; padding:10px 20px 10px 20px; backdrop-filter: blur(2px);}.controls{margin: auto; margin-top: 20px; width: 380px;}.button{width: 140px; height: 85px; background: rgb(255,255,255,0.13); border-radius: 20px; padding:20px; backdrop-filter: blur(2px);}.single-progress-bar{margin: 20px 0 20px 0; display: flex; align-items: center; justify-content: center;}#float-left{float: left;}#float-right{float: right;}progress{-webkit-appearance: none; appearance: none; width: 265px; height: 19px; border: none; padding: 2px; float:right;}progress::-webkit-progress-value{padding: 2px; background-color: #fff; border-radius: 10px; margin-top: 2px;}progress::-moz-progress-bar{padding: 2px; background-color: #fff;}progress::-webkit-progress-bar{background-color: rgba(255, 255, 255, 0.3); border-radius: 10px;}img{width: 55px; margin-bottom:-5px;}label{width: 100px; display: flex; align-items: center; justify-content: center;}a{text-decoration: none; color:white;}.data-icon{width: 30px; margin-top: -3px; margin-right: 15px;}.temp-icon{width: 19px; margin: 0px 0px 0px -3px; margin-right: 24px;}";
  server.send(200, "text/css", css);
  }

void handleLightOn() {
  digitalWrite(LIGHTS, HIGH);
  server.sendHeader("Location", "/", true);
  server.send(303);
}

void handleLightOff() {
  digitalWrite(LIGHTS, LOW);
  server.sendHeader("Location", "/", true);
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  dht.setup(DHTpin, DHTesp::DHT11);
  pinMode(LIGHTS, OUTPUT);
  digitalWrite(LIGHTS, HIGH);
  WiFi.begin("BELL449", "E1557EAE"); // connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot); // serve the web page at the root URL
  server.on("/lighton", handleLightOn); // handle the light on request
  server.on("/lightoff", handleLightOff); // handle the light off request
  server.on("/style.css", handleCSS);
  
  server.begin(); // start the server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // listen for client requests
  delay(100);
}
