#ifndef HELPERS_H
#define HELPERS_H

#ifdef DEBUGGERY
#define MSG_SERIAL(msg, val) Serial.print(msg);Serial.println(val)
#else
#define MSG_SERIAL(msg, val) 
#endif

#endif