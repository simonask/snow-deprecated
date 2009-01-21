#include "StackFrame.h"
#include <pthread.h>

namespace snow {
	static pthread_key_t current_stack_frame_key;
	
	static class StaticInitializer_StackFrames {
	public:
		StaticInitializer_StackFrames() {
			pthread_key_create(&current_stack_frame_key, NULL);
		}
	} _init;
	
	void init_stack_frame(StackFrame* frame) {
		StackFrame* previous = (StackFrame*)pthread_getspecific(current_stack_frame_key);
		frame->previous = previous;
		
		// self semantics: if call_self is set, use that, otherwise use
		// statically scoped self.
		if (frame->call_self) {
			frame->self = frame->call_self;
		} else {
			// XXX: Static self
		}
		
		for (uint64_t i = 0; i < frame->num_locals; ++i) {
			frame->locals[i] = NULL;
		}
		
		pthread_setspecific(current_stack_frame_key, (void*)frame);
	}
}