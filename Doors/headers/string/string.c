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
	//int compare_flag = 0;
	while(*first!='\0' && (*first==*second))//*second!='\0')//(*first==*second))
	{
		/*if(*first==*second)
		{
			compare_flag=10;
		}
		else
		{
			compare_flag=11;
		}*/
		first++;
		second++;
	}
	/*if(compare_flag==10)
		return compare_flag; //Za slucaj da su isti vracamo flag=10, odnosno 10
	else if(compare_flag==11)
		return compare_flag; //U suprotnom vracamo flag=11, odnosno 11
	else
		return;*/
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
