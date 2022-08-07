#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <Keypad.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#define password_Length 5
#define SS_PIN 5
#define RST_PIN 36
#define BUZZER_PIN 4

MFRC522 mfrc522(SS_PIN, RST_PIN);

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define ROW_NUM     4 
#define COLUMN_NUM  4

const char* ssid = "vivo 1938"; 
const char* password = "silviasheraaa";

char Data[password_Length]; 
char Master[password_Length] = "1980"; 
byte data_count = 0, master_count = 0;

int cek = 0;

bool rfid_flag = true;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM]      = {32, 33, 25, 26}; 
byte pin_column[COLUMN_NUM] = {27, 14, 12, 13};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

void scanWifi() {
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    delay(2000);
  }
  Serial.print("Connected to WiFi ");
  Serial.print(ssid);
  Serial.print(" (");
  Serial.print(WiFi.localIP());
  Serial.println(")");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI.begin();      
  mfrc522.PCD_Init();
  scanWifi();
  lcd.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("Connecting");
  lcd.setCursor(5,1);
  lcd.print("Internet");
  delay(1000);
//  initWiFi();
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connected!");
  delay(2000);    
  lcd.clear();                                                                                                                                                         
}

void loop() {  
  String nama_tamu = "anon";
  String status1 = "tamu";
  String keterangan = "oke";
  char key = keypad.getKey();
  
  if (key) {
    Serial.println(key);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
  }

  if ( key == '*'){
    rfid_flag = false;
  }

  if (rfid_flag == false) {
    lcd.setCursor(0,0);
    lcd.print("Masukkan pass :");
    if (key >= '0' && key <= '9') {
      Data[data_count] = key; 
      lcd.setCursor(data_count,1); 
      lcd.print(Data[data_count]); 
      data_count++;          
    }

    if ( key == '#'){
      rfid_flag = true;
    }
    
    if(data_count == password_Length-1){
      delay(200);
      lcd.clear();
    
      if(!strcmp(Data, Master)){
        lcd.print("Correct");
        delay(2000);
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Selamat Datang");
        delay(100);
        String serverName =  "http://192.168.43.70:3306/input?nama="+String(nama_tamu)+"&status="+String(status1)+"&keterangan="+String(keterangan);
        if (WiFi.status() == WL_CONNECTED) {
          WiFiClient client;
          HTTPClient http;
          http.begin(client, serverName);
          http.addHeader("Content-Type", "text/plain");
          int httpResponseCode = http.POST("");
          if (httpResponseCode == 201 or httpResponseCode == 200) {
            lcd.setCursor(0, 1);
            lcd.print("log disimpan");
            Serial.println("log saved");
          }
          else {
            lcd.setCursor(0, 1);
            lcd.print("log gagal");
            Serial.println("log not saved");
          }
          Serial.println(httpResponseCode);
          http.end();
        }
        else {
          Serial.println("WiFi Disconnected");
        }
        delay(3000);
        lcd.clear();
        //tambah relay disini
        rfid_flag == true;
      }
      else{
        lcd.print("Incorrect");
        delay(1000);
      }
      lcd.clear();
      clearData();  
    }
  }
  
  if (rfid_flag == true) {
    lcd.setCursor(0,0);
    lcd.print("tekan * atau tap");
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
      
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }  
    
    String content= "";
    
    byte letter;
    
    for (byte i = 0; i < mfrc522.uid.size; i++) {
       Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
       Serial.print(mfrc522.uid.uidByte[i], HEX);
       content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
       content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    
    content.toUpperCase();

//    nambah user start disini
    if (content.substring(1) == "BC EE E8 22") {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Akses");
      lcd.setCursor(3,1);
      lcd.print("Diterima");
      Serial.println("Diterima");
      nama_tamu = "aul";
      status1 = "nginep";
      keterangan = "capek";
      delay(1500);      
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Selamat Datang");
      delay(100);
      String serverName =  "http://192.168.43.70:3306/input?nama="+String(nama_tamu)+"&status="+String(status1)+"&keterangan="+String(keterangan);
      if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverName);
        http.addHeader("Content-Type", "text/plain");
        int httpResponseCode = http.POST("");
        if (httpResponseCode == 201 or httpResponseCode == 200) {
          lcd.setCursor(0, 1);
          lcd.print("log disimpan");
          Serial.println("log saved");
        }
        else {
          lcd.setCursor(0, 1);
          lcd.print("log gagal");
          Serial.println("log not saved");
        }
        Serial.println(httpResponseCode);
        http.end();
      }
      else {
        Serial.println("WiFi Disconnected");
      }
      delay(3000);
      lcd.clear();
    }
//sampe sini
    else if (content.substring(1) == "DA FC 9A 16") {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Akses");
      lcd.setCursor(3,1);
      lcd.print("Diterima");
      Serial.println("Diterima");
      nama_tamu = "ais";
      status1 = "nginep";
      keterangan = "melek";
      delay(1500);      
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Selamat Datang");
      delay(100);
      String serverName =  "http://192.168.43.70:3306/input?nama="+String(nama_tamu)+"&status="+String(status1)+"&keterangan="+String(keterangan);
      if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverName);
        http.addHeader("Content-Type", "text/plain");
        int httpResponseCode = http.POST("");
        if (httpResponseCode == 201 or httpResponseCode == 200) {
          lcd.setCursor(0, 1);
          lcd.print("log disimpan");
          Serial.println("log saved");
        }
        else {
          lcd.setCursor(0, 1);
          lcd.print("log gagal");
          Serial.println("log not saved");
        }
        Serial.println(httpResponseCode);
        http.end();
      }
      else {
        Serial.println("WiFi Disconnected");
      }
      delay(3000);
      lcd.clear();
    }
   
   else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Akses");
      lcd.setCursor(3,1);
      lcd.print("Ditolak");
      Serial.println("Ditolak");
      delay(2000);
    }
  }
}

void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0; 
  }
  return;
}

void openDoor() {

}
