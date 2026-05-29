#include "string.h"

int strlength(char* string)
{
	int i;
	while(string[i]!='\0')
	{
		i++;
	}
	return i;
}

char* strcopy(char* destination, const char* source)
{
	char* destptr = destination;
	while(*source!='\0')
	{
		*destptr++ = *source++; //kopiramo karakter iz source u destptr i uvecavamo ih nakon toga (prelazimo na sledeci char)
	}
	*destptr = '\0';
	return destination;
}

int strcompare(const char* first, const char* second)
{
	while(*first!='\0' && (*first==*second))//*second!='\0')//(*first==*second))
	{
		first++;
		second++;
	}
	return *(unsigned char*)first - *(unsigned char*)second;
}

char* strncopy(char* destination, const char* source, int n)
{
	char* destptr = destination;
	while(n--)
	{
		*destptr++ = *source++;
	}
	*destptr = '\0';
	return destination;
}
