#include "RegEx.h"
#include "Exception.h"

namespace snow {
	RegEx::RegEx(String* zpattern, OnigOptionType compilation_options, OnigEncoding encoding, OnigSyntaxType* syntax)
	: m_Handle(NULL)
	{
		const OnigUChar* cstr = (const OnigUChar*)zpattern->c_str();
		int len = zpattern->length();
		OnigErrorInfo err_info;
		int r = onig_new(&m_Handle, cstr, &cstr[len], compilation_options, encoding, syntax, &err_info);
		if (r != ONIG_NORMAL) {
			OnigUChar err_msg[ONIG_MAX_ERROR_MESSAGE_LEN];
			onig_error_code_to_str(err_msg, r, &err_info);
			// TODO: Consider this...
			throw_exception(gc_new<String>("Regular Expression Error: %", gc_new<String>((const char*)err_msg)));
		}
	}

	RegEx::~RegEx() {
		onig_free(m_Handle);
	}

	RegExMatch* RegEx::search(String* str, OnigOptionType options) {
		HandleScope _;
		const OnigUChar* string_start = (const OnigUChar*)str->c_str();
		size_t len = str->length();
		const OnigUChar* string_end = &string_start[len];
		const OnigUChar* search_start = string_start;
		const OnigUChar* search_range = string_end;

		OnigRegion* region = onig_region_new();
		Handle<RegExMatch> match = NULL;
		Handle<RegEx> THIS = this;

		int result = onig_search(m_Handle, string_start, string_end, search_start, search_range, region, options);
		if (result >= 0) {
			match = gc_new<RegExMatch>(THIS);
			for (int i = 0; i < region->num_regs; ++i) {
				// TODO: Optimize this
				const OnigUChar* match_start = &string_start[region->beg[i]];
				int match_len = region->end[i] - region->beg[i];
				char* match_string = new char[match_len+1];
				match_string[match_len] = '\0';
				memcpy(match_string, match_start, match_len);
				debug("match %d: %s", i, match_string);
				match->set_match(i+1, gc_new<String>(match_string));
				delete[] match_string;
			}
		}

		onig_region_free(region, 1);

		return match;
	}

	int RegEx::number_of_names() const {
		return onig_number_of_names(m_Handle);
	}

	static int add_names_to_array(const OnigUChar* start, const OnigUChar* end, int num_groups, int*, regex_t*, void* arg) {
		HandleScope _;
		Handle<Array>* parray = (Handle<Array>*)arg;
		size_t len = end-start;
		char* cstr = new char[len+1];
		cstr[len] = '\0';
		Handle<String> string = gc_new<String>(cstr);
		delete[] cstr;
		(*parray)->push(string);
		return 0;
	}

	Array* RegEx::names() {
		HandleScope _;
		Handle<RegEx> THIS = this;
		Handle<Array> array = gc_new<Array>();
		array->preallocate(number_of_names());
		onig_foreach_name(THIS->m_Handle, add_names_to_array, &array);
		return array;
	}

	int RegEx::number_of_groups() const {
		return onig_number_of_captures(m_Handle);
	}

	/// RegExMatch --------------------------

	String* RegExMatch::get_match(int group) const {
		if (!m_Matches)
			return NULL;
		ASSERT((size_t)group <= m_Matches->length());
		VALUE str = (*m_Matches)[group-1];
		String* s = object_cast<String>(str);
		ASSERT(s);
		return s;
	}

	void RegExMatch::set_match(int group, String* str) {
		HandleScope _;
		Handle<RegExMatch> THIS = this;
		if (!THIS->m_Matches) {
			THIS->m_Matches = gc_new<Array>();
			THIS->m_Matches->preallocate(THIS->m_RegEx->number_of_groups());
		}

		THIS->m_Matches->set_by_index(group-1, str);
	}

	Array* RegExMatch::get_matches(String* group_name) {
		HandleScope _;
		Handle<RegExMatch> THIS = this;
		const UChar* start = (const OnigUChar*)group_name->c_str();
		size_t len = group_name->length();
		const UChar* end = &start[len];
		int* group_numbers;
		int r = onig_name_to_group_numbers(THIS->m_RegEx->m_Handle, start, end, &group_numbers);
		if (r <= 0)
			return NULL;

		Handle<Array> array = gc_new<Array>();
		array->preallocate(r);
		for (int i = 0; i < r; ++i) {
			array->push(get_match(group_numbers[i]));
		}
		return array;
	}
}


