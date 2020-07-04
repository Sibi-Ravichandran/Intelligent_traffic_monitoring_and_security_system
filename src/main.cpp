#include <stdio.h>
#include "mbed.h"
#include "TextLCD.h"
#include "Keypad.h"

// Serial connection for PC and GSM 
Serial gsm(PTE0,PTE1);
Serial pc(USBTX,USBRX);

// Inputs from IR Sensors
DigitalIn R1(PTD0);
DigitalIn R2(PTD2);

// Pin connection for LCD 16x2:
TextLCD lcd(PTA1,PTA2,PTD4,PTA12,PTA4,PTA5, TextLCD::LCD16x2); // Rs, E, d4, d5, d6, d7

void delay()
{
    long i,j;
    for(i=0; i<10000000; i++) {
        j=i/10;
        j=j/2;
    }
}

void delay2()
{
    long i,j;
    for(i=0; i<50000000; i++) {
        j=i/10;
        j=j/2;
    }
}



int main()
{
    
    // Variable declarations: 
    char templ[4];
    int cnt;
    int gsmf1=0;
    char buf[40];
    char buf1[40];

    // baud rates for pc and gsm
    pc.baud(9600);
    gsm.baud(115200);

    // Print Traffic Intensity on the LCD:
    lcd.locate(0,0);
    lcd.printf("TRAFFIC          ");
    lcd.locate(0,1);
    lcd.printf("INTENSITY:  ");

    // send AT command via GSM and see the response in pc terminal:
    gsm.printf("AT\r\n");
    gsm.scanf("%s",buf);
    pc.printf("%s\n",buf);
    gsm.scanf("%s",buf1);
    pc.printf("%s\n",buf1);
    
   /*pc.printf("AT");
    delay2();
    pc.putc(13);
    pc.putc(10); */
    
    // send AT command via GSM and see the response in pc terminal:
    gsm.printf("AT+CMGF=1\r\n");
    gsm.scanf("%s",buf);
    gsm.scanf("%s",buf1);
    pc.printf("%s\n",buf);
    pc.printf("%s\n",buf1);
 
    /*pc.printf("AT+CMGF=1");
    delay2();
       pc.putc(13);
    pc.putc(10);*/
 
    // Increment and decrement counter based on IR inputs:
    while(1) {

        if(R1==1) {
            if(cnt<50)
                cnt=cnt+1;
            delay();
        }
        if(R2==1) {
            if(cnt>0)
                cnt=cnt-1;
            delay();
        }

        templ[0]=(cnt / 100) ;
        templ[1]=((cnt - (templ[0]*100))/10);
        templ[2]=(cnt - (templ[1]*10)- (templ[0]*100)) + 0x30;
        templ[0]=templ[0]+0x30;
        templ[1]=templ[1]+0x30;

        lcd.locate(13,1);
        lcd.putc(templ[0]);
        lcd.putc(templ[1]);
        lcd.putc(templ[2]);

        if (cnt<=5){
            lcd.locate(12,0);
            lcd.printf("    ");
           }

        if(cnt>5) {
            if(gsmf1==0) {
                lcd.locate(12,0);
                lcd.printf("HIGH");
                
                
                /* 
                pc.printf("AT+CMGS=\"4389294294\"");
                delay2();
                pc.putc(13);
                pc.putc(10);
                delay2();            
                pc.printf("High Traffic");
                delay2();
                pc.putc(26);
                delay2();
                */
                gsm.printf("AT+CMGS=\"4389294294\"\r\n");
                gsm.scanf("%s",buf);
                gsm.scanf("%s",buf1);
                pc.printf("%s\n",buf);
                pc.printf("%s\n",buf1);
                gsm.printf("High Traffic");
                gsm.scanf("%s",buf);
                gsm.scanf("%s",buf1);
                pc.printf("%s\n",buf);
                pc.printf("%s\n",buf1);
                pc.printf("end of operation");
                gsmf1=1;
 
            }
        } else {
            gsmf1=0;
        }
        delay();


    } // while(1)
} //main