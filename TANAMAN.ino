/*Plant watering system blynk
   https://srituhobby.com
*/
#define BLYNK_TEMPLATE_ID "TMPL6J6MsdfnW"
#define BLYNK_TEMPLATE_NAME "Plant Watering System"

//Include the library files
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "w1302-dPhfnSYkpFeXiKhs7_HBgF4Sn6";//Enter your Auth token
char ssid[] = "SATURN";//Enter your WIFI name
char pass[] = "061212sya";//Enter your WIFI password

BlynkTimer timer;
bool Relay = 0;

//Define component pins
#define sensor A0
#define waterPump D3

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  // Kode ini akan mematikan pompa di awal, karena relay aktif-tinggi.
  digitalWrite(waterPump, LOW); 
  lcd.init();
  lcd.backlight();

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  lcd.setCursor(1, 0);
  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();

  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
}

//Get the button value
BLYNK_WRITE(V1) {
  Relay = param.asInt();

  if (Relay == 1) { // Jika tombol ON di Blynk
    // Ini akan menyalakan pompa karena relay Anda aktif-tinggi.
    digitalWrite(waterPump, HIGH); 
    lcd.setCursor(0, 1);
    lcd.print("Motor is ON ");
  } else { // Jika tombol OFF di Blynk
    // Ini akan mematikan pompa.
    digitalWrite(waterPump, LOW); 
    lcd.setCursor(0, 1);
    lcd.print("Motor is OFF");
  }
}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value);
  lcd.setCursor(0, 0);
  lcd.print("Moisture :");
  lcd.print(value);
  lcd.print(" ");

}

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}