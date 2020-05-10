#pragma once

#define ASSERT(expr) do {\
		if(!(expr)) {\
			PANIC("ASSERT FAILED: " #expr);\
		}\
	} while(0);
