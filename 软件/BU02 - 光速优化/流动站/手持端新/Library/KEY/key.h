#ifndef __KEY_H
#define __KEY_H

#include "headfile.h"

#define S1	PCin(13)
#define S2 	PCin(14)	
#define S3  PCin(15) 
#define S4  PAin(1) 

void KEY_Init(void);
u8 KEY_Scan(void);


#endif
