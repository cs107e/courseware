#include "spi.h"
#include "mcp3008.h"

void mcp3008_init(void)  {
    spi_init(SPI_MODE_0);
}

unsigned int mcp3008_read( unsigned int channel ) {
    uint8_t tx[3];
    uint8_t rx[3];

    // "Start bit", wakes up the ADC chip. 
    tx[0] = 1;
    // "Configuration byte", single mode + channel
    tx[1] = 0x80 | ((channel & 0x7) << 4);
    tx[2] = 0;

    spi_transfer(tx, rx, sizeof(tx));
    return ((rx[1] & 0x3) << 8) + rx[2];
}
