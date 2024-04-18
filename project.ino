#include<SoftwareSerial.h>
SoftwareSerial mySerial(2,3);
#include <TinyGPS.h>
TinyGPS gps;
float flat=0, flon=0;
bool locked=false;
bool tracking=false;
float gLat=0; 
float gLon=0;
const double gRadius = 25;

void read_gps()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (mySerial.available())
    {
      char c = mySerial.read();
      if (gps.encode(c)) 
        newData = true;
    }
  }

  if (newData)
  {
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
  }
}




int m1=4;
int m2=5;
int m3=6;
int m4=7;



void setup() {
  Serial.begin(9600); 
mySerial.begin(9600);
pinMode(8,OUTPUT);
 
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(m3,OUTPUT);
  pinMode(m4,OUTPUT);
 
  digitalWrite(m1,0);
  digitalWrite(m2,0);
  digitalWrite(m3,0);
  digitalWrite(m4,0);

  delay(2000);

}

void loop() {
  if (tracking) {
      read_gps();
    if (!isWithinGeofence(flat,flon)) {
      digitalWrite(8,1);
      Serial.println("ALERT: Vehicle Theft");
      locked=true;
      delay(2000);
    }
  }
if(Serial.available())
{
  int x=Serial.read();
  if(!locked){
    if(x=='1')
    {
     Serial.println("Moving Front");
  digitalWrite(m1,1);
  digitalWrite(m2,0);
  digitalWrite(m3,1);
  digitalWrite(m4,0);
    }

     if(x=='2')
    {
             Serial.println("Moving BACK");
        digitalWrite(m1,0);
  digitalWrite(m2,1);
  digitalWrite(m3,0);
  digitalWrite(m4,1);
    }

     if(x=='3')
    {
       Serial.println("Moving LEFT");
        digitalWrite(m1,1);
  digitalWrite(m2,0);
  digitalWrite(m3,0);
  digitalWrite(m4,1);
    }

     if(x=='4')
    {
       Serial.println("Moving RIGHT");
             
        digitalWrite(m1,0);
  digitalWrite(m2,1);
  digitalWrite(m3,1);
  digitalWrite(m4,0);
    }
  }
     if(x=='5')
    {
               Serial.println("STOP");
        digitalWrite(m1,0);
  digitalWrite(m2,0);
  digitalWrite(m3,0);
  digitalWrite(m4,0);
    }
    if(x=='6'){
      read_gps();
      Serial.println("Vehicle location: http://maps.google.com/maps?q=loc:"+ String(flat, 6) + "," + String(flon, 6));
    }
    if(x=='7'){
      locked=true;
      Serial.println("LOCKED");

    }
    if(x=='8'){
      locked=false;
      Serial.println("UNLOCKED");

    }
    if(x=='0'){
      tracking=true;
      Serial.println("Theft mode ON");
      read_gps();
      gLat=float(flat);
      gLon=float(flon);
    }
    if(x=='9'){
      tracking=false;
      Serial.println("Theft mode OFF");
      digitalWrite(8,0);
    }

 }


}
bool isWithinGeofence(float flat, float flon) {
  double distanceMeters = distanceBetweenPoints(flat, flon, gLat, gLon);
  return distanceMeters <= gRadius;
}

// Function to calculate the distance between two GPS coordinates using the Haversine formula
double distanceBetweenPoints(double lat1, double lon1, double lat2, double lon2) {
  // Haversine formula
  double dLat = radians(lat2 - lat1);
  double dLon = radians(lon2 - lon1);
  double a = sin(dLat / 2) * sin(dLat / 2) + cos(radians(lat1)) * cos(radians(lat2)) * sin(dLon / 2) * sin(dLon / 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double distance = 6371000 * c; // Radius of Earth in meters (approx.)
  return distance;
}