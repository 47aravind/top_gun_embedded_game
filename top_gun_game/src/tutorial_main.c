/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name     : tutorial_main.c
* Version       : 1.0
* Device(s)     : RX63N
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* H/W Platform  : YRDKRX63N
* Description   : Top Gun game                 

*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 24.02.2012     1.00        First release              
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "platform.h" 
#include "r_switches.h"
#include "cmt_periodic_multi.h"
#include "timer_adc.h"
#include "flash_led.h"

#include "flash_led.h"			/*test*/
#include "cmt_periodic_multi.h"	/*test*/

/*******************************************************************************
Macro Definitions
*******************************************************************************/
#define TIMER_COUNT_600MS 56250  /* 600mS with 48MHz pclk/512 */
#define MCU_NAME "   RX63N    "
#define CHANNEL_0   0
#define TIMER_COUNT_100MS   9375
#define  LED4                   PORTD.PODR.BIT.B5
#define  LED5                   PORTE.PODR.BIT.B3
#define  LED6                   PORTD.PODR.BIT.B2
#define  LED7                   PORTE.PODR.BIT.B0
#define  LED8                   PORTD.PODR.BIT.B4
#define  LED9                   PORTE.PODR.BIT.B2
#define  LED10                  PORTD.PODR.BIT.B1
#define  LED11                  PORTD.PODR.BIT.B7
#define  LED12                  PORTD.PODR.BIT.B3
#define  LED13                  PORTE.PODR.BIT.B1
#define  LED14                  PORTD.PODR.BIT.B0
#define  LED15                  PORTD.PODR.BIT.B6
#define TIMER_COUNT_400MS   36375
#define TIMER_COUNT_100MS   9375 /* 0.1 second with 48MHz pclk / 512. */ 
#define CHANNEL_0   0
#define CHANNEL_1   1
/*******************************************************************************
External functions and public global variables. 
*******************************************************************************/
/* Statics test replacement variable */
uint8_t g_working_string[13] = "   STATIC   "; /* 12 characters plus NULL terminator. *//* Statics test const variable */
const uint8_t g_replacement_str[] = "TEST TEST"; /* Must be 12 chars or less. */
volatile bool g_sw1_press = false;  /*uncomment it*/
volatile bool g_sw2_press = false;
volatile bool g_sw3_press = false;
volatile bool enemy_flag = 0; 			//variable for creating enemy plane
volatile bool hit_flag = 0; 			//variable for tracking hits and hit animation
extern volatile bool g_sw1_press;		/*test*/
volatile static uint32_t g_flash_count = 200u;	/*test*/
/*******************************************************************************
Private function prototypes and global variables
*******************************************************************************/
static void statics_test(void);
static void timer_callback(void);
static void All_Red_LEDs_ON(void);
static void Blink_Red_LEDs(void);
volatile static bool g_delay_complete = false;
static void Reset_All_LEDs(void);
static void Block_Until_Switch_Press(void);
static void Block_Until_Switch_Press1(void);
static void cmt_callback_flash(void);		/*test*/
void toggle_led1(void);	
void train(void);							/*test*/
uint8_t x=0,y=0,z,valy_1,valx_1;
int c =0,d=0;
int pause_status=1;							/*1=pause;0=game_run*/
int k =0,j=0,w;								/*variables for loops*/
/******************************************************************************
* Function name: main
* Description  : Main program function. The function first initialises the debug
*                LCD, and displays the splash screen on the LCD. The function
*                then calls the flash_led, timer_adc and statics_test functions. 
*                These start by flashing the LEDs at a fixed rate until a switch is
*                pressed, then the flash rate varies with the position of the   
*                pot, RV1. Finally, statics_test displays " STATICS " on the    
*                debug LCD, which is replaced with "TEST TEST" a letter at a 
*                time.
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{	
    /* Initialize the debug LCD on the RSPI bus. */
    lcd_initialize();
   														 /* Clear LCD */
    lcd_clear();    
    													/* Display message on LCD */
    lcd_display(LCD_LINE1, "  RENESAS   ");
    lcd_display(LCD_LINE2, MCU_NAME);
	Set_Font_Bitmap();
	Set_LCD_Pos(0,0);
	Set_LCD_Char(0);
    cmt_init();											/* Initialize the CMT unit for application timing tasks. */															
	cmt_callback_set(CHANNEL_0, &Blink_Red_LEDs);		/* Start 100mS count on cmt channel 0. */    													
    cmt_start(CHANNEL_0, TIMER_COUNT_400MS);			/* Wait in a loop until the variable g_flash_count is zero. LEDs continue to
    													   flash in the meantime. If SW1 is pressed the count is zeroed early. */
  	Set_Font_Bitmap();
	Set_LCD_Pos(0,0);
	Set_LCD_Char(0);
	Block_Until_Switch_Press();
	for(k=0;k<10000000;k++)
	;
	lcd_clear();
	Set_Font_8_by_8();	 
	lcd_display(LCD_LINE1, "  Aravind   ");
	lcd_display(LCD_LINE3, "  Roshan   ");
	cmt_stop(CHANNEL_0);
	cmt_init();
	cmt_callback_set(CHANNEL_1, &Blink_Red_LEDs);	 /* Start 100mS count on cmt channel 0. */
												    
    cmt_start(CHANNEL_1, TIMER_COUNT_100MS);
    Block_Until_Switch_Press1();
    lcd_clear();
    g_sw1_press = false;							 /* Done with the flag so clear it. */						    	
	cmt_stop(CHANNEL_1);							 /* Stop counting on cmt channel 0. */    
    while (1)
    {      
	  if(PORT4.PIDR.BIT.B1==0||PORT4.PIDR.BIT.B0==0||PORT4.PIDR.BIT.B4==0)
	  { 	
		 for(k=0;k<10000000;k++)
		 ;	  							
	 	 while(pause_status==1)
	 	 {	
	 		 	lcd_clear();
				All_Red_LEDs_ON();
				Set_Font_8_by_8();		/*Pause status = 0;means game is paused*/
				lcd_display(LCD_LINE1, "  Paused   ");
				for(k=0;k<10000000;k++)
				; 
	 		 if(SW3 == 0||SW1 == 0||SW2 == 0)
			 {
			 for(k=0;k<10000000;k++)
		     ;
			 pause_status=0;	 
			 break;
			 }			 			 
	 	 }		  	  	 
	    while(pause_status==0)
	     {
				lcd_clear();	
				Reset_All_LEDs();			
				pause_status=1;			/*Pause status = 1;means game is unpaused*/
				timer_adc();
				if(SW3 == 0)
			 	{
			 	for(k=0;k<10000000;k++)
		     	;	 
			 	break;
			 	}
		}   			
       }    					
      }
} 

						/* End of function main(). */


