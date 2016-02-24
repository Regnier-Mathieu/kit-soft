#include "../inc/tcp.h"

/* Initialize everything (The RP and all the configurations) */
void init_tcp(){
	pthread_cond_init(&new_data,NULL);
	pthread_mutex_init(&mutex,NULL);

	if((data_to_send = malloc(PIXEL_BUFFER_SIZE * sizeof(char))) == NULL)
		exit(-1);

	/* Launch the UDP protocol */
	pthread_create(&tcp_server_thread, NULL, tcp_server, NULL);
}

/* End everything (Stop Acquisition, motor and RP resources) */
void end_tcp() {
	pthread_cond_destroy(&new_data);
    	pthread_mutex_destroy(&mutex);
	free(data_to_send);
}

/* Initialize the Server */
int init_connection(void) {
	/* TCP Socket */
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sin = { 0 };

	if(sock == INVALID_SOCKET) {
		perror("socket()");
		exit(errno);
	}

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(PORT);

	if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR) {
		perror("bind()");
		exit(errno);
	}

	listen(sock, MAX_CLIENTS);

	return sock;
}

/* End the Server */
void end_connection(int sock) {
	close(sock);
}

/* Control the TCP Server */
void *tcp_server (void *p_data) {
	/* Render this Thread autonomous */
	pthread_detach(pthread_self());
	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	socklen_t client_length;
	int sendingFlag = 0;
	pid_t pid;

	SOCKET sock = init_connection();
	client_length = sizeof(client_addr);

	while(!stop) {
		client_sock = accept(sock, (SOCKADDR *)&client_addr, &client_length);
		if(client_sock == SOCKET_ERROR) {
			perror("accept()");
			exit(errno);
		}

		pid=fork();
		if(!pid) {
			while(!stop){
				pthread_mutex_lock(&mutex);
				pthread_cond_wait(&new_data, &mutex);
				sendingFlag = send_data(data_to_send, client_sock);
				pthread_mutex_unlock(&mutex);
				if(sendingFlag < 0)
					break;
			}
			close(client_sock);
			exit(0);
		}
		close(client_sock);
	}
	end_connection(sock);

	pthread_exit(NULL);
}

/* Send the datas through the TCP Server */
int send_data(char* data_to_send, SOCKET client_sock) {
	int n;
	n = write(client_sock, data_to_send, strlen(data_to_send));
	if (n < 0)
		return(-1);
	return(0);
}
