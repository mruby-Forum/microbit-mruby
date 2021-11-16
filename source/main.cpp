#include "MicroBit.h"
#include "mruby.h"
#include "mruby/dump.h"
#include "mruby/_string.h"

#define MBB_BASE_ADDR   0x00060000

MicroBit uBit;

extern "C" {
    void mrb_init_mrbgems2(mrb_state *mrb);
}

int 
main()
{
    extern void blinky();
    extern const uint8_t appbin[];
    uint8_t *bin = (uint8_t*)MBB_BASE_ADDR;

    const char *msg = "HELLO Micro:bit!\n";

    mrb_state *mrb = mrb_open();
    if (mrb) {
        msg = "Hello, microbit-mruby!\n";
    }
    // uBit.serial.printf("mrb_open() done.\n");
    uBit.serial.printf(msg);

    // init libraries for micro:bit
    int ai = mrb_gc_arena_save(mrb);
    mrb_init_mrbgems2(mrb);
    mrb_gc_arena_restore(mrb, ai);

    uBit.init();

    if (*((uint32_t*)bin) == 0xffffffff) {  // TODO: Check strictly MRB header
        uBit.serial.printf("Invalid MRB!\n");
        bin = (uint8_t*)appbin;
    }
    // else {
    //     uBit.serial.printf("Flash binary is OK!\n");
    // }

    mrb_load_irep(mrb, bin);
    if (mrb->exc) {
        mrb_value val = mrb_funcall(mrb, mrb_obj_value(mrb->exc), "inspect", 0);
        uBit.serial.printf("%s\n", mrb_str_to_cstr(mrb, val));
    }

    mrb_close(mrb);
    uBit.serial.printf("mruby VM closed.\n");

    // while(1)
    //     uBit.display.scroll("HELLO WORLD!");
    blinky();
}
