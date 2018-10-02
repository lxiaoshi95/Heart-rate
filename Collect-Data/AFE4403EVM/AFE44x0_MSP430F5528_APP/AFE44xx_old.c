/****************************************************************************************************************************************************
*	AFE44xx.c  - Provides access to AFE44xx Pulse Oximeter data converter.																					*
* 		Functions: 
* 				1. AFE44xx_Init() : Initialization of AFE44xx																						*
*				2. AFE44xx_Default_Reg_Init(): 							                                                  													*
*				3. 								                                                  													*
*												                                                  													*
*****************************************************************************************************************************************************/
#include "device.h"
#include "types.h"               //Basic Type declarations
//#include "../AFE44xx_main.h"
#include "AFE44xx.h"

/**************************************************************************************************************************************************
*	        Prototypes									                                                  										  *
**************************************************************************************************************************************************/

/**************************************************************************************************************************************************
*	        Global Variables										                                  											  *
**************************************************************************************************************************************************/
unsigned char SPI_Tx_buf[10], SPI_Rx_buf[10];

#define DELAY_COUNT 2

unsigned long 	AFE44xx_Default_Register_Settings[49] = {
	//Reg0: CONTROL0: CONTROL REGISTER 0
	0x00000,
   	//Reg1:REDSTARTCOUNT: SAMPLE RED START COUNT
	6000,
        //Reg2:REDENDCOUNT: SAMPLE RED END COUNT
	7599,
        //Reg3:REDLEDSTARTCOUNT: RED LED START COUNT
	6000,
        //Reg4:REDLEDENDCOUNT: RED LED END COUNT
	7599,
	//Reg5:AMBREDSTARTCOUNT: SAMPLE AMBIENT RED START COUNT
	0000,
	//Reg6:AMBREDENDCOUNT: SAMPLE AMBIENT RED END COUNT
	1599,
	//Reg7:IRSTARTCOUNT: SAMPLE IR START COUNT
	2000,
        //Reg8:IRENDCOUNT: SAMPLE IR END COUNT
	3599,
        //Reg9:IRLEDSTARTCOUNT: IR LED START COUNT
	2000,
	//Reg10:IRLEDENDCOUNT: IR LED END COUNT
	3599,
	//Reg11:AMBIRSTARTCOUNT: SAMPLE AMBIENT IR START COUNT
	4000,
        //Reg12:AMBIRENDCOUNT: SAMPLE AMBIENT IR END COUNT
	5599,
	//Reg13:REDCONVSTART: REDCONVST
	2,
	//Reg14:REDCONVEND: RED CONVERT END COUNT
	1999,
	//Reg15:AMBREDCONVSTART: RED AMBIENT CONVERT START COUNT
	2002,
	//Reg16:AMBREDCONVEND: RED AMBIENT CONVERT END COUNT
	3999,
	//Reg17:IRCONVSTART: IR CONVERT START COUNT
	4002,
	//Reg18:IRCONVEND: IR CONVERT END COUNT
	5999,
	//Reg19:AMBIRCONVSTART: IR AMBIENT CONVERT START COUNT
	6002,
	//Reg20:AMBIRCONVEND: IR AMBIENT CONVERT END COUNT
	7999,
	//Reg21:ADCRESETSTCOUNT0: ADC RESET 0 START COUNT
	0,
	//Reg22:ADCRESETENDCOUNT0: ADC RESET 0 END COUNT
	0,
	//Reg23:ADCRESETSTCOUNT1: ADC RESET 1 START COUNT
	2000,
	//Reg24:ADCRESETENDCOUNT1: ADC RESET 1 END COUNT
	2000,
	//Reg25:ADCRESETENDCOUNT2: ADC RESET 2 START COUNT
	4000,
	//Reg26:ADCRESETENDCOUNT2: ADC RESET 2 END COUNT
	4000,
	//Reg27:ADCRESETENDCOUNT3: ADC RESET 3 START COUNT
	6000,
	//Reg28:ADCRESETENDCOUNT3: ADC RESET 3 END COUNT
	6000,
	//Reg29:PRPCOUNT: PULSE REPETITION PERIOD COUNT
	7999,
	//Reg30:CONTROL1: CONTROL REGISTER 1
	0x00107,  //timer enabled, averages=3, RED and IR LED pulse ON PD_ALM AND LED_ALM pins
	//Reg31:?: ??
	0x00000,
	//Reg32:TIAGAIN: TRANS IMPEDANCE AMPLIFIER GAIN SETTING REGISTER
	0x00000,
	//Reg33:TIA_AMB_GAIN: TRANS IMPEDANCE AAMPLIFIER AND AMBIENT CANELLATION STAGE GAIN
	0x00000,
	 //Reg34:LEDCNTRL: LED CONTROL REGISTER
	0x11414,
	//Reg35:CONTROL2: CONTROL REGISTER 2
	0x00000, //bit 9
	//Reg36:?: ??
	0x00000,
	//Reg37:?: ??
	0x00000,
	//Reg38:?: ??
	0x00000,
	 //Reg39:?: ??
	0x00000,
	 //Reg40:: ??
	0x00000,
	//Reg41:ALARM: ??
	0x00000,
	//Reg42:REDVALUE: RED DIGITAL SAMPLE VALUE
	0x00000,
	//Reg43:AMBREDVALUE: Ambient RED Digital Sample Value
	0x00000,
	//Reg44:IRVALUE: IR Digital Sample Value
	0x00000,
	//Reg45:AMBIRVALUE: Ambient IR Digital Sample Value
	0x00000,
	//Reg46:RED-AMBREDVALUE: RED-AMBIENT RED DIGITAL SAMPLE VALUE
	0x00000,
	//Reg47:IR-AMBIRVALUE: IR-AMBIENT IR DIGITAL SAMPLE VALUE
	0x00000,
	 //Reg48:DIGNOSTICS: DIAGNOSTICS FLAGS REGISTER
	0x00000
};


