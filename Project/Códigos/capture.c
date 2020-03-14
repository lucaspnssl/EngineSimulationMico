#include <p18cxxx.h>
#include "capture.h"

#if defined (CC_V1) || defined (CC_V2) || defined (CC_V3) || defined (CC_V4) ||\
    defined (CC_V5) || defined (CC_V6) || defined (CC_V7) || defined (CC_V8) || defined (CC_V8_1) ||\
	defined (ECC_V8) || defined (ECC_V8_1) || defined (ECC_V8_2) || defined (CC_V8_2) || defined (CC_V8_5) \
	|| defined (ECC_V9) || defined (ECC_V9_1) || defined (CC_V9_1) || defined (CC_V9) \
	|| defined (CC_V8_3) || defined (CC_V8_4) || defined (ECC_V8_3) || defined (ECC_V8_4) || defined (ECC_V8_5)

union capstatus CapStatus;
#endif


#if defined (CC_V1) || defined (CC_V2) || defined (CC_V3) || \
    defined (CC_V4) || defined (CC_V5) || defined (CC_V7) 
/********************************************************************
Function Prototype : void CloseCapture1(void)
 
Include            : capture.h
 
Description        : This function turns off the Input Capture module
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function disables the Input Capture interrupt 
                     and then turns off the module.                        
********************************************************************/
void CloseCapture1(void)
{
   PIE1bits.CCP1IE = 0;    // Disable the interrupt
   CCP1CON=0x00;           // Reset the CCP module
}

#elif defined (CC_V6) 
/********************************************************************
Function Prototype : void CloseCapture1(void)
 
Include            : capture.h
 
Description        : This function turns off the Input Capture module
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function disables the Input Capture interrupt 
                     and then turns off the module.                          
********************************************************************/
void CloseCapture1(void)
{
   PIE3bits.CCP1IE = 0;    // Disable the interrupt
   CCP1CON=0x00;           // Reset the CCP module
}

#endif

#if defined (CC_V1) || defined (CC_V2) || defined (CC_V3) ||\
    defined (CC_V4) || defined (CC_V5)
/*******************************************************************************
Function Prototype : void OpenCapture1(unsigned char config)
 
Include            : capture.h
 
Description        : This function configures the Input Capture module.
 
Arguments          : config - This contains the parameters to be configured in the
                              CCPxCON register as defined below
				Enable CCP Interrupts:
				    * CAPTURE_INT_ON        
				    * CAPTURE_INT_OFF  
				    * CAPTURE_INT_MASK
				Capture configuration
				     * CAP_EVERY_FALL_EDGE
				     * CAP_EVERY_RISE_EDGE 
				     * CAP_EVERY_4_RISE_EDGE  
				     * CAP_EVERY_16_RISE_EDGE
				     * CAP_MODE_MASK					
 
Return Value       : None
 
Remarks            : This function configures the input capture for idle mode, clock select,
                      capture per interrupt and mode select
********************************************************************************/	
void OpenCapture1(unsigned char config)
{
  CCP1CON = config & 0x0F;  // Configure capture
  if(config&0x80)
  {
    PIR1bits.CCP1IF = 0;   // Clear the interrupt flag
    PIE1bits.CCP1IE = 1;   // Enable the interrupt

  }
 CP1_TRIS = 1;		//TRIS direction as input
}

#elif defined (CC_V6)
/*******************************************************************************
Function Prototype : void OpenCapture1(unsigned char config)
 
Include            : capture.h
 
Description        : This function configures the Input Capture module.
 
Arguments          : config - This contains the parameters to be configured in the
                              CCPxCON register as defined below
				Enable CCP Interrupts:
				    * CAPTURE_INT_ON        
				    * CAPTURE_INT_OFF  
				    * CAPTURE_INT_MASK
				Capture configuration
				     * CAP_EVERY_FALL_EDGE
				     * CAP_EVERY_RISE_EDGE 
				     * CAP_EVERY_4_RISE_EDGE  
				     * CAP_EVERY_16_RISE_EDGE
				     * CAP_MODE_MASK					
 
Return Value       : None
 
Remarks            : This function configures the input capture for idle mode, clock select,
                      capture per interrupt and mode select
********************************************************************************/	
void OpenCapture1(unsigned char config)
{
  CCP1CON = config & 0x0F;  // Configure capture
  if(config&0x80)
  {
    PIR3bits.CCP1IF = 0;   // Clear the interrupt flag
    PIE3bits.CCP1IE = 1;   // Enable the interrupt
  } 
 CP1_TRIS = 1;		//TRIS direction as input	
}

