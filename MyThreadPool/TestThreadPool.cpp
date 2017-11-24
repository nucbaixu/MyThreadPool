// TestThreadPool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Thradpool.h"

class MyTask1: public BX::Task  
{  
public:  
    MyTask1(){}
      
    virtual int run()  
    {  
		//auto thid = this_thread::get_id();
		//std::cout<<"thread" <<thid <<":" <<(char*)this->arg_<<endl;
		printf("thread[%lu] : %s\n", 1, (char*)this->arg_);  
        
        return 0;  
    }  
};  

int _tmain(int argc, _TCHAR* argv[])
{
	char szTmp[] = "hello world";
    
    MyTask1 taskObj;
    taskObj.setArg((void*)szTmp);

    BX::ThreadPool threadPool(1);  
    for(int i = 0; i < 10; i++)  
    {
        threadPool.addTask(&taskObj);  
    }

    while(1)  
    {  
        printf("there are still %d tasks need to process\n", threadPool.Size());  
        if (threadPool.Size() == 0)
        {  
			printf("there are still %d tasks need to process\n", threadPool.Size());  
            threadPool.stop();
            printf("Now I will exit from main\n"); 
			break;  
        }  

		std::this_thread::sleep_for(std::chrono::seconds(1));
       
    }  

	return 0;
}

