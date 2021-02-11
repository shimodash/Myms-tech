/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

/*
   Sample note on/off send to MIDI Device
 */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

static BLERemoteCharacteristic* pRemoteCharacteristic;
//static BLECharacteristic* pRemoteCharacteristic;

#define MIDI_SERVICE_UUID        "03b80e5a-ede8-4b33-a751-6ce34ec4c700"
#define MIDI_CHARACTERISTIC_UUID "7772e5db-3868-4112-a1a9-f2669d106bf3"
static BLEUUID serviceUUID(MIDI_SERVICE_UUID);    // BLE MIDI Service UUID
static BLEUUID charUUID   (MIDI_CHARACTERISTIC_UUID);    // BLE MIDI Characteristic UUID
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;


uint8_t midiPacket[] = {
   0x80,  // header
   0x80,  // timestamp, not implemented 
   0x00,  // status
   0x3c,  // 0x3c == 60 == middle c
   0x00   // velocity
};

int scanTime = 5; //In seconds
BLEScan* pBLEScan;
static BLEAdvertisedDevice* myDevice;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        Serial.printf("Advertised MIDI Device: %s \n", advertisedDevice.toString().c_str());
      }
    }
};


//*************************************************************************************************
class MyClientCallback : public BLEClientCallbacks {
 void onConnect(BLEClient* pclient) {
   Serial.println("**** onConnect ****");    
 }

 void onDisconnect(BLEClient* pclient) {
   connected = false;
   Serial.println("onDisconnect");
 }
};
//*************************************************************************************************
bool connectToServer() {
   
   BLEClient*  pClient  = BLEDevice::createClient();   

   pClient->setClientCallbacks(new MyClientCallback());

   // Connect to the remove BLE Server.
   pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)

   // Obtain a reference to the service we are after in the remote BLE server.
   BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
   if (pRemoteService == nullptr) {
     Serial.print("Failed to find our service UUID: ");
     Serial.println(serviceUUID.toString().c_str());
     pClient->disconnect();
     return false;
   }
   Serial.printf(" -Connect CN29\n");    

   pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
   if (pRemoteCharacteristic == nullptr) { pClient->disconnect(); return false; } 

}

int loop_cnt = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
//  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory

  if (connectToServer()) {
       Serial.printf("CN29 Connection OK\n");
     } else {
       Serial.printf("CN29 can't connect\n");
     } 

}

void loop() {

  note_on_off_waon(60,50);
//  note_on_off(60,50);
  delay(500);
  note_on_off(62,50);
  delay(500);
  note_on_off(64,50);
  delay(500);
  note_on_off(65,50);
  delay(500);
  note_on_off(67,50);
  delay(500);
  note_on_off(65,50);
  delay(500);
  note_on_off(64,50);
  delay(500);
  note_on_off(62,50);
  delay(500);
  if (loop_cnt > 10){
    while(1);
  }

  ++loop_cnt;
}

void note_on_off(int code,int velocity){
  midiPacket[2] = 0x90; // note on, channel 0
//  midiPacket[3] = buttonConfig[i].noteNo;
  midiPacket[3] = code;
  midiPacket[4] = velocity;  // velocity

  pRemoteCharacteristic->writeValue(midiPacket, 5); // packet, length in bytes
//  pRemoteCharacteristic->canNotify();
  delay(300);

  
  midiPacket[2] = 0x80; // note off, channel 0
  midiPacket[3] = code;
  midiPacket[4] = velocity;  // velocity

  pRemoteCharacteristic->writeValue(midiPacket, 5); // packet, length in bytes
//  pRemoteCharacteristic->canNotify();


}

void note_on_off_waon(int code,int velocity){
  midiPacket[2] = 0x90; // note on, channel 0
//  midiPacket[3] = buttonConfig[i].noteNo;
  midiPacket[3] = code;
  midiPacket[4] = velocity;  // velocity
  pRemoteCharacteristic->writeValue(midiPacket, 5); // packet, length in bytes

  midiPacket[2] = 0x90; // note on, channel 0
//  midiPacket[3] = buttonConfig[i].noteNo;
  midiPacket[3] += 4;
  midiPacket[4] = velocity;  // velocity
  pRemoteCharacteristic->writeValue(midiPacket, 5); // packet, length in bytes

  midiPacket[2] = 0x90; // note on, channel 0
//  midiPacket[3] = buttonConfig[i].noteNo;
  midiPacket[3] += 3;
  midiPacket[4] = velocity;  // velocity
  pRemoteCharacteristic->writeValue(midiPacket, 5); // packet, length in bytes
//  pRemoteCharacteristic->canNotify();
  delay(300);

  
  midiPacket[2] = 0x80; // note off, channel 0
  midiPacket[3] = code;
  midiPacket[4] = velocity;  // velocity
  pRemoteCharacteristic->writeValue(midiPacket, 5); // packet, length in bytes
//  pRemoteCharacteristic->canNotify();
  midiPacket[2] = 0x80; // note off, channel 0
//  midiPacket[3] = buttonConfig[i].noteNo;
  midiPacket[3] += 4;
  midiPacket[4] = velocity;  // velocity
  pRemoteCharacteristic->writeValue(midiPacket, 5); // packet, length in bytes

  midiPacket[2] = 0x80; // note off, channel 0
//  midiPacket[3] = buttonConfig[i].noteNo;
  midiPacket[3] += 3;
  midiPacket[4] = velocity;  // velocity
  pRemoteCharacteristic->writeValue(midiPacket, 5); // packet, length in bytes
//  pRemoteCharacteristic->canNotify();


}
