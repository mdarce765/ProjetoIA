/*
 * File:          Controlador.c
 * Date:
 * Description:
 * Author:
 * Modifications:
 */

/*
 * You may need to add include files like <webots/distance_sensor.h> or
 * <webots/motor.h>, etc.
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/led.h>
#include <webots/supervisor.h>

/*
 * You may want to add macros here.
 */

#define TIME_STEP 16
#define QtddSensoresProx 8
#define QtddLeds 10
#define QtddCaixa 20
/*
 * This is the main program.
 * The arguments of the main function can be specified by the
 * "controllerArgs" field of the Robot node
 */


int main(int argc, char **argv) {
  int i=0;
  char texto[256];
  double LeituraSensorProx[QtddSensoresProx];
  double AceleradorDireito=1.0, AceleradorEsquerdo=1.0;
  double listaPos[QtddCaixa][3];
  /* necessary to initialize webots stuff */

  for(i=0;i<257;i++) texto[i]='0';
  wb_robot_init();
  //configurei MOTORES
  WbNodeRef caixa[QtddCaixa];
  char nomeCaixa[10]={0};
  for(i=0;i<QtddCaixa;i++){
       sprintf(nomeCaixa,"CAIXA%02d",i+1); //form os nomes dos sensores
       caixa[i] = wb_supervisor_node_get_from_def(nomeCaixa);
       if(caixa[i]!=NULL)
          printf("%2d. %s  -  %p\n",i,nomeCaixa,(void*)caixa[i]);
       else
          printf("Falha ao carregar a posição da %s\n",nomeCaixa);
    }
  printf("\n\n CAIXAS OK  \n\n");
  for(i=0;i<QtddCaixa;i++){ //colocando as pos iniciais
     const double *PosicaoCaixa = wb_supervisor_node_get_position(caixa[i]);
     listaPos[i][0] = PosicaoCaixa[0];
     listaPos[i][1] = PosicaoCaixa[1];
     listaPos[i][2] = PosicaoCaixa[2];
     printf("CAIXA%02d %f   %f  %f\n\n",i+1,PosicaoCaixa[0],PosicaoCaixa[1],PosicaoCaixa[2]);
  }
  WbDeviceTag MotorEsquerdo, MotorDireito;
  MotorEsquerdo = wb_robot_get_device("left wheel motor");
  MotorDireito = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(MotorEsquerdo, INFINITY);
  wb_motor_set_position(MotorDireito, INFINITY);
  wb_motor_set_velocity(MotorEsquerdo,0);
  wb_motor_set_velocity(MotorDireito,0);
  
   //configurei Sensores de Proximidade
   WbDeviceTag SensorProx[QtddSensoresProx];
   SensorProx[0] = wb_robot_get_device("ps0");
   SensorProx[1] = wb_robot_get_device("ps1");
   SensorProx[2] = wb_robot_get_device("ps2");
   SensorProx[3] = wb_robot_get_device("ps3");
   SensorProx[4] = wb_robot_get_device("ps4");
   SensorProx[5] = wb_robot_get_device("ps5");
   SensorProx[6] = wb_robot_get_device("ps6");
   SensorProx[7] = wb_robot_get_device("ps7");
   wb_distance_sensor_enable(SensorProx[0],TIME_STEP);
   wb_distance_sensor_enable(SensorProx[1],TIME_STEP);
   wb_distance_sensor_enable(SensorProx[2],TIME_STEP);
   wb_distance_sensor_enable(SensorProx[3],TIME_STEP);
   wb_distance_sensor_enable(SensorProx[4],TIME_STEP);
   wb_distance_sensor_enable(SensorProx[5],TIME_STEP);
   wb_distance_sensor_enable(SensorProx[6],TIME_STEP);
   wb_distance_sensor_enable(SensorProx[7],TIME_STEP);

    //config leds
    WbDeviceTag Leds[QtddLeds];
    Leds[0] = wb_robot_get_device("led0");
    wb_led_set(Leds[0],-1);

  /*
   * You should declare here WbDeviceTag variables for storing
   * robot devices like this:
   *  WbDeviceTag my_sensor = wb_robot_get_device("my_sensor");
   *  WbDeviceTag my_actuator = wb_robot_get_device("my_actuator");
   */

  /* main loop
   * Perform simulation steps of TIME_STEP milliseconds
   * and leave the loop when the simulation is over
   */
  while (wb_robot_step(TIME_STEP) != -1) {
    for(i=0;i<256;i++) texto[i]=0;
    //memcpy(texto,0,10);
    
    /*
     * Read the sensors :
     * Enter here functions to read sensor data, like:
     *  double val = wb_distance_sensor_get_value(my_sensor);
     */
     
    /* Process sensor data here */
    
    for(i=0;i<QtddSensoresProx;i++){
       LeituraSensorProx[i]= wb_distance_sensor_get_value(SensorProx[i])-60;
       sprintf(texto,"%s|%d: %5.2f  ",texto,i,LeituraSensorProx[i]);
    }
    printf("%s\n",texto);
    wb_led_set(Leds[0], wb_led_get(Leds[0])*-1);
    
    /*
     * Enter here functions to send actuator commands, like:
     * wb_motor_set_position(my_actuator, 10.0);
     */
    // sensores que importam ps0-ps2 e ps5-ps7
    int timeout = 0;
       if(LeituraSensorProx[0]>15 || LeituraSensorProx[7]>15 
       || LeituraSensorProx[6]>100 || LeituraSensorProx[1]>100){
        AceleradorDireito = -1;
        AceleradorEsquerdo = 1;
        double atual = wb_robot_get_time();
        
        for(int i=0;i<QtddCaixa;i++){
          const double *PosicaoCaixa = wb_supervisor_node_get_position(caixa[i]);
          printf("CAIXA%02d %f   %f  %f\n\n",i+1,PosicaoCaixa[0],PosicaoCaixa[1],PosicaoCaixa[2]);
          for (int m = 0;m<3; ++m){
            char num1[50];
            char num2[50];
            sprintf(num1,"%5.2f",listaPos[i][m]);
            sprintf(num2,"%5.2f",PosicaoCaixa[m]);
           
            if (strcmp(num1,num2) != 0){
              printf("m=%d lista:%s PosAtual:%s\n",m,num1,num2);
              printf("Mexeu\n");
              //dancinha!
              while (wb_robot_step(TIME_STEP) != -1) {
                AceleradorDireito = -1;
                AceleradorEsquerdo = 1;
                wb_motor_set_velocity(MotorEsquerdo,6.28*AceleradorEsquerdo);
                wb_motor_set_velocity(MotorDireito, 6.28*AceleradorDireito);
               }
            }
          }
        } 
       }
       else {
        AceleradorDireito = 1;
        AceleradorEsquerdo = 1;
      }
      wb_motor_set_velocity(MotorEsquerdo,6.28*AceleradorEsquerdo);
      wb_motor_set_velocity(MotorDireito, 6.28*AceleradorDireito);
  };
  /* Enter your cleanup code here */

  /* This is necessary to cleanup webots resources */
  wb_robot_cleanup();
  return 0;
}