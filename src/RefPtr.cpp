#include "RefPtr.h"

namespace snow {
	RefCounter RefCounter::Null = RefCounter(NULL);
	
	static class StaticInitializer_Null {
	public:
		StaticInitializer_Null() {
			RefCounter::Null.retain();
		}
	} null_init;
}