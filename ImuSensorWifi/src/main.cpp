#include <Arduino.h>

#include "WiFi.h"
#include "WiFiUdp.h"
#include "MPU9250.h"
#include "madg.h"

// множитель фильтра
#define BETA 0.22

const char *ssid = "innopolis-HP";
const char *pass = "8pJMUJLH";

String ModuleName = "Module1";

IPAddress broadcastIP(192, 168, 0, 255);
IPAddress iremoteIP(192, 168, 173, 82);
int iremotePort = 4442;

WiFiUDP UDP;

// переменная для хранения частоты выборок фильтра
float fps_ = 100.0;

char packetBuffer[255];
//WiFiClient client;
unsigned int FPS = 0;

bool isConnect = false;
bool isIdent = false;

unsigned int localPort = 1234;

unsigned long SendTime = 0;
unsigned int FPSTime = 0;

struct DataIMU
{
  float AccelX;
  float AccelY;
  float AccelZ;

  float GyroX;
  float GyroY;
  float GyroZ;

  float MagX;
  float MagY;
  float MagZ;

  //float Temperature;

  float q0;
  float q1;
  float q2;
  float q3;

} dataIMU[3];

MPU9250 IMU(SPI, 5);
MPU9250 IMU2(SPI, 4);
MPU9250 IMU3(SPI, 0);

// создаём объект для фильтра Madgwick
Madgwick filter1;
Madgwick filter2;
Madgwick filter3;

MPU9250 *imu[3] = {&IMU, &IMU2, &IMU3};
Madgwick *filter[3] = {&filter1, &filter2, &filter3};

void setup()
{
  Serial.begin(115200);
  Serial.println("Start init...");

  WiFi.begin(ssid, pass);
  delay(500);

  UDP.begin(localPort);
  delay(5000);

  int status_;
  for (int i = 0; i < 3; i++)
  {
    status_ = imu[i]->begin();
    if (status_ < 0)
    {
      Serial.println("IMU initialization unsuccessful");
      Serial.println("Check IMU wiring or try cycling power");
      Serial.print("Status: ");
      Serial.println(status_);
      while (1)
      {
      }
    }
  }

  Serial.println("Init end");

  broadcastIP = (uint32_t)WiFi.localIP() | ~((uint32_t)WiFi.subnetMask());
  Serial.print(F("Broadcast address: "));
  Serial.println(broadcastIP);
}

