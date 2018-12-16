
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// Update these with values suitable for your network.

char* ssid = "SSID-NAME";
char* password = "SSID-PASSWORD";
char* mqtt_server = "192.168.2.2";  //8 espbroker idi
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[1023];
String smsg="";
int value = 0;
char msgm[1023];
int SDread=0;
  String ssidtmp="";
  String sspastmp="";
  String mqtttmp="";
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    //Serial.println(mqtt_server);
    //Serial.println(ssid);
    //Serial.println(password);
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  //Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Cli-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      //Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("/HOME/KEP/OY", "KEPÇE");
      // ... and resubscribe
      client.subscribe("/HOME/KEP/KU");
    } else {
     //Serial.print("failed, rc=");
     // Serial.print(client.state());
     // Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(3000);
    }
  }
}

void setup() {
  //pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
 Serial.begin(19200);
 delay(500);
  while(SDread==0){
  char chr;
  if(Serial.available()>0)
  {
      if(SDread==0){chr=Serial.read();
                    smsg+=chr;
                    }
      
      if(chr=='<'){
          smsg=smsg.substring(smsg.indexOf(">")-1,smsg.length());
          if(smsg.indexOf(">ssid=")>-1){
            ssidtmp=smsg.substring(smsg.indexOf(">ssid=")+6,smsg.length());
            ssidtmp=ssidtmp.substring(0,ssidtmp.indexOf(";"));
            ssidtmp.toCharArray(ssid, ssidtmp.length()+1);
            //Serial.println(ssid);
          }

          if(smsg.indexOf(">sspas=")>-1){
            sspastmp=smsg.substring(smsg.indexOf(">sspas=")+7,smsg.length());
            sspastmp=sspastmp.substring(0,sspastmp.indexOf(";"));
            sspastmp.toCharArray(password, sspastmp.length()+1);
            //Serial.println(password);
          }

          if(smsg.indexOf(">mqttip=")>-1){
            mqtttmp=smsg.substring(smsg.indexOf(">mqttip=")+8,smsg.length());
            mqtttmp=mqtttmp.substring(0,mqtttmp.indexOf(";"));
            mqtttmp.toCharArray(mqtt_server, mqtttmp.length()+1);
            //Serial.println(mqtt_server);
          }
            SDread=1;


      }
  }
}

   // Serial.println(smsg);
    //Serial.println(SDread);        
    //Serial.println(mqtt_server);
    //Serial.println(ssid);
    //Serial.println(password);
  delay(250);  
  setup_wifi();
  smsg="";

  delay(100);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
    SDread=1;
    Serial.print("<<");
    delay(200);
    Serial.print("<<");
}

void loop() {

if(SDread==1){
  if(Serial.available()>0)
  {
    char chr=Serial.read();
    msgm[value]=chr;
    ++value;
    if(chr=='|'){ 
    snprintf (msg, value+1, msgm , value+1);
              //Serial.print("Publish message: ");
              //Serial.println(msgm);
    client.publish("/HOME/KEP/OY", msg);
    value=0;
    }
  }
}
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

