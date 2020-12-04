#include "hash.h"


uint32_t
algo::hash::fnv0_32(const uint8_t *octects, int len)
{
	uint32_t hash = 0;

	for (int i = 0; i < len; i++) {
		hash *= 16777619;
		hash ^= octects[i];
	}

	return hash;
}


uint64_t
algo::hash::fnv0_64(const uint8_t *octects, int len)
{
	uint64_t hash = 0;

	for (int i = 0; i < len; i++) {
		hash *= 1099511628211;
		hash ^= octects[i];
	}

	return hash;
}


uint32_t
algo::hash::fnv1_32(const uint8_t *octects, int len)
{
	uint32_t hash = 2166136261U;

	for (int i = 0; i < len; i++) {
		hash *= 16777619;
		hash ^= octects[i];
	}

	return hash;
}


uint64_t
algo::hash::fnv1_64(const uint8_t *octects, int len)
{
	uint64_t hash = 14695981039346656037U;

	for (int i = 0; i < len; i++) {
		hash *= 1099511628211;
		hash ^= octects[i];
	}

	return hash;
}


uint32_t
algo::hash::fnv1a_32(const uint8_t *octects, int len)
{
	uint32_t hash = 2166136261U;

	for (int i = 0; i < len; i++) {
		hash ^= octects[i];
		hash *= 16777619;
	}

	return hash;
}


uint64_t
algo::hash::fnv1a_64(const uint8_t *octects, int len)
{
	uint64_t hash = 14695981039346656037U;

	for (int i = 0; i < len; i++) {
		hash ^= octects[i];
		hash *= 1099511628211;
	}

	return hash;
}


uint64_t
algo::hash::djb2(const uint8_t *octects, int len)
{
	uint64_t hash = 5381;

	for (int i = 0; i < len; i++) {
		hash = ((hash << 5) + hash) + octects[i];
	}

	return hash;
}


uint64_t
algo::hash::sdbm(const uint8_t *octects, int len)
{
	uint64_t hash = 0;

	for (int i = 0; i < len; i++) {
		hash = octects[i] + (hash << 6) + (hash << 16) - hash;
	}

	return hash;

}


uint64_t
algo::hash::lose_lose(const uint8_t *octects, int len)
{
	uint64_t hash = 0;

	for (int i = 0; i < len; i++) {
		hash += octects[i];
	}

	return hash;
}


uint32_t
algo::hash::murmur3(const uint8_t *octects, int len, uint32_t seed) {
	uint32_t c1 = 0xcc9e2d51;
	uint32_t c2 = 0x1b873593;
	uint32_t r1 = 15;
	uint32_t r2 = 13;

	uint32_t m = 5;
	uint32_t n = 0xe6546b64;
	uint32_t k = 0;

	uint8_t *d = (uint8_t*)octects;
	int i = 0, l = len / 4;
	uint32_t h = seed;

	const uint32_t *chunks = (const uint32_t *)(d + l * 4);
	const uint8_t *tail = (d + l * 4);

	for (i = -l; i != 0; ++i) {
		k = chunks[i];
		k *= c1;
		k = (k << r1) | (k >> (32 - r1));
		k *= c2;
		h ^= k;
		h = (h << r2) | (h >> (32 - r2));
		h = h * m + n;
	}

	k = 0;

	switch (len & 3) {
		case 3: k ^= (tail[2] << 16);
		case 2: k ^= (tail[1] << 8);

		case 1:
		k ^= tail[0];
		k *= c1;
		k = (k << r1) | (k >> (32 - r1));
		k *= c2;
		h ^= k;
	}

	h ^= len;

	h ^= (h >> 16);
	h *= 0x85ebca6b;
	h ^= (h >> 13);
	h *= 0xc2b2ae35;
	h ^= (h >> 16);

	return h;
}
