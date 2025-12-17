#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* Define the message queue */
/* Format: K_MSGQ_DEFINE(name, type_size, max_msgs, align) */
/* We hold 10 integers as per the project requirements */
K_MSGQ_DEFINE(my_msgq, sizeof(int), 10, 4);

/* Stack size and Priority */
#define STACKSIZE 1024
#define PRODUCER_PRIORITY 7
#define CONSUMER_PRIORITY 7

/* Producer Thread Function */
void producer(void)
{
    int data;
    for (int i = 0; i < 15; i++) { // Loop 0 to 14
        data = i;
        printk("Producer: sending %d\n", data);

        /* Send data to queue. K_NO_WAIT means don't block if full (or use K_FOREVER to block) */
        while (k_msgq_put(&my_msgq, &data, K_NO_WAIT) != 0) {
            /* If full, purge old data to make room (optional strategy) or wait */
            k_msgq_purge(&my_msgq);
        }
        
        /* Sleep to simulate work and let consumer run */
        k_msleep(100);
    }
    printk("Producer: finished sending messages\n");
}

/* Consumer Thread Function */
void consumer(void)
{
    int data;
    int count = 0;
    
    while (count < 15) {
        /* Get data from queue. K_FOREVER blocks until data is available */
        if (k_msgq_get(&my_msgq, &data, K_FOREVER) == 0) {
            printk("Consumer: received %d\n", data);
            count++;
        }
    }
    printk("Consumer: finished receiving messages\n");
}

/* Define Threads */
K_THREAD_DEFINE(producer_id, STACKSIZE, producer, NULL, NULL, NULL,
        PRODUCER_PRIORITY, 0, 0);

K_THREAD_DEFINE(consumer_id, STACKSIZE, consumer, NULL, NULL, NULL,
        CONSUMER_PRIORITY, 0, 0);
