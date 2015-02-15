#include "argp.h"

#include <string.h>

/******************************************************************************/
typedef enum
{
    ARGP_PLAIN_VALUE = 0,
    ARGP_PARAM_SHORT,
    ARGP_PARAM_LONG,
    ARGP_PARAM_VALUE
} argp_states_t;


/******************************************************************************/
int static argp_get_conf_by_id(
    const argp_params_t *  params,
    const argp_params_t ** param,
    int id    
)
{
    while(params->id > 0)
    {
        if(id == params->id)
        {
            *param = params;
            return 0;
        }
        params ++;
    }
    return -1;
}

/******************************************************************************/
int static argp_get_conf_by_long(
    const argp_params_t *  params,
    const argp_params_t ** param,
    const char *   long_param    
)
{
    while(params->id > 0)
    {
        if(0 == strcmp(long_param, params->long_param))
        {
            *param = params;
            return 0;
        }
        params ++;
    }
    return -1;
}

/******************************************************************************/
argp_result_t argp_parse(
    const argp_params_t * params,
    argp_callback_t cb,
    void * data,
    int argc,
    const char ** argv    
)
{
    argp_states_t state = ARGP_PLAIN_VALUE;
    const char * cur_arg;
    int next_arg_no = 1;
    char c;
    static const char c0 = '\0';
    char long_param[ARGP_PARAM_MAX_LENGTH + 1];
    int long_param_pos = 0;
    const argp_params_t * cur_param = NULL;
    
    /* Check for number of arguments */
    if(argc <= 1)
    {
        return ARGP_NO_PARAM;
    }
    
    cur_arg = argv[next_arg_no++];    
    while(1)
    {
        c = *cur_arg;        
        if('\0' == c)
        {
            if(next_arg_no < argc)
            {
                cur_arg =argv[next_arg_no ++];
            }
            else
            {
                break;
            }
        }
        
        /* Here perform some magical state machine */
        switch(state)
        {
            /* Plain argument without any switch */
            case ARGP_PLAIN_VALUE:
                /* This argument is a switch */
                if('-' == *cur_arg)
                {
                    /* Assume it is a short switch */
                    state = ARGP_PARAM_SHORT;
                    /* Remove character from buffer */
                    cur_arg++;
                }
                else
                {
                    /* This is a plain value argument, send it to callback */
                    cb(0, cur_arg, data);
                    /* This argument is processed */
                    cur_arg = &c0;
                }
                break;
            
            /* Argument with a short switch, e.g. -i */
            case ARGP_PARAM_SHORT:                
                switch(c)
                {
                    /* This is not a short switch */
                    case '-':
                        state = ARGP_PARAM_LONG;
                        long_param_pos = 0;
                        /* Consume character */
                        cur_arg++;
                        break;
                        
                    /* Last short switch should not have value and there are 
                     * no more short switches */
                    case '\0':                        
                        state = ARGP_PLAIN_VALUE;
                        break;
                    
                    /* Decode switch */
                    default:
                        /* Consume character from argument buffer */
                        cur_arg++;
                        /* Find switch configurations */
                        if(0 == argp_get_conf_by_id(params, &cur_param, (int)c))
                        {
                            /* This switch should have a value */
                            if(cur_param->has_value > 0)
                            {
                                /* Value of this switch is the string that 
                                 * is placed after the switch */
                                if('\0' != *cur_arg)
                                {
                                    cb(cur_param->id, cur_arg, data);
                                    cur_param = NULL;
                                    cur_arg = &c0;
                                    state = ARGP_PLAIN_VALUE;
                                }
                                else
                                {
                                    /* Value will be next argument */
                                    state = ARGP_PARAM_VALUE;
                                }
                            }
                            else
                            {
                                /* Switch does not have value */
                                cb(cur_param->id, NULL, data);
                                cur_param = NULL;
                                /* Stay in ARGP_PARAM_SHORT state, other 
                                 * switches may follow */
                            }
                        }
                        else
                        {
                            /* Unknown short switch */
                            cb(-c, NULL, data);
                        }
                        break;
                }
                break;
                
            case ARGP_PARAM_LONG:               
                switch(c)
                {
                    case '=':
                        /* Move to beginning of the value string */
                        cur_arg++;
                        /* Purposely no break; here */
                    case '\0':                   
                        /* Add terminating zero after long_param name string */
                        long_param[long_param_pos] = '\0';
                        long_param_pos = 0;

                        /* Get info about long_param parameter */
                        if(0 == argp_get_conf_by_long(params, &cur_param, 
                            long_param))
                        {
                            if(cur_param->has_value > 0 && '-' != *cur_arg)
                            {                                
                                if('\0' == *cur_arg)
                                {
                                    /* After = sign, there are no parameters */
                                    cur_arg = NULL;
                                }
                                
                                cb(cur_param->id, cur_arg, data);
                                cur_arg = &c0;                            
                            }
                            else
                            {
                                cb(cur_param->id, NULL, data);                            
                            }
                            state = ARGP_PLAIN_VALUE;
                            cur_param = NULL;
                        }
                        else
                        {
                            /* Unknown long switch */
                            cb(ARGP_ID_UNKNOWN_LONG_SWITCH, long_param, data);
                            state = ARGP_PLAIN_VALUE;
                        }
                        break;
                        
                    default:  
                        if(long_param_pos < ARGP_PARAM_MAX_LENGTH)
                        {
                            long_param[long_param_pos++] = c;
                            cur_arg++;
                        }
                        else
                        {
                            /* Buffer for long param switch is not big enough*/
                            long_param[long_param_pos] = '\0';
                            long_param_pos = 0;
                            /* Tell it to callback */
                            cb(ARGP_ID_LONG_SWITCH_BUFFER_OF, long_param, data);
                            cur_arg = &c0;
                            state = ARGP_PLAIN_VALUE;
                        }
                        break;
                }
                break;
                
            /* Expecting short switch value */
            case ARGP_PARAM_VALUE:                
                if('-' == *cur_arg)
                {
                    /* No value was supplied */
                    cb(cur_param->id, NULL, data);
                    cur_param = NULL;                    
                    state = ARGP_PARAM_SHORT;
                    cur_arg++;
                }
                else
                {
                    /* Value was supplied */
                    cb(cur_param->id, cur_arg, data);
                    cur_param = NULL;
                    state = ARGP_PLAIN_VALUE;
                    cur_arg = &c0;
                }
                break;
        }        
    }
        
    if(long_param_pos > 0)
    {
        /* There is unprocessed long switch */
        long_param[long_param_pos] = '\0';
        long_param_pos = 0;
        
        /* Get info about long_param parameter */
        if(0 != argp_get_conf_by_long(params, &cur_param, 
            long_param))        
        {
            /* Unknown long switch */
            cb(-256, long_param, data);            
        }
    }
    
    /* Last parameter was expecting data, but no data were supplied */
    if(NULL != cur_param)
    {
        cb(cur_param->id, NULL, data);
        cur_param = NULL;
    }
    
    return ARGP_OK;
}
