#ifndef INTERRUPS_H
#define INTERRUPS_H

#include <idtLoader.h>

void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);

void _systemCallsHandler(void);

void _exception0Handler(void);
void _exception3Handler(void);
void _exception4Handler(void);
void _exception6Handler(void);
void _exception13Handler(void);

void _change_process(uint64_t rsp);

void _yield_process();

void _yield_interrupt(void);

void _cli(void);

void _sti(void);

void _hlt(void);

void picMasterMask(uint8_t mask);

void picSlaveMask(uint8_t mask);

void _lidt(IDTR *i);

char *cpuVendor(char *result);

//Termina la ejecución de la cpu.
void haltcpu(void);

#endif
