#ifndef ASIOAPPLICATION_PURPOSE_HPP
#define ASIOAPPLICATION_PURPOSE_HPP

/* Purpose Bytes
 *
 * Each message in client-server connection contains Purpose Byte
 *
 * This byte specifies action which server or client wants to execute.
 */

#include <cstdint>

namespace Commons::Network {

    constexpr static uint8_t MESSAGE_API_MAJOR_VERSION = 0;
    constexpr static uint8_t MESSAGE_API_MINOR_VERSION = 1;

    struct Purpose {

        enum common : uint8_t { // 0x00 .. 0x1F
            //  byte
            ACCEPTED   = 0x00, //
            DECLINED   = 0x01, //
            HEARTBEAT  = 0x02, //
            DISCONNECT = 0x03, //
        };

        enum to_server : uint8_t { // 0x20 .. 0xFF
         // NAME          // = BYTE  // CONTENT                         // ANSWER
         // ------------- // = ----- // ------------------------------- // ------------------------------------------------------------------------------- //
            HELLO            = 0x20, // API version                     // ACCEPTED; DECLINED

            LOGIN            = 0x21, // login, password                 // ACCEPTED: session_id, session_hash; DECLINED;
            LOGOFF           = 0x22, // -                               // ACCEPTED, can't be declined
            RESTORE_SESSION  = 0x23, // session_id, session_hash        // ACCEPTED; DECLINED

            GET_USER_ID      = 0x24, // nickname                        // ACCEPTED: user_id; DECLINED;
            GET_CHAT_ID      = 0x25, // chat_name                       // ACCEPTED: chat_id; DECLINED;
            CHAT_REQ_PASS    = 0x26, // chat_id                         // ACCEPTED: (byte) true/false; DECLINED;
            JOIN_CHAT        = 0x27, // chat_id [, password]            // ACCEPTED; DECLINED: reason (string)

            SEND_CHAT_MSG    = 0x28, // chat_id, chat_message_content   // ACCEPTED; DECLINED: reason (string)
            REQUEST_CHAT_MSG = 0x29, // chat_id, datetime               // ACCEPTED: message pack; DECLINED: reason (string)
            MARK_SEEN        = 0x2A, // chat_id, timestamp // TODO: or msg_id?
        };

        enum to_client : uint8_t {
          // NAME          // = BYTE  // CONTENT                         // ANSWER
          // ------------- // = ----- // ------------------------------- // ------------------------------------------------------------------------------- //
            MSG_NOTIFY        = 0x20, // TODO: count?                    //
            SESSION_DATA      = 0x21, // session_id, session_hash        // ACCEPTED; DECLINED
            DEAUTH            = 0x22, // -                               // ACCEPTED

        };

    };

}

#endif //ASIOAPPLICATION_PURPOSE_HPP
