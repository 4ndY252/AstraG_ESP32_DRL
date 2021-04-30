#include <FastLED.h>
#define NUM_LEDS 8
CRGBArray<NUM_LEDS> ledsL;
CRGBArray<NUM_LEDS> ledsR;

#include <WiFi.h>

const char *ssid = "ESP32";
const char *password = "258058Ab";

const int pinLeft = 25;
const int pinRight = 33;

WiFiServer server(80);
String header;
int turnSpeed = 35.5; //rychlost animacie smerovky - musi byt synchronizovana so smerovkou na aute
int turnFreeze = 37;

void setup()
{
  pinMode(pinLeft, INPUT_PULLDOWN); // musi byt PULLDOWN, pretoze napatie nikdy nie je presne 0V alebo 3.3V -> nechcene spustanie animacie
  pinMode(pinRight, INPUT_PULLDOWN);
  xTaskCreatePinnedToCore( // definicia kde ma co bezat - LED animacie jadro 2, WiFi 1
      taskLED,
      "taskLED",
      10000,
      NULL,
      0,
      NULL,
      1);

  xTaskCreatePinnedToCore(
      taskWifi,
      "taskWifi",
      10000,
      NULL,
      0,
      NULL,
      0);

  FastLED.addLeds<NEOPIXEL, 14>(ledsL, NUM_LEDS).setCorrection(TypicalSMD5050); // korekcia farieb, bez toho svietia LEDky modrym odtienom
  FastLED.addLeds<NEOPIXEL, 13>(ledsR, NUM_LEDS).setCorrection(TypicalSMD5050);

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();

  server.begin();
}

void startUp()
{ // postupne rozsvietenie pri zapnuti
  FastLED.setBrightness(0);
  fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::White);
  fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::White);
  for (int i = 0; i <= 255; i = i + 5)
  {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(50);
  }
}
void signal()
{ // animacia smeroviek, TO DO: switch namiesto if
  int turnLeft = digitalRead(pinLeft);
  int turnRight = digitalRead(pinRight);

  if (turnLeft == HIGH && turnRight == HIGH)
  { // vystrazne
    for (int i = NUM_LEDS - 1, j = 0; i >= 0 && j <= NUM_LEDS - 1; i--, j++)
    {
      ledsR[j] = CRGB(255, 50, 0);
      ledsL[i] = CRGB(255, 50, 0);
      FastLED.show();
      if (digitalRead(pinLeft) == LOW && digitalRead(pinRight == LOW))
      {
        break;
      }
      FastLED.delay(turnSpeed);
    }
delay(turnFreeze);
    fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::Black);
    fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::Black);
    FastLED.show();
    for (int i = 0; i <= 350; i++)
    {
      delay(1);
      i++;
      if (digitalRead(pinLeft) == HIGH || digitalRead(pinRight) == HIGH) //ked sa opacna smerovka zapne, loop sa prerusi
      {
        break;
      }
    }
  }
  else if (turnRight == HIGH)
  {
    fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::White);
    FastLED.show();
    for (int i = 0; i <= NUM_LEDS - 1; i++)
    {
      ledsR[i] = CRGB(255, 50, 0);
      FastLED.show();
      if (digitalRead(pinRight) == LOW)
      {
        break;
      }
      FastLED.delay(turnSpeed);
    }
        delay(turnFreeze);
    fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::Black);
    FastLED.show();
    for (int i = 0; i <= 350; i++)
    {
      delay(1);
      i++;
      if (digitalRead(pinLeft) == HIGH || digitalRead(pinRight) == HIGH) //ked sa opacna smerovka zapne, loop sa prerusi
      {
        break;
      }
    }
  }
  else if (turnLeft == HIGH)
  {
    fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::White);
    FastLED.show();
    for (int i = NUM_LEDS - 1; i >= 0; i--)
    {
      ledsL[i] = CRGB(255, 50, 0);
      FastLED.show();
      FastLED.delay(turnSpeed);
      if (digitalRead(pinLeft) == LOW)
      {
        break;
      }
    }
        delay(turnFreeze);
    fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::Black);
    FastLED.show();
    for (int i = 0; i <= 350; i++)
    {
      delay(1);
      i++;
      if (digitalRead(pinRight) == HIGH || digitalRead(pinLeft) == HIGH) //ked sa opacna smerovka zapne, loop sa prerusi
      {
        break;
      }
    }
  }
  else if (turnLeft == LOW && turnRight == LOW)
  {
      fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::White);
      fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::White);
      FastLED.show();
  }
}

