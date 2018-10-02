#ifndef AFE44xx_H_
#define AFE44xx_H_


#define AFE_RESETZ      BIT3
#define AFE_PDNZ        BIT4
#define AFE_ADC_DRDY    BIT5
#define AFE_PD_ALM      BIT6
#define AFE_LED_ALM     BIT7
#define AFE_DIAG_END    BIT2

/****************************************************************/
/* Global functions*/
/****************************************************************/
void Init_AFE44xx_Resource(void);
void AFE44xx_Default_Reg_Init(void);
void AFE44xx_Reg_Write(unsigned char Reg_address, unsigned long Reg_data);
unsigned long AFE44xx_Reg_Read(unsigned char Reg_address);


void AFE44x0_Reg_Write (unsigned char reg_address, unsigned long data);
unsigned long AFE44x0_Reg_Read(unsigned char Reg_address);
void Enable_AFE44x0_SPI_Read (void);
void Disable_AFE44x0_SPI_Read (void);

void Init_AFE44xx_DRDY_Interrupt (void);
void Enable_AFE44xx_DRDY_Interrupt (void);
void Disable_AFE44xx_DRDY_Interrupt (void);
void Set_GPIO(void);
void Set_UCB1_SPI(void);
void AFE44xx_Read_All_Regs(unsigned long AFE44xxeg_buf[]);
void AFE44xx_PowerOn_Init(void);



#endif /*AFE44xx_H_*/
