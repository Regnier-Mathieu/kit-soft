#include "../inc/implementer.h"

/* Initialize everything */
void init() {
	/* RP Initialization */
	if (rp_Init() != RP_OK) {
		perror("Red Pitaya API init failed!");
		exit(EXIT_FAILURE);
	}

	init_control();

	stop = 0;
}

/* End everything */
void end(){
	stop = 1;

	end_control();
	rp_Release();
}

/* Main routine */
void routine(float* buffer, char* pixel_buffer){
	int i = 0;
//	int j = 0;
/*	for(j = 0; j < BUFFER_SIZE; j++)
		buffer[j] = 0.33;*/

//	FILE* file = fopen("capture.txt", "w");

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
		/* Waiting for the firing command */
/*		pulse(PULSE_PIN);
		usleep(66);
		ramp(RAMP_PIN);
		usleep(100);*/
		buffer = acquireADC(BUFFER_SIZE, buffer);
/*		for(j = 0; j < BUFFER_SIZE; j++)
			fprintf(file, "%f\n", buffer[j]);*/
		pixel_buffer = calcul_pixel(buffer, i, pixel_buffer);
//		fprintf(file, "%s\n", pixel_buffer);
//		fflush(file);
		i++;
	}

//	fclose(file);
}
