#include <p18cxxx.h>
#include "timers.h"

/********************************************************************
*    Function Name:  CloseTimer0                                    *
*    Return Value:   void                                           *
*    Parameters:     void                                           *
*    Description:    This routine disables the Timer0 interrupt.    *
********************************************************************/
#if  defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
     defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	 defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	  || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
void CloseTimer0(void)
{
  T0CONbits.TMR0ON = 0;  // Disable Timer1
  INTCONbits.TMR0IE = 0;   // Disable Timer1 overflow interrupts
}

#endif

/********************************************************************
*    Function Name:  OpenTimer0                                     *
*    Return Value:   void                                           *
*    Parameters:     config: bit definitions to configure Timer0    *
*    Description:    This routine resets the Timer0 regs to the     *
*                    POR state and configures the interrupt, clock  *
*                    source, edge and prescaler.                    *
*    Notes:          The bit definitions for config can be found    *
*                    in the timers.h file.                          *
********************************************************************/
#if  defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
     defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	 defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	 || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
 
void OpenTimer0(unsigned char config)
{
  T0CON = (0x7f & config);  // Configure timer, but don't start it yet
  TMR0H = 0;                // Reset Timer0 to 0x0000
  TMR0L = 0;
  INTCONbits.TMR0IF = 0;      // Clear Timer0 overflow flag

  if(config&0x80)           // If interrupts enabled
    INTCONbits.TMR0IE = 1;    // Enable Timer0 overflow interrupt
  else
    INTCONbits.TMR0IE = 0;

  T0CONbits.TMR0ON = 1;
}
#endif

/********************************************************************
*    Function Name:  ReadTimer0                                     *
*    Return Value:   int: Timer0 16-bit value                       *
*    Parameters:     void                                           *
*    Description:    This routine reads the 16-bit value from       *
*                    Timer0.                                        *
********************************************************************/
#if  defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
     defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	 defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	  || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)

unsigned int ReadTimer0(void)
{
  union Timers timer;

  timer.bt[0] = TMR0L;  // Copy Timer0 low byte into union
  timer.bt[1] = TMR0H;  // Copy Timer0 high byte into union

  return (timer.lt);    // Return the int
}
#endif


/********************************************************************
*    Function Name:  WriteTimer0                                    *
*    Return Value:   void                                           *
*    Parameters:     int: value to write to Timer0                  *
*    Description:    This routine writes a 16-bit value to Timer0   *
*                    Timer0.                                        *
********************************************************************/
#if  defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) || \
     defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	 defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	  || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)

void WriteTimer0(unsigned int timer0)
{
  union Timers timer;

  timer.lt = timer0;    // Copy timer value into union

  TMR0H = timer.bt[1];  // Write high byte to Timer0
  TMR0L = timer.bt[0];  // Write low byte to Timer0
}
#endif


/********************************************************************
*    Function Name:  CloseTimer1                                    *
*    Return Value:   void                                           *
*    Parameters:     void                                           *
*    Description:    This routine disables Timer1 and interrupt.    *
********************************************************************/
#if defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
    defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	 defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	  || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
void CloseTimer1(void)
{
  T1CONbits.TMR1ON = 0;  // Disable Timer1
  PIE1bits.TMR1IE = 0;   // Disable Timer1 overflow interrupts 
}
#endif


/********************************************************************
*    Function Name:  OpenTimer1                                     *
*    Return Value:   void                                           *
*    Parameters:     config: bit definitions to configure Timer1    *
*    Description:    This routine first resets the Timer1 regs      *
*                    to the POR state and then configures the       *
*                    interrupt, clock source and 8/16-bit mode.     *
*    Notes:          The bit definitions for config can be found    *
*                    in the timers.h file.                          *
********************************************************************/
#if defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
    defined (TMR_V4) || defined (TMR_V5) 
void OpenTimer1(unsigned char config)
{
  T1CON = (0x7e & config); // Set everything except 8/16 mode, and
                            // don't start timer yet

  if( config & 0x40 )       // The 8/16 selection bit isn't in the
      T1CONbits.RD16 = 1;   // right place -- we have to move it
  else
      T1CONbits.RD16 = 0;

  TMR1H=0;             // Clear out timer registers
  TMR1L=0;
  PIR1bits.TMR1IF=0;

  if(config&0x80)      // Enable interrupts if selected
    PIE1bits.TMR1IE=1;
  else
    PIE1bits.TMR1IE=0;
  
  T1CONbits.TMR1ON = 1;  // Start Timer1
}

#endif

#if defined (TMR_V6)  || defined (TMR_V7) || defined (TMR_V7_1)\
	|| defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
	 
