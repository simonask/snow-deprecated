#include "StackFrame.h"
#include <pthread.h>

namespace snow {
	static pthread_key_t current_stack_frame_key;
	
	class StaticInitializer_StackFrames {
	public:
		StaticInitializer_StackFrames() {
			pthread_key_create(&current_stack_frame_key, NULL);
		}
	} _init;
	
	void create_stack_frame(StackFrame* frame) {
		StackFrame* previous = (StackFrame*)pthread_getspecific(current_stack_frame_key);
		frame->previous = previous;
		pthread_setspecific(current_stack_frame_key, (void*)frame);
	}
}