#include "test.h"
#include <iostream>
#include <sys/time.h>
using namespace std;

class Timer {
private:
	struct timeval m_Start;
public:
	Timer() {
		gettimeofday(&m_Start, NULL);
	}
	
	double seconds() {
		struct timeval end;
		gettimeofday(&end, NULL);
		double sec = end.tv_sec - m_Start.tv_sec;
		double usec = end.tv_usec - m_Start.tv_usec;
		return sec+(usec/1000000.0);
	}
};

int main (int argc, char *argv[])
{
	FILE* f;
	char git_line[81];
	memset(git_line, '\0', sizeof(git_line));
	f = popen("git log --pretty=oneline -n 1","r");
	fread(git_line,1,sizeof(git_line)-1,f);
	fclose(f);
	
	printf("Last commit: %s\n", git_line);
	Timer t;
	snow::test::Case::run_all(argc, argv);
	printf("Time: %.2lf seconds\n", t.seconds());
	return 0;
}
