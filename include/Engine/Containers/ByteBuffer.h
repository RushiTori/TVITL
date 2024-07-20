#ifndef BYTE_BUFFER_H
#define BYTE_BUFFER_H

#include "Common.h"

#define IsLittleEndian                               \
	({                                               \
		volatile uint32_t tmp = 1;                   \
		volatile uint8_t endian = *(char*)(&tmp); \
		endian;                                      \
	})

#define SwapBytes16(val)                   \
	({                                     \
		uint8_t* bytes = (uint8_t*)(&val); \
		SwapValues(bytes[0], bytes[1]);    \
		*(typeof(val)*)(bytes);            \
	})

#define SwapBytes32(val)                   \
	({                                     \
		uint8_t* bytes = (uint8_t*)(&val); \
		SwapValues(bytes[0], bytes[3]);    \
		SwapValues(bytes[1], bytes[2]);    \
		*(typeof(val)*)(bytes);            \
	})

#define SwapBytes64(val)                   \
	({                                     \
		uint8_t* bytes = (uint8_t*)(&val); \
		SwapValues(bytes[0], bytes[7]);    \
		SwapValues(bytes[1], bytes[6]);    \
		SwapValues(bytes[2], bytes[5]);    \
		SwapValues(bytes[3], bytes[4]);    \
		*(typeof(val)*)(bytes);            \
	})

#define SwapBytes(val)                         \
	({                                         \
		typeof(val) tmp = val;                 \
                                               \
		if (sizeof(typeof(tmp)) == 2) {        \
			tmp = SwapBytes16(tmp);            \
		} else if (sizeof(typeof(tmp)) == 4) { \
			tmp = SwapBytes32(tmp);            \
		} else if (sizeof(typeof(tmp)) == 8) { \
			tmp = SwapBytes64(tmp);            \
		}                                      \
                                               \
		tmp;                                   \
	})

typedef struct ByteBuffer {
	uint8_t* data;
	uint32_t capacity;
	uint32_t size;
	uint32_t dataPtr;
} ByteBuffer;

ByteBuffer CreateByteBufferFromFile(const char* filePath);
ByteBuffer CreateByteBufferAsArray(uint32_t baseCapacity);
void FreeByteBuffer(ByteBuffer* buffer);

uint32_t ByteBufferReadRaw(ByteBuffer* restrict buffer, void* restrict dest, uint32_t destSize);
char* ByteBufferReadString(ByteBuffer* restrict buffer, bool nullTerminated, uint32_t* restrict stringLen);

uint8_t ByteBufferRead_UInt8(ByteBuffer* buffer);
uint16_t ByteBufferRead_UInt16(ByteBuffer* buffer);
uint32_t ByteBufferRead_UInt32(ByteBuffer* buffer);
uint64_t ByteBufferRead_UInt64(ByteBuffer* buffer);
int8_t ByteBufferRead_Int8(ByteBuffer* buffer);
int16_t ByteBufferRead_Int16(ByteBuffer* buffer);
int32_t ByteBufferRead_Int32(ByteBuffer* buffer);
int64_t ByteBufferRead_Int64(ByteBuffer* buffer);
float ByteBufferRead_Float(ByteBuffer* buffer);
double ByteBufferRead_Double(ByteBuffer* buffer);

#define ByteBufferRead(type, buffer)     \
	_Generic((typeof(type)){0},          \
		uint8_t: ByteBufferRead_UInt8,   \
		uint16_t: ByteBufferRead_UInt16, \
		uint32_t: ByteBufferRead_UInt32, \
		uint64_t: ByteBufferRead_UInt64, \
		int8_t: ByteBufferRead_Int8,     \
		int16_t: ByteBufferRead_Int16,   \
		int32_t: ByteBufferRead_Int32,   \
		int64_t: ByteBufferRead_Int64,   \
		float: ByteBufferRead_Float,     \
		double: ByteBufferRead_Double)(buffer)

void ByteBufferPushRaw(ByteBuffer* restrict buffer, const void* restrict src, uint32_t count);
void ByteBufferPushString(ByteBuffer* restrict buffer, const char* restrict str, bool nullTerminated);

void ByteBufferPush_UInt8(ByteBuffer* buffer, uint8_t val);
void ByteBufferPush_UInt16(ByteBuffer* buffer, uint16_t val);
void ByteBufferPush_UInt32(ByteBuffer* buffer, uint32_t val);
void ByteBufferPush_UInt64(ByteBuffer* buffer, uint64_t val);
void ByteBufferPush_Int8(ByteBuffer* buffer, int8_t val);
void ByteBufferPush_Int16(ByteBuffer* buffer, int16_t val);
void ByteBufferPush_Int32(ByteBuffer* buffer, int32_t val);
void ByteBufferPush_Int64(ByteBuffer* buffer, int64_t val);
void ByteBufferPush_Float(ByteBuffer* buffer, float val);
void ByteBufferPush_Double(ByteBuffer* buffer, double val);

#define ByteBufferPush(buffer, val)      \
	_Generic((typeof(val)){0},            \
		uint8_t: ByteBufferPush_UInt8,   \
		uint16_t: ByteBufferPush_UInt16, \
		uint32_t: ByteBufferPush_UInt32, \
		uint64_t: ByteBufferPush_UInt64, \
		int8_t: ByteBufferPush_Int8,     \
		int16_t: ByteBufferPush_Int16,   \
		int32_t: ByteBufferPush_Int32,   \
		int64_t: ByteBufferPush_Int64,   \
		float: ByteBufferPush_Float,     \
		double: ByteBufferPush_Double)(buffer, val)

#endif	// BYTE_BUFFER_H