void OpenTimer1(unsigned char config, unsigned char config1)
{
  T1GCON = (0XF3 & config1);
  
  T1CON = (0x7F & config) << 1 ;    
    	
  TMR1H=0;             // Clear out timer registers
  TMR1L=0;
  
  PIR1bits.TMR1IF=0;

  if(config&0x80)      // Enable interrupts if selected
    PIE1bits.TMR1IE=1;
  else
    PIE1bits.TMR1IE=0;

#ifndef TMR_V6
#if defined(TMR_INT_V2) || defined (TMR_INT_V7_3) || defined (TMR_INT_V7_5)
	PIR3bits.TMR1GIF =0;
#elif defined(TMR_V7_1_INT_V1)
	PIR5bits.TMR1GIF =0;	
#else  
	PIR1bits.TMR1GIF =0;
#endif 
#endif

#ifndef TMR_V6
#if defined(TMR_INT_V2) || defined (TMR_INT_V7_3) || defined (TMR_INT_V7_5)
  if(config1 & 0x04)
    PIE3bits.TMR1GIE=1;
  else
    PIE3bits.TMR1GIE=0;
#elif defined(TMR_V7_1_INT_V1)
  if(config1 & 0x04)
    PIE5bits.TMR1GIE=1;
  else
    PIE5bits.TMR1GIE=0;	
#else  
  if(config1 & 0x04)
    PIE1bits.TMR1GIE=1;
  else
    PIE1bits.TMR1GIE=0;
#endif	
#endif
    
  if(T1GCONbits.TMR1GE)	
      T1GCONbits.T1GGO = 1;
  
  T1CONbits.TMR1ON = 1;  // Start Timer1
  
}
#endif 


/********************************************************************
*    Function Name:  ReadTimer1                                     *
*    Return Value:   char: Timer1 16-bit value                      *
*    Parameters:     void                                           *
*    Description:    This routine reads the 16-bit value from       *
*                    Timer1.                                        *
********************************************************************/
#if defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
    defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	 || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
unsigned int ReadTimer1(void)
{
  union Timers timer;

  timer.bt[0] = TMR1L;    // Read Lower byte
  timer.bt[1] = TMR1H;    // Read upper byte

  return (timer.lt);      // Return the 16-bit value
}
#endif


/********************************************************************
*    Function Name:  WriteTimer1                                    *
*    Return Value:   void                                           *
*    Parameters:     int: value to write to Timer1                  *
*    Description:    This routine writes a 16-bit value to Timer1.  *
********************************************************************/
#if defined (TMR_V1) || defined (TMR_V2) || defined (TMR_V3) ||\
    defined (TMR_V4) || defined (TMR_V5) || defined (TMR_V6) ||\
	defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2)\
	 || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
void WriteTimer1(unsigned int timer1)
{
  union Timers timer;

  timer.lt = timer1;    // Save the 16-bit value in local

  TMR1H = timer.bt[1];  // Write high byte to Timer1 High byte
  TMR1L = timer.bt[0];  // Write low byte to Timer1 Low byte
}

#endif

/********************************************************************
*    Function Name:  CloseTimer2                                    *
*    Return Value:   void                                           *
*    Parameters:     void                                           *
*    Description:    This routine disables the Timer2 and the       *
*                    interrupt.                                     *
********************************************************************/
#if defined (TMR_V2) || defined (TMR_V3) || defined (TMR_V4) ||\
    defined (TMR_V5) || defined (TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1) \
	|| defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
void CloseTimer2(void)
{
  T2CONbits.TMR2ON = 0;  // Disable Timer2
  PIE1bits.TMR2IE = 0;   // Disable Timer2 overflow interrupts
}

#endif

/********************************************************************
*    Function Name:  OpenTimer2                                     *
*    Return Value:   void                                           *
*    Parameters:     config: bit definitions to configure Timer2    *
*    Description:    This routine first resets the Timer2 regs      *
*                    to the POR state and then configures the       *
*                    interrupt and clock source.                    *
*    Notes:          The bit definitions for config can be found    *
*                    in the timers.h file.                          *
********************************************************************/
#if defined (TMR_V2) || defined (TMR_V3) || defined (TMR_V4) ||\
    defined (TMR_V5) || defined (TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1)\
	|| defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
void OpenTimer2(unsigned char config)
{
  T2CON = (0xfb & config);  // Set all configuration values, but
                            // don't start timer yet 								

  TMR2 = 0;                 // Clear Timer2
  PIR1bits.TMR2IF = 0;

  if(config & 0x80)         // Enable timer interrupts?
    PIE1bits.TMR2IE = 1;
  else
    PIE1bits.TMR2IE = 0;

  T2CONbits.TMR2ON = 1; // Turn on Timer2
}
#endif

