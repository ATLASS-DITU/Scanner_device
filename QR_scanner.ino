#include <Arduino.h>
#include <ESP32QRCodeReader.h>

ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);

#define red 12
#define green 13
#define buttonInput 4
#define white 2
int Input = 0;

const char *url = "";

void onQrCodeTask(void *pvParameters)
{
  struct QRCodeData qrCodeData;

  while (true)
  {
    if (reader.receiveQrCode(&qrCodeData, 100))
    {
      if (qrCodeData.valid)
      {
        url = (const char *)qrCodeData.payload;
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  reader.setup();

  Serial.println("Setup QRCode Reader");

  reader.beginOnCore(1);

  Serial.println("Begin on Core 1");

  xTaskCreate(onQrCodeTask, "onQrCode", 4 * 1024, NULL, 4, NULL);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buttonInput, INPUT);
  pinMode(white, OUTPUT);
  
  digitalWrite(red, LOW);
  delay(1000);
  digitalWrite(red, HIGH);
  delay(1000);
  
  digitalWrite(green, HIGH);
  delay(1000);
  digitalWrite(green, LOW);
  delay(1000);
  digitalWrite(white, LOW);  
}

void loop()
{
  digitalWrite(red, HIGH);
  delay(1000);


  Input = digitalRead(buttonInput);
  if(Input == HIGH)
  {
    Serial.print("Input Detected\n");
    digitalWrite(green, HIGH);
    if(url == "")
    {}
    else
    {
        Serial.println(url);
        digitalWrite(white, HIGH);   
    }
    delay(100000);
  }
  digitalWrite(green, LOW);
  digitalWrite(white, LOW);
  url = "";
}
