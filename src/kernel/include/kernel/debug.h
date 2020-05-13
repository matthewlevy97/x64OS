#pragma once

#include <kernel/printk.h>

#define S__LINE__ STR_(__LINE__)

#ifdef _KERNEL_DEBUG
	#define debug(...) printk(__VA_ARGS__)
	#define debug_info(...) \
		do {debug("[INFO]    "); debug(__VA_ARGS__);} while(0);
	#define debug_ok(...) \
		do {debug("[OK]      "); debug(__VA_ARGS__);} while(0);
	#define debug_warning(...) \
		do {debug("[WARNING] "); debug(__VA_ARGS__);} while(0);
	#define debug_error(...) \
		do {debug("[ERROR]   "); debug(__VA_ARGS__);} while(0);
#else
	#define debug(...)
	#define debug_info(...)
	#define debug_ok(...)
	#define debug_warning(...)
	#define debug_error(...)
#endif

#undef PANIC
#define PANIC(...) do {\
		debug("\n\n-----------------------------\n");\
		debug("KERNEL PANIC! [%s:%d]\n", __FILE__, __LINE__);\
		debug("Git Hash:   " GITHASH "\n");\
		debug("Git Date:   " GITDATE "\n");\
		debug("Git Branch: " GITBRANCH "\n");\
		debug(__VA_ARGS__);\
		\
		volatile int _override = 0;\
		while(!_override);\
		\
		debug("\n");\
	} while(0);
