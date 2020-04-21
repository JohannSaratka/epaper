//*****************************************************************************
//
// wdt.h - Driver for the WDT+ Module.
//
//*****************************************************************************

#ifndef __MSP430WARE_WDT_A_H__
#define __MSP430WARE_WDT_A_H__

#include "inc/hw_memmap.h"

class WDT_Regs
{
public:
    enum ClockSource : uint8_t
    {
        SOURCE_SMCLK = 0, SOURCE_ACLK = WDTSSEL,
    };

    enum ClockDivider : uint8_t
    {
        DIVIDER_32K = 0,
        DIVIDER_8192 = WDTIS0,
        DIVIDER_512 = WDTIS1,
        DIVIDER_64 = WDTIS0 + WDTIS1,
    };

    //*****************************************************************************
    //
    //! \brief Holds the Watchdog Timer.
    //!
    //! This function stops the watchdog timer from running, that way no interrupt
    //! or PUC is asserted.
    //!
    //! \return None
    //
    //*****************************************************************************
    void hold();

    //*****************************************************************************
    //
    //! \brief Starts the Watchdog Timer.
    //!
    //! This function starts the watchdog timer functionality to start counting
    //! again.
    //!
    //! \return None
    //
    //*****************************************************************************
    void start();

    //*****************************************************************************
    //
    //! \brief Resets the timer counter of the Watchdog Timer.
    //!
    //! This function resets the watchdog timer to 0x0000h.
    //!
    //! \return None
    //
    //*****************************************************************************
    void resetTimer();

    //*****************************************************************************
    //
    //! \brief Sets the clock source for the Watchdog Timer in watchdog mode.
    //!
    //! This function sets the watchdog timer in watchdog mode, which will cause a
    //! PUC when the timer overflows. When in the mode, a PUC can be avoided with a
    //! call to WDT_A_resetTimer() before the timer runs out.
    //!
    //! \param clockSelect is the clock source that the watchdog timer will use.
    //!        \see ClockSource
    //! \param clockDivider is the divider of the clock source, in turn setting the
    //!        watchdog timer interval. \see ClockDivider
    //!
    //! \return None
    //
    //*****************************************************************************
    void initWatchdogTimer(ClockSource clockSelect,
        ClockDivider clockDivider);

    //*****************************************************************************
    //
    //! \brief Sets the clock source for the Watchdog Timer in timer interval mode.
    //!
    //! This function sets the watchdog timer as timer interval mode, which will
    //! assert an interrupt without causing a PUC.
    //!
    //! \param clockSelect is the clock source that the watchdog timer will use.
    //!        \see ClockSource
    //! \param clockDivider is the divider of the clock source, in turn setting the
    //!        watchdog timer interval. \see ClockDivider
    //!
    //!
    //! \return None
    //
    //*****************************************************************************
    void initIntervalTimer(ClockSource clockSelect, ClockDivider clockDivider);
private:
    DeviceRegister16 WDTCTL;
};

namespace hal
{
    extern WDT_Regs &wdt;
}

#endif // __MSP430WARE_WDT_A_H__
