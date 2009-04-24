#ifndef IGARBAGECOLLECTOR_H_5HJSAS
#define IGARBAGECOLLECTOR_H_5HJSAS

namespace snow {
	class IGarbageCollector {
	public:
		enum GCOperation {
			MARK,
			UPDATE
		};

		virtual void root_callback(GCOperation op, void*& root) = 0;
		virtual void collect() = 0;
	};
}

#endif // IGARBAGECOLLECTOR_H_5HJSAS
