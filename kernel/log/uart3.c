
/***************************************************************************

 MODULE:  uart3 

 DESCRIPTION:  Early kernal initialization uart code to send blocked
  data out the port.

 AUTHOR: Chen Zhigang


 Copyright (c) 2005 by HP Incorporated.  All Rights Reserved.
*****************************************************************************/



/*===========================================================================
 when       who     what, where, why
--------   ---     ----------------------------------------------------------
 6/10/08   czg   Original code 
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "boothw.h"
#include "clkregim.h"
#include "bsp.h"
#include "tlmm_gpio_msm.h"
#include "uart3.h"
#include "stdarg.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/*===========================================================================
  Constant / Define Declarations
===========================================================================*/
#define NUM_BAUD_ENTRY  5
#define CONFIG_BAUDRATE 115200

#define HWIO_UART3_IPR_OUT HWIO_UART3_UART_IPR_OUT
#define HWIO_UART3_SIM_CFG_OUT HWIO_UART3_UART_SIM_CFG_OUT
#define HWIO_UART3_HCR_OUT HWIO_UART3_UART_HCR_OUT
#define HWIO_UART3_SR_IN HWIO_UART3_UART_SR_IN
#define HWIO_UART3_SR_RXRDY_BMSK HWIO_UART3_UART_SR_RXRDY_BMSK
#define HWIO_UART3_SR_TXRDY_BMSK HWIO_UART3_UART_SR_TXRDY_BMSK
//////

#define	GPIO_92_SHIFT	24
#define	GPIO_92_MASK	0x1000000

//#define CONFIG_BUDRATE 38400

/*===========================================================================
  Type Declarations
===========================================================================*/

/*-------------------------------------------------------------------------
   Baud rate structure for configuration.  What rate and the constant needed
   to set that rate in the clock settings.
 --------------------------------------------------------------------------*/
struct baud_struct {
    unsigned int desired_baudrate;
    unsigned short csr_clk_sel_val;
};

/*===========================================================================
  Local Object Definitions
===========================================================================*/
struct  baud_struct  baud_set[NUM_BAUD_ENTRY] = {
                   {9600,   0x0099},
                   {19200,  0x00BB},
                   {38400,  0x00DD},
                   {57600,  0x00EE},
                   {115200, 0x00FF}
};

/*===========================================================================
  Forward Declarations
===========================================================================*/


/*===========================================================================
                              Function Definitions
===========================================================================*/

static int uart_initialized = 0;

/*===========================================================================
FUNCTION serial_init

DESCRIPTION Initialize the serial port to the baud defined above
      
DEPENDENCIES
  None
                                                       
RETURN VALUE
    
SIDE EFFECTS
  None
  
===========================================================================*/
static int serial_init (void)
{

    uart_initialized = 1;

	gpio_tlmm_config(UART3_RX_DATA);
	gpio_tlmm_config(UART3_TX);
 
  /* Set the UART clock to TCXO/4 */
    clk_regime_msm_disable(CLKRGM_UART3_CLK);
    clk_regime_sel_uart_clk_src(CLKRGM_UART3_CLK, CLKRGM_UART_CLK_SRC_TCX04);
    clk_regime_msm_enable(CLKRGM_UART3_CLK);


      
    HWIO_OUT(UART3_UART_IRDA, 0x00);  /* Ensure that IRDA mode is disabled */
	HWIO_OUT(UART3_UART_MREG, 0x00C0);  // M/N Counter Clock Setup
	HWIO_OUT(UART3_UART_NREG, 0x00B2);  // M/N Counter Clock Setup
	HWIO_OUT(UART3_UART_DREG, 0x007D);  // M/N Counter Clock Setup
	HWIO_OUT(UART3_UART_MNDREG,0x001C);  // M/N Counter Clock Setup
	HWIO_OUT(UART3_UART_CR, 0x0015);     // Reset Rx 
	HWIO_OUT(UART3_UART_CR, 0x0025);     // Reset Tx 
	HWIO_OUT(UART3_UART_CR, 0x0035);     // Reset Error status 
	HWIO_OUT(UART3_UART_CR, 0x0045);     // Reset Rx Break interrupt 
	HWIO_OUT(UART3_UART_CR, 0x0075);     // Reset CTS Status 
	HWIO_OUT(UART3_UART_CR, 0x00D5);     // Reset RFR  
	HWIO_OUT(UART3_UART_IPR, 0x07BF);    // Stale_Timeout is 630 * Bit rate
	HWIO_OUT(UART3_UART_IMR, 0x0002);    // mask all but RXHUNT interrupt
	HWIO_OUT(UART3_UART_RFWR, 115);      // Rx watermark = (58 x 2) - 1 = 115  
	HWIO_OUT(UART3_UART_TFWR, 400);      // Tx watermark = 400 
	HWIO_OUT(UART3_UART_CSR, 0x00FF);    // Set UART Baud Rate  =115200
    HWIO_OUT(UART3_UART_SIM_CFG, 0x0000);// IRDA = 0x1 , RS232 = 0x0
	HWIO_OUT(UART3_UART_HCR, 0x001E);    // HUNT character is 254 = 0xFE 
	HWIO_OUT(UART3_UART_MR1, 0x0074);     // RFR/ and CTS/ on, 500 character character RFR/ threshhold
	HWIO_OUT(UART3_UART_MR2, 0x0034);    // 8N1 format
	HWIO_OUT(UART3_UART_CR, 0x0005);     // Enable Tx and Rx 

    return 0;
}

