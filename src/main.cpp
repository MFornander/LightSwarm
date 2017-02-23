#include <SPI.h>
#include <RH_RF95.h>
#include <Arduino.h>

RH_RF95 rf95(8, 3); // Rocket Scream Mini Ultra Pro with the RFM95W
//#define Serial SerialUSB

const bool s_serverFlag = (SERVER == 1);
int s_messageCount = 0;


void blink(int count = 1, int onTime = 100, int offTime = 900)
{
  for (int i=0; i<count; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(onTime);
    digitalWrite(LED_BUILTIN, LOW);
    delay(offTime);
  }
}


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
    digitalWrite(LED_BUILTIN, HIGH);
    return;
  }

  //rf95.setFrequency(915.0);
  rf95.setModemConfig(RH_RF95::ModemConfigChoice::Bw31_25Cr48Sf512);
  rf95.setTxPower(23);
  blink();

  if (s_serverFlag)
  {
    Serial.println("Server");
    blink(10, 100, 100);
  }
  else
  {
    Serial.println("Client");
  }
}

void loop()
{
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (s_serverFlag)
  {
    sprintf(reinterpret_cast<char*>(buf), "Woop! #%d", s_messageCount++);
    rf95.send(buf, 32);
    rf95.waitPacketSent();
    Serial.println("Sent a Woop!");

    blink(1, 100, 1900);
  }
  else //client
  {


    if (rf95.waitAvailableTimeout(4000))
    {
      // Should be a reply message for us now
      if (rf95.recv(buf, &len))
      {
        Serial.print("Got message: ");
        Serial.print((char*)buf);
        Serial.print("   RSSI: ");
        Serial.print(rf95.lastRssi(), DEC);
        Serial.print("   SNR: ");
        Serial.println(rf95.lastSNR());

        blink(2, 100, 100);
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
