

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 6
#define LED_PIN_2 5
#define LED_PIN_3 3

#define LED_COUNT 8
unsigned long colorTimeManager;
Adafruit_NeoPixel strip_1(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_2(LED_COUNT, LED_PIN_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_3(LED_COUNT, LED_PIN_3, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip_1
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
bool chargerlock = false;
void setup()
{
    Serial.begin(9600);
    // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
    // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif
    // END of Trinket-specific code.

    strip_1.begin();           // INITIALIZE NeoPixel strip_1 object (REQUIRED)
    strip_1.show();            // Turn OFF all pixels ASAP
    strip_1.setBrightness(150); // Set BRIGHTNESS to about 1/5 (max = 255)

    strip_2.begin();           // INITIALIZE NeoPixel strip_1 object (REQUIRED)
    strip_2.show();            // Turn OFF all pixels ASAP
    strip_2.setBrightness(150); // Set BRIGHTNESS to about 1/5 (max = 255)

    strip_3.begin();           // INITIALIZE NeoPixel strip_1 object (REQUIRED)
    strip_3.show();            // Turn OFF all pixels ASAP
    strip_3.setBrightness(150); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop()
{
    // Fill along the length of the strip_1 in various colors...
        // colorWipe(strip_1.Color(255, 0, 0), 50); // Red
        // colorWipe(strip_2.Color(0, 255, 0), 50); // Green
        // colorWipe(strip_3.Color(0, 0, 255), 50); // Blue
        colorWipe_4(strip_1.Color(255, 0, 0),strip_2.Color(255, 0, 0),strip_3.Color(255, 0, 0), 100);
    // theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
}

// Some functions of our own for creating animated effects -----------------

// Fill strip_1 pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip_1.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait)
{
    for (int i = 0; i < strip_1.numPixels() && checkCharger(); i++)
    {                                  // For each pixel in strip_1...
        strip_1.setPixelColor(i, color); //  Set pixel's color (in RAM)
        strip_2.setPixelColor(i, color); //  Set pixel's color (in RAM)
        strip_3.setPixelColor(i, color); //  Set pixel's color (in RAM)
        strip_1.show();                  //  Update strip_1 to match
        strip_2.show();                  //  Update strip_1 to match
        strip_3.show();                  //  Update strip_1 to match
        delay(wait);                   //  Pause for a moment
    }
}
void colorWipe_4(uint32_t color,uint32_t color_2,uint32_t color_3, int wait)
{
    for (int i = 0; i < strip_1.numPixels() && checkCharger(); i++)
    {                                  // For each pixel in strip_1...
        strip_1.setPixelColor(i, color); //  Set pixel's color (in RAM)
        strip_2.setPixelColor(i, color_2); //  Set pixel's color (in RAM)
        strip_3.setPixelColor(i, color_3); //  Set pixel's color (in RAM)
        strip_1.show();                  //  Update strip_1 to match
        strip_2.show();                  //  Update strip_1 to match
        strip_3.show();                  //  Update strip_1 to match
        delay(wait);                   //  Pause for a moment
    }
}
bool checkCharger()
{
    int sensorValue = analogRead(A0);
    // print out the value you read:
    // delay(1);
    while (sensorValue > 300)
    {
        // Serial.println("bug");
        if (!chargerlock)
        {
            chargerlock = true;
            Serial.println("Charger pluged in");
            for (int i = 0; i < strip_1.numPixels(); i++)
            {
                strip_1.setPixelColor(i, (0, 0, 0));
                strip_1.show();
                delay(5);
            }
        }
        sensorValue = analogRead(A0);
    }

    if (chargerlock)
    {
        Serial.println("Charger pluged out");
        chargerlock = false;
        loop();
    }
    return true;
}
