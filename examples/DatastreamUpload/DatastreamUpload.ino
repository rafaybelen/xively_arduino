#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Pachube.h>

// MAC address for your Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Your Pachube key to let you upload data
char pachubeKey[] = "xW0uK8RZ-SrBgxH0-wO2MguKXdGDwMFjXGiOA6EB9KQ";

// Analog pin which we're monitoring (0 and 1 are used by the Ethernet shield)
int sensorPin = 2;

// Define the strings for our datastream IDs
char sensorId[] = "sensor_reading";
Datastream datastreams[] = {
  Datastream(sensorId, strlen(sensorId), DATASTREAM_FLOAT),
};
// Finally, wrap the datastreams into a feed
PachubeFeed feed(15552, datastreams, 1 /* number of datastreams */);

EthernetClient client;
PachubeClient pachubeclient(client);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  Serial.println("Starting single datastream upload to Pachube...");
  Serial.println();

  while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  datastreams[0].setFloat(sensorValue);

  Serial.print("Read sensor value ");
  Serial.println(datastreams[0].getFloat());

  Serial.println("Uploading it to Pachube");
  int ret = pachubeclient.put(feed, pachubeKey);
  Serial.print("pachubeclient.put returned ");
  Serial.println(ret);

  Serial.println();
  delay(15000);
}
