#include <stdio.h>
#include "argp.h"

/*
 * \brief   Aarray of parameters, this array defines all allowed switches.
 */
argp_params_t argp_conf[] = 
{
    {'i', "info", 0},
    {'t', "test", 1},
    {0}
};

/**
 * \brief Argp callback function
 * 
 * \param[in] id    Parameter identification 
 * \param[in] val   Value of parameter
 * \param[in] data  Data structure to obtain decoded parameters
 * 
 * \return Parameter decoding result
 */
int static cb (int id, const char * val, void * data)
{
    /* Switch was detected */
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
    /* Only value without switch */
    else if (0 == id)
    {
        printf("Plain value: %s\n", val);
    }
    /* Unknown short switch detected */
    else if (id > -256)
    {
        printf("Unknown short switch: %c\n", -id);
    }
    /* Unknown long switch detected */
    else if (ARGP_ID_UNKNOWN_LONG_SWITCH == id)
    {
        printf("Unknown long switch: %s\n", val);
    }
    /* Detected long switch longer than longest of defined long switches */
    else if (ARGP_ID_LONG_SWITCH_BUFFER_OF == id)
    {
        printf("Long switch buffer overflow, at switch starting with: %s\n", 
            val);
    }
    else
    {
            /* Program should never get here */
            printf("Unknown error. \n");
    }
    return ARGP_OK;
}

/**
 * \brief Application's entry point
 * 
 * \param[in] argc  Count of command line arguments
 * \param[in] argv  Array of command line arguments
 * 
 * \return Application's return code
 */
int main(int argc, char** argv) 
{
   
    switch(argp_parse(argp_conf, cb, NULL, 
        argc, (const char **)argv))
    {
        /* Arguments parsing complete */
        case ARGP_OK:
            return 0;
        
        /* No arguments were supplied */
        case ARGP_NO_PARAM:            
            return -1;       
        
    }   
    
    return 0;
}


