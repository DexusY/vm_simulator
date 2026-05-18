#include <stdio.h>
#include <stdint.h>

#include "vm_config.h"

// computes and stores ZF, CF, SF, OF based on a 16-bit result and the original operands
static void update_flags(CPU *cpu, uint16_t result16, uint8_t a, uint8_t b) {
    uint8_t result = (uint8_t)result16;
    cpu->flags = 0;
    if (result == 0)       cpu->flags |= FLAG_ZF;
    if (result16 > 0xFF)   cpu->flags |= FLAG_CF;
    if (result & 0x80)     cpu->flags |= FLAG_SF;
    // signed overflow: operands share a sign but result has a different sign
    if ((~(a ^ b) & (a ^ result)) & 0x80) cpu->flags |= FLAG_OF;
}

static void print_flags(CPU *cpu) {
    printf("[Flags] ZF=%d CF=%d SF=%d OF=%d\n",
        !!(cpu->flags & FLAG_ZF),
        !!(cpu->flags & FLAG_CF),
        !!(cpu->flags & FLAG_SF),
        !!(cpu->flags & FLAG_OF));
}