#include "xparameters.h"
#include "ucos_axiuartlite.h"

#ifdef __cplusplus
extern "C" {
#endif
char inbyte(void);
#ifdef __cplusplus
}
#endif 

char inbyte(void) {
	 return AXIUARTLite_RecvByte(STDIN_BASEADDRESS);
}
