#include <stdint.h>
#include <idtLoader.h>
#include <defs.h>
#include <interrupts.h>

DESCR_INT idt[255];	// IDT de 255 entradas
IDTR idtr;			// IDTR

static void setup_IDT_entry (int index, uint64_t offset);


void load_idt()
{
  _cli();

  setup_IDT_entry (0x00, (uint64_t)&_exception0Handler);
  setup_IDT_entry (0x03, (uint64_t)&_exception3Handler);
  setup_IDT_entry (0x04, (uint64_t)&_exception4Handler);
  setup_IDT_entry (0x06, (uint64_t)&_exception6Handler);
  setup_IDT_entry (0x0D, (uint64_t)&_exception13Handler);
  setup_IDT_entry (0x20, (uint64_t)&_irq00Handler);
  setup_IDT_entry (0x21, (uint64_t)&_irq01Handler);
  setup_IDT_entry (0x70, (uint64_t)&_yield_interrupt);
  setup_IDT_entry (0x80, (uint64_t)&_systemCallsHandler);

  idtr.base = 0;
	idtr.base += (uint64_t) &idt;
	idtr.limit = (uint16_t) sizeof(idt)-1;

	_lidt(&idtr);

  picMasterMask(0xFC);
	picSlaveMask(0xFF);

  _sti();
}

static void setup_IDT_entry (int index, uint64_t offset)
{
  idt[index].selector = 0x08;
  idt[index].offset_l = offset & 0xFFFF;
  idt[index].offset_m = (offset >> 16) & 0xFFFF;
  idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
  idt[index].access = ACS_INT;
  idt[index].cero = 0;
  idt[index].other_cero = (uint64_t) 0;
}