/*===========================================================================
FUNCTION check_tx_ready

DESCRIPTION
     See if the transmitter on the serial port is ready.
 
DEPENDENCIES
  None
                                                       
RETURN VALUE
   0 not ready
   !0 ready
    
SIDE EFFECTS
  None
  
===========================================================================*/
static int check_tx_ready (void)
{

    unsigned int data;

    data = HWIO_IN(UART3_UART_SR);

    return (data & HWIO_UART3_SR_TXRDY_BMSK);

}

/*===========================================================================
FUNCTION check_tx_empty

DESCRIPTION
     See if the transmitter on the serial port is done sending bytes (FIFO is empty).
 
DEPENDENCIES
  None
                                                       
RETURN VALUE
  0 = there are still bytes to be sent
  1 = TX queues is empty, no bytes pending TX.
    
SIDE EFFECTS
  None
  
===========================================================================*/

static int check_tx_empty(void)
{
    unsigned long data;

    data = HWIO_IN(UART3_UART_SR); 

    return (data & HWIO_UART3_SR_RXRDY_BMSK);

}

/*===========================================================================
FUNCTION check_rx_ready

DESCRIPTION
       See if the receiver has a character.
 
DEPENDENCIES
  None
                                                       
RETURN VALUE
  0 no character
  !0 character ready    

SIDE EFFECTS
  None
  
===========================================================================*/
static int check_rx_ready (void)
{
    unsigned long data;

    data = HWIO_IN(UART3_UART_SR);

    return (data & HWIO_UART3_SR_RXRDY_BMSK);


}

/*===========================================================================
FUNCTION serial_putc

DESCRIPTION
     Put a character out to the serial port. 
     Waits for the seril port to become ready...or a timeout loop 

DEPENDENCIES
  None
                                                       
RETURN VALUE
    
SIDE EFFECTS
  None
  
===========================================================================*/
static void serial_putc (char c)
{
	int i=0;
    if (uart_initialized)
	{  
	    while (!check_tx_ready ()&& i<1000)
	    {
	    	i++;
	    }

	    HWIO_OUT(UART3_UART_TF,c);
	}
}


/*===========================================================================
FUNCTION serial_puts

DESCRIPTION
     Put a string out to the serial port.
 
DEPENDENCIES
  None
                                                       
RETURN VALUE
    
SIDE EFFECTS
  None
  
===========================================================================*/
static void serial_puts (const char *s)
{
	while (*s) {
	  serial_putc (*s++);
	}
}

/*===========================================================================
FUNCTION  serial_getc

DESCRIPTION
      get a char from the uart 
DEPENDENCIES
  None
                                                       
RETURN VALUE
   the char on success
   -1 if timeout
 
SIDE EFFECTS
  None
  
===========================================================================*/
static int serial_getc (void)
{
	int data;
	int i=0;

    if (uart_initialized)
    {
    /* Wait until there is a char in the RX FIFO */
    while (!check_rx_ready ()&& i<1000)
    {
    	i++;
    }

    /* read the character */
    data = HWIO_IN(UART3_UART_RF); 
    }
    else
    {
        data = -1;
    }

    return  data;
}


