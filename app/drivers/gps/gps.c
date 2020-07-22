#include "gps.h"

static char gps_buffer[MINMEA_MAX_LENGTH];
static char *gps_buffer_ptr;

static float latitude_parsed, longitude_parsed;
static int32_t altitude_parsed, tracked_satellites_parsed;

static void gps_minmea_parser(char *line);
static void gps_stop_dma(void);
static void gps_start_dma(void);
static void gps_parse_rmc(char *line);
static void gps_parse_gga(char *line);
static int gps_parse_buffer_to_search_string(char *string);

#ifdef GPS_DEBUG
static void gps_print_buffer (void);
static void gps_minmea_print_rmc(struct minmea_sentence_rmc *frame);
static void gps_minmea_print_gga(struct minmea_sentence_gga *frame);
#endif

static osMutexDef (mutex);
static osMutexId  (mutex_id);

void gps_init(void)
{
	HAL_StatusTypeDef ret;
	__HAL_UART_CLEAR_FEFLAG(&GPS_USART);
	ret = HAL_UART_Receive_DMA(&GPS_USART, (uint8_t *)&gps_buffer, sizeof(gps_buffer));
	if (ret != HAL_OK) {
		error_handler();
	}
	 mutex_id = osMutexCreate(osMutex(mutex));
}

void gps_get_parsed_data(float *latitude, float *longitude, int32_t *altitude, int32_t *tracked_satellites)
{
	osMutexWait(mutex_id, osWaitForever);
	*latitude = latitude_parsed;
	*longitude = longitude_parsed;
	*altitude = altitude_parsed;
	*tracked_satellites = tracked_satellites_parsed;
	osMutexRelease(mutex_id);
}

void gps_minmea_process(void) 
{
	gps_minmea_parser(gps_buffer_ptr);
	gps_start_dma();
}

void gps_callback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == GPS_USART_INSTANCE) {
		gps_stop_dma();
		if (gps_parse_buffer_to_search_string("$GPRMC")) {
			osSignalSet(gpsTaskHandle, GPS_TASK_SIGNAL);
			return;
		} 
		if (gps_parse_buffer_to_search_string("$GPGGA")) {
			osSignalSet(gpsTaskHandle, GPS_TASK_SIGNAL);
			return;
		} else {
			gps_start_dma();
		}
	}
}

void gps_callback_error(UART_HandleTypeDef *huart)
{
    printf("UART_HandleTypeDef->ErrorCode %ld \n\r", huart->ErrorCode);
}

static void gps_minmea_parser(char *line)
{
	switch (minmea_sentence_id(line, false)) {
	case MINMEA_SENTENCE_RMC:
		gps_parse_rmc(line);
		break;
	case MINMEA_SENTENCE_GGA:
		gps_parse_gga(line);
		break;
	case MINMEA_INVALID:
	case MINMEA_UNKNOWN:
	case MINMEA_SENTENCE_GSA:
	case MINMEA_SENTENCE_GLL:
	case MINMEA_SENTENCE_GST:
	case MINMEA_SENTENCE_GSV:
	default:
#ifdef GPS_DEBUG
		printf("MINMEA_INVALID buffer\n\r");
#endif
		break;
    }
}

static void gps_parse_gga(char *line)
{
	struct minmea_sentence_gga frame;
	if (minmea_parse_gga(&frame, line)) {
		osMutexWait(mutex_id, osWaitForever);
		altitude_parsed = frame.altitude.value;
		tracked_satellites_parsed = frame.satellites_tracked;
		osMutexRelease(mutex_id);
#ifdef GPS_DEBUG
		gps_minmea_print_gga(&frame);
	} else {
		printf("$xxGGA sentence is not parsed\n\r");
#endif
	}
}

static void gps_parse_rmc(char *line)
{
	struct minmea_sentence_rmc frame;
	if (minmea_parse_rmc(&frame, line)) {
		osMutexWait(mutex_id, osWaitForever);
		latitude_parsed = minmea_tocoord(&frame.latitude);
		longitude_parsed  = minmea_tocoord(&frame.longitude);
		osMutexRelease(mutex_id);
#ifdef GPS_DEBUG
		gps_minmea_print_rmc(&frame);
	} else {
			printf("$xxRMC sentence is not parsed\n\r");
#endif
	}
}

static int gps_parse_buffer_to_search_string(char *string)
{
	gps_buffer_ptr = strstr(gps_buffer, string);
	if (gps_buffer_ptr != NULL) {
#ifdef GPS_DEBUG
		gps_print_buffer();
#endif
		return 0;
	} else {
		return 1;
	}
}

static void gps_stop_dma(void)
{
	HAL_NVIC_DisableIRQ(GPS_DMA_IRQn);
	HAL_NVIC_DisableIRQ(GPS_USART_IRQn);
}

static void gps_start_dma(void)
{
	HAL_StatusTypeDef ret;
	HAL_NVIC_EnableIRQ(GPS_DMA_IRQn);
	HAL_NVIC_EnableIRQ(GPS_USART_IRQn);
	ret = HAL_UART_Receive_DMA(&GPS_USART, (uint8_t *)&gps_buffer, sizeof(gps_buffer));
	if (ret != HAL_OK) {
		error_handler();
	}
}

#ifdef GPS_DEBUG
static void gps_print_buffer (void)
{
	printf("Buffer \n\r");
	for (int i = 0; i < MINMEA_MAX_LENGTH; i++) {
		printf("%c", gps_buffer_ptr[i]);
	}
	printf("\n\n\r");
}

static void gps_minmea_print_rmc(struct minmea_sentence_rmc *frame) 
{
	printf("Fixed-point Latitude...........: %ld\n\r", minmea_rescale(&frame->latitude, 1000));
	printf("Fixed-point Longitude..........: %ld\n\r", minmea_rescale(&frame->longitude, 1000));
	printf("Fixed-point Speed..............: %ld\n\r", minmea_rescale(&frame->speed, 1000)); 
	printf("Floating point degree latitude.: %2.6f\n\r", minmea_tocoord(&frame->latitude));
	printf("Floating point degree longitude: %2.6f\n\r", minmea_tocoord(&frame->longitude));
	printf("Floating point speed...........: %2.6f\r", minmea_tocoord(&frame->speed));
}

static void gps_minmea_print_gga(struct minmea_sentence_gga *frame)
{
	printf("Fix quality....................: %d\n\r", frame->fix_quality);
	printf("Altitude.......................: %ld\n\r", frame->altitude.value);
	printf("Tracked satellites.............: %d\n\r", frame->satellites_tracked);
}
#endif