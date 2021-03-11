/*
 * log for aoa
 * write to /tmp/aoa*
 * huangliquan@xa.com
 * 2019/12/25
*/
#include "log.h"
#include <time.h>

#define DEBUG_TX_FILE_NAME "/tmp/aoa_tx.log"
#define DEBUG_RX_FILE_NAME "/tmp/aoa_rx.log"
#define DEBUG_STATUS_NAME "/tmp/aoa_status"
#define AOA_VERSION "/tmp/aoa_version"

int XagLog::aoa_version(const char *str){

	FILE* aoa_status = fopen(AOA_VERSION, "w+");
	if(aoa_status != NULL){
		fprintf(aoa_status, str);
		fprintf(aoa_status, "compile time %s-%s\n", __DATE__, __TIME__);
		fprintf(aoa_status, "egg: huangliquan@xa.com\n");
		fclose(aoa_status);
		return 0;
	}

	return -1;
}


bool XagLog::debug_message = 0;
FILE* XagLog::debug_tx_file = NULL;
FILE* XagLog::debug_rx_file = NULL;

int XagLog::log_open(void) {
	debug_message = 1;

	debug_tx_file = fopen(DEBUG_TX_FILE_NAME, "a+");
	if(debug_tx_file == NULL){
		logError("cat not open debug tx file\n");
		return 1;
	}
	printf("aoa tx debug mode\n");

	debug_rx_file = fopen(DEBUG_RX_FILE_NAME, "w+");
	if(debug_rx_file == NULL){
		logError("cat not open debug rx file\n");
		return 2;
	}
	printf("aoa rx debug mode\n");
	return 0;
}

int XagLog::log_close(void) {
	debug_message = 0;
	if(debug_rx_file != NULL){
		fclose(debug_rx_file);
		remove(DEBUG_RX_FILE_NAME);	
	}

	if(debug_tx_file != NULL){	
		fclose(debug_tx_file);
		remove(DEBUG_TX_FILE_NAME);
	}

	remove(DEBUG_STATUS_NAME);
	return 0;
}

int XagLog::aoa_status(const char *str) {
	FILE* aoa_status = fopen(DEBUG_STATUS_NAME, "w+");
	if(aoa_status != NULL){
		fprintf(aoa_status, str);
		fclose(aoa_status);
	}
	return 0;
}

int XagLog::log_tx_write(uint8_t* buffer, int rxBytes) {
	if(debug_message){
		if(debug_tx_file != NULL){
			if(ftell(debug_tx_file) >= 512*1024){
				fseek(debug_tx_file,0L,SEEK_SET); /* 定位到文件开头 */
			}
			fprintf(debug_tx_file, "aoa received %d bytes\n", rxBytes);
			for(int i = 0; i < rxBytes ; i++){
				fprintf(debug_tx_file, "%x ",buffer[i]);
			}
			fprintf(debug_tx_file, "\n");
		}
	}
	return 0;
}

int XagLog::log_rx_write(uint8_t* buffer, int txBytes) {
	if(debug_message){
		if(debug_rx_file != NULL){
			if(ftell(debug_rx_file) >= 512*1024){
				fseek(debug_rx_file,0L,SEEK_SET); /* 定位到文件开头 */
			}
			fprintf(debug_rx_file, "udp received %d bytes\n", txBytes);
			for(int i = 0; i < txBytes; i++){
				fprintf(debug_rx_file, "%x ",buffer[i]);
			}
			fprintf(debug_rx_file, "\n");
		}
	}
	return 0;
}
