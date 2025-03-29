CC = gcc
CFLAGS = -pthread -lrt
PRODUCER = producer
CONSUMER = consumer

all: $(PRODUCER) $(CONSUMER)

$(PRODUCER): producer.c
	$(CC) $(CFLAGS) -o $(PRODUCER) producer.c

$(CONSUMER): consumer.c
	$(CC) $(CFLAGS) -o $(CONSUMER) consumer.c

run:
	@./$(PRODUCER) &
	@./$(CONSUMER) &
	@echo "Producer and Consumer started."


# This part was generated by chatGPT
# just because I didn't want to kill it with PID everytime manually
stop:
	@ps -ef | grep './producer' | grep -v grep | awk '{print $$2}' | xargs -r kill -9 && echo "Producer killed" || echo "Producer not found"
	@ps -ef | grep './consumer' | grep -v grep | awk '{print $$2}' | xargs -r kill -9 && echo "Consumer killed" || echo "Consumer not found"

clean:
	@rm -f $(PRODUCER) $(CONSUMER)
	@echo "Cleaned binaries."

purge:
	rm -f producer consumer
	rm -f /dev/shm/sem.empty /dev/shm/sem.full /dev/shm/sem.mutex
	rm -f /dev/shm/shared_mem
	@echo "Cleaned shared memory and semaphores."
