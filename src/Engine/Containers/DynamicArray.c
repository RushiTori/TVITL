
#include "DynamicArray.h"

#define DYNAMIC_ARRAY_DEFAULT_CAPACITY 8
#define DYNAMIC_ARRAY_GROWTH_FACTOR 2

static inline void* ReallocDynamicArrayContainer(void* container, uint32_t newSize) {
	if (!newSize) {
		free(container);
		return NULL;
	}

	if (!container) {
		container = malloc(newSize);
		if (!container) TraceLog(LOG_FATAL, "Couldn't allocate a dynamic array container.\n");
	} else {
		void* tmp = realloc(container, newSize);
		if (!tmp) TraceLog(LOG_FATAL, "Couldn't reallocate a dynamic array container.\n");
		container = tmp;
	}

	return container;
}

#define DEFINE_DYNAMIC_ARRAY_FUNCTIONS(type, structName)                                     \
	structName Create##structName() {                                                        \
		structName arr = {0};                                                                \
		arr.capacity = DYNAMIC_ARRAY_DEFAULT_CAPACITY;                                       \
		arr.data = ReallocDynamicArrayContainer(arr.data, sizeof(type) * arr.capacity);      \
		return arr;                                                                          \
	}                                                                                        \
                                                                                             \
	void Free##structName(structName* arr) {                                                 \
		free(arr->data);                                                                     \
		*arr = (structName){0};                                                              \
	}                                                                                        \
                                                                                             \
	void Reserve##structName(structName* arr, uint32_t capacity) {                           \
		if (arr->capacity >= capacity) return;                                               \
		arr->data = ReallocDynamicArrayContainer(arr->data, sizeof(type) * capacity);        \
		arr->capacity = capacity;                                                            \
	}                                                                                        \
                                                                                             \
	void Shrink##structName(structName* arr) {                                               \
		if (arr->capacity == arr->size) return;                                              \
		arr->data = ReallocDynamicArrayContainer(arr->data, sizeof(type) * arr->size);       \
		arr->capacity = arr->size;                                                           \
	}                                                                                        \
                                                                                             \
	void PushBack##structName(structName* arr, type item) {                                  \
		if (arr->size == arr->capacity) {                                                    \
			uint32_t newCapacity = arr->capacity * DYNAMIC_ARRAY_GROWTH_FACTOR;              \
			if (newCapacity == 0) newCapacity = DYNAMIC_ARRAY_DEFAULT_CAPACITY;              \
			arr->data = ReallocDynamicArrayContainer(arr->data, sizeof(type) * newCapacity); \
			arr->capacity = newCapacity;                                                     \
		}                                                                                    \
		arr->data[arr->size++] = item;                                                       \
	}                                                                                        \
                                                                                             \
	void Remove##structName(structName* arr, uint32_t idx) {                                 \
		if (idx >= arr->size) return;                                                        \
                                                                                             \
		memmove(arr->data + idx, arr->data + idx + 1, sizeof(type) * (arr->size - idx));     \
		arr->size--;                                                                         \
                                                                                             \
		if (arr->size <= arr->capacity / DYNAMIC_ARRAY_GROWTH_FACTOR) {                      \
			uint32_t newCapacity = arr->capacity / DYNAMIC_ARRAY_GROWTH_FACTOR;              \
			arr->data = ReallocDynamicArrayContainer(arr->data, sizeof(type) * newCapacity); \
			arr->capacity = newCapacity;                                                     \
		}                                                                                    \
	}

DEFINE_DYNAMIC_ARRAY_FUNCTIONS(uint32_t, IndexArray)
DEFINE_DYNAMIC_ARRAY_FUNCTIONS(Vector2, Vector2Array)
DEFINE_DYNAMIC_ARRAY_FUNCTIONS(Mover, MoverArray)
DEFINE_DYNAMIC_ARRAY_FUNCTIONS(Orbiter, OrbiterArray)
DEFINE_DYNAMIC_ARRAY_FUNCTIONS(Movorbiter, MovorbiterArray)
DEFINE_DYNAMIC_ARRAY_FUNCTIONS(Shooter, ShooterArray)
DEFINE_DYNAMIC_ARRAY_FUNCTIONS(ZoneGroup, ZoneGroupArray)
DEFINE_DYNAMIC_ARRAY_FUNCTIONS(Bullet, BulletArray)
DEFINE_DYNAMIC_ARRAY_FUNCTIONS(ZoneTypeChange, ZoneTypeChangeArray)
