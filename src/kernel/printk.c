#include <io/serial.h>
#include <kernel/printk.h>
#include <stddef.h>
#include <stdarg.h>

static int putstr(char *s);
static void putchar(char c);
static char* convert(size_t number, int base, char* buf, size_t buf_len);

int printk(const char *format, ...)
{
	char buf[32];
	char *ptr;
	int bytes_written;
	
	va_list arg;
	va_start(arg, format);
	
	ptr = (char*)format;
	bytes_written = 0;
	while(*ptr) {
		if(*ptr != '%') {
			putchar(*ptr++);
			bytes_written++;
			continue;
		}
		
		ptr++;
		switch(*ptr) {
		case 0:
			// XXX: Undefined behavior
			break;
		case '%':
			putchar(*ptr);
			
			ptr++;
			bytes_written++;
			break;
		case 'c':
			putchar(va_arg(arg, int));
			
			ptr++;
			bytes_written++;
			break;
		case 'd':
			bytes_written += putstr(convert(va_arg(arg, unsigned long long), 10, buf, sizeof(buf)));
			ptr++;
			break;
		case 'x':
			bytes_written += putstr(convert(va_arg(arg, unsigned long long), 16, buf, sizeof(buf)));
			ptr++;
			break;
		}
	}
	
	return bytes_written;
}

static inline int putstr(char *s)
{
	int bytes_written = 0;
	while(*s) {
		putchar(*s++);
		bytes_written++;
	}
	return bytes_written;
}
static inline void putchar(char c)
{
	serial_write(c);
}

static char* convert(size_t number, int base, char* buf, size_t buf_len)
{
	static char hex_table[] = "0123456789ABCDEF";

	if(buf_len == 0)
		return NULL;
	
	buf[--buf_len] = '\0';
	if(buf_len == 0)
		return &buf[buf_len];

	do {
		buf[--buf_len] = hex_table[number % base];
		number /= base;
	} while(buf_len && number);

	return &buf[buf_len];
}