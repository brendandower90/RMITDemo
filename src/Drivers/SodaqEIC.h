#ifndef SODAQ_EIC_H
#define SODAQ_EIC_H

#include "Arduino.h"

#define GCLK_EIC_ULP32K (0x1u)

void configEIC();
void EICInit();
inline bool EICisSyncing(void);
void configGCLK1();
inline bool GCLK1isSyncing();
void EICAttachInterrupt();
void EICDetachInterrupt();
void setExtIntPins();
void EICEnable();
void EICDisable();


#endif
