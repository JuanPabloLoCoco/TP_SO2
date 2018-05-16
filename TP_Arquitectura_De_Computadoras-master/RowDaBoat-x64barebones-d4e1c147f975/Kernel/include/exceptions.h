#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 6
#define BREAKPOINT 3
#define OVERFLOW_ID 4
#define GENERAL_PROTECTION_FAULT 13
#define REGISTERS 17

static void zero_division(uint64_t* rsp);
static void invalid_opcode(uint64_t* rsp);
static void printRegisters(uint64_t* rsp);
static void overflow(uint64_t* rsp);
static void general_protection_fault(uint64_t* rsp);
static void breakpoint(uint64_t* rsp);

#endif
