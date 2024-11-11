#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display



const char* ssid     = "shuo";       // 连接WiFi名（此处使用shuo为示例）
                                            // 请将您需要连接的WiFi名填入引号中
const char* password = "123456789";          // 连接WiFi密码（此处使用123456789为示例）
                                            // 请将您需要连接的WiFi密码填入引号中
 
const char* host = "api.seniverse.com";     // 将要连接的服务器地址  
const int httpPort = 80;                    // 将要连接的服务器端口      
 
// 心知天气HTTP请求所需信息
String reqUserKey = "SLCDhO3YLZzWseA7J";   // 私钥
String reqLocation = "qingdao";            // 城市
String reqUnit = "c";                      // 摄氏/华氏

void setup(){
  Serial.begin(9600);          
  Serial.println("");
  
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Qingdao Weather:");
  
  // 连接WiFi
  connectWiFi();
}
 
void loop(){
  // 建立心知天气API当前天气请求资源地址
  String reqRes = "/v3/weather/now.json?key=" + reqUserKey +
                  + "&location=" + reqLocation + 
                  "&language=en&unit=" +reqUnit;
 
  // 向心知天气服务器服务器请求信息并对信息进行解析
  httpRequest(reqRes);
  delay(5000);
}
 
// 向心知天气服务器服务器请求信息并对信息进行解析
void httpRequest(String reqRes){
  WiFiClient client;
 
  // 建立http请求信息
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
                              "Host: " + host + "\r\n" + 
                              "Connection: close\r\n\r\n";
  Serial.println(""); 
  Serial.print("Connecting to "); Serial.print(host);
 
  // 尝试连接服务器
  if (client.connect(host, 80)){
    Serial.println(" Success!");
 
    // 向服务器发送http请求信息
    client.print(httpRequest);
    Serial.println("Sending request: ");
    Serial.println(httpRequest);  
 
    // 获取并显示服务器响应状态行 
    String status_response = client.readStringUntil('\n');
    Serial.print("status_response: ");
    Serial.println(status_response);
 
    // 使用find跳过HTTP响应头
    if (client.find("\r\n\r\n")) {
      Serial.println("Found Header End. Start Parsing.");
    }
    
    // 利用ArduinoJson库解析心知天气响应信息
    parseInfo(client); 
  } else {
    Serial.println(" connection failed!");
  }   
  //断开客户端与服务器连接工作
  client.stop(); 
}
 
void connectWiFi(){
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
  
  int i = 0;
  int timeout = 30; // Timeout after 30 seconds
  while (WiFi.status() != WL_CONNECTED && i < timeout) {      
    delay(1000);                       
    Serial.print(i++); Serial.print(' ');      
  }                                            
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("Connection established!");   
    Serial.print("IP address:    ");             
    Serial.println(WiFi.localIP());              
  } else {
    Serial.println("");
    Serial.println("Connection failed!");
  }
} 
// 利用ArduinoJson库解析心知天气响应信息
void parseInfo(WiFiClient client){
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 230;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, client);
  JsonObject results_0 = doc["results"][0];
  
  JsonObject results_0_now = results_0["now"];
  const char* results_0_now_text = results_0_now["text"]; // "Sunny"
  const char* results_0_now_temperature = results_0_now["temperature"]; // "32"
  
  // 通过串口监视器显示以上信息
  String results_0_now_text_str = results_0_now["text"].as<String>(); 
  String results_0_now_temperature_str = "Tem:"+results_0_now["temperature"].as<String>();  
 
  Serial.println(F("======Weahter Now======="));
  Serial.print(F("Weather Now: "));
  Serial.println(results_0_now_text_str);
  Serial.print(F("Temperature: "));
  Serial.println(results_0_now_temperature_str);
  Serial.println(F("========================"));
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(1,1);
  lcd.print(results_0_now_text_str);
  lcd.setCursor(9,1);
  lcd.print(results_0_now_temperature_str);
}
