#include "xparameters.h"
#include "ucos_axiuartlite.h"

#ifdef __cplusplus
extern "C" {
#endif
void outbyte(char c); 

#ifdef __cplusplus
}
#endif 

void outbyte(char c) {
	 AXIUARTLite_SendByte(STDOUT_BASEADDRESS, c);
}
