#include "adbuffer.h"

void adBufferWrite(adBuffer* a_buffer, const void* a_data, const int a_size)
{
  memcpy(a_buffer->pos, a_data, a_size);
  a_buffer->pos += a_size;
}

void adBufferWriteUint32(adBuffer* a_buffer, const uint32_t a_data)
{
  memcpy(a_buffer->pos, &a_data, sizeof(uint32_t));
  a_buffer->pos += sizeof(uint32_t);
}

void adBufferWriteUint16(adBuffer* a_buffer, const uint16_t a_data)
{
  memcpy(a_buffer->pos, &a_data, sizeof(uint16_t));
  a_buffer->pos += sizeof(uint16_t);
}

uint32_t adBufferReuint32_t(const void* a_buffer)
{
  const uint8_t* buffer = a_buffer;
  uint32_t a = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
  return a;
}
