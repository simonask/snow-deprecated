#ifndef IGARBAGE_H_ATSZNMRU
#define IGARBAGE_H_ATSZNMRU

namespace snow {
	class IGarbage {
	public:
		virtual ~IGarbage() {}
		virtual void gc_mark() = 0;
	};
}

#endif /* end of include guard: IGARBAGE_H_ATSZNMRU */
