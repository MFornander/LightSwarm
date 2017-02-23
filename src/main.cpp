#include <SPI.h>
#include <RH_RF95.h>
#include <Arduino.h>

RH_RF95 rf95(8, 3); // Rocket Scream Mini Ultra Pro with the RFM95W
//#define Serial SerialUSB

const bool server = (SERVER == 1);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Ensure serial flash is not interfering with radio communication on SPI bus
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  Serial.begin(9600);
  //while (!Serial) ; // Wait for serial port to be available

  if (!rf95.init())
  {

    Serial.println("Init failed");
    return;
  }

  //rf95.setFrequency(915.0);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);

  if (server)
  {
    Serial.println("Server");
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
  }
  else
  {
    Serial.println("Client");
  }
}

void loop()
{

  if (server)
  {
    digitalWrite(LED_BUILTIN, HIGH);

    uint8_t data[] = "Woop!";
    rf95.send(data, sizeof(data));
    rf95.waitPacketSent();
    Serial.println("Sent a Woop!");

    delay(100);
    digitalWrite(LED_BUILTIN, LOW);

    delay(2000);
  }
  else //client
  {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.waitAvailableTimeout(4000))
    {
      // Should be a reply message for us now
      if (rf95.recv(buf, &len))
      {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.print("Got message: ");
        Serial.println((char*)buf);
        Serial.print("RSSI: ");
        Serial.println(rf95.lastRssi(), DEC);

        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
      }
      else
      {
        Serial.println("recv failed");
      }
    }
    else
    {
      Serial.println("No message, is server running?");
    }
  }

}