/******************************************************************************
* Function name: timer_callback
* Description  : This function is called from the cmt_timer compare-match ISR.
*              : It sets a global flag that is polled for delay loop control.
* Arguments    : None
* Return value : None
******************************************************************************/
void timer_callback(void)
{
    cmt_stop(CHANNEL_0);
    g_delay_complete = true;
    
} /* End of function timer_callback() */



/******************************************************************************
* Function name: sw1_callback
* Description  : Callback function that is executed when SW1 is pressed.
*                Called by sw1_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw1_callback(void)
{
    g_sw1_press = true; /* The switch was pressed. */
} /* End of function sw1_callback() */


/******************************************************************************
* Function name: sw2_callback
* Description  : Callback function that is executed when SW2 is pressed.
*                Called by sw2_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw2_callback(void)
{
    g_sw2_press = true; /* The switch was pressed. */
} 						/* End of function sw2_callback() */


/******************************************************************************
* Function name: sw3_callback
* Description  : Callback function that is executed when SW3 is pressed.
*                Called by sw3_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw3_callback(void)
{
    g_sw3_press = true; /* The switch was pressed. */
} 						/* End of function sw3_callback() */

/******************************************************************************
* Function name: Reset_All_LEDs
* Description  : Resets all the LEDS
* Arguments    : none
* Return value : none
******************************************************************************/

