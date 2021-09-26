#include <Arduino.h>
#include <string>
#include "debug.h"

std::string debugStr("");
bool traceStampRequired = true;

void traceStamp(const char* x, const char* y, bool z, const char* function, int line) {                       
    if (traceStampRequired)                            
    {                                                  
        Serial.print("[");                             
        Serial.print(x);                               
        Serial.print("]");                             
        Serial.print("[");                             
        Serial.print(function);                    
        Serial.print(":");                             
        Serial.print(line);                        
        Serial.print("] ");                            
    }                                                  
    debugStr = y;                                      
    if (z || debugStr.find("\n") != std::string::npos)
        traceStampRequired = true;                     
    else                                               
        traceStampRequired = false;
}

void traceStamp(const char* x, uint8_t y, bool z, const char* function, int line) {                       
    if (traceStampRequired)                            
    {                                                  
        Serial.print("[");                             
        Serial.print(x);                               
        Serial.print("]");                             
        Serial.print("[");                             
        Serial.print(function);                    
        Serial.print(":");                             
        Serial.print(line);                        
        Serial.print("] ");                            
    }                                                  
    debugStr = y;                                      
    if (z || debugStr.find("\n") != std::string::npos)
        traceStampRequired = true;                     
    else                                               
        traceStampRequired = false;
}