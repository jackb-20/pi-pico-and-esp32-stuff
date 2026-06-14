#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// Initialize the display using the I2C address 0x3C (common for most 128x64 OLEDs)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int sensorPin = A0; // Soil moisture sensor pin

// Calibration values (Adjust these based on your own testing)
const int DryValue = 800;  // Raw ADC value when sensor is completely dry
const int WetValue = 400;  // Raw ADC value when sensor is fully submerged in water

void setup() {
  Serial.begin(9600);

  // Initialize OLED display. If it fails, halt program.
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // Read the raw analog value from the sensor
  int rawValue = analogRead(sensorPin);

  // Convert raw value to a percentage (0% to 100%)
  // map(value, fromLow, fromHigh, toLow, toHigh)
  int moisturePercent = map(rawValue, DryValue, WetValue, 0, 100);

  // Restrict values strictly within the 0-100 range
  moisturePercent = constrain(moisturePercent, 0, 100);

  // Debugging output sent to the Arduino Serial Monitor
  Serial.print("Raw Value: ");
  Serial.print(rawValue);
  Serial.print(" | Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  // Format and update the OLED screen
  display.clearDisplay();     // Wipe the previous frame
  
  display.setTextSize(1);     // Small text size for header
  display.setCursor(0, 0);    // Start at top-left corner
  display.print("PLANT MONITOR");
  
  display.setTextSize(2);     // Larger text size for moisture metric
  display.setCursor(0, 25);   // Move cursor lower down
  display.print("Soil: ");
  display.print(moisturePercent);
  display.print("%");

  display.display();          // Push everything to the physical screen
  
  delay(1000);                // Wait 1 second before refreshing
}
