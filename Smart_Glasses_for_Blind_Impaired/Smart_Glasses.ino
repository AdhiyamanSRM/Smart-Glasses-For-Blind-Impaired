#include <Wire.h>
#include <MPU6050.h>

#define TRIG 12
#define ECHO 13
#define AUDIO 4

MPU6050 mpu;

long duration;
float distance;

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {

  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(AUDIO, OUTPUT);

  Wire.begin(14,15);

  Serial.println("Initializing MPU6050...");
  mpu.initialize();

  if(mpu.testConnection()){
    Serial.println("MPU6050 connected");
  }
  else{
    Serial.println("MPU6050 connection failed");
  }

}

void loop() {

  // -------- Ultrasonic --------

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);

  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if(distance < 30){

    Serial.println("Danger: object very close");

    tone(AUDIO,2000);
    delay(200);
    noTone(AUDIO);

  }
  else if(distance < 80){

    Serial.println("Warning: object ahead");

    tone(AUDIO,1200);
    delay(200);
    noTone(AUDIO);

  }

  // -------- Accelerometer --------

  mpu.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);

  Serial.print("Accel X: ");
  Serial.print(ax);
  Serial.print(" Y: ");
  Serial.print(ay);
  Serial.print(" Z: ");
  Serial.println(az);

  //  fall detection

  if(abs(ax) > 20000 || abs(ay) > 20000){

    Serial.println("Fall detected!");

    for(int i=0;i<3;i++){
      tone(AUDIO,2500);
      delay(200);
      noTone(AUDIO);
      delay(200);
    }

  }

  delay(500);

}