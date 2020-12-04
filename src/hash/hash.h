#ifndef ALGO_HASH_H
#define ALGO_HASH_H


#include <cstdint>


namespace algo::hash
{
	//fnv family
	uint32_t fnv0_32(const uint8_t *octects, int len);

	uint64_t fnv0_64(const uint8_t *octects, int len);

	uint32_t fnv1_32(const uint8_t *octects, int len);

	uint64_t fnv1_64(const uint8_t *octects, int len);

	uint32_t fnv1a_32(const uint8_t *octects, int len);

	uint64_t fnv1a_64(const uint8_t *octects, int len);


	//djb2
	uint64_t djb2(const uint8_t *octects, int len);


	//sdbm
	uint64_t sdbm(const uint8_t *octects, int len);


	//lose lose
	uint64_t lose_lose(const uint8_t *octects, int len);


	//murmur
	uint32_t murmur3(const uint8_t *octects, int len, uint32_t seed = 0);
}


#endif
