#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
//#include "log.h"
#include "led.h"

#define SYSFS_LED "/sys/class/leds/xag9342:sts/brightness"

int XagLed::led_fd = -1;

XagLed::XagLed(void) {
	led_fd = open(SYSFS_LED, O_RDWR);
	if(led_fd < 0){
        //logError("xag led error\n");
        printf("xag led error\n");
    }
}

XagLed::~XagLed(void) {
	close(led_fd);
	led_fd = NULL;
}

void XagLed::led_on(void) {
	write(led_fd, "1", 1); //LED ON
}

void XagLed::led_off(void) {
	write(led_fd, "0", 1); //LED OFF
}

