#ifndef ARGP_H
#define	ARGP_H

/**
 * \brief Id number returned to callback on unknown long switch
 */
#define ARGP_ID_UNKNOWN_LONG_SWITCH     -256

/**
 * \brief Id number returned to callback on long switch buffer overflow
 */
#define ARGP_ID_LONG_SWITCH_BUFFER_OF   -257

/**
 * \brief This structure describes properties of each known parameter
 */
typedef struct
{
    int             id;		/** < Identification of parameter, if is 
                                 *    printable, can be used as short 
                                 *    switch, e.g. -i */
    const char *    long_param;	/** < String describing long parameter 
                                 *    form, e.g. --info */
    int             has_value;	/** < If non-zero, parser will expect 
                                 *    value after switch */
} argp_params_t;

/**
 * /brief Parsing results / callback return values
 */
typedef enum
{
    ARGP_NO_MEM = -2,   /** < Could not allocate memory */
    ARGP_NO_PARAM = -1,	/** < No arguments were supplied */
    ARGP_OK = 0,	/** < Everything is ok */    
    ARGP_CB_BREAK = 1   /** < All positive integers returned by callback will
                         *    cause end to a parsing process and will be 
                         *    returned by argp_parse */
} argp_result_t;

/**
 * \brief Calback function
 */
typedef int (*argp_callback_t)(
    int id, 
    const char * value, 
    void * data
);

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
int argp_parse(
    const argp_params_t * conf,
    argp_callback_t cb,
    void * data,
    int argc,
    const char ** argv    
);


#endif	/* #ifndef ARGP_H */

