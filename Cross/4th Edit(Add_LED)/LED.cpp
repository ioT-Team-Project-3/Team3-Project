#define LED_DEV "/dev/led"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

const unsigned char ledArray[4] = {
    0xFE, //R
    0xFD, //Y
    0xF7, //B
};
class LED
{
private:
    /* data */
    int offset;
    int led_fd;

    void openLED() { led_fd = open(LED_DEV, O_WRONLY); }
    void closeLED() { close(led_fd); }

public:
    LED()
    {
        offset = 0;
        led_fd = -1;
    }

    void red() { offset = 0; }

    void yellow() { offset = 1; }

    void blue() { offset = 2; }

    void draw(int microSec)
    {
        openLED();
        write(led_fd, &ledArray[offset], sizeof(unsigned char));
        usleep(microSec);
        closeLED();
    }
};