/***********************************************************************************************************
*	        Variables for the SPI Interaction                                                          	   *
***********************************************************************************************************/


/**********************************************************************************************************
*	External Variables																			          *
**********************************************************************************************************/

/**********************************************************************************************************
* Init_AFE44xx_DRDY_Interrupt												                                          *
**********************************************************************************************************/
void Init_AFE44xx_DRDY_Interrupt (void)
{
    P2DIR &= ~AFE_ADC_DRDY;
    P2REN |= AFE_ADC_DRDY;                              // Enable P2.5 internal resistance
    P2OUT |= AFE_ADC_DRDY;                            	// Set P2.5 as pull-Up resistance
    P2IES |= AFE_ADC_DRDY;                              // P2.5 Hi/Lo edge
    P2IFG &= ~AFE_ADC_DRDY;                           	// P2.5 IFG cleared
    P2IE &= ~AFE_ADC_DRDY;                             	// P2.5 interrupt disabled
}

/**********************************************************************************************************
* Enable_AFE44xx_DRDY_Interrupt												                                          *
**********************************************************************************************************/
void Enable_AFE44xx_DRDY_Interrupt (void)
{
    P2IFG &= ~AFE_ADC_DRDY;                           	// P2.5 IFG cleared
    P2IE |= AFE_ADC_DRDY;                             	// P2.5 interrupt enabled
}

/**********************************************************************************************************
* Disable_AFE44xx_DRDY_Interrupt												                                          *
**********************************************************************************************************/
void Disable_AFE44xx_DRDY_Interrupt (void)
{
    P2IFG &= ~AFE_ADC_DRDY;                           	// P2.5 IFG cleared
    P2IE &= ~AFE_ADC_DRDY;                             	// P2.5 interrupt disabled
}