/*===========================================================================
FUNCTION   OEMDebugInit

DESCRIPTION
   Initialze the serial port for debug and show that we are ready.
   
DEPENDENCIES

RETURN VALUE
   TRUE
   
SIDE EFFECTS
   Displays message on the serial port
===========================================================================*/
#if defined(DEBUG_AMSS)
int OEMDebugInit(void)
{
  serial_init();
  serial_puts("AMSS Debug Serial Port Ready on UART3...\r\n");

  return 1;
}
#else
int OEMDebugInit(void)
{
}
#endif

/*===========================================================================
FUNCTION   OEMDebugDeinit

DESCRIPTION
   Deinitialze the serial port.
   
DEPENDENCIES

RETURN VALUE
   TRUE
   
SIDE EFFECTS
   Serial Debug Stops dumping data
===========================================================================*/
#if defined(DEBUG_AMSS)
void OEMDebugDeinit(void)
{
    uart_initialized = 0;
}
#else
void OEMDebugDeinit(void)
{
}
#endif
/*===========================================================================
FUNCTION   OEMWriteDebugByte

DESCRIPTION
   Write a character out the debug serial port.
   
DEPENDENCIES

RETURN VALUE
   None
   
SIDE EFFECTS
  Character is sent out the serial port.
===========================================================================*/
static void OEMWriteDebugByte(byte b)
{
  serial_putc(b);
}


/*===========================================================================
FUNCTION   OEMReadDebugByte

DESCRIPTION
   Read a byte of data from the serial port.  This is a WinCE prototype
   to support the OAL layer.
   
DEPENDENCIES

RETURN VALUE
   Returns the character at the serial port or OEM_DEBUG_READ_NODATA if no
   data is available.
   
SIDE EFFECTS

===========================================================================*/
static int OEMReadDebugByte(void)
{
    if ( uart_initialized == 0 || 0 == check_rx_ready())
    {
        return -1;
    }

    return serial_getc();

}

/*===========================================================================
FUNCTION   OutputChar

DESCRIPTION

  When using the UART for outputting debug messages, we queue the character to a FIFO which is serviced
  by a low priority user task. Although this requires extra memory, it is necessary since the UART HW FIFO
  is shallow and blocking the caller results in a large performance hit.

  The only exception is very early in the bootup, when the user-mode driver is not up.
   
DEPENDENCIES
   None
   
RETURN VALUE
   None.
   
SIDE EFFECTS

===========================================================================*/

static void OutputChar(const unsigned char c)
{
    OEMWriteDebugByte(c);
}


/*===========================================================================
FUNCTION   OutputString

DESCRIPTION

  When using the UART for outputting debug messages, we queue the character to a FIFO which is serviced
  by a low priority user task. Although this requires extra memory, it is necessary since the UART HW FIFO
  is shallow and blocking the caller results in a large performance hit.

  The only exception is very early in the bootup, when the user-mode driver is not up.
   
DEPENDENCIES
   None
   
RETURN VALUE
   None.
   
SIDE EFFECTS
  The user task is signaled that a new string has been written.

===========================================================================*/

static unsigned char* OutputString(unsigned char* str)
{ 
  {
    while (*str != L'\0')
    {
      OEMWriteDebugByte((unsigned char)*(str++));
    }
  }
  return str;
}

/*===========================================================================
FUNCTION   convert_dec_to_ascii

DESCRIPTION
  converts an integer to a base 10 ASCII string.
   
DEPENDENCIES

RETURN VALUE
  The point in "buf" where the string starts. Since the routine copies from back to front, 
  the string may not start at "buf" passed in as an input.

SIDE EFFECTS
===========================================================================*/

static unsigned char* convert_dec_to_ascii(uint32 num, unsigned char* buf, uint32 max)
{

  if(max < 2 || buf == NULL) 
  {
    return buf;
  }

  // Start at the end of the buffer and work backwards
  buf += max-1;

  *buf = '\0';

  do
  {
     *(--buf) = ((unsigned char) (num % 10)) + '0';
     num /= 10;
  } while(num >= 1);

  return buf;
}

static void OutputNumHex (unsigned long n, long depth)
{
    if (depth) 
	{
        depth--;
    }
    
    if ((n & ~0xf) || depth) 
	{
        OutputNumHex(n >> 4, depth);
        n &= 0xf;
    }
    
    if (n < 10) 
	{
        OutputChar((unsigned char)(n + '0'));
    }
	else 
    { 
    	OutputChar((unsigned char)(n - 10 + 'A'));
	}
}


