/* ---------------Attach interrupt to PIN A1-------------------
*  1. Configure Pin A1 and assign to EXTIN0
*  2. Configure External Interrupt Controller
*  3. Configure the Generic Clock 1 to take the OSCULP32K input
*  4. Assign the OSCULP32K as a clock source for the interrupt
*-------------------------------------------------------------- */

#include "SodaqEIC.h"
#include "HeaderFiles/LoRaExercise.h"
//#include "commonMacros.h"


void configEIC()
{
// ---------------Attach interrupt to PIN A1-------------------
//  1. Configure Pin A1 and assign to EXTIN0
//  2. Configure External Interrupt Controller
//  3. Configure the Generic Clock 1 to take the OSCULP32K input
//  4. Assign the OSCULP32K as a clock source for the interrupt


//Enable EIC clock in power manager
PM->APBAMASK.reg |= PM_APBAMASK_EIC;

   // 1. Configure Pins A1,A2,A3
      debugl("Configure pin A1");
      for (int i = 0;i<3;i++){
         PORT->Group[1].DIRCLR.reg = 1;
         PORT->Group[1].PINCFG[i].reg= PORT_PINCFG_INEN |   //Input Enable
                                       PORT_PINCFG_PMUXEN | //MUX on
                                       PORT_PINCFG_PULLEN;  //Pull Resistor

         PORT->Group[1].DIR.reg = 0;   //Configure as Input
         PORT->Group[1].OUT.reg = 0;   //Configure as Pull_down
      }


      //Assign Pins A1,A2,A3 to EXTINT0,EXTINT1,EXTINT2.
      PORT->Group[1].PMUX[0].bit.PMUXE = MUX_PB00A_EIC_EXTINT0;
      PORT->Group[1].PMUX[1].bit.PMUXE = MUX_PB01A_EIC_EXTINT1;
      PORT->Group[1].PMUX[2].bit.PMUXE = MUX_PB02A_EIC_EXTINT2;
      debugl("   - Pins Configured\n");


   // 2. Configure External Interrupt Controller
      debugl("Configure EIC");
      EIC->INTENSET.reg |= EIC_INTENSET_EXTINT0 |     //Enable EXTINT0
                           EIC_INTENSET_EXTINT1 |     //Enable EXTINT1
                           EIC_INTENSET_EXTINT2;      //Enable EXTINT2

      EIC->CONFIG[0].reg |=  EIC_CONFIG_SENSE0_RISE |   //Detect rising edge
                             EIC_CONFIG_SENSE1_RISE |
                             EIC_CONFIG_SENSE2_RISE |
                             EIC_CONFIG_FILTEN0 |       //Filter signal
                             EIC_CONFIG_FILTEN1 |
                             EIC_CONFIG_FILTEN2;

      EIC->WAKEUP.reg|= EIC_WAKEUP_WAKEUPEN0 |   //Wakeup on Interrup0
                        EIC_WAKEUP_WAKEUPEN1 |   //Wakeup on Interrupt1
                        EIC_WAKEUP_WAKEUPEN2;    //Wakeup on Interrupt2

      EIC->EVCTRL.reg|= EIC_EVCTRL_EXTINTEO0 |   //Enable for every detection
                        EIC_EVCTRL_EXTINTEO1 |
                        EIC_EVCTRL_EXTINTEO2;

      debugl("   - EIC Configured\n");


   // 3. Configure the Generic Clock 1 to take the OSCULP32K input
      debugl("Configuring GCLK1");
      GCLK->GENDIV.reg =   GCLK_GENDIV_ID(GCLK_EIC_ULP32K) |
                           GCLK_GENDIV_DIV(1);  //Dont slow down clock. Div by 1
                           debugl("enabled");
         //Oscillator Stabilisation
         while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY){
          // Wait for synchronization
         }

      //Write the configuration
      debugl("Writing config");
      GCLK->GENCTRL.reg =  GCLK_GENCTRL_ID(GCLK_EIC_ULP32K) |
                           GCLK_GENCTRL_SRC_OSCULP32K |
                           GCLK_GENCTRL_DIVSEL |
                           GCLK_GENCTRL_GENEN;
                           debugl("config written");

         //Oscillator Stabilisation
            while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY ){
               // Wait for synchronization
               }
               debugl("   - GCLK1 Configured\n");


   // 4. Assign the OSCULP32K as a clock source for the interrupt
      debugl("Assign GCLK to EIC");
      GCLK->CLKCTRL.reg =  ((EIC_GCLK_ID << GCLK_CLKCTRL_ID_Pos) |
                           GCLK_CLKCTRL_GEN_GCLK2 |   //Use Generic Clock 'GEN2'
                           GCLK_CLKCTRL_CLKEN);       //Enable Generic Clock

         while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY ){
            // Wait for synchronization
         }
         debugl("   - Ext Interrupts Configured\n");

   // 6. Enable Exte;rnal Interrupt Controller (EIC)
      EIC->CTRL.reg |= EIC_CTRL_ENABLE;            //Enable EIC
      while(EIC->STATUS.bit.SYNCBUSY & EIC_STATUS_SYNCBUSY);
      NVIC_EnableIRQ(EIC_IRQn);
      NVIC_SetPriority(EIC_IRQn, 0);

}











