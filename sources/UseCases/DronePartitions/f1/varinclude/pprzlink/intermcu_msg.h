/** @file
 *	@brief PPRZLink messages header built from message_definitions/v1.0/messages.xml
 *	@see http://paparazziuav.org
 */
#ifndef _VAR_MESSAGES_intermcu_H_
#define _VAR_MESSAGES_intermcu_H_

#define PPRZLINK_PROTOCOL_VERSION "1.0"
#define PPRZLINK_PROTOCOL_VERSION_MAJOR 1
#define PPRZLINK_PROTOCOL_VERSION_MINOR 0

#ifdef __cplusplus
extern "C" {
#endif

#include "pprzlink/pprzlink_device.h"
#include "pprzlink/pprzlink_transport.h"
#include "pprzlink/pprzlink_utils.h"

#if DOWNLINK
#define DL_IMCU_COMMANDS 1
#define PPRZ_MSG_ID_IMCU_COMMANDS 1
#define DL_IMCU_RADIO_COMMANDS 2
#define PPRZ_MSG_ID_IMCU_RADIO_COMMANDS 2
#define DL_IMCU_SPEKTRUM_SOFT_BIND 3
#define PPRZ_MSG_ID_IMCU_SPEKTRUM_SOFT_BIND 3
#define DL_MSG_intermcu_NB 3


#define DOWNLINK_SEND_IMCU_COMMANDS(_trans, _dev, status, nb_values, values) pprz_msg_send_IMCU_COMMANDS(&((_trans).trans_tx), &((_dev).device), AC_ID, status, nb_values, values)
static inline void pprz_msg_send_IMCU_COMMANDS(struct transport_tx *trans, struct link_device *dev, uint8_t ac_id, uint8_t *_status, uint8_t nb_values, int16_t *_values) {
  if (trans->check_available_space(trans->impl, dev, trans->size_of(trans->impl, 0+1+1+nb_values*2 +2 /* msg header overhead */))) {
    trans->count_bytes(trans->impl, dev, trans->size_of(trans->impl, 0+1+1+nb_values*2 +2 /* msg header overhead */));
    trans->start_message(trans->impl, dev, 0+1+1+nb_values*2 +2 /* msg header overhead */);
    trans->put_bytes(trans->impl, dev, DL_TYPE_UINT8, DL_FORMAT_SCALAR, 1, &ac_id);
    trans->put_named_byte(trans->impl, dev, DL_TYPE_UINT8, DL_FORMAT_SCALAR, DL_IMCU_COMMANDS, "IMCU_COMMANDS");
    trans->put_bytes(trans->impl, dev, DL_TYPE_UINT8, DL_FORMAT_SCALAR, 1, (void *) _status);
    trans->put_bytes(trans->impl, dev, DL_TYPE_ARRAY_LENGTH, DL_FORMAT_SCALAR, 1, (void *) &nb_values);
    trans->put_bytes(trans->impl, dev, DL_TYPE_INT16, DL_FORMAT_ARRAY, nb_values*2, (void *) _values);
    trans->end_message(trans->impl, dev);
  } else
    trans->overrun(trans->impl, dev);
}

#define DOWNLINK_SEND_IMCU_RADIO_COMMANDS(_trans, _dev, status, nb_values, values) pprz_msg_send_IMCU_RADIO_COMMANDS(&((_trans).trans_tx), &((_dev).device), AC_ID, status, nb_values, values)
static inline void pprz_msg_send_IMCU_RADIO_COMMANDS(struct transport_tx *trans, struct link_device *dev, uint8_t ac_id, uint8_t *_status, uint8_t nb_values, int16_t *_values) {
  if (trans->check_available_space(trans->impl, dev, trans->size_of(trans->impl, 0+1+1+nb_values*2 +2 /* msg header overhead */))) {
    trans->count_bytes(trans->impl, dev, trans->size_of(trans->impl, 0+1+1+nb_values*2 +2 /* msg header overhead */));
    trans->start_message(trans->impl, dev, 0+1+1+nb_values*2 +2 /* msg header overhead */);
    trans->put_bytes(trans->impl, dev, DL_TYPE_UINT8, DL_FORMAT_SCALAR, 1, &ac_id);
    trans->put_named_byte(trans->impl, dev, DL_TYPE_UINT8, DL_FORMAT_SCALAR, DL_IMCU_RADIO_COMMANDS, "IMCU_RADIO_COMMANDS");
    trans->put_bytes(trans->impl, dev, DL_TYPE_UINT8, DL_FORMAT_SCALAR, 1, (void *) _status);
    trans->put_bytes(trans->impl, dev, DL_TYPE_ARRAY_LENGTH, DL_FORMAT_SCALAR, 1, (void *) &nb_values);
    trans->put_bytes(trans->impl, dev, DL_TYPE_INT16, DL_FORMAT_ARRAY, nb_values*2, (void *) _values);
    trans->end_message(trans->impl, dev);
  } else
    trans->overrun(trans->impl, dev);
}

#define DOWNLINK_SEND_IMCU_SPEKTRUM_SOFT_BIND(_trans, _dev) pprz_msg_send_IMCU_SPEKTRUM_SOFT_BIND(&((_trans).trans_tx), &((_dev).device), AC_ID)
static inline void pprz_msg_send_IMCU_SPEKTRUM_SOFT_BIND(struct transport_tx *trans, struct link_device *dev, uint8_t ac_id) {
  if (trans->check_available_space(trans->impl, dev, trans->size_of(trans->impl, 0 +2 /* msg header overhead */))) {
    trans->count_bytes(trans->impl, dev, trans->size_of(trans->impl, 0 +2 /* msg header overhead */));
    trans->start_message(trans->impl, dev, 0 +2 /* msg header overhead */);
    trans->put_bytes(trans->impl, dev, DL_TYPE_UINT8, DL_FORMAT_SCALAR, 1, &ac_id);
    trans->put_named_byte(trans->impl, dev, DL_TYPE_UINT8, DL_FORMAT_SCALAR, DL_IMCU_SPEKTRUM_SOFT_BIND, "IMCU_SPEKTRUM_SOFT_BIND");
    trans->end_message(trans->impl, dev);
  } else
    trans->overrun(trans->impl, dev);
}

#else // DOWNLINK

#define DOWNLINK_SEND_IMCU_COMMANDS(_trans, _dev, status, nb_values, values) {}
static inline void pprz_send_msg_IMCU_COMMANDS(struct transport_tx *trans __attribute__((unused)), struct link_device *dev __attribute__((unused)), uint8_t ac_id __attribute__((unused)), uint8_t *_status __attribute__((unused)), uint8_t nb_values __attribute__((unused)), int16_t *_values __attribute__((unused))) {}

#define DOWNLINK_SEND_IMCU_RADIO_COMMANDS(_trans, _dev, status, nb_values, values) {}
static inline void pprz_send_msg_IMCU_RADIO_COMMANDS(struct transport_tx *trans __attribute__((unused)), struct link_device *dev __attribute__((unused)), uint8_t ac_id __attribute__((unused)), uint8_t *_status __attribute__((unused)), uint8_t nb_values __attribute__((unused)), int16_t *_values __attribute__((unused))) {}

#define DOWNLINK_SEND_IMCU_SPEKTRUM_SOFT_BIND(_trans, _dev) {}
static inline void pprz_send_msg_IMCU_SPEKTRUM_SOFT_BIND(struct transport_tx *trans __attribute__((unused)), struct link_device *dev __attribute__((unused)), uint8_t ac_id __attribute__((unused))) {}

#endif // DOWNLINK

#define DL_IMCU_COMMANDS_status(_payload) _PPRZ_VAL_uint8_t(_payload, 2)
#define DL_IMCU_COMMANDS_values_length(_payload) _PPRZ_VAL_uint8_t(_payload, 3)
#define DL_IMCU_COMMANDS_values(_payload) _PPRZ_VAL_int16_t_array(_payload, 4)

#define DL_IMCU_RADIO_COMMANDS_status(_payload) _PPRZ_VAL_uint8_t(_payload, 2)
#define DL_IMCU_RADIO_COMMANDS_values_length(_payload) _PPRZ_VAL_uint8_t(_payload, 3)
#define DL_IMCU_RADIO_COMMANDS_values(_payload) _PPRZ_VAL_int16_t_array(_payload, 4)




#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _VAR_MESSAGES_intermcu_H_
