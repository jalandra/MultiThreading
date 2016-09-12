#include<iostream>

extern "C" 
{
    #include <pthread.h>
}
 
using namespace std;

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;

class OddEven{
    private:
        int count;
    public:
        OddEven(int c):count(c){
        }
        void* printEven(void *a){
            pthread_mutex_lock( &count_mutex );
            if(count>1000){
                pthread_exit(NULL);
                return NULL;
            }
            if(count%2 !=0){
                pthread_cond_wait(&condition_var, &count_mutex);   
            } else{
                cout<<count<<" ";
                count = count+1;
                pthread_cond_signal(&condition_var);
            }
            pthread_mutex_unlock( &count_mutex );
            printEven(NULL);
        }
        
        void* printOdd(void *a){
            pthread_mutex_lock(&count_mutex);
            if(count>1000){
                pthread_exit(NULL);
                return NULL;
            }
            if(count%2 ==0){
                pthread_cond_wait(&condition_var, &count_mutex);   
            } else{
                cout<<count<<" ";
                count = count+1;
                pthread_cond_signal(&condition_var);
            }
            pthread_mutex_unlock(&count_mutex);
            printOdd(NULL);
        }
        ~OddEven(){
        }       
};

struct wrap {
    OddEven *obj;
    wrap(OddEven &f){
        obj = &f;
    }
};

extern "C" void* call_func_even( void *f )
{
    wrap *w = static_cast< wrap* >( f );
    w->obj->printEven(NULL);
    return 0;
}

extern "C" void* call_func_odd( void *f )
{
    wrap *w= static_cast< wrap* >( f ) ;
    w->obj->printOdd(NULL);
    return 0;
}


int main(){
    OddEven obj(0);
    wrap* w = new wrap(obj);
    pthread_t evenThread, oddThread;
    pthread_create(&evenThread,NULL,call_func_even, (void *)w);
    pthread_create(&oddThread,NULL,call_func_odd, (void *)w);
    pthread_join(evenThread,NULL);
    pthread_join(oddThread,NULL);
    pthread_exit(NULL);
    return 0;
}
