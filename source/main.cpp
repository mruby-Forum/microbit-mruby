#include "MicroBit.h"
#include "mruby.h"
#include "mruby/dump.h"

MicroBit uBit;

int 
main()
{
    extern void blinky();
    extern const uint8_t appbin[];

    const char *msg = "HELLO Micro:bit!\n";

    mrb_state *mrb = mrb_open();
    if (mrb) {
        msg = "Hello, mruby!\n";
    }
    uBit.serial.printf("mrb_open() done.\n");
    uBit.serial.printf(msg);

    uBit.init();

    mrb_load_irep(mrb, appbin);
    if (mrb->exc) {
        uBit.serial.printf("mrb_load_irep(): Exception occured.\n");
    }

    mrb_close(mrb);
    uBit.serial.printf("mrb_close() done.\n");

    // while(1)
    //     uBit.display.scroll("HELLO WORLD!");
    blinky();
}
