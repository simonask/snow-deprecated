#ifndef SUPERFASTHASH_H_IAMCSCY
#define SUPERFASTHASH_H_IAMCSCY

#include "base/Basic.h"
#include <string>

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
