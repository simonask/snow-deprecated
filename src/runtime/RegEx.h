#ifndef _REGEX_H_45HSAJSDG
#define _REGEX_H_45HSAJSDG

#include "runtime/Object.h"
#include "runtime/SnowString.h"
#include "runtime/Array.h"
#include "3rdparty/onig/oniguruma.h"

namespace snow {
	Object* regex_prototype();
	Object* regex_match_prototype();

	class RegExMatch;

	class RegEx : public ThinObject {
		friend class RegExMatch;
	private:
		regex_t* m_Handle;
	public:
		NO_INIT;

		RegEx(String* zpattern,	OnigOptionType compilation_options = ONIG_OPTION_NONE, OnigEncoding encoding = ONIG_ENCODING_UTF8, OnigSyntaxType* syntax = ONIG_SYNTAX_DEFAULT);
		~RegEx();
		IObject* copy() const { return NULL; }

		Array* search(String* str, OnigOptionType options = ONIG_OPTION_NONE);
		RegExMatch* match(String* str, OnigOptionType options = ONIG_OPTION_NONE);

		int number_of_names() const;
		Array* names();
		int number_of_groups() const;

		static const char* oniguruma_version();
	};


	class RegExMatch : public ThinObject {
	private:
		RegEx* m_RegEx;
		Array* m_Matches;
		GC_ROOTS { GC_SUPER(ThinObject); GC_ROOT(m_RegEx); GC_ROOT(m_Matches); }

		IObject* copy() const { return NULL; }
	public:
		NO_INIT;

		RegExMatch(RegEx* regex) : m_RegEx(regex), m_Matches(NULL) {}
		~RegExMatch() {}

		String* get_match(int group) const;
		void set_match(int group, String*);
		Array* get_matches(String* group_name);

		size_t num_matches() const { return m_Matches ? m_Matches->length() : 0; }
	};
}

#endif // _REGEX_H_45HSAJSDG

