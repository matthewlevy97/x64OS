#pragma once

#define isdigit(c) (((char)(c)) >= '0' && ((char)(c)) <= '9')

#define islower(c) (((char)(c)) >= 'a' && ((char)(c)) <= 'z')
#define isupper(c) (((char)(c)) >= 'A' && ((char)(c)) <= 'Z')
#define isalpha(c) (islower(c) || isupper(c))