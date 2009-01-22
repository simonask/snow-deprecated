#include "SuperFastHash.h"

namespace snow {
	uint32_t super_fast_hash(const char* data, int len) {
		uint32_t hash = len;
		uint32_t tmp;
		
		if (len <= 0 || !data)
			return 0;
			
		int32_t rem = len & 3;
		len >>= 2;
			
		/* Main loop */
		for (; len > 0; --len) {
			hash += get16bits(data);
			tmp = (get16bits(data+2) << 11) ^ hash;
			hash = (hash << 16) ^ tmp;
			data += 2*sizeof(uint64_t);
			hash += hash >> 11;
		}
		
		switch (rem) {
			case 3:
				hash += get16bits(data);
				hash ^= hash << 16;
				hash ^= data[sizeof(uint64_t)] << 18;
				hash += hash >> 11;
				break;
			case 2:
				hash += get16bits(data);
				hash ^= hash << 11;
				hash += hash >> 17;
				break;
			case 1:
				hash += *data;
				hash ^= hash << 10;
				hash += hash >> 1;
				break;
		}
		
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;
		
		return hash;
	}
}