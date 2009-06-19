#include "SuperFastHash.h"

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

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
			data += 2*sizeof(uintx);
			hash += hash >> 11;
		}
		
		switch (rem) {
			case 3:
				hash += get16bits(data);
				hash ^= hash << 16;
				hash ^= data[sizeof(uintx)] << 18;
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