/********************************************************************
*    Function Name:  ReadTimer2                                     *
*    Return Value:   char: Timer2 8-bit value                       *
*    Parameters:     void                                           *
*    Description:    This routine reads the 8-bit value from        *
*                    Timer2.                                        *
********************************************************************/
#if defined (TMR_V2) || defined (TMR_V3) || defined (TMR_V4) || \
    defined (TMR_V5) || defined (TMR_V6) || defined (TMR_V7) || defined (TMR_V7_1)\
	|| defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
#undef ReadTimer2
unsigned char ReadTimer2(void)
{
  unsigned char timer;

  timer = TMR2;      // Read Timer2

  return (timer);    // Return 8-bits
}

#endif

/********************************************************************
*    Function Name:  WriteTimer2                                    *
*    Return Value:   void                                           *
*    Parameters:     char: value to write to Timer2                 *
*    Description:    This routine writes a 8-bit value to Timer2.   *
********************************************************************/
#if defined (TMR_V2) || defined (TMR_V3) || defined (TMR_V4) || \
    defined (TMR_V5) || defined (TMR_V6)  || defined (TMR_V7) || defined (TMR_V7_1)\
	|| defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
#undef WriteTimer2
void WriteTimer2(unsigned char timer2)
{
  TMR2 = timer2;    // Write byte to Timer2
}

#endif


/********************************************************************
*    Function Name:  CloseTimer3                                    *
*    Return Value:   void                                           *
*    Parameters:     void                                           *
*    Description:    This routine disables the Timer3 and the       *
*                    interrupt.                                     *
********************************************************************/
#if defined (TMR_V2) || defined (TMR_V4)
void CloseTimer3(void)
{
  T3CONbits.TMR3ON = 0;  // Disable Timer3
  PIE2bits.TMR3IE = 0;   // Disable Timer3 overflow interrupts
}
#endif

#if defined (TMR_V6)
void CloseTimer3(void)
{
  T3CONbits.TMR3ON = 0;  // Disable Timer3
  PIE2bits.TMR3IE = 0;   // Disable Timer3 overflow interrupts
  PIE3bits.TMR3GIE=0;  
}
#endif

#if defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
void CloseTimer3(void)
{
  T3CONbits.TMR3ON = 0;  // Disable Timer3
  PIE2bits.TMR3IE = 0;   // Disable Timer3 overflow interrupts
#if defined(TMR_INT_V2) || defined (TMR_INT_V7_3) || defined (TMR_INT_V7_5) || defined (TMR_V7_1_INT_V1)
	PIR3bits.TMR3GIF =0;
	PIE3bits.TMR3GIE=0;
#else  
	PIR2bits.TMR3GIF =0;
	PIE2bits.TMR3GIE=0;
#endif
}
#endif


/********************************************************************
*    Function Name:  OpenTimer3                                     *
*    Return Value:   void                                           *
*    Parameters:     config: bit definitions to configure Timer3    *
*    Description:    This routine first resets the Timer3 regs      *
*                    to the POR state and then configures the       *
*                    interrupt, clock source.                       *
*    Notes:          The bit definitions for config can be found    *
*                    in the timers.h file.                          *
*                                                                   *
*                    The MPLAB CXX Reference Guide says that        *
*                    OpenTimer1(config) can select Timer 1 as the   *
*                    source for the CCPs.  However, this function   *
*                    does not write the necessary values to bits 6  *
*                    and 3 of T3CON.  Instead, the function         *
*                    incorrectly affects bit 3 of T1CON.            *
*                                                                   *
*                    Because the power-on default is timer1 source  *
*                    for both CCPs, we do not need to even have     *
*                    these values.  Should a user want to change    *
*                    this value, the user would need select the     *
*                    appropriate value when calling OpenTimer3.     *
*                                                                   *
********************************************************************/
#if defined (TMR_V2) || defined (TMR_V4)
void OpenTimer3(unsigned char config)
{
  T3CON = (0x36 & config);  // Set everything except 8/16 mode and Timer as CCP source
                            // don't start timer yet

  TMR3H=0;          //Clear timer3-related registers
  TMR3L=0;
  PIR2bits.TMR3IF = 0;

  if(config & 0x80)   // Interrupt on/off
    PIE2bits.TMR3IE = 1;
  else
    PIE2bits.TMR3IE = 0;



  if(config & ~T3_8BIT_RW) // Select between 8-bit and 16-bit modes
  {
    T3CONbits.RD16 = 1;
  }
  else
  {
    T3CONbits.RD16 = 0;
  }

  T3CONbits.TMR3ON = 1;   // Turn on Timer3
}

#endif

