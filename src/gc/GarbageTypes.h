#ifndef GARBAGETYPES_H_VYWTGWOY
#define GARBAGETYPES_H_VYWTGWOY

namespace snow {
	typedef void(*GarbageFreeFunc)(void* ptr, size_t size);
	
	// Constants that can be tweaked
	static const size_t NURSERY_SIZE = (1<<23);     // 1 Mb

	// Must be 0x10 (16), otherwise we can't tell pointers from immediates.
	static const size_t ALIGNMENT = 0x10;
	
	struct GarbageHeader {
		// sizeof(Header) should be == 16 (128 bits)
		unsigned size           : 32;
		unsigned flags          : 16;
		unsigned generation     : 16;
		GarbageFreeFunc free_func;  // : 64;
	};

	enum GarbageFlags {
		GC_NO_FLAGS        = 0,
		GC_FLAG_REACHABLE  = 1,        // Object is referenced, don't delete
		GC_FLAG_BLOB       = 1 << 1,   // Object doesn't have a destructor
		GC_FLAG_DESTRUCTED = 1 << 2,   // Object has been destroyed manually, so don't call destructor again.
	};
	
	enum GCOperation {
		MARK,
		UPDATE
	};
}

#endif /* end of include guard: GARBAGETYPES_H_VYWTGWOY */
