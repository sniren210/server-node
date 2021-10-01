#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Arduino_JSON.h>

ESP8266WebServer server(80); // variabel objek server

JSONVar Object;

const char *ssid = "Ha"; //ganti nama wifi
const char *pass = "123456789";//ganti password

String page = "<h1>Simple NodeMCU Web Server</h1>";

int ph = 6.8;
String warna = "jernih";
//int LEDPin = D4;
void setup()
{
  Object["ph air"] = 5,0;
  Object["warna air"] = "Jernih";
  // Setting Mode pin 
  pinMode(16,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(0,OUTPUT);
  
Serial.begin(9600);
delay(10);

  // Koneksi WiFi
Serial.print(" Menghubungkan ke : ");
Serial.println(ssid);

WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print("….");
}
Serial.print("\n");
Serial.print("IP address : ");
Serial.print(WiFi.localIP());
Serial.print("\n");
Serial.print("GateWay : ");
Serial.println(WiFi.gatewayIP().toString().c_str());
Serial.println(" ");
Serial.print("Terhubung dengan : ");
Serial.println(WiFi.SSID());

// Kode bagian Server
  server.on("/setpin", HTTP_POST, setPin);
  server.on("/getdata", HTTP_GET, getData);
//  server.on("/relay/toggle", HTTP_POST, [](){
//    server.send(200, "text/plain","{\"ph air\": \"ph\"},{\"warna air\": \"jernih\"}");
//    });

  server.on("/", [](){
    server.send(200, "text/html", page);
  });
  server.begin();
  Serial.println("Server berjalan...");
//  Serial.println(Object);
}

void loop() {
    server.handleClient();
}

void getData(){
//  server.send(200, "text/html", String("<h1>") + ph + "</h1>");
  server.send(200, "text/plain",String("{\"ph_air\": \"") + ph + String("\",\"warna_air\": \"") + warna+ String("\"}"));
}

// Fungsi untuk menangani request /setpin
void setPin() {
  
  if (server.hasArg("pin") && server.hasArg("state")) {
    int statusPin = server.arg("state").toInt();
    // Konversi nama pin ke alamat pin
    String namaPin = server.arg("pin");
    static const uint8_t pinAddress[] = {16,  5,  4,  0,  2, 14, 12, 13, 15,  3,  1};
    
    digitalWrite(pinAddress[namaPin.substring(1).toInt()], statusPin);
    server.send(200, "text/html", "Perintah sudah di jalankan");
  }
  else {
    server.send(200, "text/html", "Format perintah salah / tidak ada");
  }
}
