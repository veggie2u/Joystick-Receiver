/* 
Ralph Bacon https://github.com/RalphBacon/224-Superior-Serial.print-statements

Logging (from Serial.print statements) can be customised by setting the verbosity here:
  
    E - errors that you really, really should see
    W - warnings that help you find out what's going wrong
    D - debugging messages that you only want to see when debugging
    V - verbose meaning everything is printed

    To use, just #include this header file in your standard Arduino Code and then
    define the level of logging you want by setting the DEBUGLEVEL to one of the values
    listed here.

    In your code, instead of using Serial.print and Serial.println statements use
    debugE / debuglnE, debugW / debuglnW, D, V (as you see the level of logging required)
    and only those messages will appear.

    Example:

    You want Errors, Warnings and General Debugging output but not those marked as Verbose.
    #define DEBUGLEVEL DEBUGLEVEL_DEBUGGING
*/

#ifndef debugger_H
#define debugger_H

//#include <string>

// User picks debugging level from this list
#define DEBUGLEVEL_ERRORS 1
#define DEBUGLEVEL_WARNINGS 2
#define DEBUGLEVEL_DEBUGGING 3
#define DEBUGLEVEL_VERBOSE 4
#define DEBUGLEVEL_NONE 0

// We want to debug everything! Overide this in your sketch
#ifndef DEBUGLEVEL
#define DEBUGLEVEL DEBUGLEVEL_DEBUGGING
#endif

// The tracestamp looks like [D][mainfunction:45]
void traceStamp(const char* x, const char* y, boolean z, const char* function, int line);
void traceStamp(const char* x, uint8_t y, boolean z, const char* function, int line);

// This is a macro that turns simple (one string) Serial.println statements on and off
#if DEBUGLEVEL > 0
#define debugE(x)              \
    traceStamp("E", x, false); \
    Serial.print(x);
#define debuglnE(x)           \
    traceStamp("E", x, true); \
    Serial.println(x)
#else
#define debugE(x)   // Nothing to see here
#define debuglnE(x) // Or here
#endif

#if DEBUGLEVEL > 1
#define debugW(x)              \
    traceStamp("W", x, false, __FUNCTION__, __LINE__); \
    Serial.print(x);
#define debuglnW(x)           \
    traceStamp("W", x, true, __FUNCTION__, __LINE__); \
    Serial.println(x)
#else
#define debugW(x)   // Nothing to see here
#define debuglnW(x) // Or here
#endif

#if DEBUGLEVEL > 2
#define debugD(x)              \
    traceStamp("D", x, false, __FUNCTION__, __LINE__); \
    Serial.print(x);
#define debuglnD(x)           \
    traceStamp("D", x, true, __FUNCTION__, __LINE__); \
    Serial.println(x)
#else
#define debugD(x)   /* Nothing to see here */
#define debuglnD(x) // Or here
#endif

#if DEBUGLEVEL > 3
#define debugV(x)              \
    traceStamp("V", x, false, __FUNCTION__, __LINE__); \
    Serial.print(x);
#define debuglnV(x)           \
    traceStamp("V", x, true, __FUNCTION__, __LINE__); \
    Serial.println(x);
#else
#define debugV(x);
#define debuglnV(x);
#endif
#endif