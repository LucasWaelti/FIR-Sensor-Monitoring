// BY: Akshaya Niraula
// ON: Nov 11, 2016
// AT: http://www.embedded-lab.com/

#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"

#define LED_BUILTIN 1
#define SENSOR      2

uint8_t var = 0;
uint8_t prev = 0;

const char* ssid = "NETGEAR33";
const char* password = "aquaticfinch165";

// The ID below comes from Google Sheets.
// https://script.google.com/macros/s/-Your-WebApp-ID-/exec
// Add ?tag=MovementDetected&value=1 later in the code to URL. 
const char *GScriptId = "-Your-WebApp-ID-";

// Push data on this interval
const int dataPostDelay = 900000;     // 15 minutes = 15 * 60 * 1000

const char* host = "script.google.com";
const char* googleRedirHost = "script.googleusercontent.com";

const int httpsPort = 443;
HTTPSRedirect client(httpsPort);

// Prepare the url (without the varying data)
String url = String("/macros/s/") + GScriptId + "/exec?";

const char* fingerprint = "F0 5C 74 77 3F 6B 25 D7 3B 66 4D 43 2F 7E BC 5B E9 28 86 AD";

void setup() {
    Serial.begin(115200);
    Serial.println("Connecting to wifi: ");
    Serial.println(ssid);
    Serial.flush();

    //Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
    }
    Serial.println(" IP address: ");
    Serial.println(WiFi.localIP());

    //Connect to host
    Serial.print(String("Connecting to "));
    Serial.println(host);
  
    bool flag = false;
    for (int i=0; i<5; i++){
            int retval = client.connect(host, httpsPort);
            if (retval == 1) {
                        flag = true;
                        break;
            }
            else
                    Serial.println("Connection failed. Retrying…");
    }

    // Connection Status, 1 = Connected, 0 is not.
    Serial.println("Connection Status: " + String(client.connected()));
    Serial.flush();
    
    if (!flag){
            Serial.print("Could not connect to server: ");
            Serial.println(host);
            Serial.println("Exiting…");
            Serial.flush();
            return;
    }

    // Data will still be pushed even certification don’t match.
    if (client.verify(fingerprint, host)) {
            Serial.println("Certificate match.");
    } else {
            Serial.println("Certificate mis-match");
    }
}

// This is the main method where data gets pushed to the Google sheet
void postData(String tag)//, float value)
{
    if (!client.connected()){
            Serial.println("Connecting to client again…");
            client.connect(host, httpsPort);
    }
    String urlFinal = url + "tag=" + tag + "&value=PIR";
    client.printRedir(urlFinal, host, googleRedirHost);
}

// Main loop monitoring the sensor value
void loop() 
{
  //Read GPIO2's input
  var = digitalRead(SENSOR);
  //Light up if input is true
  if(var && !prev)
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    postData("MovementDetected");
    delay(10000); //Wait 10 sec 
  }
  //Shut off if input is false
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000); //Wait 1 sec
  }
  prev = var;
} 