#if defined (TMR_V6)
void OpenTimer3(unsigned char config, unsigned char config1)
{
  T3GCON = (0XF3 & config1);

  T3CON = (0x7F & config) << 1;  // Set everything except 8/16 mode, and
                            // don't start timer yet
  if(config&0x04)
    T1CONbits.T1OSCEN=1;

  TMR3H=0;          //Clear timer3-related registers
  TMR3L=0;
  PIR2bits.TMR3IF = 0;
  PIR3bits.TMR3GIF =0;

  if(config & 0x80)   // Interrupt on/off
    PIE2bits.TMR3IE = 1;
  else
    PIE2bits.TMR3IE = 0;

  if(config1 & 0x04)
    PIE3bits.TMR3GIE=1;
  else
    PIE3bits.TMR3GIE=0;

  if(T3GCONbits.TMR3GE)
      T3GCONbits.T3GGO = 1;

  T3CONbits.TMR3ON = 1;   // Turn on Timer3
}

#endif


#if defined (TMR_V7) || defined (TMR_V7_1) || defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
void OpenTimer3(unsigned char config, unsigned char config1)
{
  T3GCON = (0XF3 & config1);

  T3CON = (0x7F & config) << 1;  // Set everything except 8/16 mode, and
                            // don't start timer yet

  TMR3H=0;          //Clear timer3-related registers
  TMR3L=0;
  PIR2bits.TMR3IF = 0;
  
#if defined(TMR_INT_V2) || defined (TMR_INT_V7_3) || defined (TMR_INT_V7_5) || defined (TMR_V7_1_INT_V1)
	PIR3bits.TMR3GIF =0;
#else  
  PIR2bits.TMR3GIF =0;
#endif  

  if(config & 0x80)   // Interrupt on/off
    PIE2bits.TMR3IE = 1;
  else
    PIE2bits.TMR3IE = 0;

#if defined(TMR_INT_V2) || defined (TMR_INT_V7_3) || defined (TMR_INT_V7_5) || defined (TMR_V7_1_INT_V1)
  if(config1 & 0x04)
    PIE3bits.TMR3GIE=1;
  else
    PIE3bits.TMR3GIE=0;
#else  
  if(config1 & 0x04)
    PIE2bits.TMR3GIE=1;
  else
    PIE2bits.TMR3GIE=0;
#endif	


  if(T3GCONbits.TMR3GE)
      T3GCONbits.T3GGO = 1;

  T3CONbits.TMR3ON = 1;   // Turn on Timer3
}

#endif

/********************************************************************
*    Function Name:  ReadTimer3                                     *
*    Return Value:   int: Timer3 16-bit value                       *
*    Parameters:     void                                           *
*    Description:    This routine reads the 16-bit value from       *
*                    Timer3.                                        *
********************************************************************/
#if defined (TMR_V2) || defined (TMR_V4) || defined (TMR_V6) || defined (TMR_V7)\
	|| defined (TMR_V7_1) || defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
unsigned int ReadTimer3(void)
{
  union Timers timer;

  timer.bt[0] = TMR3L;   // Read low byte of Timer3
  timer.bt[1] = TMR3H;   // Read high byte of Timer3

  return (timer.lt);     // Return 16-bit value
}

#endif

/********************************************************************
*    Function Name:  WriteTimer3                                    *
*    Return Value:   void                                           *
*    Parameters:     int: value to write to Timer3                  *
*    Description:    This routine writes a 16-bit value to Timer3   *
********************************************************************/
#if defined (TMR_V2) || defined (TMR_V4) || defined (TMR_V6) || defined (TMR_V7)\
	|| defined (TMR_V7_1) || defined (TMR_V7_2) || defined (TMR_V7_3) || defined (TMR_V7_4) || defined (TMR_V7_5)
void WriteTimer3(unsigned int timer3)
{
  union Timers timer;

  timer.lt = timer3;    // Save value in local variable
  TMR3H = timer.bt[1];  // Write high byte to Timer3
  TMR3L = timer.bt[0];  // Write low byte to Timer3
}

#endif

/********************************************************************
    Function Name:  SetTmrCCPSrc     
                               
    Return Value:    void                                          

    Parameters:      config: bit definitions to configure Timer as a clock source to CCP module    

    Description:     This routine configures the timer to work as a clock source to CCP module.
                           Required timer has to be enabled by calling respective function.
  
    Notes:             The bit definitions for config can be found in the timers.h file.                         
********************************************************************/
#if defined (TMR_V2) || defined (TMR_V4) 
void SetTmrCCPSrc(unsigned char config)
{
  T3CON  = T3CON & 0xB7 ;
  T3CON  = T3CON | (config & 0x48); // Timer to CCPx enable  
}
#endif

#if defined (TMR_V6)
void SetTmrCCPSrc(unsigned char config)
{
 TCLKCON = TCLKCON & 0xFC;
 TCLKCON = TCLKCON | (config & 0x03);
}
#endif



