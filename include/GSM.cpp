#include "GSM.h"

GSM::GSM(uint8_t rx, uint8_t tx): SIM800L(rx, tx)
{
    Rx_Pin = rx;
    Tx_Pin = tx;
}