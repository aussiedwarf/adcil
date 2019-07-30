#ifndef ADBUFFER_H
#define ADBUFFER_H

#include <stdint.h>

#include "adcil_types.h"

typedef struct {
  uint8_t* buffer;
  uint8_t* start;
  uint8_t* pos;
} adBuffer;

void adBufferWrite(adBuffer* a_buffer, const void* a_data, const int a_size);
void adBufferWriteUint32(adBuffer* a_buffer, const uint32_t a_data);
void adBufferWriteUint16(adBuffer* a_buffer, const uint16_t a_data);

uint32_t adBufferReadUint32(const void* a_buffer);



#endif
