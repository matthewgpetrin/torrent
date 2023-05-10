#include <Math.h>
#include <Arduino.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SPI.h>
#include "trnt_data.h"


SoftwareSerial mySerial(23,22);
Adafruit_GPS GPS(&mySerial);

#define GPSECHO  true

uint32_t timer = millis();
int angles[TRNT_N_ANTENNAS];

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  Serial.begin(115200);
  for(int n=24; n<32;n++) {
    pinMode (n,OUTPUT);
  }

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);


  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); 
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  //if(!bno.begin())
  //{
    /* There was a problem detecting the BNO055 ... check your connections */
    //Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    //while(1);
  //}
  
  delay(1000);
    
  bno.setExtCrystalUse(true);
  
}

void loop() {
  digitalWrite(24,HIGH);
  digitalWrite(25,HIGH);
  digitalWrite(26,HIGH);
  digitalWrite(27,HIGH);
  digitalWrite(41,HIGH);

  if (Serial.available()>0) {
    String button = Serial.readString();
    readGPS();
    imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

    int ang = scanTRNT(GPS.lat, GPS.lon);
    updatePhases(ang);

    Serial.print(GPS.lat);
    Serial.print(GPS.lon);
    Serial.print(ang);
  }

  readGPS();
  imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

  for(int i =0 ;i<8;i++) {
   digitalWrite(i+24,LOW);
   SPI.transfer(angles[i]);
   digitalWrite(i+24,HIGH);
  }

  Serial.print(mag.x());
  Serial.print(mag.y());
  Serial.print(mag.z());
}

void readGPS(){
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if ((c) && (GPSECHO)){
    Serial.write(c);
  }
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
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
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
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

int scanTRNT(float lat, float lon) {
  int idx = -1;
  for (int i = 0; i < TRNT_N_COORDS; i++){
    float coord[2] = trnt::coords[i];
    if (coord[0] > (lat - TRNT_ERROR) &&
        coord[0] < (lat + TRNT_ERROR) &&
        coord[1] < (lon - TRNT_ERROR) &&
        coord[1] > (lon + TRNT_ERROR)){
      idx = i;
    }
  }

  if(idx > -1) {
    return idx;
  }
  else {
    return 0;
  }  
}

void updatePhases(int angle){
  float start_angle = PI + (PI / TRNT_N_ANTENNAS);

  float position[TRNT_N_ANTENNAS][3];
  for(int i = 0; i < TRNT_N_ANTENNAS; i++){
    
    float position[3] = {
      TRNT_DIAMETER / 2 * cos(start_angle + (i * 2 * PI / TRNT_N_ANTENNAS)),
      TRNT_DIAMETER / 2 * sin(start_angle + (i * 2 * PI / TRNT_N_ANTENNAS)),
      0};

    float direction[3] = {-cos(angle), -sin(angle), 0};

    float tau = 0;
    for(int i = 0; i < 3; i++){
      tau += position[i] * direction[i];
    }
    tau /= 3e8;
    
    angles[i] = tau * 360 * TRNT_FREQUENCY * PI / 180;
  }
}
