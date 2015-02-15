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
 * \brief This structure describes properties of each known parameter
 */
typedef struct
{
	int             id;			/** < Identification of parameter, if is printable, can be used as short switch, e.g. -i */
	const char *    long_param;	/** < String describing long parameter form, e.g. --info */
	int             has_value;	/** < If non-zero, parser will expect value after switch */
} argp_params_t;

/**
 * /brief Parsing results
 */
typedef enum
{
	ARGP_OK = 0,	/** < Everything is ok */
	ARGP_NO_PARAM	/** < No arguments were supplied */
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


#endif	/* #ifndef ARGP_H */

