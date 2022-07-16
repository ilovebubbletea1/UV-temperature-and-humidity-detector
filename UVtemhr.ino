#include <SPI.h>//
#include <Wire.h>//
#include <Adafruit_GFX.h>//
#include <Adafruit_SSD1306.h>//
#include <Adafruit_AHTX0.h>//
#include <Fonts/FreeSans9pt7b.h>
int UVOUT = A0; //Output from the sensor
int REF_3V3 = A1; //3.3V power on the Arduino board
#define RED 0xF800
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_AHTX0 aht;
void setup()
{
  Serial.begin(9600);
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
  Serial.println("ML8511 example");

// Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setRotation(0); 
  
  if (aht.begin()) {
    Serial.println("Found AHT20");
  } else {
    Serial.println("Didn't find AHT20");
  }  

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 
  ///
}

void loop()
{
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);

  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;

  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level
  sensors_event_t humidity, temp;

  
  display.setCursor(22, 2 ); 
  display.setTextSize(1);
  display.println("UV Ray Intensity");
  Serial.print("UV :");
  Serial.print(uvIntensity);
  Serial.println();
  display.setCursor(30, 15); //oled display
  display.setTextSize(1);display.println(uvIntensity);

  display.setCursor(60, 15); //oled display
  display.setTextSize(1);display.println("mW/cm^2");



 

  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  display.setCursor(0,25);display.println("Temperature:");  
  display.setCursor(72,25);display.print(temp.temperature);display.print((char)247);display.print("C");

  display.setCursor(0, 40);display.println("Humidity:");
  display.setCursor(72, 40);display.print(humidity.relative_humidity);display.println("%");


  Serial.print("Temperature: ");Serial.print(temp.temperature);Serial.println("  degrees");
  Serial.print("Humidity: ");Serial.print(humidity.relative_humidity);Serial.println(" %");
  display.display();
  delay(300);
  display.clearDisplay();


}

//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;

  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return (runningValue);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;


}
