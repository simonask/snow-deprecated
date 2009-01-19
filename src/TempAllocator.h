#ifndef TEMPALLOCATOR_H_BIJOGMF5
#define TEMPALLOCATOR_H_BIJOGMF5

#include "Basic.h"

namespace snow {
	template <typename T, size_t SPACE = 1 << 20>
	class TempAllocator {
	private:
		static byte* ms_Data;
		static size_t ms_Offset;
	public:
		void* operator new(size_t sz) throw() {
			if (!ms_Data)
				ms_Data = new byte[SPACE];
			if (ms_Offset + sz > SPACE)
				return NULL;
			void* ret = &ms_Data[ms_Offset];
			ms_Offset += sz;
			return ret;
		}
		inline void operator delete(void*) {}
		
		static void flush() {
			delete[] ms_Data;
			ms_Data = NULL;
			ms_Offset = 0;
		}
	};
	
	template <typename T, size_t SPACE>
	byte* TempAllocator<T, SPACE>::ms_Data = NULL;
	template <typename T, size_t SPACE>
	size_t TempAllocator<T, SPACE>::ms_Offset = 0;
}

#endif /* end of include guard: TEMPALLOCATOR_H_BIJOGMF5 */
