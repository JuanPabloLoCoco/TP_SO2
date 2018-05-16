#ifndef IDTLOADER_H
#define IDTLOADER_H

void load_idt();

#pragma pack(push)
#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de interrupcion */
typedef struct {
  uint16_t      offset_l,
            selector;
  uint8_t      cero,
            access;
  uint16_t   offset_m;
  uint32_t	    offset_h,
              other_cero;
} DESCR_INT;

#pragma pack(pop)

#endif
