#include <clk.h>
#include <common.h>
#include <gpio.h>
#include <printf.h>
#include <riscv64.h>
#include <stdio.h>
#include <stdlib.h>
#include <uart.h>
#include <TinyMaix_config.h>

void system_init() {
  sys_clock_init();
  sys_uart0_init();
  table_val_set();
}

void show_banner() {
  printf("    ___                    \r\n");
  printf("   /   |____  ___  ____ ___ \r\n");
  printf("  / /| /_  / / _ \\/ __ `__ \\\r\n");
  printf(" / ___ |/ /_/  __/ / / / / /\r\n");
  printf("/_/  |_/___/\\___/_/ /_/ /_/ \r\n");
  printf("=======================================\r\n");
  printf("Version: %s, Commit: %s\r\n", PROJECT_VER, PROJECT_GIT_HASH);
  printf("=======================================\r\n");
}

#define WATCHDOG_IRQ

int main(void) {
  system_init();
  show_banner();

  all_interrupt_enable();

  uint64_t i = 0;

  while (1) {
    printf("[TinyMaix INFO]: Tick Time: %d\r\n", i);
    i++;
  }
  return 0;
}
