#include <pthread.h>
#include <stdio.h>

int count= 0;
pthread_mutex_t mutex ;
void *print_msg(void *arg){
        int i=0;
        for(i=0;i<15;i++){
		count++;
        pthread_mutex_lock(&mutex);
                printf("output : %d index : %d\n",i, count);
        pthread_mutex_unlock(&mutex);
        usleep(1000);
        }
}
int main(int argc,char** argv){
        pthread_t id1;
        pthread_t id2;
        pthread_mutex_init(&mutex,NULL);
        pthread_create(&id1,NULL,print_msg,NULL);
        pthread_create(&id2,NULL,print_msg,NULL);
        pthread_join(id1,NULL);
        pthread_join(id2,NULL);
        pthread_mutex_destroy(&mutex);
        return 1;
}