static void  Reset_All_LEDs(void)
{
		 
	LED4=1;
	LED5=1;	
	LED6=1;
	LED7=1;
	LED8=1;
	LED9=1;
	LED10=1;
	LED11=1;
	LED12=1;
	LED13=1;
	LED14=1;
	LED15=1;
	for(k=0;k<10000000;k++)
	  	{
			  ;
	 	 }
}
/******************************************************************************
* Function name: Block_Until_Switch_Press
* Description  : Infinite loop,stoips executing and control goes back to main 
*                when any of the switch is pressed
* Arguments    : none
* Return value : none
******************************************************************************/
static void Block_Until_Switch_Press(void)
{
	int k;
	while(1)
	{
	if((PORT4.PIDR.BIT.B0==0)||(PORT4.PIDR.BIT.B1==0)||(PORT4.PIDR.BIT.B4==0))
	{
	
	break;
	}
	
	}
	for(k=0;k<10000000;k++)
	{
	
	}
}
/******************************************************************************
* Function name: Block_Until_Switch_Press1
* Description  : Infinite loop,stoips executing and control goes back to main 
*                when any of the switch is pressed
* Arguments    : none
* Return value : none
******************************************************************************/
static void Block_Until_Switch_Press1(void)
{
	int k;
	while(1)
	{
	if((PORT4.PIDR.BIT.B0==0)||(PORT4.PIDR.BIT.B1==0)||(PORT4.PIDR.BIT.B4==0))
	{
	
	break;
	}
	
	}
	for(k=0;k<100;k++)
	{
	
	}
}
/******************************************************************************
* Function name: Blink_Red_LEDs
* Description  : Blinks all the red Leds.
*                
* Arguments    : none
* Return value : none
******************************************************************************/

static void Blink_Red_LEDs(void)
{	 
	LED6=~LED6;
	LED7=~LED7;
	LED10=~LED10;
	LED11=~LED11;
	LED14=~LED14;
	LED15=~LED15;		
}
/******************************************************************************
* Function name: create_Obstacle
* Description  : Creates the enemy jets and the main jets and the loop to control the 
*                main jetie moving up and down.
* Arguments    : none
* Return value : none
******************************************************************************/
void create_Obstacle(void)
{	
	while(1)
	{
	Set_Font_Bitmap();
	for(k=0;k<1000000;k++)
	;
	lcd_clear();
	if(PORT4.PIDR.BIT.B0==0)			/* To move the jet up */
	{ 
		y=y-8;
		if(y<0)
			 {
			 	y=0;
			 }
		for(k=0;k<1000000;k++)
		;
		Set_Font_Bitmap();
		Set_LCD_Pos(0,y);
		Set_LCD_Char(3);
		Set_Font_Bitmap();
	} 
	else if(PORT4.PIDR.BIT.B1==0)
	{
		lcd_clear();
		y=y+8;
		if(y>48)
		{
		y=48;
		}
		for(k=0;k<1000000;k++)
		;
		Set_Font_Bitmap();
		Set_LCD_Pos(0,y);
		Set_LCD_Char(3);	
	}
	if(PORT4.PIDR.BIT.B4==0)
	{
		break;
		for(k=0;k<10000000;k++)
				;
	}
	if(!enemy_flag)
    { 							//create enemy plane
		valx_1 = 80;		 			//spawn on the other side of the screen
		valy_1 = 8*(rand() % 8);
		enemy_flag = 1;
	}
	else
	{
		if(valx_1 <= 23) //check for collision
		{
			if((valy_1 <= (y + 16)) && (valy_1 >= y))
				{	
					enemy_flag = 0; 		//respawn enemy
					hit_flag = 1;
				}
		}
		if(valx_1 == 0)
		enemy_flag = 0;
		else
		{
			valx_1 = valx_1 - 5;
			if(hit_flag == 0)
			{
				Set_LCD_Pos(valx_1, valy_1);
				Set_LCD_Char(7);		 //draw jet
				Set_LCD_Pos(0,y);
				Set_LCD_Char(3);
			}
			else
			{
			Set_LCD_Pos(valx_1,valy_1);
			Set_LCD_Char(7);
			Set_LCD_Pos(x, y);
			Set_LCD_Char(1);
			for(k=0;k<10000000;k++)
			;
			lcd_clear();
			Set_Font_8_by_8();	 
			lcd_display(LCD_LINE2, "  Game Over   ");
			for(k=0;k<100000000;k++)
			;
			hit_flag = 1;
			}
		}
	for( c=0 ; c<5000;c++)
		{
			for( d=0 ; d<250;d++)
			{
				;
			}	
		}		
	}	
				
	}
}
/******************************************************************************
* Function name: All_Red_LEDs_ON
* Description  : Turns On all the red LEDs
*                
* Arguments    : none
* Return value : none
******************************************************************************/
static void  All_Red_LEDs_ON(void)
{	
	LED6=0;
	LED7=0;
	LED10=0;
	LED11=0;
	LED14=0;
	LED15=0;
}
