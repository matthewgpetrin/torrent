#include <Math.h>
#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SPI.h>
#include "TRNT_canavan.h"

// GPS SETUP -----------------------------------------
#define GPSSerial Serial1

Adafruit_GPS GPS(&GPSSerial);

#define GPSECHO false

uint32_t timer = millis();


// TRNT SETUP ----------------------------------------
float tx_coords[TRNT_N_COORDS][2] = TRNT_COORDS;
int tx_angles[TRNT_N_COORDS] = TRNT_ANGLES;

int phase_angles[TRNT_N_ANTENNAS];

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  // SPI INIT
  Serial.begin(115200);

  for (int n = 24; n < 32; n++)
    pinMode(n, OUTPUT);

  SPI.begin();

  SPI.setBitOrder(MSBFIRST);


  //GPS INIT
  GPS.begin(9600);

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  Serial.println("Orientation Sensor Test");


  /* Initialise the sensor */
  //if(!bno.begin())
  //{
  /* There was a problem detecting the BNO055 ... check your connections */
  //Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
  //while(1);
  //}
  for(int i = 0; i < TRNT_N_COORDS; i++){
    Serial.print(tx_coords[i][0], 6);
    Serial.print(", ");
    Serial.println(tx_coords[i][1], 6);
  }

  delay(1000);

  bno.setExtCrystalUse(true);
}

unsigned long previousMillis = 0;
unsigned long interval = 1000; // 1 second interval
int curr_tx_ang;

void loop() {
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(41, HIGH);
  

  unsigned long currentMillis = millis();

  // ChatGPT says this is a non-interfering delay? Idk but angle would always default without it
  if (currentMillis - previousMillis >= interval) {
    curr_tx_ang = scanTRNT(GPS.latitudeDegrees, GPS.longitudeDegrees);
    previousMillis = currentMillis;
  }

  readGPS();
  updatePhases(curr_tx_ang);

  Serial.print("\nlat: ");
  Serial.print(GPS.latitudeDegrees, 6);
  Serial.print(", lon: ");
  Serial.print(GPS.longitudeDegrees, 6);
  Serial.print(", ang: ");
  Serial.println(curr_tx_ang);

  imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);


// Nothing works in here
  if (Serial.available()) {
    //updatePhases(curr_tx_ang);

    // Serial.print("\nlat: ");
    // Serial.print(GPS.latitudeDegrees, 6);
    // Serial.print(", lon: ");
    // Serial.print(GPS.longitudeDegrees, 6);
    // Serial.print(", ang: ");
    // Serial.println(curr_tx_ang);
  }

  for (int i = 0; i < 8; i++) {
    digitalWrite(i + 24, LOW);
    SPI.transfer(phase_angles[i]);
    digitalWrite(i + 24, HIGH);
  }
  

  //Serial.print(mag.x());
  //Serial.print(mag.y());
  //Serial.print(mag.z());
}

void readGPS() {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if ((c) && (GPSECHO)) {
    Serial.write(c);
  }
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))  // this also sets the newNMEAreceived() flag to false
      return;                        // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis();  // reset the timer
    if (GPS.fix) {
      Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.println(GPS.lon);
    } else {
      Serial.println("Getting Fix");
    }
  }
}

void printEvent(sensors_event_t* event) {
  double x = -1000000, y = -1000000, z = -1000000;  //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    Serial.print("Accl:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  } else if (event->type == SENSOR_TYPE_ORIENTATION) {
    Serial.print("Orient:");
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  } else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    Serial.print("Mag:");
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  } else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    Serial.print("Gyro:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  } else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    Serial.print("Rot:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  } else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    Serial.print("Linear:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  } else if (event->type == SENSOR_TYPE_GRAVITY) {
    Serial.print("Gravity:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  } else {
    Serial.print("Unk:");
  }

  Serial.print("\tx= ");
  Serial.print(x);
  Serial.print(" |\ty= ");
  Serial.print(y);
  Serial.print(" |\tz= ");
  Serial.println(z);
}

int scanTRNT(float lat, float lon) {
  int idx = -1;
  for (int i = 0; i < TRNT_N_COORDS; i++) {
    float lat_i = tx_coords[i][0];
    float lon_i = tx_coords[i][1];
    // Serial.print(lat_idx);
    // Serial.print(", ");
    // Serial.println(lon_idx);
    if (lat > (lat_i - TRNT_RESOLUTION) &&
        lat < (lat_i + TRNT_RESOLUTION) && 
        lon > (lon_i - TRNT_RESOLUTION) && 
        lon < (lon_i + TRNT_RESOLUTION)) {
      idx = i;
    }
  }

  if (idx > -1) {
    Serial.println(idx);
    return tx_angles[idx];
  } else {
    return -35;
  }
}

float decimalDeg(float gps_ang){
  int degrees = gps_ang / 100;
  float minutes = gps_ang - (degrees * 100);

  return degrees + (minutes / 60);
}

void updatePhases(int angle) {
  float start_angle = PI + (PI / TRNT_N_ANTENNAS);

  float position[TRNT_N_ANTENNAS][3];
  for (int i = 0; i < TRNT_N_ANTENNAS; i++) {

    float position[3] = {
      TRNT_DIAMETER / 2 * cos(start_angle + (i * 2 * PI / TRNT_N_ANTENNAS)),
      TRNT_DIAMETER / 2 * sin(start_angle + (i * 2 * PI / TRNT_N_ANTENNAS)),
      0
    };

    float direction[3] = { -cos(angle), -sin(angle), 0 };

    float tau = 0;
    for (int i = 0; i < 3; i++) {
      tau += position[i] * direction[i];
    }
    tau /= 3e8;

    phase_angles[i] = tau * 360 * TRNT_FREQUENCY * PI / 180;
  }
}
