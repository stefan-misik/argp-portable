
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
    if(NULL != val)
    {
        printf("Id: %c, Value: %s\n", id, val);
    }
    else
    {
        printf("Id: %c\n", id);
    }
}

int main(int argc, char** argv) 
{
    /*arg_print_usage();*/
    
    
    argp_parse(argp_conf, cb, NULL, argc, (const char **)argv);
    
    
    return 0;
}


