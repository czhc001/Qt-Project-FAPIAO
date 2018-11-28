#include "invoice.h"
#include <iostream>

Invoice::Invoice()
{
    running = true;
}

void Invoice::run(){
    int a = 0;
    int b = 1;
    long c = 0;
    while(true){
        while(running){
            for(int i = 0; i < 90000000; ++i);
            if(running){
                a = (a+1)%1000000;
                b = (b+3)%1000000;
                ++c;
                //std::cout << a << "___" << b << std::endl;
                if(a % 30 > 15)
                    message(true, a, b, c);
                else
                    message(false, a, b, c);
            }
        }
    }
}


void Invoice::message(bool, int, int, long){

}
