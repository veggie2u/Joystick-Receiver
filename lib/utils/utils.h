#ifndef UTILS_H 
#define UTILS_H 

//************************************************************************
//****** Real Bit Level Boolean Utilisation Macro Definitions  ***********
//****** Created by Trevor D.BEYDAG 09 May 2016                ***********
//****** http://lab.dejaworks.com                              ***********
//****** Please keep credits intact when using and/or reproducing ********
//************************************************************************
#define BB_TRUE(bp,bb)    bp |= bb
#define BB_FALSE(bp,bb)   bp &= ~(bb)
#define BB_READ(bp,bb)    bool(bp & bb)

// #define DEBUG 1

// #if DEBUG == 1
// #define debug(x) Serial.print(x)
// #define debugln(x) Serial.println(x)
// #else
// #define debug(x)
// #define debugln(x)
// #endif

#endif
