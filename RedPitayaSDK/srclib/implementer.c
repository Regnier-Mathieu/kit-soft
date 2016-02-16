#include "../inc/implementer.h"

/* Initialize everything */
void init() {
	/* RP Initialization */
	if (rp_Init() != RP_OK) {
		perror("Red Pitaya API init failed!");
		exit(EXIT_FAILURE);
	}

#if(!DECIMATE8)
	if((data_to_send = malloc(sizeof(float)+(1+BUFFER_SIZE/PIXEL_SIZE)*sizeof(char))) == NULL)
		exit(-1);
#elif(DECIMATE8)
	if((data_to_send = malloc(sizeof(float)+(1+BUFFER_SIZE)*sizeof(char))) == NULL)
		exit(-1);
#endif

	init_control();
	init_tcp();

	stop = 0;
}

/* End everything */
void end(){
	stop = 1;

	end_control();
	end_tcp();
	free(data_to_send);
	rp_Release();
}

/* Main routine */
void routine(float* buffer, char* pixel_buffer){
	int i = 0, j = 0;
//	struct timeval init_time, end_time;
	for(j = 0; j < BUFFER_SIZE; j++)
		buffer[j] = 2.34;

	/***
	 * For each shot:
	 * Pulse
	 * Wait 66us
	 * Send a ramp
	 * Acquire the data
	 * Calculate the Pixel
	 * Send it to the TCP Server
	***/
	while(i < NB_TIRS) {
		//gettimeofday(&init_time,NULL);

		/* Waiting for the firing command */
		//while(!FIRE_CONTROL_PIN);
		pulse(PULSE_PIN);
		usleep(66);
		ramp(RAMP_PIN);
		usleep(100);
		fprintf(stdout, "OK\n");
		//buffer = acquireADC(BUFFER_SIZE, buffer);
		pixel_buffer = calcul_pixel(buffer, i, pixel_buffer);
		pthread_mutex_lock(&mutex);
		fprintf(stdout, "%s", pixel_buffer);
		sprintf(data_to_send, "%s", pixel_buffer);
		pthread_cond_signal(&new_data);
		pthread_mutex_unlock(&mutex);
		i++;
		//gettimeofday(&end_time,NULL);
		//printf("%ld\n", (int)end_time.tv_usec-init_time.tv_usec);
	}
}