#elif defined (CC_V7)
/*******************************************************************************
Function Prototype : void OpenCapture1(unsigned char config)
 
Include            : capture.h
 
Description        : This function configures the Input Capture module.
 
Arguments          : config - This contains the parameters to be configured in the
                              CCPxCON register as defined below
				Enable CCP Interrupts:
				    * CAPTURE_INT_ON        
				    * CAPTURE_INT_OFF  
				    * CAPTURE_INT_MASK
				Capture configuration
				     * CAP_EVERY_FALL_EDGE
				     * CAP_EVERY_RISE_EDGE 
				     * CAP_EVERY_4_RISE_EDGE  
				     * CAP_EVERY_16_RISE_EDGE
				     * CAP_MODE_MASK					
 
Return Value       : None
 
Remarks            : This function configures the input capture for idle mode, clock select,
                      capture per interrupt and mode select
********************************************************************************/	
void OpenCapture1(unsigned char config)
{
  CCP1CON = config & 0x0F;  // Configure capture
  if(config&0x80)
  {
    PIR1bits.CCP1IF = 0;   // Clear the interrupt flag
    PIE1bits.CCP1IE = 1;   // Enable the interrupt
  }
}

#endif


#if defined (CC_V1) || defined (CC_V2) || defined (CC_V3) ||\
    defined (CC_V4) || defined (CC_V5) || defined (CC_V7)
/*******************************************************************************
Function Prototype : unsigned int ReadCapture1(void)
 
Include            : capture.h
 
Description        : This function reads the pending Input Capture buffer.
 
Arguments          : None
 
Return Value       : This routine reads the CA1L and CA1H into the union Cap of type CapResult 
			that is defined in global data  space. The int result is then returned. 
 
Remarks            : This function reads the pending Input Capture buffer
*******************************************************************************/
unsigned int ReadCapture1(void)
{
  union CapResult Cap;

  CapStatus.Cap1OVF = 0;    // Clear the overflow flag in the
                            // status variable

  if(PIR1bits.CCP1IF)       // If the overflow flag is set
    CapStatus.Cap1OVF = 1;  // Set the overflow flag

  Cap.bc[0] = CCPR1L;       // Read CA1L into the lower byte
  Cap.bc[1] = CCPR1H;       // Read CA1H into the high byte

  return (Cap.lc);          // Return the int
}

#elif defined (CC_V6)
/*******************************************************************************
Function Prototype : unsigned int ReadCapture1(void)
 
Include            : capture.h
 
Description        : This function reads the pending Input Capture buffer.
 
Arguments          : None
 
Return Value       : This routine reads the CA1L and CA1H into the union Cap of type CapResult 
			that is defined in global data  space. The int result is then returned. 
 
Remarks            : This function reads the pending Input Capture buffer
*******************************************************************************/
unsigned int ReadCapture1(void)
{
  union CapResult Cap;

  CapStatus.Cap1OVF = 0;    // Clear the overflow flag in the
                            // status variable

  if(PIR3bits.CCP1IF)       // If the overflow flag is set
    CapStatus.Cap1OVF = 1;  // Set the overflow flag

  Cap.bc[0] = CCPR1L;       // Read CA1L into the lower byte
  Cap.bc[1] = CCPR1H;       // Read CA1H into the high byte

  return (Cap.lc);          // Return the int
}
#endif


/********************************************************************
*    Function Name:    CloseCapture2                                *
*    Return Value:     void                                         *
*    Parameters:       void                                         *
*    Description:      This routine disables the capture interrupt. *
********************************************************************/

#if defined (CC_V2) || defined (CC_V3) || defined (CC_V4) ||\
    defined (CC_V7) || defined (CC_V8_5)
