#pragma once

#ifndef PANIC
#define PANIC(...)
#endif

#define ASSERT(expr) do {\
		if(!(expr)) {\
			PANIC("ASSERT FAILED: " #expr);\
		}\
	} while(0);
