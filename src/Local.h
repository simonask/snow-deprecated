#ifndef LOCAL_H_DJFEOYD4
#define LOCAL_H_DJFEOYD4

#include <string>

namespace snot {
	class Local {
	private:
		static int ms_UnnamedCounter;
		std::string m_Name;
		int m_StackIndex;
	public:
		explicit Local(int index, const char* name = NULL);
		virtual ~Local() {}
		
		std::string name() const { return m_Name; }
		int index() const { return m_StackIndex; }
	};
}

#endif /* end of include guard: LOCAL_H_DJFEOYD4 */
