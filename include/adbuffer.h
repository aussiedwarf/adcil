#ifndef ADBUFFER_H
#define ADBUFFER_H

#include "adcil_types.h"

typedef struct {
  void* buffer;
  void* start;
  void* pos;
}adBuffer;

void adBufferWrite(adBuffer* a_buffer, const void* a_data, const int a_size);
void adBufferWriteUint32(adBuffer* a_buffer, const adUint32 a_data);
void adBufferWriteUint16(adBuffer* a_buffer, const adUint16 a_data);

#endif
