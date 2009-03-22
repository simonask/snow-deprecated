#include "Kernel.h"
#include "parser/Driver.h"
#include "Codegen.h"

#include <vector>
#include <string>

namespace snow {
	using namespace std;
	
	struct CmdOptions {
		string cmd;
		vector<string> filenames;
	};
	
	void parse_args(int argc, const char** argv, CmdOptions& opts) {
		opts.cmd = argv[0];
		for (int i = 1; i < argc; ++i) {
			opts.filenames.push_back(argv[i]);
			// TODO: Real option parsing.
		}
	}
	
	int main(int argc, const char** argv) {
		Kernel::init();
		
		CmdOptions opts;
		parse_args(argc, argv, opts);
		
		for each (iter, opts.filenames) {
			Kernel::require(*iter);
		}
		
		if (opts.filenames.size() == 0) {
			warn("no input files :)");
			return -1;
		}
		
		return 0;
	}
}

int main (int argc, const char** argv) {
	return snow::main(argc, argv);
}