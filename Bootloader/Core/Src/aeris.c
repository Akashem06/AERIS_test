#include "aeris.h"
#include "aeris_prv.h"

static aeris_state_data prv_aeris = {
    .config = NULL,
    .state = AERIS_STATE_UNINITIALIZED,
    .error = AERIS_ERR_NONE
};

aeris_error aeris_bootloader_init(aeris_config const *const config) {
    if (config == NULL || config->uart_transmit == NULL || config->uart_receive == NULL) {
        return AERIS_ERR_INVALID_ARGS;
    } else {
        prv_aeris.config = config;
        prv_aeris.state = AERIS_STATE_IDLE;
        prv_aeris.error = AERIS_ERR_NONE;
        prv_aeris.message_error = AERIS_MSG_ERR_NONE;
        prv_aeris.app_failed_crc32 = 0;
        prv_aeris.message_failed_crc16 = 0;
        memset(prv_aeris.message_buffer, 0, sizeof(prv_aeris.message_buffer));
        memset(prv_aeris.ack_message_buffer, 0, sizeof(prv_aeris.ack_message_buffer));
    }
    return AERIS_ERR_NONE;
}

aeris_error aeris_bootloader_run(void) {
    aeris_error ret = AERIS_ERR_NONE;

    do {
    // get desired state change

    // make the state change
    ret = aeris_bootloader_change_state(prv_aeris.state, prv_aeris.state_request);

    if (ret != AERIS_ERR_NONE) {
        break;
    }

    // run the new state
    aeris_bootloader_run_state(prv_aeris.state);
    } while (0);
}

aeris_error aeris_bootloader_change_state(aeris_state current_state, aeris_state_request desired_state) {
    aeris_error ret = AERIS_ERR_NONE;

    switch (current_state) {
        case AERIS_STATE_IDLE:
            switch (desired_state) {
                case AERIS_REQUEST_JUMP_APP:
                    prv_aeris.state = AERIS_STATE_JUMP_APP;
                    prv_aeris.valid_start_received = 0;
                    break;
                case AERIS_REQUEST_DFU:
                    prv_aeris.state = AERIS_STATE_DFU;
                    prv_aeris.valid_start_received = 1;
                    break;
                default:
                    ret = AERIS_ERR_INVALID_ARGS;
            }
            break;

        case AERIS_STATE_DFU:
            switch (desired_state) {
                case AERIS_REQUEST_JUMP_APP:
                    prv_aeris.state = AERIS_STATE_JUMP_APP;
                    break;
                case AERIS_REQUEST_IDLE:
                    prv_aeris.state = AERIS_STATE_IDLE;
                    break;
                default:
                    ret = AERIS_ERR_INVALID_ARGS;
            }
            break;

        case AERIS_STATE_JUMP_APP:
            if (desired_state == AERIS_REQUEST_IDLE) {
                prv_aeris.state = AERIS_STATE_IDLE;
            }
            break;

        default:
            ret = AERIS_ERR_INVALID_ARGS;
    }

    return ret;
}

aeris_error aeris_bootloader_run_state(aeris_state state) {

}

aeris_error aeris_bootloader_run_dfu(void) {
    aeris_error ret = AERIS_ERR_NONE;
    
    if (ret == AERIS_ERR_NONE) {
        do {
            ret = aeris_bootloader_receive(prv_aeris.message_buffer, sizeof(prv_aeris.message_buffer));
            if (ret != AERIS_ERR_NONE) {
                break;
            }

            ret = aeris_bootloader_process(prv_aeris.message_buffer, sizeof(prv_aeris.message_buffer));
            if (ret != AERIS_ERR_NONE) {
                break;
            }

        } while (false);
    }
    return ret;
}

aeris_error aeris_bootloader_receive(*uint8_t buffer, size_t buffer_size) {

}

aeris_error aeris_bootloader_process(*uint8_t buffer, size_t buffer_size) {

}

// Custom CRC calculation

static uint16_t prv_aeris_package_crc16(aeris_message_t *message, int nBytes) {

} 

static uint32_t prv_aeris_package_crc32(aeris_message_t *message, int nBytes) {
    
} 