void loop()
{

  if (WiFi.status() == WL_CONNECTED)
  {
    if (isConnect == false)
    {
      Serial.println("wifi connected");
      isConnect = true;
    }

    if (!isIdent)
    {
      int packetSize = UDP.parsePacket();
      if (packetSize)
      {
        int len = UDP.read(packetBuffer, 255);

        if (len > 0)
          packetBuffer[len] = 0;

        if (strcmp(packetBuffer, "MasterIP") == 0)
        {
          iremoteIP = UDP.remoteIP();
          isIdent = true;
        }
      }

      UDP.beginPacket(broadcastIP, iremotePort);
      UDP.print("Ident ");
      UDP.print(ModuleName);
      UDP.endPacket();

      delay(200);
    }
    else
    {
      // запоминаем текущее время
      unsigned long startMillis = millis();

      for (int i = 0; i < 3; i++)
      {
        // read the sensor
        imu[i]->readSensor();

        dataIMU[i].AccelX = imu[i]->getAccelX_mss();
        dataIMU[i].AccelY = imu[i]->getAccelY_mss();
        dataIMU[i].AccelZ = imu[i]->getAccelZ_mss();

        dataIMU[i].GyroX = imu[i]->getGyroX_rads();
        dataIMU[i].GyroY = imu[i]->getGyroY_rads();
        dataIMU[i].GyroZ = imu[i]->getGyroZ_rads();

        dataIMU[i].MagX = imu[i]->getMagX_uT();
        dataIMU[i].MagY = imu[i]->getMagY_uT();
        dataIMU[i].MagZ = imu[i]->getMagZ_uT();

        //dataIMU[i].Temperature = imu[i]->getTemperature_C();

        filter[i]->setKoeff(fps_, BETA);
        // обновляем входные данные в фильтр
        filter[i]->update(dataIMU[i].GyroX, dataIMU[i].GyroY, dataIMU[i].GyroZ, dataIMU[i].AccelX, dataIMU[i].AccelY, dataIMU[i].AccelZ, dataIMU[i].MagX, dataIMU[i].MagY, dataIMU[i].MagZ);
      }

      if (SendTime == 0)
        SendTime = micros();
      if ((micros() - SendTime) >= 500) //500
      {
        SendTime = 0;
        //FPS++;
        for (int i = 0; i < 3; i++)
          filter[i]->readQuaternions(&dataIMU[i].q0, &dataIMU[i].q1, &dataIMU[i].q2, &dataIMU[i].q3);

        UDP.beginPacket(iremoteIP, iremotePort);
        UDP.write((byte *)&dataIMU, sizeof(dataIMU));
        UDP.endPacket();
      }

      /*
      if (Serial.available() > 0)
      {
        int val = Serial.read();
        // если пришёл символ 's'
        if (val == 's')
        {
          for (int i = 0; i < 3; i++)
            filter[i]->readQuaternions(&dataIMU[i].q[0], &dataIMU[i].q[1], &dataIMU[i].q[2], &dataIMU[i].q[3]);
          // выводим кватернионы в serial-порт
          Serial.print(dataIMU[1].q[0]);
          Serial.print(",");
          Serial.print(dataIMU[1].q[1]);
          Serial.print(",");
          Serial.print(dataIMU[1].q[2]);
          Serial.print(",");
          Serial.println(dataIMU[1].q[3]);
        }
      }
      */
      /*
      if (FPSTime == 0)
        FPSTime = startMillis;
      if (startMillis - FPSTime >= 1000)
      {
        FPSTime = 0;
        Serial.print("FPS: ");
        Serial.println(FPS);
        FPS = 0;
      }
      */

      // вычисляем затраченное время на обработку данных
      unsigned long deltaMillis = millis() - startMillis;
      // вычисляем частоту обработки фильтра
      fps_ = 1000.0 / deltaMillis;
    }
  }
  else if (millis() > 60000)
    ESP.restart();

  //----------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------
  /*
  if (SendTime == 0)
    SendTime = micros();
  if ((micros() - SendTime) >= 500)
  {
    SendTime = 0;
    FPS++;
    // read the sensor
    IMU2.readSensor();
    IMU3.readSensor();
    IMU.readSensor();
  }

  // display the data
  //Serial.print(IMU2.getAccelX_mss(), 6);

  //Serial.print("\t");
  //Serial.print(IMU.getAccelX_mss(), 6);
  
  Serial.print("\t");
  Serial.print(IMU2.getAccelY_mss(), 6);
  Serial.print("\t");
  Serial.print(IMU2.getAccelZ_mss(), 6);
  Serial.print("\t");
  Serial.print(IMU2.getGyroX_rads(), 6);
  Serial.print("\t");
  Serial.print(IMU.getGyroY_rads(), 6);
  Serial.print("\t");
  Serial.print(IMU.getGyroZ_rads(), 6);
  Serial.print("\t");
  Serial.print(IMU.getMagX_uT(), 6);
  Serial.print("\t");
  Serial.print(IMU.getMagY_uT(), 6);
  Serial.print("\t");
  Serial.print(IMU.getMagZ_uT(), 6);
  Serial.print("\t");
  */
  //Serial.println(IMU.getTemperature_C(), 6);

  //delay(100);
  /*
  if (FPSTime == 0)
    FPSTime = millis();
  if (millis() - FPSTime >= 1000)
  {
    FPSTime = 0;
    Serial.print("FPS: ");
    Serial.println(FPS);
    FPS = 0;
  }
  */
}
