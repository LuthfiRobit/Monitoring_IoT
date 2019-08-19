#include <ESP8266WiFi.h>                                                // esp8266 library
#include <FirebaseArduino.h>                                             // firebase library

#define FIREBASE_HOST "iot-test-9751b.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "7O1RWnyGW5bdsC3lTk1PxLjWu745Sz03RJ5F1k2p"                    // the secret key generated from firebase
#define WIFI_SSID "Rooster"                                          // input your home or public wifi name 
#define WIFI_PASSWORD "hurufkecil"                                    //password of wifi ssid

String fireStatus = "";                                                     // led status received from firebase
int data;
int led = D5;                                                                // for external led
void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);      
  pinMode(led, OUTPUT);                 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
}

void loop() {
  //Kirim data ke firebase
  data = random(0, 100);
  send_data(data);
  //Terima dari ke firebase
  fireStatus = Firebase.getString("NILAI");                                      // get ld status input from firebase
  Serial.println(fireStatus);
}

void send_data(int a){
    //http://192.168.43.51:81/Telemetri/insert.php?data1=10&data2=45&data3=50
    WiFiClient client;   
    //Kirim Data Ke Firebase
   if (isnan(a)) {                                                // Check if any reads failed.
    Serial.println(F("Failed to read from sensor!"));
    return;
  }
String sensor = String(a); 
  delay(100);
  Firebase.setString("/Ultrasonik/Sensor", sensor);                                  //setup path and send readings
    if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error()); 
      return;
  }
//  if (!client.connect(host, httpPort)) {
//    Serial.println("connection failed");
//    return;
//  }
//  client.print(String("GET ") + pathfile + " HTTP/1.1\r\n" +
//               "Host: " + host + "\r\n" + 
//               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 500000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
 }
