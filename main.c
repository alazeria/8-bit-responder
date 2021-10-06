#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int



//                        0    1    2 
//code uchar seven_seg[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
code uchar seven_seg[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
code uchar seven_bit[]={0xfe,0xfd,0xfb,0xf7};



sbit    run_key =   P3^2;            //????
sbit    rst_key =   P3^3;            //??¦Ë
sbit    add_key =   P3^6;            //?????
sbit    dec_key =   P3^7;            //?????
sbit    buzzer  =   P3^5;            //??????

uchar   player;                      //??????????????
uint    player_num  =   0;           //????
uchar   now_time    =   25;          //????????
uchar   next_time;
bit     run_flag    =   0;          //??????????????
bit     time_off    =   0;          //????????????????????????
uint    Count_A     =   0;          //?????????????
uint    Count_B     =   0;          //?????????????



void    delay(uint  ms)
{
    uint    i,j;
    for(i=ms;i>0;i--)
        for(j=100;j>0;j--);
}

void    key_Scan()
{
    if(run_key==0)                              //???????
    {
        delay(5);                               //????
        if(run_key==0)
        {
            run_flag    =   1;                  //??1??????
            buzzer      =   0;                  //???????
            TR0         =   1;                  //?????????
            TR1         =   0;                  
        }
        while(run_key==0);                      //???????
    }
    if(rst_key==0)                              //??¦Ë
    {
        delay(5);
        if(rst_key==0)
        {
            run_flag    =   0;                  //??0??????
            TR0         =   0;                  //???????
            TR1         =   0;
            Count_A     =   0;  
            Count_B     =   0;
            player_num  =   0;
            time_off    =   0;
            now_time    =   next_time;          //???????¦Ë
            buzzer      =   1;                  //?????????
        }
        while(rst_key==0);
    }
    if(run_flag==0&&time_off==0)                //?????????????????
    {
        if(add_key==0)                  
        {
            delay(5);
            if(add_key==0)                      //????
            {
                next_time++;
                now_time++;
            }
            while(add_key==0);
        }
        if(dec_key==0)                         
        {
            delay(5);
            if(dec_key==0)                      //????
            {
                next_time--;
                now_time--;
            }
            while(dec_key==0);
        }
    }
} 

void    display(uint a,uint b)          //a???????b????
{
    P2  =   seven_bit[0];
    P0  =   seven_seg[a];               //???????
    delay(5);
    P2  =   seven_bit[1];
    P0  =   0x40;                       //??????
    delay(5);
    P2  =   seven_bit[2];
    P0  =   seven_seg[b/10];            //???????¦Ë
    delay(5);
    P2  =   seven_bit[3];
    P0  =   seven_seg[b%10];            //???????¦Ë
    delay(5);
    P2  =   0xff;
}

void    EX_Init()
{
    TMOD    =   0x11;
    TH0     =   0xd8;
    TL0     =   0xf0;
    TH1     =   0xd8;
    TL1     =   0xf0;
    IE      =   0x8b;
    IT0     =   1;
    IT1     =   1;
    PX0     =   1;
    TR0     =   0;
    TR1     =   0;
}
    
    
void main()
{
    EX_Init();
    buzzer  =   1;
    P2      =   0xff;
    P1      =   0xff;
    P0      =   0xff;
    next_time   =   now_time;
    while(1)
    {
        key_Scan();
        display(player_num,now_time);
        
    }
}

void    Time0() interrupt   1
{
    TH0 =   0xd8;
    TL0 =   0XF0;
    //player_num    =   9;
    Count_A++;
    Count_B++;
    if(Count_B<50)
        buzzer  =   0;
    else
        buzzer  =   1;
    if(Count_A==100)
    {
        Count_A =   0;
        now_time--;
        if(now_time<=0)                         //???????????????????
        {
            now_time    =   0;
            run_flag    =   0;
            TR0         =   0;
            TR1         =   0;
            time_off    =   1;
            buzzer      =0;
        }
    
    }
    if(run_flag==1)
    {
        player  =   P1;
        switch(player)
        {
            case 0xfe:  player_num=1;run_flag=0;time_off=1;TR0 = 0;TR1=0;break;
            case 0xfd:  player_num=2;run_flag=0;time_off=1;TR0 = 0;TR1=0;break;            
            case 0xfb:  player_num=3;run_flag=0;time_off=1;TR0 = 0;TR1=0;break;
            case 0xf7:  player_num=4;run_flag=0;time_off=1;TR0 = 0;TR1=0;break;            
            case 0xef:  player_num=5;run_flag=0;time_off=1;TR0 = 0;TR1=0;break;
            case 0xdf:  player_num=6;run_flag=0;time_off=1;TR0 = 0;TR1=0;break;            
            case 0xbf:  player_num=7;run_flag=0;time_off=1;TR0 = 0;TR1=0;break;
            case 0x7f:  player_num=8;run_flag=0;time_off=1;TR0 = 0;TR1=0;break;    
            case 0xff:  break;          //??????????????
            default:    player_num=9;break;         //???????
        }
    }
}

//void    Time1() interrupt   3
//{
//    TH1 =   0xd8;
//    TL1 =   0xf0;
//    Count_A++;
//    Count_B++;
//    if(Count_B<50)
//        buzzer  =   0;
//    else
//        buzzer  =   1;
//    if(Count_A==100)
//    {
//        Count_A =   0;
//        now_time--;
//        if(now_time<=0)                         //???????????????????
//        {
//            now_time    =   0;
//            run_flag    =   0;
//            TR0         =   0;
//            TR1         =   0;
//            time_off    =   1;
//            buzzer      =0;
//        }
//    
//    }
//}
