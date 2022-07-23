#include <LiquidCrystal.h>
#include <USBAPI.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Digital pins to which you connect the LCD
const int inPin = 0;                   // A0 is where you connect the sensor
const int buttonPin = 8;

const int arrayLength = 12;
int samples[arrayLength];
int sample = 0;

int sampleDelay = 5000;

void setup()
{
    Serial.begin(9600);
    lcd.begin(16,2);
    lcd.print("Initialized");

    pinMode(buttonPin, INPUT);

    for (int i = 0; i < arrayLength; ++i) {
        samples[i] = analogRead(inPin); // read the value from the sensor
    }
}

void sampleLM35()
{
    float value = analogRead(inPin); // read the value from the sensor
    if (sample < arrayLength - 1)
    {
        samples[sample] = value;
        Serial.print("value ");
        Serial.print(sample);
        Serial.print(": ");
        sample++;
        Serial.print(value);
        Serial.print("\n");
    }
    else
    {
        sample = 0;
    }
}

float averageSample()
{
    float sum = 0;
    for (int i = 0; i < arrayLength; ++i) {
        sum += samples[i];
    }
    return sum / arrayLength;
}

void loop()
{
    sampleLM35();
    float value = averageSample();
    Serial.print("avg: ");
    Serial.print(value);
    Serial.print ("     ---     ");
    float millivolts = (value / 1024.0) * 5000;
    if (digitalRead(buttonPin) == HIGH)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("sensor val: ");
        lcd.print(value);
        lcd.setCursor(0, 1);
        lcd.print(millivolts);
        lcd.print( "mV");
        delay(sampleDelay);
    }
    else {
        lcd.setCursor(0, 1);
        float celsius = millivolts / 10;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(celsius);
        lcd.print("C");
        lcd.setCursor(0, 1);
        lcd.print((celsius * 9) / 5 + 32); //turning the celsius into fahrehait
        lcd.print("F");
        delay(sampleDelay);
    }
}