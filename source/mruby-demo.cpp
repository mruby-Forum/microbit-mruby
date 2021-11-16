#include "MicroBit.h"
#include "NRF52Pin.h"
#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/array.h"

extern MicroBit uBit;

extern "C" {

void mbprintf(const char *msg)
{
  uBit.serial.printf(msg);
}

static mrb_value
led_init(mrb_state *mrb, mrb_value self)
{
  mrb_int x=3, y=3;
  static bool init = false;

  // initialize display
  if (!init) {
    uBit.display.disable();
    init = true;
  }

  mrb_get_args(mrb, "|ii", &x, &y);
  uBit.serial.printf("LED.new(%d, %d)\n", x, y);

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@x"), mrb_fixnum_value(x));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@y"), mrb_fixnum_value(y));

  return self;
}

static void
set_io_row(mrb_int row, int v)
{
  switch (row) {
  case 1: uBit.io.row1.setDigitalValue(v); break;
  case 2: uBit.io.row2.setDigitalValue(v); break;
  case 3: uBit.io.row3.setDigitalValue(v); break;
  case 4: uBit.io.row4.setDigitalValue(v); break;
  case 5: uBit.io.row5.setDigitalValue(v); break;
  }
}

static void
set_io_col(mrb_int row, int v)
{
  switch (row) {
  case 1: uBit.io.col1.setDigitalValue(v); break;
  case 2: uBit.io.col2.setDigitalValue(v); break;
  case 3: uBit.io.col3.setDigitalValue(v); break;
  case 4: uBit.io.col4.setDigitalValue(v); break;
  case 5: uBit.io.col5.setDigitalValue(v); break;
  }
}

static mrb_value
led_on(mrb_state *mrb, mrb_value self)
{
  mrb_value ox = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@x"));
  mrb_value oy = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@y"));
  mrb_int x = mrb_fixnum(ox);
  mrb_int y = mrb_fixnum(oy);

  uBit.serial.printf("LED[%d, %d].on\n", x, y);

  set_io_row(y, 1);
  set_io_col(x, 0);

  return self;
}

static mrb_value
led_off(mrb_state *mrb, mrb_value self)
{
  mrb_value ox = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@x"));
  mrb_value oy = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@y"));
  mrb_int x = mrb_fixnum(ox);
  mrb_int y = mrb_fixnum(oy);

  uBit.serial.printf("LED[%d, %d].off\n", x, y);

  set_io_row(y, 1);
  set_io_col(x, 1);
  set_io_row(y, 0);

  return self;
}

static mrb_value
delay(mrb_state *mrb, mrb_value self)
{
  mrb_int ms;
  mrb_get_args(mrb, "i", &ms);
  uBit.sleep(ms);
  return mrb_nil_value();
}

static mrb_value
sleep(mrb_state *mrb, mrb_value self)
{
  mrb_float sec;
  mrb_get_args(mrb, "f", &sec);
  uBit.sleep((int)(sec * 1000.0f + 0.5f));
  return mrb_nil_value();
}

static mrb_value
dsp_scroll(mrb_state *mrb, mrb_value self)
{
  const char *str;
  mrb_get_args(mrb, "z", &str);
  uBit.display.scroll(str);
  return self;
}

static mrb_value
accel_get(mrb_state *mrb, mrb_value self)
{
  mrb_int x = uBit.accelerometer.getX();
  mrb_int y = uBit.accelerometer.getY();
  mrb_int z = uBit.accelerometer.getZ();
  uBit.serial.printf("acceler [%d, %d, %d]\n", x, y, z);
  mrb_value ary = mrb_ary_new_capa(mrb, 3);
  mrb_ary_push(mrb, ary, mrb_fixnum_value(x));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(y));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(z));
  return ary;
}

static mrb_value
accel_x(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(uBit.accelerometer.getX());
}

static mrb_value
accel_y(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(uBit.accelerometer.getY());
}

static mrb_value
accel_z(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(uBit.accelerometer.getZ());
}

static mrb_value
compass_get(mrb_state *mrb, mrb_value self)
{
  mrb_int x = uBit.compass.getX();
  mrb_int y = uBit.compass.getY();
  mrb_int z = uBit.compass.getZ();
  // uBit.serial.printf("Compass [%d, %d, %d]\n", x, y, z);
  mrb_value ary = mrb_ary_new_capa(mrb, 3);
  mrb_ary_push(mrb, ary, mrb_fixnum_value(x));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(y));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(z));
  return ary;
}

static mrb_value
compass_x(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(uBit.compass.getX());
}

static mrb_value
compass_y(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(uBit.compass.getY());
}

static mrb_value
compass_z(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(uBit.compass.getZ());
}

static mrb_value
mrb_stdout_putc(mrb_state *mrb, mrb_value self)
{
  char *str;
  size_t len;
  char cz[2] = {0, 0};
  mrb_get_args(mrb, "s", &str, &len);
  if (len > 0) {
    cz[0] = *str;
    uBit.serial.printf("%s", cz);
  }
  return mrb_fixnum_value(cz[0]);
}

#ifndef MRB_NO_GEMS
void
mrb_init_mrbgems2(mrb_state *mrb)
{
  mbprintf("mrb_init_mrbgems() start.\n");

  // LED class
  struct RClass *led = mrb_define_class(mrb, "LED", mrb->object_class);
  mrb_define_method(mrb, led, "initialize", led_init, MRB_ARGS_ARG(1, 1));
  mrb_define_method(mrb, led, "on", led_on, MRB_ARGS_NONE());
  mrb_define_method(mrb, led, "off", led_off, MRB_ARGS_NONE());

  // Display class
  struct RClass *dsp = mrb_define_class(mrb, "Display", mrb->object_class);
  mrb_define_class_method(mrb, dsp, "scroll", dsp_scroll, MRB_ARGS_REQ(1));

  // Kernel function
  mrb_define_method(mrb, mrb->kernel_module, "delay", delay, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb->kernel_module, "sleep", sleep, MRB_ARGS_REQ(1));

  // Accelerometer class
  struct RClass *acc = mrb_define_class(mrb, "Accelerometer", mrb->object_class);
  mrb_define_method(mrb, acc, "get", accel_get, MRB_ARGS_NONE());
  mrb_define_method(mrb, acc, "x", accel_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, acc, "y", accel_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, acc, "z", accel_z, MRB_ARGS_NONE());

  // Compass class
  struct RClass *cmp = mrb_define_class(mrb, "Compass", mrb->object_class);
  mrb_define_method(mrb, cmp, "get", compass_get, MRB_ARGS_NONE());
  mrb_define_method(mrb, cmp, "x", compass_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, cmp, "y", compass_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, cmp, "z", compass_z, MRB_ARGS_NONE());

  struct RClass *cout = mrb_define_class(mrb, "STDOUT", mrb->object_class);
  mrb_define_class_method(mrb, cout,  "putc", mrb_stdout_putc, MRB_ARGS_REQ(1));

  // mrb_define_method(mrb, mrb->object_class, "ppp", ppp, MRB_ARGS_REQ(1));

  // uBit.display.disable();

  // for (int i=0; i<10; i++) {
  //   led_on(mrb, mrb_nil_value());
  //   uBit.sleep(500);
  //   led_off(mrb, mrb_nil_value());
  //   uBit.sleep(500);
  // }
  // uBit.sleep(1000);

  mbprintf("mrb_init_mrbgems() end.\n");
}
#endif

}
