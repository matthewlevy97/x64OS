#pragma once

#ifndef PANIC
#define PANIC(x)
#endif

#define ASSERT(expr) do {\
		if(!(expr)) {\
			PANIC("ASSERT FAILED: " #expr);\
		}\
	} while(0);
