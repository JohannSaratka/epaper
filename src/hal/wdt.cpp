//*****************************************************************************
//
// wdt.cpp - Driver for the wdt+ Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup wdt_api wdt
//! @{
//
//*****************************************************************************

#include "inc/hw_memmap.h"

#include "wdt.h"

namespace hal
{
    WDT_Regs &wdt = *reinterpret_cast<WDT_Regs*>((uint16_t *)&WDTCTL);
}


void WDT_Regs::hold()
{
    // Set Hold bit
    uint16_t newWDTStatus = (WDTCTL & 0x00FF) | WDTHOLD;

    WDTCTL = WDTPW + newWDTStatus;
}

void WDT_Regs::start()
{
    // Reset Hold bit
    uint16_t newWDTStatus = (WDTCTL & 0x00FF) & ~(WDTHOLD);

    WDTCTL = WDTPW + newWDTStatus;
}

void WDT_Regs::resetTimer()
{
    // Set Counter Clear bit
    WDTCTL |= WDTPW + WDTCNTCL;
}

void WDT_Regs::initWatchdogTimer(ClockSource clockSelect,
    ClockDivider clockDivider)
{
    WDTCTL =
    WDTPW + WDTCNTCL + WDTHOLD + clockSelect + clockDivider;
}

void WDT_Regs::initIntervalTimer(ClockSource clockSelect,
    ClockDivider clockDivider)
{
    WDTCTL =
    WDTPW + WDTCNTCL + WDTHOLD + WDTTMSEL + clockSelect + clockDivider;
}

//*****************************************************************************
//
//! Close the doxygen group for wdt_api
//! @}
//
//*****************************************************************************
