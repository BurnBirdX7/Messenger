#ifndef ASIOAPPLICATION_PURPOSEBYTES_HPP
#define ASIOAPPLICATION_PURPOSEBYTES_HPP

/* Purpose Bytes
 *
 * Each message in client-server connection contains Purpose Byte
 *
 * This byte specifies action which server or client wants to execute.
 */

#include <cstdint>

namespace Commons::Network {

    struct PurposeBytes {

        enum common : uint8_t { // 0x00 .. 0x1F
            //  byte
            ACCEPTED  = 0x00, //
            DECLINED  = 0x01, //
            HEARTBEAT = 0x02, //
        };

        enum to_server : uint8_t {
            //  byte                    // CONTENT
            HELLO            = 0x20, // API version
            LOGIN            = 0x21, // login, password
            LOGOFF           = 0x22, // -
            RESTORE_SESSION  = 0x23, // session_id, session_hash
            SEND_CHAT_MSG    = 0x24, // chat_id, chat_message_content
            REQUEST_CHAT_MSG = 0x25, // chat_id, datetime
        };

        enum to_client : uint8_t {
            MSG_NOTIFY   = 0x20, // chat_id, msg_count // TODO: count?
            SESSION_DATA = 0x21, // session_id, session_hash
                                 // Session data also can be sent with ACCEPTED message in answer for LOGIN
        };

    };

}

#endif //ASIOAPPLICATION_PURPOSEBYTES_HPP
