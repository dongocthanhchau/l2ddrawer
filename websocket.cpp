#include "websocket.h"

#define USE_SERIAL Serial
WebSocketsClient webSocket;
float _l,_r,_pen,_delay=0;
int _counter=-1;
void pushData(float &left, float &right, float &pen, float &delayTime, int &counter){
  if (counter==_counter) return;
  if (_l>30&&_l<150&&_r>30&&_r<150&&_pen>30&&_pen<150){
    left=_l;
    right=_r;
    pen=_pen;
    delayTime=_delay;
    counter = _counter;
    
    
  } else {
    webSocket.sendTXT("{status\":\"out of range\",\"count\":"+String(_counter)+"}");
    counter = _counter;
  }
}
void sendResponse(int counter){
  webSocket.sendTXT("{status\":\"done\",\"count\":"+String(counter)+"}");
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
      // String input;

        StaticJsonDocument<128> doc;
        //USE_SERIAL.printf(payload);
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
          return;
        }

        const char* command = doc["command"]; // "draw"
        if (strstr(command, "draw")!=NULL){
          _l = doc["left"]; // 90.1
          _r = doc["right"]; // 90.1
          _pen = doc["pen"]; // 90.1
          _counter = doc["count"]; // 5
          _delay = 4;
        }
        if (strstr(command, "move")!=NULL){
          _l = doc["left"]; // 90.1
          _r = doc["right"]; // 90.1
          _pen = doc["pen"]; // 90.1
          _counter = doc["count"]; // 5
          _delay = 0;
        }
        
        if (strstr(command,"check")!=NULL){
          webSocket.sendTXT("{status\":\"OK\",\"device\":\"DRAW2\"}");
        }
        
       
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
	webSocket.begin("node.creta.work", 1888, "/bot/draw");

	// event handler
	webSocket.onEvent(webSocketEvent);

	// use HTTP Basic Authorization this is optional remove if not needed
	//webSocket.setAuthorization("user", "Password");

	// try ever 5000 again if connection has failed
	webSocket.setReconnectInterval(5000);
}
void webSocketLoop(void){
    webSocket.loop();
	//check connection
	if (millis()%600000<10) 
		if (!webSocket.sendTXT("Connected"))
			ESP.restart();
}
