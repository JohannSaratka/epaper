#ifndef __HW_MEMMAP__
#define __HW_MEMMAP__

//*****************************************************************************
//
// Include device specific header file
//
//*****************************************************************************
#include <msp430.h>

#include <cstdint>

//*****************************************************************************
//
// SUCCESS and FAILURE for API return value
//
//*****************************************************************************
#define STATUS_SUCCESS  0x01
#define STATUS_FAIL     0x00

//*****************************************************************************
//
// Macro for enabling assert statements for debugging
//
//*****************************************************************************
#define NDEBUG

//*****************************************************************************
//
// Macros for hardware access
//
//*****************************************************************************
typedef uint32_t volatile DeviceRegister32;
typedef uint16_t volatile DeviceRegister16;
typedef uint8_t volatile DeviceRegister8;

#endif // #ifndef __HW_MEMMAP__
