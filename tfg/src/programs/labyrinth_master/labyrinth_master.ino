/**
 * @file    labyrinth_master.ino
 * @brief   the master robot resolve labyrinth and send data to 
 *          the rest of the colony
 *
 * @author  María Blázquez Partido <maria.b.partido@alumnos.uc3m.es>
 * @date    2014-08
 *
 *  -- Asociacion de Robotica, University Carlos III of Madrid
 */

#include <Bluetooth.h>
#include <Robot.h>
#include <Servomotor.h>
#include <UltrasonicSensor.h>
#include <Ultrasonic.h>
#include <Servo.h>

float distr;
float distl;
int move=0;
int aux=6;
int cycle=0;
int i=0;
char d='0';
bool equal=false;
int *buffer = (int*)malloc(10*sizeof(int));
int buffer_size=0;
int buffer_limit=10;
char tmpChar='1';
bool end_pos=false;
bool paired=false;
Robot my_bot(1,10,9,5,7,6,8,3,4);

void setup()
{
    Serial.begin(38400);
    my_bot.bt.hear();//set bluetooth module into slave role
}
void loop()
{
    if (Serial.available())//if there's any data available
    {
        tmpChar = Serial.read();//read data
    }
  
    if(tmpChar=='s') // avoid obstacles to resolve labyrinth
    {
		//get sensor distance
        distr= my_bot.us_r.get_distance();
        distl= my_bot.us_l.get_distance();

        if(distr<15||distl<15)
        {
            if(distr<8||distl<8)
            {
                if((distr<4)||(distl<4))
                {
                    my_bot.move_back(2);//move the robot back: speed 2
                    move=1;
                }
                else
                {
                    if(distr<distl)
                    {
                        my_bot.turn_left(4);//turn the robot left: speed 4
                        move=2;
                    }
                    else
                    {
                        my_bot.turn_right(4);//turn the robot right: speed 4
                        move=3;
                    }
                }
            }
            else
            {
                if(distr <distl)
                {
                    my_bot.turn_left(2);//turn the robot left: speed 2
                    move=4;
                }
                else
                {
                    my_bot.turn_right(2);//turn the robot right: speed 2
                    move=5;
                }
            }
        }
        else
        {
            my_bot.move_forward(4);//move the robot forward:speed 4
            move=6;
        }
        delay(100);
        if(aux==move)//if last move is equal to current
        {
            buffer[i]=move;//save move value into the movemment buffer
            buffer[i+1]=cycle;//save cycle value into the movemment buffer
            cycle++;
            equal=true; 
        }
        else
        {
            if(buffer_size==buffer_limit)//if we have reached the buffer limit
            {
                buffer = (int*) realloc (buffer, buffer_limit +25 * sizeof(int));//make the buffer bigger
                buffer_limit=buffer_limit +25;//new buffer limit 
            }
            buffer[i]=aux;//save the last value into the movemment buffer
            buffer[i+1]=cycle;//save cycle value into the movemment buffer
            cycle=0;
            i=i+2;//move two positions into the buffer
            buffer_size=buffer_size+2;
            equal=false;
        }
        aux=move;

    }
    else if (tmpChar=='0')//stop labyrinth
    {
        if(end_pos==false)//if master robot isn't in the final position
        {
            my_bot.move_forward(4);//move the robot forward: speed 4
            delay(3000);
            end_pos=true;//the robot is in the final position
        }
        my_bot.stop_robot();//stop the robot

        delay(500);
        if(equal) //in the particular case that the last position it equal than the previous one
        {
            buffer_size=buffer_size+2;
        }

        if (paired==false)//if the master robot isn't paired qith any robot yet
        {
            for (int n=2; n<5; n++)//pair with the 4 robots
            {
                paired=my_bot.bt.pair(n);//pair
                delay(2000);
                send_buffer(n);//send data
                my_bot.bt.bt_reset();//reset the module
            }
            free (buffer);
        }
    }
}

void send_buffer(int n)//send the buffer
{
    d='6';
    my_bot.bt.send_data(d);//set the robot into the begin position
    switch(n)
    {
        case 2: delay(1000); break;
        case 3: delay(1700); break;
        case 4: delay(2000); break;
        case 5: delay(2500); break;
    }
    d='0';
    my_bot.bt.send_data(d);
    delay(1000);
    for(int posm=0; posm<buffer_size-1; posm=posm+2)//pos movemment
    {
        switch (buffer[posm])
        {
            case 1: d='1'; break;
            case 2: d='2'; break;
            case 3: d='3'; break;
            case 4: d='4'; break;
            case 5: d='5'; break;
            case 6: d='6'; break;
            default: d='6'; break;
        }
        my_bot.bt.send_data(d);
        for(int vcycle=0; vcycle < buffer[posm+1] + 1; vcycle ++)
        {
            delay(100);
        }//wait until every cycle is done
    }
    d='6';
    my_bot.bt.send_data(d);//set the robot into the final position
    switch(n)
    {
        case 2: delay(2000); break;
        case 3: delay(1500); break;
        case 4: delay(1000); break;
         case 5: delay(500); break;
    }
    d='0';
    my_bot.bt.send_data(d);
}



