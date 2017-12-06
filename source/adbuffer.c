#include "adbuffer.h"

void adBufferWrite(adBuffer* a_buffer, const void* a_data, const int a_size)
{
  memcpy(a_buffer->pos, a_data, a_size);
  a_buffer->pos += a_size;
}

void adBufferWriteUint32(adBuffer* a_buffer, const adUint32 a_data)
{
  memcpy(a_buffer->pos, &a_data, sizeof(adUint32));
  a_buffer->pos += sizeof(adUint32);
}

void adBufferWriteUint16(adBuffer* a_buffer, const adUint16 a_data)
{
  memcpy(a_buffer->pos, &a_data, sizeof(adUint16));
  a_buffer->pos += sizeof(adUint16);
}
