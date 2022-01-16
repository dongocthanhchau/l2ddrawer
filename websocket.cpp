#include "websocket.h"

#define USE_SERIAL Serial
WebSocketsClient webSocket;
float _x1=-1.0,_y1,_x2,_y2;
void pushData(float &x1, float &y1, float &x2, float &y2){
  x1=_x1;
  x2=_x2;
  y1=_y1;
  y2=_y2;
  _x1=-1.0;
}

void hexdump(const void *mem, uint32_t len, uint8_t cols) {
	const uint8_t* src = (const uint8_t*) mem;
	USE_SERIAL.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
	
	for(uint32_t i = 0; i < len; i++) {
		if(i % cols == 0) {
			USE_SERIAL.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
		}
		USE_SERIAL.printf("%02X ", *src);
		src++;
	}
	USE_SERIAL.printf("\n");
  
  //Code here
}


void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

	switch(type) {
		case WStype_DISCONNECTED:
			USE_SERIAL.printf("[WSc] Disconnected!\n");
			break;
		case WStype_CONNECTED:
			USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);

			// send message to server when Connected
			webSocket.sendTXT("Connected");
			break;
		case WStype_TEXT:{
			USE_SERIAL.printf("[WSc] get text: %s\n", payload);
            //carControl(int(payload[0])-48);
			// send message to server
      //JSON
      StaticJsonDocument<192> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
      return;
      }

      const char* command = doc["command"]; // "Line"
      if (strchr(command,'L')>0){
        _x1 = doc["A"][0]; // 1.1
        _y1 = doc["A"][1]; // 2.1

        _x2 = doc["B"][0]; // 2.4
        _y2 = doc["B"][1]; // 4.4
      }
      //END OF JSON
//			webSocket.sendTXT("Line Done");
		}
			break;
		case WStype_BIN:
			USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);
      
      
			// send data to server
			// webSocket.sendBIN(payload, length);
			break;
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }

}
void webSocketInit(void){
    // server address, port and URL
	webSocket.begin("ws.uudamstudio.com", 1880, "/drawcommand");

	// event handler
	webSocket.onEvent(webSocketEvent);

	// use HTTP Basic Authorization this is optional remove if not needed
	//webSocket.setAuthorization("user", "Password");

	// try ever 5000 again if connection has failed
	webSocket.setReconnectInterval(100);
}
void webSocketLoop(void){
    webSocket.loop();
	//check connection
	if (millis()%600000<10) 
		if (!webSocket.sendTXT("Connected"))
			ESP.restart();
}
