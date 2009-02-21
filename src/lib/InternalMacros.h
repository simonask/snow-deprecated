#ifndef INTERNALMACROS_H_2V3YBTK3
#define INTERNALMACROS_H_2V3YBTK3

#define SELF scope->self()
#define ARGS (*scope->arguments())
#define ARG(idx) scope->arguments()->get_by_index(idx)
#define NUM_ARGS scope->arguments()->length()

#endif /* end of include guard: INTERNALMACROS_H_2V3YBTK3 */
