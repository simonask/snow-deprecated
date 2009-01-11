#include "RefPtr.h"

#include <unordered_map>

namespace snow {
	RefCounter RefCounter::Null = RefCounter(NULL);
	
	static class StaticInitializer_RefPtr {
	public:
		StaticInitializer_RefPtr() {
			RefCounter::Null.retain();
		}
	} refptr_init;
}