/**********************************************************************************************************
* Set_GPIO														                                          *
**********************************************************************************************************/
void Set_GPIO(void)
{
  //port set..
  //Port 2.3 - AFE_RESETZ, P2.4 - AFE_PDNZ, P2.5 - ADC_RDY, P2.6- PD_ALM, P2.7 - LED_ALM,
  //P5.7 - DIAG_END
  
  //P1SEL |= BIT0;
  //P1DIR |= BIT0;
  //P1OUT |= BIT0;
 
  P2DIR |= (AFE_RESETZ + AFE_PDNZ);
  P2OUT |= (AFE_RESETZ + AFE_PDNZ);
  P2DIR &= ~(AFE_ADC_DRDY + AFE_PD_ALM + AFE_LED_ALM);
  P2DIR &= ~AFE_DIAG_END;
  
  P2SEL = 0x00;
  //P2OUT |= (BIT0 + BIT1 + BIT2);
  //P2DIR |= (BIT0 + BIT1 + BIT2);
}

/**********************************************************************************************************
* Set_UCB1_SPI													                                          *
**********************************************************************************************************/
void Set_UCB1_SPI(void)
{ 
    P4SEL |= BIT1+BIT2+BIT3;  			// Set SPI peripheral bits
    P4DIR |= BIT0+BIT1+BIT3;			// STE, SCLK, and DOUT as output
    P4DIR &= ~BIT2;                         	// Din as input
    P4OUT |=BIT0;				// Set STE high
    UCB1CTL1 |= UCSWRST;               		// Enable SW reset
    UCB1CTL0 |= UCMSB+UCCKPH+UCMST+UCSYNC;	// [b0]   1 -  Synchronous mode
                                                // [b2-1] 00-  3-pin SPI
                                                // [b3]   1 -  Master mode
                                                // [b4]   0 - 8-bit data
                                                // [b5]   1 - MSB first
                                                // [b6]   0 - Clock polarity high.
                                                // [b7]   1 - Clock phase - Data is captured on the first UCLK edge and changed on the following edge.
  
    UCB1CTL1 |= UCSSEL_2;               	// SMCLK
    //UCB1BR0 = 0x0C;                             // 2 MHz
    UCB1BR0 = 0x01;                             // 8 MHz
    UCB1BR1 = 0;                                //
    UCB1CTL1 &= ~UCSWRST;              		// Clear SW reset, resume operation
    UCB1IE =0x0;
}

/**********************************************************************************************************
* Init_AFE44xx_Resource						                                          *
**********************************************************************************************************/

void Init_AFE44xx_Resource(void)
{
    Set_GPIO();										// Initializes AFE44xx's input control lines
    Set_UCB1_SPI();									// Initialize SPI regs.    
}

/*********************************************************************************************************
* AFE44xx_Reg_Write																	                 *
**********************************************************************************************************/

void AFE44xx_Reg_Write (unsigned char reg_address, unsigned long data)
{
    unsigned char dummy_rx;
    
    //Set Control0 - Disable SPI Read bit
    //Write to register - byte wise transfer, 8-Bit transfers    
    P4OUT&= ~0x01;   //  SEN LOW FOR TRANSMISSION.
    // Loop unrolling for machine cycle optimization
    UCB1TXBUF = 0;                              // Send the first byte to the TX Buffer: Address of register
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    UCB1TXBUF = 0;                              // Send the second byte to the TX Buffer: Data[23:16]
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    UCB1TXBUF = 0;                              // Send the third byte to the TX Buffer: Data[15:8]
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    UCB1TXBUF = 0;                              // Send the first byte to the TX Buffer: Data[7:0]
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    P4OUT|=0x01;  // SEN HIGH

    //Write to register - byte wise transfer, 8-Bit transfers   
    P4OUT&= ~0x01;   //  SEN LOW FOR TRANSMISSION.    
    // Loop unrolling for machine cycle optimization
    UCB1TXBUF = reg_address;                    // Send the first byte to the TX Buffer: Address of register
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    UCB1TXBUF = (unsigned char)(data >>16);     // Send the second byte to the TX Buffer: Data[23:16]
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    UCB1TXBUF = (unsigned char)(((data & 0x00FFFF) >>8));       // Send the third byte to the TX Buffer: Data[15:8]
    while ( (UCB1STAT & UCBUSY) );		                // USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			                // Dummy Read Rx buf
    
    UCB1TXBUF = (unsigned char)(((data & 0x0000FF)));           // Send the first byte to the TX Buffer: Data[7:0]
    while ( (UCB1STAT & UCBUSY) );		                // USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			                // Dummy Read Rx buf
    
    P4OUT|=0x01;  // SEN HIGH
}

