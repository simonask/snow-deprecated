#ifndef SUPERFASTHASH_H_IAMCSCY
#define SUPERFASTHASH_H_IAMCSCY

#include "Basic.h"

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
	
	/*
		Courtesy of Paul Hsieh, http://www.azillionmonkeys.com/qed/hash.html
	*/
	
	uint32_t super_fast_hash(const char* data, int len);
	
	class SuperFastHash {
	public:
		uint32_t operator()(const std::string& input) const {
			return super_fast_hash(input.c_str(), input.size());
		}
	};
}

#endif /* end of include guard: SUPERFASTHASH_H_IAMCSCY */
