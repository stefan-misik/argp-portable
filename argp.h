#ifndef ARGP_H
#define	ARGP_H

/**
 * \brief Calback function
 */
typedef void (*argp_callback_t)(
    int id, 
    const char * value, 
    void * data
);

/**
 * \brief 
 */
typedef struct
{
    int             id;
    const char *    long_param;
    int             has_value;
} argp_params_t;

/**
 * /brief Parsing results
 */
typedef enum
{
    ARGP_OK = 0,
    ARGP_NO_PARAM
} argp_result_t;



/**
 * \brief Function to parse array of argumets to application parameters
 * 
 * \param[in]  conf     Array of structures which describe all possible 
 *                      parameters 
 * \param[in]  cb       Callback which is called when parameter is parsed
 * \param[in]  data     Data pointer which will be passed to callback
 * \param[in]  argc     Number of arguments in argv array 
 * \param[in]  argv     Array of arguments
 * 
 * \return 0 on success
 */
argp_result_t argp_parse(
    const argp_params_t * conf,
    argp_callback_t cb,
    void * data,
    int argc,
    const char ** argv    
);

/**
 * \brief Print usage string to standard output
 */
void arg_print_usage(
    void
);


#endif	/* #ifndef ARGP_H */