/*********************************************************************************************************
* AFE44xx_Reg_Read																	                 *
**********************************************************************************************************/
unsigned long AFE44xx_Reg_Read(unsigned char Reg_address)
{
    unsigned char dummy_rx;
    unsigned long retVal, SPI_Rx_buf[10];
    
    retVal = 0;
    
    //Read register and set bit 0 to 1, to enable read
    //Set Control0 - Enable SPI Read bit
    P4OUT&= ~0x01;   //  SEN LOW FOR TRANSMISSION.
    // Loop unrolling for machine cycle optimization
    UCB1TXBUF = 0;                              // Send the first byte to the TX Buffer: Address of register
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    UCB1TXBUF = 0;                              // Send the second byte to the TX Buffer: Data[23:16]
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    UCB1TXBUF = 0;                              // Send the third byte to the TX Buffer: Data[15:8]
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    UCB1TXBUF = 1;                              // Send the first byte to the TX Buffer: Data[7:0]
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    P4OUT|=0x01;  // set HIGH at end of transmission
    
    //Read from register - byte wise transfer, 8-Bit transfers
    P4OUT&= ~0x01;   //  SEN LOW FOR TRANSMISSION.
    // Loop unrolling for machine cycle optimization
    UCB1TXBUF = Reg_address;                    // Send the first byte to the TX Buffer: Address of register
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    SPI_Rx_buf[0] = UCB1RXBUF;			// Read Rx buf
    
    UCB1TXBUF = 0;                              // Send the second byte to the TX Buffer: dummy data
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    SPI_Rx_buf[1] = UCB1RXBUF;			// Read Rx buf: Data[23:16]
    
    UCB1TXBUF = 0;                              // Send the third byte to the TX Buffer: dummy data
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    SPI_Rx_buf[2] = UCB1RXBUF;			// Read Rx buf: Data[15:8]
    
    UCB1TXBUF = 0;                              // Send the first byte to the TX Buffer: dummy data
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    SPI_Rx_buf[3] = UCB1RXBUF;			// Read Rx buf: Data[7:0]
    
    P4OUT|=0x01;  // set HIGH at end of transmission
        
    //Set Control0 - Disable SPI Read bit
    //Write to register - byte wise transfer, 8-Bit transfers
    P4OUT&= ~0x01;   //  SEN LOW FOR TRANSMISSION.
    // Loop unrolling for machine cycle optimization
    UCB1TXBUF = 0;                              // Send the first byte to the TX Buffer: Address of register
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    UCB1TXBUF = 0;                              // Send the second byte to the TX Buffer: Data[23:16]
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    UCB1TXBUF = 0;                              // Send the third byte to the TX Buffer: Data[15:8]
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    UCB1TXBUF = 0;                              // Send the first byte to the TX Buffer: Data[7:0]
    while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
    dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
    
    P4OUT|=0x01;  // set HIGH at end of transmission
  
    retVal = (SPI_Rx_buf[1]<<16)|(SPI_Rx_buf[2]<<8)|(SPI_Rx_buf[3]);
    return 	retVal;
}
/**********************************************************************************************************
*	        AFE44xx default Initialization          				                  					  *
**********************************************************************************************************/

void AFE44xx_Default_Reg_Init(void)
{
    unsigned char Reg_Init_i;
    for ( Reg_Init_i = 0; Reg_Init_i < 49; Reg_Init_i++)
    {
      AFE44xx_Reg_Write(Reg_Init_i, AFE44xx_Default_Register_Settings[Reg_Init_i]);
    }
}