/*===========================================================================
FUNCTION   OutputDecimalNum

DESCRIPTION

  Takes an unsigned int number and outputs its ASCII decimal representation.
   
DEPENDENCIES
   None
   
RETURN VALUE
   None.
   
SIDE EFFECTS
   None.

===========================================================================*/

static void OutputDecimalNum(uint32 num)
{
  static unsigned char buf[20];
  unsigned char* buf_ptr;
  uint32 i;
  
  buf_ptr = convert_dec_to_ascii(num, buf, 20);

  for(i=0; i<20 && *buf_ptr; ++i)
  {
    OutputChar(*buf_ptr);
    buf_ptr++;
  }
}


/*****************************************************************************
*
*
*   @func   void    | EdbgOutputDebugString | Simple formatted debug output string routine
*
*   @rdesc  none
*
*   @parm   LPCSTR  |   sz,... |
*               Format String:
*
*               @flag Format string | type
*               @flag u | unsigned
*               @flag d | int
*               @flag c | char
*               @flag s | string
*               @flag x | 4-bit hex number
*               @flag B | 8-bit hex number
*               @flag H | 16-bit hex number
*               @flag X | 32-bit hex number
*
*   @comm
*           Same as FormatString, but output to serial port instead of buffer.
*/

#if defined(DEBUG_AMSS)
void AMSSDebug(unsigned char* sz, ...)
{
    unsigned char    c;
    va_list         vl;
	int CR_output = 0;
	int LF_output = 0;

#if 0
	uint32 gpio_ownership_mask = 0x0;
	int uart_switch = 0;

	gpio_ownership_mask = HWIO_IN(GPIO_OWNER_3);
	 
    HWIO_OUTM(GPIO_OWNER_3,GPIO_92_MASK,0x0 );

    uart_switch = (HWIO_INM(GPIO_IN_3, GPIO_92_MASK))>>GPIO_92_SHIFT;

    HWIO_OUT(GPIO_OWNER_3, gpio_ownership_mask);
  
    if (uart_switch == GPIO_HIGH_VALUE)  
  	  return;
#endif

	if ( sz == NULL ) return;

	if ( uart_initialized	!= 1 ) 
      OEMDebugInit();

	if((*(sz-1) == L'\r') || (*(sz-2) == L'\r'))
	{
	    CR_output = 1;
	}

	if((*(sz-1) == L'\n') || (*(sz-2) == L'\n'))
	{
	    LF_output = 1;
	}
	
    
    va_start(vl, sz);
    
    while (*sz) {
        c = *sz++;
        switch (c) {
        case '%':
            c = *sz++;
            switch (c) { 
            case 'x':
                OutputNumHex(va_arg(vl, unsigned long), 0);
                break;
            case 'B':
                OutputNumHex(va_arg(vl, unsigned long), 2);
                break;
            case 'H':
                OutputNumHex(va_arg(vl, unsigned long), 4);
                break;
            case 'X':
                OutputNumHex(va_arg(vl, unsigned long), 8);
                break;
            case 'd':
                {
                    long    l;
                
                    l = va_arg(vl, long);
                    if (l < 0) { 
                        OutputChar('-');
                        l = - l;
                    }
                    OutputDecimalNum((uint32)l);
                }
                break;
            case 'u':
                OutputDecimalNum(va_arg(vl, unsigned long));
                break;
            case 's':
                OutputString(va_arg(vl, unsigned char *));
                break; 
            case '%':
                OutputChar('%');
                break;
            case 'c':
                c = va_arg(vl, unsigned char);
                OutputChar(c);
                break;
                
            default:
                OutputChar(' ');
                break;
            }
            break;
        case '\r':
            if (*sz == '\n')
                sz ++;
            c = '\n';
            // fall through
        case '\n':
            OutputChar('\r');
            // fall through
        default:
            OutputChar(c);
        }
    }

    va_end(vl);

	
	if(CR_output == 0)
	{
	    OutputChar('\r');
	}

	if(LF_output == 0)
	{
	    OutputChar('\n');
	} 
}

#else
void AMSSDebug(unsigned char* sz, ...)
{
}
#endif



