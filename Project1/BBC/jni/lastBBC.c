#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h> 

sem_t rack_mutex;
sem_t burgerready;
sem_t order;
sem_t cashier_ready;
sem_t cust_coming;
sem_t restcust_mutex;

void* cooks(void*  a);
void* cashiers(void* a);
void* customers(void* a);
int rack_num = 0;
int rack_max;
int rest_cust;
int main(int argu,char *argv[]){
    if (argv[1]==NULL || argv[2]==NULL || argv[3]==NULL || argv[4]==NULL)
        {
            perror("Input error!");
            //return 1;
        }
    int num_cook = atoi(argv[1]);
    int num_cashier = atoi(argv[2]);
    int num_customer = atoi(argv[3]);
    int rack_MAX = atoi(argv[4]);
    rack_max = rack_MAX;
    rest_cust = num_customer;
    int i = 0;
    int cook_array[num_cook];
    int cashier_array[num_cashier];
    int cust_array[num_customer];
    // initialize the semaphore
    if(sem_init(&rack_mutex,0,0)!=0)
        perror("Semaphore initialization failed");
    sem_post(&rack_mutex);
    if(sem_init(&burgerready,0,0)!=0)
        perror("Semaphore initialization failed");
    if(sem_init(&order,0,0)!=0)
        perror("Semaphore initialization failed");
    if(sem_init(&restcust_mutex,0,0)!=0)
        perror("Semaphore restcust_mutex initialization failed");
    sem_post(&restcust_mutex);
    if(sem_init(&cashier_ready,0,num_cashier)!=0)
        perror("Semaphore initialization failed");
    if(sem_init(&cust_coming,0,num_customer)!=0)
        perror("Semaphore initialization failed");
    // create spaces for threads
    pthread_t* threadCook = (pthread_t*)   malloc(num_cook*sizeof(pthread_t));
    pthread_t* threadCashier =(pthread_t*)   malloc(num_cashier*sizeof(pthread_t));
    pthread_t* threadCustomer = (pthread_t*)   malloc(num_customer*sizeof(pthread_t));
    printf("Cooks [%d], Cashiers [%d], Customers [%d]\n",num_cook,num_cashier,num_customer);
    printf("Begin run.\n");
    // be careful if "&i" will cause all thread IDs to be the same
    for(i = 0; i < num_cook; i++)   
    {
        cook_array[i] = i+1;
        pthread_create(&threadCook[i],NULL,(void*)cooks,&cook_array[i]);
    //  printf("cook[%d] ready!",i);
    }
    for(i = 0; i < num_cashier; i++)    
    {
        cashier_array[i] = i+1;
        pthread_create(&threadCashier[i],NULL,(void*)cashiers,&cashier_array[i]);
    }   
    for(i = 0; i < num_customer; i++)
    {
        cust_array[i] = i+1;
        pthread_create(&threadCustomer[i],NULL,(void*)customers,&cust_array[i]);
    }

    for(i = 0; i < num_cook; i++)   {pthread_join(threadCook[i],NULL);}
    for(i = 0; i < num_cashier; i++)    {pthread_join(threadCashier[i],NULL);}
    for(i = 0; i < num_customer; i++)   {pthread_join(threadCustomer[i],NULL);}

    sem_destroy(&rack_mutex);
    sem_destroy(&burgerready);
    sem_destroy(&order);
    sem_destroy(&cashier_ready);
    sem_destroy(&cust_coming);
    sem_destroy(&restcust_mutex);
    
    free(threadCustomer);
    free(threadCashier);
    free(threadCook);
    // in fact may not get into use
    return 0;
}

void* cooks(void*  a){
    int ID = *(int *) a;
    while(1){
        if(rest_cust <= 0) 
        {
            sem_post(&rack_mutex); // avoid stunking
            break;
        }
        sem_wait(&rack_mutex);
        
        if(rest_cust <= 0)   // for exit
        {
            sem_post(&rack_mutex);
            break;
        }

        if (rack_num <= rack_max)
        {
            sleep(1);
            rack_num++;
            printf("Cook [%d] make a burger.\n",ID);
            sem_post(&rack_mutex);
        }
        else 
        {
            sem_post(&rack_mutex);
        }
    }
    pthread_exit(0);
    //return  (void*) 0;
}

void* cashiers(void* a){
    int ID = *(int *) a;
        //int i;
    while(1){
        if(rest_cust <= 0) 
        {
            sem_post(&cust_coming);
            sem_post(&restcust_mutex);
            sem_post(&order);
            sem_post(&rack_mutex);
            break;
        }           
        sem_post(&cashier_ready);
        sem_wait(&cust_coming);
        if(rest_cust <= 0) 
        {
            sem_post(&cust_coming); // avoid stunking
            break;
        }
        sem_wait(&order);
        if(rest_cust <= 0)   // for exit
        {
            sem_post(&order);
            break;
        }
        printf("Cashier [%d] accepts an order.\n",ID);
        printf("cashier [%d] wait for burger\n",ID);
        while(1)
        {
            sem_wait(&rack_mutex);
            if(rest_cust <= 0)   // for exit
            {
                sem_post(&rack_mutex);
                break;
            }
            if (rack_num >= 1)
            {
                rack_num--;
                printf("Cashier [%d] takes a burger to a customer.\n",ID);

                sem_wait(&restcust_mutex);  // calculate the rest customers' num to quit
                if(rest_cust <= 0) 
                {
                    sem_post(&restcust_mutex); // avoid stunking
                    break;
                }
                rest_cust --;
                sem_post(&restcust_mutex);

                sem_post(&burgerready);
                sem_post(&rack_mutex);
                break;
            }
            else
            {
                sem_post(&rack_mutex);
            }
        }
    }
    pthread_exit(0);
    //return  (void*) 0;
}

void* customers(void* a){
    int ID = *(int *) a;
    while(1){
        sleep(rand()%3);
        printf("customer [%d] comes.\n",ID);
        sem_post(&cust_coming);
        sem_wait(&cashier_ready);
        //order one burger
        //sleep(1);
        sem_post(&order);
        sem_wait(&burgerready);
        break;
    }
    pthread_exit(0);
    //return (void*) 0;
}