/**********************************************************************************************************
*	        AFE44xx_Read_All_Regs          				                  					  *
**********************************************************************************************************/

void AFE44xx_Read_All_Regs(unsigned long AFE44xxeg_buf[])
{
    unsigned char Regs_i;
    for ( Regs_i = 0; Regs_i < 50; Regs_i++)
    {
      AFE44xxeg_buf[Regs_i] = AFE44xx_Reg_Read(Regs_i);
    }
}
/*********************************************************************************************************/
/**********************************************************************************************************
*	        AFE44xx_PowerOn_Init          				                  					  			  *
***********************************************************************************************************/
void AFE44xx_PowerOn_Init(void)
{
    volatile unsigned short Init_i, j;
    Init_AFE44xx_Resource();
    for (j = 0; j < DELAY_COUNT; j++)
    {
      for ( Init_i =0; Init_i < 20000; Init_i++);
      for ( Init_i =0; Init_i < 20000; Init_i++);
      for ( Init_i =0; Init_i < 20000; Init_i++);
    }
    Init_AFE44xx_DRDY_Interrupt();
    
    for ( Init_i = 0; Init_i < 49; Init_i++)
    {
      AFE44xx_Reg_Write(Init_i, 0x00);
    }
    for (j = 0; j < DELAY_COUNT; j++)
    {
      for ( Init_i =0; Init_i < 20000; Init_i++);
      for ( Init_i =0; Init_i < 20000; Init_i++);
      for ( Init_i =0; Init_i < 20000; Init_i++);
    }
    AFE44xx_Default_Reg_Init();
}

/**********************************************************************************************************
*	        AFE44x0_Reg_Write           					  			  *
***********************************************************************************************************/
void AFE44x0_Reg_Write (unsigned char reg_address, unsigned long data)
{
  unsigned char dummy_rx;

  P4OUT&= ~0x01;   //  SEN LOW FOR TRANSMISSION.
  // Loop unrolling for machine cycle optimization
  UCB1TXBUF = reg_address;                    // Send the first byte to the TX Buffer: Address of register
  while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
  dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
  UCB1TXBUF = (unsigned char)(data >>16);     // Send the second byte to the TX Buffer: Data[23:16]
  while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
  dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
  UCB1TXBUF = (unsigned char)(((data & 0x00FFFF) >>8));       // Send the third byte to the TX Buffer: Data[15:8]
  while ( (UCB1STAT & UCBUSY) );		                // USCI_B1 TX buffer ready?
  dummy_rx = UCB1RXBUF;			                // Dummy Read Rx buf
  UCB1TXBUF = (unsigned char)(((data & 0x0000FF)));           // Send the first byte to the TX Buffer: Data[7:0]
  while ( (UCB1STAT & UCBUSY) );		                // USCI_B1 TX buffer ready?
  dummy_rx = UCB1RXBUF;			                // Dummy Read Rx buf
  P4OUT|=0x01;  // SEN HIGH
}

