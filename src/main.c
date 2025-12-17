#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* --- UPGRADE 1: Complex Data Structure --- */
/* Instead of sending a simple 'int', we send a full data packet.
 * This simulates a real IoT sensor reading.
 */
struct sensor_packet {
    uint32_t id;         // Unique ID for the reading
    int temperature;     // Simulated temperature value
    int64_t timestamp;   // Time when the data was created
};

/* Define message queue to hold 10 'sensor_packet' items */
/* Note: The size is now 'sizeof(struct sensor_packet)' */
K_MSGQ_DEFINE(my_msgq, sizeof(struct sensor_packet), 10, 4);

#define STACKSIZE 1024
#define PRODUCER_PRIORITY 7
#define CONSUMER_PRIORITY 7

void producer(void)
{
    struct sensor_packet data;
    
    for (int i = 0; i < 20; i++) {
        /* Simulate gathering data */
        data.id = i;
        
        /* Generate fake temperature variation (20C to 30C) */
        data.temperature = 20 + (i % 10); 
        
        /* Capture system uptime (RTOS Timer) */
        data.timestamp = k_uptime_get();

        printk("[SENSOR] Generated: ID=%d | Temp=%dC | Time=%lld ms\n", 
               data.id, data.temperature, data.timestamp);

        /* Send packet to queue */
        /* UPGRADE 2: Error Handling */
        /* If queue is full, we purge old data to make room for new (Real-time behavior) */
        while (k_msgq_put(&my_msgq, &data, K_NO_WAIT) != 0) {
            k_msgq_purge(&my_msgq);
        }
        
        /* Sleep to simulate sensor sampling rate (e.g., read every 100ms) */
        k_msleep(100);
    }
    printk("[SENSOR] Sampling complete.\n");
}

void consumer(void)
{
    struct sensor_packet data;
    int64_t current_time;
    int64_t latency;
    
    /* Run indefinitely or until a limit */
    for (int i = 0; i < 20; i++) {
        /* Block forever until data arrives */
        if (k_msgq_get(&my_msgq, &data, K_FOREVER) == 0) {
            
            /* --- UPGRADE 3: Latency Calculation --- */
            /* We calculate how long the data sat in the queue.
             * This proves we understand RTOS timing.
             */
            current_time = k_uptime_get();
            latency = current_time - data.timestamp;

            printk("    [CLOUD] Uploaded: ID=%d | Temp=%dC | Latency=%lld ms\n", 
                   data.id, data.temperature, latency);
        }
    }
    printk("    [CLOUD] Upload complete.\n");
}

/* Define Threads */
K_THREAD_DEFINE(producer_id, STACKSIZE, producer, NULL, NULL, NULL,
        PRODUCER_PRIORITY, 0, 0);

K_THREAD_DEFINE(consumer_id, STACKSIZE, consumer, NULL, NULL, NULL,
        CONSUMER_PRIORITY, 0, 0);
