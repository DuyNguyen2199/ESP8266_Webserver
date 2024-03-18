#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "DHT.h"
#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE, 15); 
MDNSResponder mdns;
 
ESP8266WebServer server(80);
String webPage;
const char* ssid     = "wifi";      //Thay tên wifi ở đây
const char* password = "password";  //Thay tên password ở đây
 
void setup() {
 
  pinMode(12, OUTPUT);  //led chân 12
  pinMode(13, OUTPUT);  //led chân 13
 
  webPage += "<h1>ESP8266 Web Server</h1><p>Socket #1 <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></p>";
  webPage += "<p>Socket #2 <a href=\"socket2On\"><button>ON</button></a>&nbsp;<a href=\"socket2Off\"><button>OFF</button></a></p>";
  
  Serial.begin(115200);
  delay(100);
  dht.begin();
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) 
    Serial.println("MDNS responder started");
 
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on("/socket1On", [](){
    server.send(200, "text/html", webPage);
    //Bật led 12
    digitalWrite(12, HIGH);
    delay(1000);
  });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", webPage);
    //Tắt led 12
    digitalWrite(12, LOW);
    delay(1000); 
  });
  server.on("/socket2On", [](){
    server.send(200, "text/html", webPage);
    //Bật led 13
    digitalWrite(13, HIGH);
    delay(1000);
  });
  server.on("/socket2Off", [](){
    server.send(200, "text/html", webPage);
    //Tắt led 13
    digitalWrite(13, LOW);
    delay(1000); 
  });
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop() {

  // Kiem tra khi co client ket noi
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Doi client gui ket noi
  Serial.println("Co mot client moi ket noi xem du lieu");
  while(!client.available()){
    delay(1);
  }
  
  // Doc do am
  float h = dht.readHumidity();
  // Doc nhiet do o do C
  float t = dht.readTemperature();
  
  // Doc dong dau tien cua yeu cau gui len.
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Chuan bi tao web de phan hoi
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<head>";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  s += "<meta http-equiv=\"refresh\" content=\"60\" />";
  s += "<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>";
  s += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">";
  s += "<style>body{font-size: 24px;} .voffset {margin-top: 30px;}</style>";
  s += "</head>";
  
  s += "<div class=\"container\">";
  s += "<h1>Theo doi nhiet do va do am</h1>";
  s += "<div class=\"row voffset\">";
  s += "<div class=\"col-md-3\">Nhiet do: </div><div class=\"col-md-3\">" + String(t) + "</div>";
  s += "<div class=\"col-md-3\">Do am: </div><div class=\"col-md-3\">" + String(h) + "</div>";
  s += "</div>";
   
  // Gui phan hoi toi client (o day la giao dien web)
  client.print(s);
  delay(1);
  Serial.println("Client da thoat");

  // Sau khi nhan duoc thong tin thi se tu dong ngat ket noi
    
}
}
