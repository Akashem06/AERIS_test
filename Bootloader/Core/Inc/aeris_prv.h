#ifndef AERIS_PRIVATE_H
#define AERIS_PRIVATE_H

#include <stdbool.h>
#include "aeris.h"

// Packet type
typedef enum {
  AERIS_PACKET_ID_UNKNOWN,    
  AERIS_PACKET_TYPE_START,   // Start
  AERIS_PACKET_TYPE_DATA,    // Data
  AERIS_PACKET_TYPE_ACK      // Ack
} aeris_message_id;

typedef struct {
  uint32_t app_size; // 3 bytes not 4
  uint32_t app_crc;
} aeris_meta_info_t;

typedef struct {
  uint8_t   packet_sof;
  uint8_t   packet_id;
  union {
    struct {
      aeris_meta_info_t meta_info;
    } start_packet;
    struct {
      uint8_t *app_data;
      uint8_t app_size;
    } data_packet;
    struct {
      uint8_t ack_status;
      uint32_t error_data;
    } error_packet;
  } packet_payload;
  uint32_t  packet_crc;
  uint8_t   packet_eof;
} aeris_message_t;

typedef struct {
  aeris_config *config;
  aeris_state state;
  aeris_state_request state_request;
  aeris_error error;
  aeris_message_error message_error;

  bool app_failed_crc32;
  bool message_failed_crc16;
  bool valid_start_received;

  uint8_t message_buffer[AERIS_MAX_APP_SIZE];
  uint8_t ack_message_buffer[AERIS_ACK_MESSAGE_SIZE];
} aeris_state_data;

#endif