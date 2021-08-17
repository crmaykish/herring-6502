#include "pico/stdlib.h"

int main()
{
    const uint LED = PICO_DEFAULT_LED_PIN;

    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    while(true)
    {
        gpio_put(LED, 1);
        sleep_ms(2000);
        gpio_put(LED, 0);
        sleep_ms(2000);
    }

}
