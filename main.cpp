#include "Kernel.h"
#include "parser/Driver.h"
#include "Codegen.h"

#include <vector>
#include <string>
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>
#include <sstream>

namespace snow {
	using namespace std;
	
	struct CmdOptions {
		string cmd;
		vector<string> filenames;
		bool interactive;
	};
	
	void parse_args(int argc, const char** argv, CmdOptions& opts) {
		opts.cmd = argv[0];
		opts.interactive = false;
		for (int i = 1; i < argc; ++i) {
			std::string current(argv[i]);
			if (current == "--interactive" || current == "-i") {
				opts.interactive = true;
			} else {
				opts.filenames.push_back(argv[i]);
			}
			// TODO: Real option parsing.
		}
	}

	void interactive_prompt();
	
	int main(int argc, const char** argv) {
		Kernel::init();

		Kernel::require("prelude.sn");
		
		CmdOptions opts;
		parse_args(argc, argv, opts);
		
		for each (iter, opts.filenames) {
			Kernel::require(*iter);
		}
		
		if (opts.interactive) {
			interactive_prompt();
		} else if (opts.filenames.size() == 0) {
			warn("no input files :)");
			return -1;
		}
		
		return 0;
	}

	bool is_expr_unfinished(const std::string& str) {
		// return true if the number of starting and ending brackets is not equal
		
		// count quotation marks
		int dquot_count = 0;
		const char* c = str.c_str();
		bool escaped = false;
		int curly_count = 0;
		int bracket_count = 0;
		int parens_count = 0;
		bool in_string = false;
		while (*c) {
			if (*c == '\"' && !escaped)
			{
				dquot_count++;
			}
			if (!escaped && *c == '\\')
				escaped = true;
			else
				escaped = false;

			in_string = dquot_count & 1;

			if (!in_string) {
				switch (*c) {
					case '{': curly_count++; break;
					case '}': curly_count--; break;
					case '[': bracket_count++; break;
					case ']': bracket_count--; break;
					case '(': parens_count++; break;
					case ')': parens_count--; break;
				}
			}

			++c;
		}

		return in_string || curly_count || bracket_count || parens_count;
	}

	void interactive_prompt() {
		const char* global_prompt = "snow> ";
		const char* unfinished_prompt = "snow*> ";
		bool unfinished_expr = false;
		std::stringstream buffer;

		char* line;
		while ((line = readline(unfinished_expr ? unfinished_prompt : global_prompt)) != NULL) {
			if (*line) // strlen(line) != 0
				add_history(line);
			buffer << line;
			free(line);

			unfinished_expr = is_expr_unfinished(buffer.str());
			if (!unfinished_expr) {
				VALUE result = Kernel::eval_in_global_scope(buffer.str());
				std::cout << "=> " << value_to_string(snow::call_method(result, "inspect", 0)) << std::endl;
				buffer.str(""); 
			}
		}
	}
}

int main (int argc, const char** argv) {
	return snow::main(argc, argv);
}
