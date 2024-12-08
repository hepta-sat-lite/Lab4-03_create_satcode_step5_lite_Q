#include "mbed.h"
#include "LITE_CDH.h"
#include "LITE_EPS.h"
#include "LITE_SENSOR.h"
LITE_CDH cdh(PB_5, PB_4, PB_3, PA_8, "sd", PA_3);
LITE_EPS eps(PA_0,PA_4);
LITE_SENSOR sensor(PA_7,PB_7,PB_6);
DigitalOut cond[]={PB_1, PA_5};
RawSerial gs(USBTX,USBRX,9600);
Timer sattime;
int rcmd = 0, cmdflag = 0; //command variable
 
//getting command and flag
void commandget()
{
    rcmd=gs.getc();
    cmdflag = 1;
}
//interrupting 
void receive(int rcmd, int cmdflag)
{
    gs.attach(commandget,Serial::RxIrq);
}
//initialzing
void initialize()
{
    rcmd = 0;
    cmdflag = 0;
    cond[0] = 0;
}

int main() {
    gs.printf("From Sat : Operation Start...\r\n");
    int flag = 0;     // condition
    float batvol,temp; //Voltage, Temerature
    receive(rcmd,cmdflag); //interupting by ground station command
    sattime.start();
    for(int i = 0; i < 100; i++) {
        //Sensing and Transmitting HK data
        eps.vol(&batvol);
        temp = 28.5;
        gs.printf("HEPTASAT::Condition = %d, Time = %f [s], BatVol = %.2f [V],Temp = %.2f [C]\r\n",flag,sattime.read(),batvol,temp);
        //Condition
        cond[0] = 1;
        //Power Saving Mode
        if(batvol <= 3.5) {
            eps.shut_down_regulator();
            gs.printf("Power saving mode ON\r\n"); 
            flag = 1;
        } else if((flag == 1) & (batvol > 3.7)) {
            eps.turn_on_regulator();
            flag = 0;
        }
        //Contents of command
        if (cmdflag == 1) {
            if (rcmd == 'a') {
               for(int j=0;j<5;j++){
                gs.printf("HEPTASAT::Hello World!\r\n");
                cond[0] = 0;
                wait_ms(1000);
                cond[0] = 1;
               }
            }else if (rcmd == 'b') {
                char str[100];
                mkdir("/sd/mydir", 0777);
                FILE *fp = fopen("/sd/mydir/test.txt","w");
                if(fp == NULL) {
                    error("Could not open file for write\r\n");
                }
                for(int i = 0; i < 10; i++) {
                    eps.vol(&batvol);
                    fprintf(fp,"%f\r\n",batvol);
                }
                fclose(fp);
                fp = fopen("/sd/mydir/test.txt","r");
                for(int j = 0; j < 10; j++) {
                    fgets(str,100,fp);
                    gs.puts(str);
                }
                fclose(fp);
            }else if (rcmd == 'c') {
                gs.printf("Hepta-Sat Lite Uplink Ok\r\n");
                float ax,ay,az;
                sensor.set_up();
                for(int i = 0; i<10; i++) {
                    sensor.sen_acc(&ax,&ay,&az);
                    gs.printf("acc : %f,%f,%f\r\n",ax,ay,az);
                    wait_ms(1000);
                }
            }else if (rcmd == 'd') {
                gs.printf("Hepta-Sat Lite Uplink Ok\r\n");
                float gx,gy,gz;
                sensor.set_up();
                for(int i = 0; i<10; i++) {
                    sensor.sen_gyro(&gx,&gy,&gz);
                    gs.printf("gyro: %f,%f,%f\r\n",gx,gy,gz);
                    wait_ms(1000);
                }
            }
            initialize();
        }
        
        //Operation Interval
        wait_ms(1000);
    }
    cond[0] = 0;
    sattime.stop();
    gs.printf("From Sat : Operation Stop...\r\n");
}
           