#include <ArduinoBLE.h>

//----------------------------------------------------------------------------------------------------------------------
// BLE UUIDs
//----------------------------------------------------------------------------------------------------------------------
#include <Servo.h>


# define Op 7
# define LED 4
# define Po A0
# define Fu 9

boolean y = 0;

Servo myservoR;  // create servo object to control a servo
Servo myservoL;

// https://www.bluetooth.com/specifications/gatt/services//
// https://www.bluetooth.com/specifications/gatt/characteristics/

#define BLE_UUID_ANGLE_SERVICE                  "180F"
#define BLE_UUID_ANGLE_LEVEL                    "2A19"

#define BLE_MAX_PERIPHERALS 2
#define BLE_SCAN_INTERVALL 10000

BLEDevice peripherals[BLE_MAX_PERIPHERALS];
BLECharacteristic angleLevelCharacteristics[BLE_MAX_PERIPHERALS];
int peripheralsConnected = 0;

const int BLE_LED_PIN = LED_BUILTIN;
const int BLE_SCAN_LED_PIN = LED_BUILTIN;

void setup()
{
  Serial.begin( 9600 );
  //while ( !Serial );

  pinMode( BLE_SCAN_LED_PIN, OUTPUT );

  BLE.begin();

  digitalWrite( BLE_SCAN_LED_PIN, HIGH );
  BLE.scanForUuid( BLE_UUID_ANGLE_SERVICE );

  int peripheralCounter = 0;
  unsigned long startMillis = millis();
  while ( millis() - startMillis < BLE_SCAN_INTERVALL && peripheralCounter < BLE_MAX_PERIPHERALS )
  {
    BLEDevice peripheral = BLE.available();

    if ( peripheral )
    {
      if ( peripheral.localName() == "angleMonitor" )
      {
        boolean peripheralAlreadyFound = false;
        for ( int i = 0; i < peripheralCounter; i++ )
        {
          if ( peripheral.address() == peripherals[i].address() )
          {
            peripheralAlreadyFound = true;
          }
        }
        if ( !peripheralAlreadyFound )
        {
          peripherals[peripheralCounter] = peripheral;
          peripheralCounter++;
        }
      }
    }
  }

  BLE.stopScan();
  digitalWrite( BLE_SCAN_LED_PIN, LOW );

  for ( int i = 0; i < peripheralCounter; i++ )
  {
    peripherals[i].connect();
    peripherals[i].discoverAttributes();
    BLECharacteristic angleLevelCharacteristic = peripherals[i].characteristic( BLE_UUID_ANGLE_LEVEL );
    if ( angleLevelCharacteristic )
    {
      angleLevelCharacteristics[i] = angleLevelCharacteristic;
      angleLevelCharacteristics[i].subscribe();
    }
  }
  peripheralsConnected = peripheralCounter;
  ///-----------------------------------------------------------------------
  myservoR.attach(2);
  myservoL.attach(3);
  pinMode(Op, INPUT_PULLUP);
  pinMode(Fu, INPUT);
  pinMode(Po, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  myservoR.write(65);
  myservoL.write(0);

  ///-----------------------------------------------------------------------
}

void loop()
{
  int angleLevel_int;
  uint8_t angleLevels[BLE_MAX_PERIPHERALS];
  bool newDataPrint = false;
  for ( int i = 0; i < peripheralsConnected; i++ )
  {
    if ( angleLevelCharacteristics[i].valueUpdated() )
    {
      newDataPrint = true;
      uint8_t angleLevel;
      angleLevelCharacteristics[i].readValue(angleLevel );
      angleLevel = angleLevels[i];
    }
  }

  if ( newDataPrint )
  {
    for ( int i = 0; i < peripheralsConnected; i++ )
    {
      uint8_t angleLevel;
      angleLevel = angleLevels[i];
      if (angleLevel > 90 ) {
        angleLevel_int = -256 + angleLevel;
      }
      else
        angleLevel_int = angleLevel;
      Serial.print(angleLevel_int);
      //Serial.print( "," );
    }
    
  }
  if (digitalRead(Fu) == HIGH) {
    if (digitalRead(Op) == HIGH)
    {
      y = !y;
      while (digitalRead(Op) == HIGH);
      delay(250);
    }
    if (y == 1)
    {
      myservoR.write(0);
      myservoL.write(65);
      digitalWrite(LED, LOW);
    }
    else
    {
      myservoR.write(65);
      myservoL.write(0);
      digitalWrite(LED, HIGH);
    }
    Serial.println("manual");
  }
  else {
    int Povalue = analogRead(Po);
    Povalue = map(Povalue, 0, 1023, -20, 90);
    if (-10 <= Povalue) {
      
 Serial.println("auto");
      myservoR.write(65);
      myservoL.write(0);
      digitalWrite(LED, HIGH);
    }
    else {
      
      myservoR.write(0);
      myservoL.write(65);
      digitalWrite(LED, LOW);
    }
    Serial.print( "\n" );
  }
  //--------------------------------------------------------------------
  
}
