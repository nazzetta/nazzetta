#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#ifndef APSSID
#define APSSID "APSSID"
#define APPSK  "APPSK"
#endif

const int FW_VERSION = 23;

String FW_HTTP_VERSION  = "http://nazzetta.com/OTAupdate/version.json";
String FW_HTTP_DOWNLOAD = "http://nazzetta.com/OTAupdate/OTAupdate.ino.d1_mini.bin";

ESP8266WiFiMulti wifiMulti;

void setup() {

	pinMode(LED_BUILTIN, OUTPUT);	digitalWrite(LED_BUILTIN, HIGH);

	Serial.begin(115200);
	// Serial.setDebugOutput(true);

	Serial.println();
	Serial.println();
	Serial.println();

	for (uint8_t t = 4; t > 0; t--) {
		Serial.printf("[SETUP] WAIT %d...\n", t);
		Serial.flush();
		delay(500);
	}

	WiFi.mode(WIFI_STA);
	WiFi.setAutoReconnect(true);

	wifiMulti.addAP("NaNaNaNaNaNa", "wonderwoman");
	wifiMulti.addAP("ZeroUI-2.4","zeroui@10570");
	// wifiMulti.addAP((String("ZIRAA"+String(KitID))).c_str(), "zirokits");
	// wifiMulti.run();


}

void loop() {
	// wait for WiFi connection
	if ((wifiMulti.run() == WL_CONNECTED)) {

		WiFiClient client;

		HTTPClient httpClient;
		httpClient.begin( FW_HTTP_VERSION );

		int httpCode = httpClient.GET();
		if( httpCode == 200 ) {
			String newFWVersion = httpClient.getString();

			Serial.print( "\n\nCurrent firmware version: " );
			Serial.println( FW_VERSION );
			Serial.print( "Available firmware version: " );
			Serial.println( newFWVersion );

			int newVersion = newFWVersion.toInt();

			int sec = millis() / 1000;
			int min = sec / 60;
			int hr = min / 60;
			Serial.printf("Uptime: %02i:%02i:%02i \n" ,hr, min % 60, sec % 60);

			if( newVersion > FW_VERSION ) {
				Serial.println( "Preparing to update." );

				ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
				t_httpUpdate_return ret = ESPhttpUpdate.update(FW_HTTP_DOWNLOAD);

				switch (ret) {
					case HTTP_UPDATE_FAILED:
						Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
						break;

					case HTTP_UPDATE_NO_UPDATES:
						Serial.println("HTTP_UPDATE_NO_UPDATES");
						break;

					case HTTP_UPDATE_OK:
						Serial.println("HTTP_UPDATE_OK");
						break;
				}
			}

			else {
				Serial.println( "No new version Available" );

			}

			httpClient.end();
		}

		// The line below is optional. It can be used to blink the LED on the board during flashing
		// The LED will be on during download of one buffer of data from the network. The LED will
		// be off during writing that buffer to flash
		// On a good connection the LED should flash regularly. On a bad connection the LED will be
		// on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
		// value is used to put the LED on. If the LED is on with HIGH, that value should be passed
		// ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

		// t_httpUpdate_return ret = ESPhttpUpdate.update("http://nazzetta.com/OTAupdate/OTAupdate.ino.d1_mini.bin");
		// // Or:
		// //t_httpUpdate_return ret = ESPhttpUpdate.update(client, "server", 80, "file.bin");

		// Serial.println(ret);
		// switch (ret) {
		// 	case HTTP_UPDATE_FAILED:
		// 		Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
		// 		break;

		// 	case HTTP_UPDATE_NO_UPDATES:
		// 		Serial.println("HTTP_UPDATE_NO_UPDATES");
		// 		break;

		// 	case HTTP_UPDATE_OK:
		// 		Serial.println("HTTP_UPDATE_OK");
		// 		break;
		// }

		delay(10000);
	}
}