/**********************************************************************************************************
*	        AFE44x0_Reg_Read           					  			  *
***********************************************************************************************************/
unsigned long AFE44x0_Reg_Read(unsigned char Reg_address)
{
  unsigned char SPI_Rx_buf[4];
  unsigned long retVal;
  retVal = 0;
  P4OUT&= ~0x01;   //  SEN LOW FOR TRANSMISSION.
  // Loop unrolling for machine cycle optimization
  UCB1TXBUF = Reg_address;                    // Send the first byte to the TX Buffer: Address of register
  while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
  SPI_Rx_buf[0] = UCB1RXBUF;			// Read Rx buf
  UCB1TXBUF = 0;                              // Send the second byte to the TX Buffer: dummy data
  while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
  SPI_Rx_buf[1] = UCB1RXBUF;			// Read Rx buf: Data[23:16]
  UCB1TXBUF = 0;                              // Send the third byte to the TX Buffer: dummy data
  while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
  SPI_Rx_buf[2] = UCB1RXBUF;			// Read Rx buf: Data[15:8]
  UCB1TXBUF = 0;                              // Send the first byte to the TX Buffer: dummy data
  while ( (UCB1STAT & UCBUSY) );		// USCI_B1 TX buffer ready?
  SPI_Rx_buf[3] = UCB1RXBUF;			// Read Rx buf: Data[7:0]
  P4OUT|=0x01;  // set HIGH at end of transmission
  retVal = SPI_Rx_buf[1];
  retVal = (retVal << 8) | SPI_Rx_buf[2];
  retVal = (retVal << 8) | SPI_Rx_buf[3];
  //retVal = (SPI_Rx_buf[1]<<16)|(SPI_Rx_buf[2]<<8)|(SPI_Rx_buf[3]);
  return 	retVal;
}

/**********************************************************************************************************
*	        Enable_AFE44x0_SPI_Read        	                  					  *
**********************************************************************************************************/
void Enable_AFE44x0_SPI_Read (void)
{
  unsigned char dummy_rx;
  //Set Control0 - Enable SPI Read bit
  P4OUT&= ~0x01;                        // CS LOW for start of transmission.
  // Loop unrolling for machine cycle optimization
  UCB1TXBUF = 0;                        // Send the first byte to the TX Buffer: Address of register
  while ( (UCB1STAT & UCBUSY) );	// USCI_B1 TX buffer ready?
  dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
  UCB1TXBUF = 0;                        // Send the second byte to the TX Buffer: Data[23:16]
  while ( (UCB1STAT & UCBUSY) );	// USCI_B1 TX buffer ready?
  dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
  UCB1TXBUF = 0;                        // Send the third byte to the TX Buffer: Data[15:8]
  while ( (UCB1STAT & UCBUSY) );	// USCI_B1 TX buffer ready?
  dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
  UCB1TXBUF = 1;                        // Send the first byte to the TX Buffer: Data[7:0]
  while ( (UCB1STAT & UCBUSY) );	// USCI_B1 TX buffer ready?
  dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
  P4OUT|=0x01;                          // CS High for end of transmission
}

/**********************************************************************************************************
*	        Disable_AFE44x0_SPI_Read       	                  					  *
**********************************************************************************************************/
void Disable_AFE44x0_SPI_Read (void)
{
  unsigned char dummy_rx;
  //Set Control0 - Disable SPI Read bit
  P4OUT&= ~0x01;                        // CS LOW for start of transmission.
  // Loop unrolling for machine cycle optimization
  UCB1TXBUF = 0;                        // Send the first byte to the TX Buffer: Address of register
  while ( (UCB1STAT & UCBUSY) );	// USCI_B1 TX buffer ready?
  dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
  UCB1TXBUF = 0;                        // Send the second byte to the TX Buffer: Data[23:16]
  while ( (UCB1STAT & UCBUSY) );	// USCI_B1 TX buffer ready?
  dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
  UCB1TXBUF = 0;                        // Send the third byte to the TX Buffer: Data[15:8]
  while ( (UCB1STAT & UCBUSY) );	// USCI_B1 TX buffer ready?
  dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
  UCB1TXBUF = 0;                        // Send the first byte to the TX Buffer: Data[7:0]
  while ( (UCB1STAT & UCBUSY) );	// USCI_B1 TX buffer ready?
  dummy_rx = UCB1RXBUF;			// Dummy Read Rx buf
  P4OUT|=0x01;                          // CS High for end of transmission
}


/**********************************************************************************************************/
// Echo character
#pragma vector=USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
{
    switch(__even_in_range(UCB1IV,4))
    {
    case 0:break;                             // Vector 0 - no interrupt
    case 2:                                   // Vector 2 - RXIFG
      break;
    case 4:break;                             // Vector 4 - TXIFG
    default: break;
    }
}


// End of file