void white()
{ // iba biela farba
  fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::White);
  fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::White);
  FastLED.show();
}

void red()
{
  fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::Red);
  fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::Red);
  FastLED.show();
}

void green()
{
  fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::Green);
  fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::Green);
  FastLED.show();
}

void blue()
{
  fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::Blue);
  fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::Blue);
  FastLED.show();
}

void flash()
{ // funguje pri hociakej farbe, ovladanie iba jasu
  FastLED.setBrightness(0);
  FastLED.show();
  delay(100);
  FastLED.setBrightness(255);
  FastLED.show();
  delay(100);
}

void pulse()
{
  for (int i = 255; i >= 0; i = i - 5)
  {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(25);
  }
  for (int i = 0; i <= 255; i = i + 5)
  {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(25);
  }
}

bool end = false;
void knightRider()
{
  if (end == false)
  {
    for (int i = 0; i <= NUM_LEDS - 1; i++)
    {
      ledsL[i].setRGB(255, 0, 0);
      if (i > 0)
      {
        ledsL[i - 1].setRGB(100, 0, 0);
        if (i > 1)
        {
          ledsL[i - 2].setRGB(0, 0, 0);
        }
      }
      FastLED.show();
      FastLED.delay(40);
    }
    fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::Black);
    FastLED.show();

    for (int i = 0; i <= NUM_LEDS - 1; i++)
    {
      ledsR[i].setRGB(255, 0, 0);
      if (i > 0)
      {
        ledsR[i - 1].setRGB(100, 0, 0);
        if (i > 1)
        {
          ledsR[i - 2] = CRGB::Black;
        }
      }
      FastLED.show();
      FastLED.delay(40);

      if (i == NUM_LEDS - 1)
      {
        end = true;
        fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::Black);
        FastLED.show();
      }
    }
  }
  else if (end == true)
  {
    for (int i = NUM_LEDS - 1; i >= 0; i--)
    {
      ledsR[i].setRGB(255, 0, 0);
      if (i < NUM_LEDS)
      {
        ledsR[i + 1].setRGB(100, 0, 0);
        if (i < NUM_LEDS - 1)
        {
          ledsR[i + 2] = CRGB::Black;
        }
      }

      FastLED.show();
      FastLED.delay(40);
    }
    fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::Black);
    FastLED.show();
    for (int i = NUM_LEDS - 1; i >= 0; i--)
    {
      ledsL[i].setRGB(255, 0, 0);
      if (i < NUM_LEDS)
      {
        ledsL[i + 1].setRGB(100, 0, 0);
        if (i < NUM_LEDS - 1)
        {
          ledsL[i + 2] = CRGB::Black;
        }
      }
      FastLED.show();
      FastLED.delay(40);

      if (i == 0)
      {
        end = false;
        fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::Black);
        FastLED.show();
      }
    }
  }
}

void ambient()
{
  fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::OrangeRed);
  fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::OrangeRed);
  FastLED.show();
}

void redBlueFlash()
{
  for (int i = 0; i <= 2; i++)
  {
    fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::Blue);
    FastLED.show();
    FastLED.delay(50);
    fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::Black);
    FastLED.show();
    FastLED.delay(50);
  }

  delay(10);
  for (int i = 0; i <= 2; i++)
  {
    fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::Red);
    FastLED.show();
    FastLED.delay(50);
    fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::Black);
    FastLED.show();
    FastLED.delay(50);
  }
}

int rainbowHue = 0;
void rainbow()
{
  fill_rainbow(&(ledsL[0]), NUM_LEDS, rainbowHue, 10);
  fill_rainbow(&(ledsR[0]), NUM_LEDS, rainbowHue + 127, 10);
  FastLED.show();
  if (rainbowHue >= 255)
  {
    rainbowHue = 0;
  }
  else
  {
    rainbowHue = rainbowHue + 20;
  }
  delay(25);
}

