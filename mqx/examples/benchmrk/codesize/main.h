#ifndef __main_h_
#define __main_h_


#define MAIN_TASK 1


extern void Main_task(uint_32);



  
#if CSCFG_MAX 
    #define IF_MAX(x)       x
    #define IF_TYPICAL(x)   x
    #define IF_SMALL(x)     x
    #define IF_TINIEST(x)   x
#elif CSCFG_TYPICAL 
    #define IF_MAX(x)       
    #define IF_TYPICAL(x)   x
    #define IF_SMALL(x)     x
    #define IF_TINIEST(x)   x
#elif CSCFG_SMALL
    #define IF_MAX(x)       
    #define IF_TYPICAL(x)   
    #define IF_SMALL(x)     x
    #define IF_TINIEST(x)   x
#elif CSCFG_TINIEST
    #define IF_MAX(x)       
    #define IF_TYPICAL(x)   
    #define IF_SMALL(x)     
    #define IF_TINIEST(x)   x
#else
    #define IF_MAX(x)       
    #define IF_TYPICAL(x)   
    #define IF_SMALL(x)     
    #define IF_TINIEST(x)      
#endif




#endif /* __main_h_ */

