
#include <stdio.h>
#include "argp.h"


argp_params_t argp_conf[] = 
{
    {'i', "info", 0},
    {'a', "ab", 1},
    {0}
};

void cb (int id, const char * val, void * data)
{
	if (id > 0)
	{
		if (NULL != val)
		{
			printf("Id: %c, Value: %s\n", id, val);
		}
		else
		{
			printf("Id: %c\n", id);
		}
	}
	else if (0 == id)
	{
		printf("Plain value: %s\n", val);
	}
	else if (id > -256)
	{
		printf("Unknown short switch: %c\n", -id);
	}
	else if (ARGP_ID_UNKNOWN_LONG_SWITCH == id)
	{
		printf("Unknown long switch: %s\n", val);
	}
	else if (ARGP_ID_LONG_SWITCH_BUFFER_OF == id)
	{
		printf("Long switch buffer overflow, switch starts with: %s\n", val);
	}
	else
	{
		/* Program should never get here */
		printf("Unknown error. \n");
	}
}

int main(int argc, char** argv) 
{
    /*arg_print_usage();*/
    
    
    argp_parse(argp_conf, cb, NULL, argc, (const char **)argv);
    
    
    return 0;
}