int modeRGB = -1;
void taskLED(void *parameter)
{
  while (1)
  {
    switch (modeRGB)
    {
    case -1:
      startUp();
      modeRGB = 0;
      break;
    case 0:
      signal();
      break;
    case 1:
      white();
      break;
    case 2:
      red();
      break;
    case 3:
      green();
      break;
    case 4:
      blue();
      break;
    case 5:
      flash();
      break;
    case 6:
      pulse();
      break;
    case 7:
      knightRider();
      break;
    case 8:
      ambient();
      break;
    case 9:
      redBlueFlash();
      break;
    case 10:
      rainbow();
      break;
    case 11:
      fill_solid(&(ledsR[0]), NUM_LEDS, CRGB::Black);
      fill_solid(&(ledsL[0]), NUM_LEDS, CRGB::Black);
      FastLED.show();
      break;
    }
  }
  vTaskDelete(NULL);
}

void loop()
{
  while (1)
  {
    delay(1000);
  }
}
void taskWifi(void *TaskParameters_t)
{
  while (1)
  {
    WiFiClient client = server.available();
    if (client)
    {
      String currentLine = "";
      while (client.connected())
      {
        if (client.available())
        {
          char c = client.read();
          header += c;
          if (c == '\n')
          {
            if (currentLine.length() == 0)
            {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              if (header.indexOf("GET /0/") >= 0)
              {
                modeRGB = 0;
              }
              else if (header.indexOf("GET /1/") >= 0)
              {
                modeRGB = 1;
              }
              else if (header.indexOf("GET /2/") >= 0)
              {
                modeRGB = 2;
              }
              else if (header.indexOf("GET /3/") >= 0)
              {
                modeRGB = 3;
              }
              else if (header.indexOf("GET /4/") >= 0)
              {
                modeRGB = 4;
              }
              else if (header.indexOf("GET /5/") >= 0)
              {
                modeRGB = 5;
              }
              else if (header.indexOf("GET /6/") >= 0)
              {
                modeRGB = 6;
              }
              else if (header.indexOf("GET /7/") >= 0)
              {
                modeRGB = 7;
              }
              else if (header.indexOf("GET /8/") >= 0)
              {
                modeRGB = 8;
              }
              else if (header.indexOf("GET /9/") >= 0)
              {
                modeRGB = 9;
              }
              else if (header.indexOf("GET /10/") >= 0)
              {
                modeRGB = 10;
              }
              else if (header.indexOf("GET /11/") >= 0)
              {
                modeRGB = 11;
              }

              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" charset=iso-8859-2>");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println("body {background-color: #303030;}");
              client.println("h1 {color: white;}");
              client.println(".button2 {background-color: #555555;}</style></head>");

              client.println("<body style><h1>ESP32 Web Server</h1>");

              client.println("<p><a href=\"/0/\"><button class=\"button\">Default</button></a></p>");
              client.println("<p><a href=\"/1/\"><button class=\"button\">White</button></a></p>");
              client.println("<p><a href=\"/2/\"><button class=\"button\">Red</button></a></p>");
              client.println("<p><a href=\"/3/\"><button class=\"button\">Green</button></a></p>");
              client.println("<p><a href=\"/4/\"><button class=\"button\">Blue</button></a></p>");
              client.println("<p><a href=\"/5/\"><button class=\"button\">Flashing</button></a></p>");
              client.println("<p><a href=\"/6/\"><button class=\"button\">Breathing</button></a></p>");
              client.println("<p><a href=\"/7/\"><button class=\"button\">Knight Rider</button></a></p>");
              client.println("<p><a href=\"/8/\"><button class=\"button\">Ambient</button></a></p>");
              client.println("<p><a href=\"/9/\"><button class=\"button\">158</button></a></p>");
              client.println("<p><a href=\"/10/\"><button class=\"button\">Rainbow</button></a></p>");
              client.println("<p><a href=\"/11/\"><button class=\"button\">OFF</button></a></p>");

              client.println("</body></html>");
              client.println();
              break;
            }
            else
            {
              currentLine = "";
            }
          }
          else if (c != '\r')
          {
            currentLine += c;
          }
        }
      }
      header = "";
      client.stop();
    }
  }
}