#ifndef _websocket__h_
#define _websocket__h_
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

#include <WebSocketsClient.h>

void pushData(float &x1, float &y1, float &x2, float &y2);
void hexdump(const void *mem, uint32_t len, uint8_t cols = 16);
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
void webSocketInit(void);
void webSocketLoop(void);
#endif