/********************************************************************
Function Prototype : void CloseCapture2(void)
 
Include            : capture.h
 
Description        : This function turns off the Input Capture module
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function disables the Input Capture interrupt 
                     and then turns off the module. The Interrupt                                  
********************************************************************/
void CloseCapture2(void)
{
   PIE2bits.CCP2IE = 0; // Disable the interrupt
   CCP2CON=0x00;        // Reset the CCP module
}
#elif defined (CC_V6) || defined (CC_V8_2)
/********************************************************************
Function Prototype : void CloseCapture2(void)
 
Include            : capture.h
 
Description        : This function turns off the Input Capture module
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function disables the Input Capture interrupt 
                     and then turns off the module. The Interrupt                                  
********************************************************************/
void CloseCapture2(void)
{
   PIE3bits.CCP2IE = 0; // Disable the interrupt
   CCP2CON=0x00;        // Reset the CCP module
}
#endif


#if defined (CC_V2) || defined (CC_V3) || defined (CC_V4)
/*******************************************************************************
Function Prototype : void OpenCapture2(unsigned char config)
 
Include            : capture.h
 
Description        : This function configures the Input Capture module.
 
Arguments          : config - This contains the parameters to be configured in the
                              CCPxCON register as defined below
				Enable CCP Interrupts:
				    * CAPTURE_INT_ON        
				    * CAPTURE_INT_OFF  
				    * CAPTURE_INT_MASK
				Capture configuration
				     * CAP_EVERY_FALL_EDGE
				     * CAP_EVERY_RISE_EDGE 
				     * CAP_EVERY_4_RISE_EDGE  
				     * CAP_EVERY_16_RISE_EDGE
				     * CAP_MODE_MASK					
 
Return Value       : None
 
Remarks            : This function configures the input capture for idle mode, clock select,
                      capture per interrupt and mode select
********************************************************************************/
void OpenCapture2(unsigned char config)
{

#define __CONFIG3L 0x300004
#define __CONFIG3H 0x300005
char pmmode; /* will be set iff either bit 0 AND bit 1 or bit 5 AND bit 4 of __CONFIG3L are set */
char ccp2mx; /* will be set iff bit 0 of __CONFIG3H is set */


  CCP2CON = config&0x0F; //  Configure capture
 
   	if (((__CONFIG3H) & 0b00000001))
    	ccp2mx=0xff;
 	else
     	ccp2mx = 0;

#if defined CC_CONFIG3L_V1
  	if (((*(unsigned char far rom *)__CONFIG3L) & 0b00000011) == 0b00000011)
     	pmmode=0xff;
  	else
     	pmmode = 0;
#elif defined CC_CONFIG3L_V2
	if (((*(unsigned char far rom *)__CONFIG3L) & 0b00110000) == 0b00110000)
     	pmmode=0xff;
  	else
    	pmmode = 0;
#endif

  if(config&0x80)
  {
    PIR2bits.CCP2IF = 0;  // Clear the interrupt flag
    PIE2bits.CCP2IE = 1;  // Enable the interrupt
  }

  	#if defined CC2_IO_V0	
		TRISCbits.TRISC1 = 1;
	#elif defined CC2_IO_V1	
		TRISBbits.TRISB3 = 1;
	#elif defined CC2_IO_V2	
		TRISEbits.TRISE7 = 1;
	#elif defined CC2_IO_V3
		if(ccp2mx)	TRISCbits.TRISC1 = 1;
		else TRISBbits.TRISB3 = 1;
	#elif defined CC2_IO_V4
		if(ccp2mx)	TRISCbits.TRISC1 = 1;
		else TRISEbits.TRISE7 = 1;
	#elif defined CC2_IO_V5
		if(ccp2mx)	TRISCbits.TRISC1 = 1;
		else if(pmmode)TRISEbits.TRISE7 = 1;
		else TRISBbits.TRISB3 = 1;
	#endif	

}

