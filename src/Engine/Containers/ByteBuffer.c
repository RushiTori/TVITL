#include "ByteBuffer.h"

ByteBuffer CreateByteBufferFromFile(const char* filePath) {
	ByteBuffer buffer = {0};
	if (!FileExists(filePath)) {
		TraceLog(LOG_FATAL, "Couldn't open file \"%s\" : file doesn't exist.", filePath);
		return (ByteBuffer){0};
	}

	buffer.data = LoadFileData(filePath, (int32_t*)&buffer.size);
	buffer.capacity = buffer.size;
	return buffer;
}

ByteBuffer CreateByteBufferAsArray(uint32_t baseCapacity) {
	ByteBuffer buffer = {0};
	buffer.capacity = max(1U, baseCapacity);
	buffer.data = malloc(sizeof(uint8_t) * buffer.capacity);

	if (!buffer.data) {
		TraceLog(LOG_FATAL, "Couldn't allocate buffer.data\n");
		return (ByteBuffer){0};
	}

	return buffer;
}

void FreeByteBuffer(ByteBuffer* buffer) { free(buffer->data); }

uint32_t ByteBufferReadRaw(ByteBuffer* restrict buffer, void* restrict dest, uint32_t destSize) {
	uint32_t readSize = min(buffer->size - buffer->dataPtr, destSize);
	memcpy(dest, buffer->data + buffer->dataPtr, readSize);
	buffer->dataPtr += readSize;
	return readSize;
}

char* ByteBufferReadString(ByteBuffer* restrict buffer, bool nullTerminated, uint32_t* restrict stringLen) {
	if (buffer->dataPtr >= buffer->size) {
		TraceLog(LOG_WARNING, "Couldn't read from buffer : reached the end of buffer.");
		return NULL;
	}

	uint32_t readSize = 0;
	bool foundNullTerm = false;

	if (nullTerminated) {
		uint8_t* tmp = memchr(buffer->data + buffer->dataPtr, '\0', buffer->size - buffer->dataPtr);
		if (tmp) {
			readSize = tmp - (buffer->data + buffer->dataPtr) + 1;
			foundNullTerm = true;
		} else {
			readSize = buffer->size - buffer->dataPtr;
		}
	} else {
		readSize = ByteBufferRead(uint32_t, buffer);
	}

	if (stringLen) *stringLen = readSize;

	if (!readSize) return NULL;

	char* str = malloc(sizeof(char) * (readSize + (!nullTerminated || !foundNullTerm)));

	if (!str) {
		TraceLog(LOG_FATAL, "Couldn't allocate string.");
		return NULL;
	}

	memcpy(str, buffer->data + buffer->dataPtr, readSize);

	if (!nullTerminated || !foundNullTerm) str[readSize] = '\0';

	buffer->dataPtr += readSize;

	return str;
}

#define ByteBufferReadSafe(type)                                                                                      \
	({                                                                                                                \
		type tmp = 0;                                                                                                 \
		uint32_t readBytes = ByteBufferReadRaw(buffer, &tmp, sizeof(type));                                           \
		if (readBytes != sizeof(type)) {                                                                              \
			TraceLog(LOG_WARNING, "Couldn't read enough bytes : asked for %d but read %d, missing %d.", sizeof(type), \
					 readBytes, sizeof(type) - readBytes);                                                            \
		}                                                                                                             \
		if (!IsLittleEndian) tmp = SwapBytes(tmp);                                                                    \
		tmp;                                                                                                          \
	})

uint8_t ByteBufferRead_UInt8(ByteBuffer* buffer) { return ByteBufferReadSafe(uint8_t); }
uint16_t ByteBufferRead_UInt16(ByteBuffer* buffer) { return ByteBufferReadSafe(uint16_t); }
uint32_t ByteBufferRead_UInt32(ByteBuffer* buffer) { return ByteBufferReadSafe(uint32_t); }
uint64_t ByteBufferRead_UInt64(ByteBuffer* buffer) { return ByteBufferReadSafe(uint64_t); }
int8_t ByteBufferRead_Int8(ByteBuffer* buffer) { return ByteBufferReadSafe(int8_t); }
int16_t ByteBufferRead_Int16(ByteBuffer* buffer) { return ByteBufferReadSafe(int16_t); }
int32_t ByteBufferRead_Int32(ByteBuffer* buffer) { return ByteBufferReadSafe(int32_t); }
int64_t ByteBufferRead_Int64(ByteBuffer* buffer) { return ByteBufferReadSafe(int64_t); }
float ByteBufferRead_Float(ByteBuffer* buffer) { return ByteBufferReadSafe(float); }
double ByteBufferRead_Double(ByteBuffer* buffer) { return ByteBufferReadSafe(double); }

void ByteBufferPushRaw(ByteBuffer* restrict buffer, const void* restrict src, uint32_t count) {
	if (buffer->size + count > buffer->capacity) {
		uint8_t* tmp = realloc(buffer->data, sizeof(uint8_t) * buffer->capacity * 2);
		if (!tmp) {
			TraceLog(LOG_FATAL, "Couldn't reallocate buffer.data\n");
			return;
		}
		buffer->data = tmp;
		buffer->capacity *= 2;
	}

	memcpy(buffer->data + buffer->size, src, count);
	buffer->size += count;
}

void ByteBufferPushString(ByteBuffer* restrict buffer, const char* restrict str, bool nullTerminated) {
	uint32_t stringLen = strlen(str);
	if (!nullTerminated) ByteBufferPush(buffer, stringLen);
	ByteBufferPushRaw(buffer, str, stringLen + nullTerminated);
}

#define ByteBufferPushSafe(val)                                 \
	{                                                           \
		typeof(val) bytes = val;                                \
		if (!IsLittleEndian) bytes = SwapBytes(bytes);          \
		ByteBufferPushRaw(buffer, &bytes, sizeof(typeof(val))); \
	}

void ByteBufferPush_UInt8(ByteBuffer* buffer, uint8_t val) { ByteBufferPushSafe(val); }
void ByteBufferPush_UInt16(ByteBuffer* buffer, uint16_t val) { ByteBufferPushSafe(val); }
void ByteBufferPush_UInt32(ByteBuffer* buffer, uint32_t val) { ByteBufferPushSafe(val); }
void ByteBufferPush_UInt64(ByteBuffer* buffer, uint64_t val) { ByteBufferPushSafe(val); }
void ByteBufferPush_Int8(ByteBuffer* buffer, int8_t val) { ByteBufferPushSafe(val); }
void ByteBufferPush_Int16(ByteBuffer* buffer, int16_t val) { ByteBufferPushSafe(val); }
void ByteBufferPush_Int32(ByteBuffer* buffer, int32_t val) { ByteBufferPushSafe(val); }
void ByteBufferPush_Int64(ByteBuffer* buffer, int64_t val) { ByteBufferPushSafe(val); }
void ByteBufferPush_Float(ByteBuffer* buffer, float val) { ByteBufferPushSafe(val); }
void ByteBufferPush_Double(ByteBuffer* buffer, double val) { ByteBufferPushSafe(val); }
