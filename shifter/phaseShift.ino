#include <Math.h>
#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SPI.h>
#include "TRNT_canavan.h"

// GPS SETUP ------------------------------------------------------------------

#define GPSSerial Serial1

Adafruit_GPS GPS(&GPSSerial);

#define GPSECHO false

uint32_t timer = millis();


// TRNT SETUP -----------------------------------------------------------------

float tx_coords[TRNT_N_COORDS][2] = TRNT_COORDS;
int   tx_angles[TRNT_N_COORDS] = TRNT_ANGLES;

int   phase_angles[TRNT_N_ANTENNAS];

// MAGNETOMETER SETUP ---------------------------------------------------------

Adafruit_BNO055 bno = Adafruit_BNO055(55);

// SETUP FUNCTION -------------------------------------------------------------

void setup() {
  // spi init
  Serial.begin(115200);

  for (int n = 24; n < 32; n++)
    pinMode(n, OUTPUT);

  SPI.begin();

  SPI.setBitOrder(MSBFIRST);

  // gps init
  GPS.begin(9600);

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  Serial.println("Orientation Sensor Test");

  // mag init
  // ! may not work, needs to be tested
  if(!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  bno.setExtCrystalUse(true);
}

// MAIN LOOP ------------------------------------------------------------------

unsigned long prev_millis = 0;
unsigned long interval = 1000; // 1 second interval
int curr_tx_ang;


void loop() {
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(41, HIGH);

  unsigned long curr_millis = millis();

  if (curr_millis - prev_millis >= interval) {
    curr_tx_ang = scanTRNT(GPS.latitudeDegrees, GPS.longitudeDegrees);
    prev_millis = curr_millis;
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

  for (int i = 0; i < 8; i++) {
    digitalWrite(i + 24, LOW);
    SPI.transfer(phase_angles[i]);
    digitalWrite(i + 24, HIGH);
  }
  
  // ! confirm functional
  Serial.print(mag.x());
  Serial.print(mag.y());
  Serial.print(mag.z());
}

// FUNCTIONS ------------------------------------------------------------------

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

void readGPS() {
  char c = GPS.read();

  if ((c) && (GPSECHO)) 
    Serial.write(c);

  // check for updates
  if (GPS.newNMEAreceived()) {
    
     if (!GPS.parse(GPS.lastNMEA()))  
      return;                        

    // print current stats
    if (millis() - timer > 2000) {
    timer = millis();  
      if (GPS.fix) {
        Serial.print(GPS.lat);
        Serial.print(", ");
        Serial.println(GPS.lon);
      } else {
        Serial.println("Getting Fix");
      }
    }
  }
}

void printEvent(sensors_event_t* event) {
  double x = -1000000, y = -1000000, z = -1000000;  
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    Serial.print("Accl:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  } 
  else if (event->type == SENSOR_TYPE_ORIENTATION) {
    Serial.print("Orient:");
    x = event->orientation.x;
    y = event->orientation.y;
    z = event->orientation.z;
  } 
  else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD) {
    Serial.print("Mag:");
    x = event->magnetic.x;
    y = event->magnetic.y;
    z = event->magnetic.z;
  } 
  else if (event->type == SENSOR_TYPE_GYROSCOPE) {
    Serial.print("Gyro:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  } 
  else if (event->type == SENSOR_TYPE_ROTATION_VECTOR) {
    Serial.print("Rot:");
    x = event->gyro.x;
    y = event->gyro.y;
    z = event->gyro.z;
  } 
  else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION) {
    Serial.print("Linear:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  } 
  else if (event->type == SENSOR_TYPE_GRAVITY) {
    Serial.print("Gravity:");
    x = event->acceleration.x;
    y = event->acceleration.y;
    z = event->acceleration.z;
  } 
  else {
    Serial.print("Unk:");
  }

  Serial.print("\tx= ");
  Serial.print(x);
  Serial.print(" |\ty= ");
  Serial.print(y);
  Serial.print(" |\tz= ");
  Serial.println(z);
}