#elif defined (CC_V6)
/*******************************************************************************
Function Prototype : void OpenCapture2(unsigned char config)
 
Include            : capture.h
 
Description        : This function configures the Input Capture module.
 
Arguments          : config - This contains the parameters to be configured in the
                              CCPxCON register as defined below
				Enable CCP Interrupts:
				    * CAPTURE_INT_ON        
				    * CAPTURE_INT_OFF  
				    * CAPTURE_INT_MASK
				Capture configuration
				     * CAP_EVERY_FALL_EDGE
				     * CAP_EVERY_RISE_EDGE 
				     * CAP_EVERY_4_RISE_EDGE  
				     * CAP_EVERY_16_RISE_EDGE
				     * CAP_MODE_MASK					
 
Return Value       : None
 
Remarks            : This function configures the input capture for idle mode, clock select,
                      capture per interrupt and mode select
********************************************************************************/
void OpenCapture2(unsigned char config)
{
	#define __CONFIG3L 0x300004
	#define __CONFIG3H 0x300005
	char pmmode; /* will be set iff either bit 0 AND bit 1 or bit 5 AND bit 4 of __CONFIG3L are set */
	char ccp2mx; /* will be set iff bit 0 of __CONFIG3H is set */

  CCP2CON = config&0x0F; //  Configure capture
 
   	if (((*(unsigned char far rom *)__CONFIG3H) & 0b00000001))
    	ccp2mx=0xff;
 	else
     	ccp2mx = 0;

#if defined CC_CONFIG3L_V1
  	if (((*(unsigned char far rom *)__CONFIG3L) & 0b00000011) == 0b00000011)
     	pmmode=0xff;
  	else
     	pmmode = 0;
#elif defined CC_CONFIG3L_V2
	if (((*(unsigned char far rom *)__CONFIG3L) & 0b00110000) == 0b00110000)
     	pmmode=0xff;
  	else
    	pmmode = 0;
#endif

  if(config&0x80)
  {
    PIR3bits.CCP2IF = 0;  // Clear the interrupt flag
    PIE3bits.CCP2IE = 1;  // Enable the interrupt
  }

  	#if defined CC2_IO_V0	
		TRISCbits.TRISC1 = 1;
	#elif defined CC2_IO_V1	
		TRISBbits.TRISB3 = 1;
	#elif defined CC2_IO_V2	
		TRISEbits.TRISE7 = 1;
	#elif defined CC2_IO_V3
		if(ccp2mx)	TRISCbits.TRISC1 = 1;
		else TRISBbits.TRISB3 = 1;
	#elif defined CC2_IO_V4
		if(ccp2mx)	TRISCbits.TRISC1 = 1;
		else TRISEbits.TRISE7 = 1;
	#elif defined CC2_IO_V5
		if(ccp2mx)	TRISCbits.TRISC1 = 1;
		else if(pmmode)TRISEbits.TRISE7 = 1;
		else TRISBbits.TRISB3 = 1;
	#endif	

}

#elif defined (CC_V7)
/*******************************************************************************
Function Prototype : void OpenCapture2(unsigned char config)
 
Include            : capture.h
 
Description        : This function configures the Input Capture module.
 
Arguments          : config - This contains the parameters to be configured in the
                              CCPxCON register as defined below
				Enable CCP Interrupts:
				    * CAPTURE_INT_ON        
				    * CAPTURE_INT_OFF  
				    * CAPTURE_INT_MASK
				Capture configuration
				     * CAP_EVERY_FALL_EDGE
				     * CAP_EVERY_RISE_EDGE 
				     * CAP_EVERY_4_RISE_EDGE  
				     * CAP_EVERY_16_RISE_EDGE
				     * CAP_MODE_MASK					
 
Return Value       : None
 
Remarks            : This function configures the input capture for idle mode, clock select,
                      capture per interrupt and mode select
********************************************************************************/
void OpenCapture2(unsigned char config)
{
  CCP2CON = config&0x0F; //  Configure capture   
  
  if(config&0x80)
  {
    PIR2bits.CCP2IF = 0;  // Clear the interrupt flag
    PIE2bits.CCP2IE = 1;  // Enable the interrupt
  } 
  
}

#elif defined (CC_V8_2)
/*******************************************************************************
Function Prototype : void OpenCapture2(unsigned char config)
 
Include            : capture.h
 
Description        : This function configures the Input Capture module.
 
Arguments          : config - This contains the parameters to be configured in the
                              CCPxCON register as defined below
				Enable CCP Interrupts:
				    * CAPTURE_INT_ON        
				    * CAPTURE_INT_OFF  
				    * CAPTURE_INT_MASK
				Capture configuration
				     * CAP_EVERY_FALL_EDGE
				     * CAP_EVERY_RISE_EDGE 
				     * CAP_EVERY_4_RISE_EDGE  
				     * CAP_EVERY_16_RISE_EDGE
				     * CAP_MODE_MASK					
 
Return Value       : None
 
Remarks            : This function configures the input capture for idle mode, clock select,
                      capture per interrupt and mode select
********************************************************************************/
void OpenCapture2(unsigned char config)
{
  CCP2CON = config&0x0F; //  Configure capture   

  //configure timer source for CCP
  CCPTMRS &= 0b11111101;
  CCPTMRS |= ((config&0b00010000)>>3); 
  
  if(config&0x80)
  {
    PIR3bits.CCP2IF = 0;  // Clear the interrupt flag
    PIE3bits.CCP2IE = 1;  // Enable the interrupt
  } 
  CP2_TRIS=1;
}

