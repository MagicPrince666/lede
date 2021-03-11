#ifndef __LED_H__
#define __LED_H__


class XagLed
{
public:
    XagLed();
    ~XagLed();
    static void led_on(void);
    static void led_off(void);

private:
    static int led_fd;
};

#endif
