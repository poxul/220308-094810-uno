#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include "debugLogger.h"

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

DebugLoggerListItem::DebugLoggerListItem(char* str) {
    content = str;
}

void DebugLoggerListItem::setNext(DebugLoggerListItem* newNext) {
    nextItem = newNext;
}

DebugLoggerListItem* DebugLoggerListItem::getNext() {
    return nextItem;
}

char* DebugLoggerListItem::getContent() {
    return content;
}

DebugLogger::DebugLogger() {
    locked = false;
}

bool DebugLogger::lockList() {
    if (locked) return false;
    locked = true;
    return true;
}

void DebugLogger::unlockList() {
    locked = false;
}

void DebugLogger::print(char* str) {
    DebugLoggerListItem* newItem = new DebugLoggerListItem(str);
    if (!lockList()) { // If unable to acquire the log the message gets discarded.
        return;
    }
    DebugLoggerListItem* i = list;
    if (!i) { list = newItem; }
    else {
        while (i->getNext()) { i = i->getNext(); }
        i->setNext(newItem);
    }
    unlockList();
}

char* DebugLogger::getBuffer() {
    char* str = (char*) malloc(MAX_DEBUG_STR_LEN);
    return str;
}

char* DebugLogger::resizeBuffer(char* str) {
    return (char*) realloc(str, strlen(str) + 1);
}

void DebugLogger::printf(const char* format, ...) {
    if (freeMemory() < MIN_FREE_HEAP) return;
    char* str = getBuffer();
    va_list argptr;
    va_start(argptr, format);
    vsprintf(str, format, argptr);
    va_end(argptr);
    str = resizeBuffer(str);
    print(str);
}

void DebugLogger::printAllWithSerial() {
    noInterrupts();
    if (!lockList()) {
        Serial.print("Logger is currently being filled. ");
        Serial.println("Printing will resume in the next iteration.");
        return;
    }
    DebugLoggerListItem* i = list;
    while (i) {
        Serial.print(i->getContent());
        DebugLoggerListItem* old = i;
        i = i->getNext();
        free(old->getContent());
        delete old;
    }
    list = NULL;
    unlockList();
    interrupts();
}