#elif defined (CC_V8_5)
/*******************************************************************************
Function Prototype : void OpenCapture2(unsigned char config)
 
Include            : capture.h
 
Description        : This function configures the Input Capture module.
 
Arguments          : config - This contains the parameters to be configured in the
                              CCPxCON register as defined below
				Enable CCP Interrupts:
				    * CAPTURE_INT_ON        
				    * CAPTURE_INT_OFF  
				    * CAPTURE_INT_MASK
				Capture configuration
				     * CAP_EVERY_FALL_EDGE
				     * CAP_EVERY_RISE_EDGE 
				     * CAP_EVERY_4_RISE_EDGE  
				     * CAP_EVERY_16_RISE_EDGE
				     * CAP_MODE_MASK					
 
Return Value       : None
 
Remarks            : This function configures the input capture for idle mode, clock select,
                      capture per interrupt and mode select
********************************************************************************/
void OpenCapture2(unsigned char config)
{
  CCP2CON = config&0x0F; //  Configure capture   

  //configure timer source for CCP
  CCPTMRS &= 0b11110111;
  CCPTMRS |= ((config&0b00010000)>>1); 

  if(config&0x80)
  {
    PIR2bits.CCP2IF = 0;  // Clear the interrupt flag
    PIE2bits.CCP2IE = 1;  // Enable the interrupt
  } 
  CP2_TRIS=1;
}

#endif

#if defined (CC_V2) || defined (CC_V3) || defined (CC_V4) ||\
    defined (CC_V7)
/*******************************************************************************
Function Prototype : unsigned int ReadCapture2(void)
 
Include            : capture.h
 
Description        : This function reads the pending Input Capture buffer.
 
Arguments          : None
 
Return Value       : This routine reads the CA1L and CA1H into the union Cap of type CapResult 
			that is defined in global data  space. The int result is then returned. 
 
Remarks            : This function reads the pending Input Capture buffer
*******************************************************************************/
unsigned int ReadCapture2(void)
{
  union CapResult Cap;

  CapStatus.Cap2OVF = 0;   // Clear the overflow flag in the
                           // status variable

  if(PIR2bits.CCP2IF)      // If the overflow flag is set
    CapStatus.Cap2OVF = 1; // Set the overflow flag

  Cap.bc[0] = CCPR2L;      // Read CA2L into the lower byte
  Cap.bc[1] = CCPR2H;      // Read CA2H into the high byte

  return (Cap.lc);         // Return the int
}

#elif defined (CC_V6) || defined (CC_V8_2) || defined (CC_V8_5)
/*******************************************************************************
Function Prototype : unsigned int ReadCapture2(void)
 
Include            : capture.h
 
Description        : This function reads the pending Input Capture buffer.
 
Arguments          : None
 
Return Value       : This routine reads the CA1L and CA1H into the union Cap of type CapResult 
			that is defined in global data  space. The int result is then returned. 
 
Remarks            : This function reads the pending Input Capture buffer
*******************************************************************************/
unsigned int ReadCapture2(void)
{
  union CapResult Cap;

  CapStatus.Cap2OVF = 0;   // Clear the overflow flag in the
                           // status variable
#if defined (CC_V6) || defined (CC_V8_2)
  if(PIR3bits.CCP2IF )      // If the overflow flag is set
    CapStatus.Cap2OVF = 1; // Set the overflow flag
#elif defined (CC_V8_5)
  if(PIR2bits.CCP2IF )      // If the overflow flag is set
    CapStatus.Cap2OVF = 1; // Set the overflow flag
#endif

  Cap.bc[0] = CCPR2L;      // Read CA2L into the lower byte
  Cap.bc[1] = CCPR2H;      // Read CA2H into the high byte

  return (Cap.lc);         // Return the int
}
#endif

