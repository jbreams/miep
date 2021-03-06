#ifndef __DEBUG_CONSOLE__H__
#define __DEBUG_CONSOLE__H__

#include <map>
#include <string>

#include <ncurses.h>

class processor;

typedef enum { C_WHITE = 0, C_GREEN, C_YELLOW, C_BLUE, C_MAGENTA, C_CYAN, C_RED } dc_color_t;

class debug_console
{
private:
	WINDOW *win_regs, *win_logs, *win_term;
	int max_x, max_y;
	bool nc;
	unsigned int refresh_counter, refresh_limit;
	bool refresh_limit_valid;
	bool had_logging;
	std::map<std::string, long int> instruction_counts;

	void recreate_terminal();
	void create_windows();

protected:
	double start_ts;
	long long int n_ticks;

public:
	debug_console();
	virtual ~debug_console();

	virtual void init();

	virtual void tick(processor *p);

	virtual void dc_log(const char *fmt, ...);

	virtual void dc_term(const char *fmt, ...);
};

#endif