/*
void EICInit()
{

   //Enable EIC clock in power manager
   PM->APBAMASK.reg |= PM_APBAMASK_EIC;


   setExtIntPins();


   // 2. Configure External Interrupt Controller
   debugl("Configure EIC");
   EIC->INTENSET.reg =  EIC_INTENSET_EXTINT0; //Enable EXTINT0
   EIC->CONFIG[0].reg=  EIC_CONFIG_SENSE0_RISE |   //Detect rising edge
                        EIC_CONFIG_FILTEN0;        //Filter signal
   EIC->WAKEUP.reg = EIC_WAKEUP_WAKEUPEN0;   //Wakeup on Interrup0
   EIC->EVCTRL.reg = EIC_EVCTRL_EXTINTEO0;   //Enable for every detection

   debugl("   - EIC Configured\n");

   configGCLK1();

}


void setExtIntPins()
{
   // 1. Configure Pin A1 and Assign to EXTIN0
      debugl("Configure pin A1");
      PORT->Group[1].DIRCLR.reg = 1;
      PORT->Group[1].PINCFG[0].reg= PORT_PINCFG_INEN |   //Input Enable
                                    PORT_PINCFG_PMUXEN | //MUX on
                                    PORT_PINCFG_PULLEN;  //Pull Resistor

      PORT->Group[1].DIR.reg = 0;   //Configure as Input
      PORT->Group[1].OUT.reg = 0;   //Configure as Pull_down

      EICAttachInterrupt();
}


void configGCLK1()
{
   // 3. Configure the Generic Clock 1 to take the OSCULP32K input
      debugl("Configuring GCLK1");
      GCLK->GENDIV.reg =   GCLK_GENDIV_ID(GCLK_EIC_ULP32K) |
                           GCLK_GENDIV_DIV(8);  //Slow Down Clock. Div by 8.

                           debugl("enabled");
         //Oscillator Stabilisation
         while ( GCLK1isSyncing()){
          // Wait for synchronization
         }

      //Write the configuration
      debugl("Writing config");
      GCLK->GENCTRL.reg =  GCLK_GENCTRL_ID(GCLK_EIC_ULP32K) |
                           GCLK_GENCTRL_SRC_OSCULP32K |
                           GCLK_GENCTRL_DIVSEL |
                           GCLK_GENCTRL_GENEN;
                           debugl("config written");

         //Oscillator Stabilisation
         while ( GCLK1isSyncing() )
               debugl("   - GCLK1 Configured\n");


   // 4. Assign the OSCULP32K as a clock source for the interrupt
      debugl("Assign GCLK to EIC");
      GCLK->CLKCTRL.reg =  ((EIC_GCLK_ID << GCLK_CLKCTRL_ID_Pos) |
                           GCLK_CLKCTRL_GEN_GCLK2 |   //Use Generic Clock 'GEN2'
                           GCLK_CLKCTRL_CLKEN);       //Enable Generic Clock

      while (GCLK1isSyncing())
         debugl("   - Ext Interrupts Configured\n");

}




void EICEnable()
{
   EIC->CTRL.reg |= EIC_CTRL_ENABLE;   //Turn on External Interrupts
      while(EICisSyncing());

}

void EICDisable()
{
   EIC->CTRL.reg &= ~EIC_CTRL_ENABLE;   //Turn on External Interrupts
      while(EICisSyncing());

}


void EICAttachInterrupt()
{
   //Assign Pin A1 to EXTINT0
   PORT->Group[1].PMUX[0].bit.PMUXE = MUX_PB00A_EIC_EXTINT0;
   PORT->Group[1].PMUX[0].bit.PMUXE = MUX_PB00A_EIC_EXTINT1;
   PORT->Group[1].PMUX[0].bit.PMUXE = MUX_PB00A_EIC_EXTINT2;
   debugl("   - Pin Configured\n");

   NVIC_EnableIRQ(EIC_IRQn);
   NVIC_SetPriority(EIC_IRQn, 0);
}

void EICDetachInterrupt()
{

   NVIC_DisableIRQ(EIC_IRQn);

}
*/

inline bool GCLK1isSyncing()
{
   return (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
}

inline bool EICisSyncing()
{
   return(EIC->STATUS.bit.SYNCBUSY & EIC_STATUS_SYNCBUSY);

}
