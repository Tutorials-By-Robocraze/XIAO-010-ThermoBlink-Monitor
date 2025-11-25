#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C pins for OLED
#define SDA_PIN 4
#define SCL_PIN 5

// LED pin
#define LED_PIN 10   // you can change this to any available GPIO

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("Temp Monitor Ready!");
  display.display();
  delay(2000);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C  Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  // 🧠 LED ALERT LOGIC
  if (t > 30.0) {
    // Blink fast if temperature > 30°C
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  } else {
    // LED OFF when below threshold
    digitalWrite(LED_PIN, LOW);
    delay(2000);
  }

  // 🖥️ Update OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.print("T: ");
  display.print(t);
  display.println("C");

  display.setCursor(10, 40);
  display.print("H: ");
  display.print(h);
  display.println("%");
  display.display();
}
