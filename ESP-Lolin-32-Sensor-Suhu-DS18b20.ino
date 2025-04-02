#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char *ssid = "Baso";          //Nama Wifi
const char *password = "Baso123";   // pass wifi
const int oneWireBus = 4;           //Pin Sensor     

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {
  delay(1000);
  Serial.begin(115200);  
  //Untuk memulai koneksi ke jaringan WiFi
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting");

  // Tunggu Sampai Terhubung
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Terhubung ke Wi-Fi ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address ESP
  Serial.println("proses kirim data...");
}

void loop() {
  kirim_data();
  delay(500);
}

void kirim_data() {
  sensors.begin();
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float data, sensor;
  data = temperatureC;  // ubah dengan data dari sensor
  sensor = random(0, 25);
  String postData = (String) "data=" + data + "&sensor=" + sensor;

  HTTPClient http;
  http.begin("http://192.168.163.123/Websensor/kirimdata.php"); //Alamat web server di localhost
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  auto httpCode = http.POST(postData);
  String payload = http.getString();

  Serial.println(postData);
  Serial.println(payload);

  http.end();
}