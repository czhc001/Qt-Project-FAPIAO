#include "invoice.h"
#include <iostream>

Invoice::Invoice()
{
    running = true;
}

void Invoice::run(){
    int a = 0;
    int b = 1;
    while(true){
        while(running){
            for(int i = 0; i < 600000000; ++i);
            if(running){
                a = (a)%185;
                b = (b+1)%2000;
                std::cout << a << "___" << b << std::endl;
                message(a, b);
            }
        }
    }
}


void Invoice::message(int, int){

}
