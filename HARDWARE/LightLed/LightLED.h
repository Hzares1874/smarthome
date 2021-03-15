#ifndef _LDR_H
#define _LDR_H

#define LDR PEin(8)
#define LDRLEDSTATUS PFout(10)

#include "stm32f4xx.h"
#include "sys.h"
#include "mqtt.h"
#include "control.h"
#include "string.h"

extern char *LEDSTATUS;

void LDR_Init(void);
void ldr_work(void);

#endif

