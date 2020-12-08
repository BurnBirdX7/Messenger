#ifndef ASIOAPPLICATION_PURPOSE_HPP
#define ASIOAPPLICATION_PURPOSE_HPP

/* Purpose Bytes
 *
 * Each message in client-server connection contains Purpose Byte
 * This byte specifies action which server or client wants to execute.
 *
 * More information in Purpose.md
 */

#include <cstdint>

namespace Commons::Network {

    constexpr static uint8_t MESSAGE_API_MAJOR_VERSION = 0;
    constexpr static uint8_t MESSAGE_API_MINOR_VERSION = 1;

    struct Purpose {

        enum common : uint8_t { // 0x00 .. 0x1F
            //  byte
            ACCEPTED   = 0x00, // Type: ANSWER
            DECLINED   = 0x01, // Type: ANSWER
            HEARTBEAT  = 0x02, // HEARTBEAT, Content: [string_to_return], Answer: [string_to_return]
            DISCONNECT = 0x03, // DISCONNECT, Content: none, Answer: none
        };

        enum to_server : uint8_t { // 0x20 .. 0xFF
         // NAME          // = BYTE  // CONTENT                          // ANSWER
         // ------------- // = ----- // -------------------------------- // ------------------------------------------------------------------------------- //
            HELLO            = 0x20, // API version                      // ACCEPTED; DECLINED
                                     //                                  //
            LOGIN            = 0x21, // login, password                  // ACCEPTED: session_id, session_hash; DECLINED;
            LOGOFF           = 0x22, // -                                // ACCEPTED, DECLINED: reason;
            RESTORE_SESSION  = 0x23, // session_id, session_hash         // ACCEPTED; DECLINED;
            REGISTER_USER    = 0x24, // login, password                  // ACCEPTED; DECLINED;
                                     //                                  //
            GET_USER_ID      = 0x2A, // nickname                         // ACCEPTED: user_id; DECLINED;
            GET_CHAT_ID      = 0x2B, // chat_name                        // ACCEPTED: chat_id; DECLINED;
            GET_CHAT_BY_ID   = 0x2C, // chat_id                          // ACCEPTED: chat_info; DECLINED: reason;
            GET_CHAT_BY_NAME = 0x2D, // chat_name                        // ACCEPTED: chat_info; DECLINED: reason;
                                     //                                  //
            JOIN_CHAT        = 0x30, // chat_id [, password]             // ACCEPTED; DECLINED: reason;
            LEAVE_CHAT       = 0x31, // chat_id                          // ACCEPTED; DECLINED: reason;
            CREATE_CHAT      = 0x32, // chat_name [, password]           // ACCEPTED: chat_id; DECLINED: reason;
            START_CHAT       = 0x33, // user_id                          // ACCEPTED: chat_id; DECLINED: reason;
            DELETE_CHAT      = 0x34, // user_id                          // ACCEPTED; DECLINED: reason;
            RENAME_CHAT      = 0x35, // chat_id, chat_name               // ACCEPTED; DECLINED: reason;
            CH_CHAT_TITLE    = 0x36, // chat_id, chat_title              // ACCEPTED; DECLINED: reason;
            SET_CHAT_ADMIN   = 0x37, // chat_id, user_id                 // ACCEPTED; DECLINED: reason;
            REM_CHAT_ADMIN   = 0x38, // chat_id, user_id                 // ACCEPTED; DECLINED: reason;
            GET_USR_CHATS    = 0x39, // -                                // ACCEPTED: chat_info_pack; DECLINED: reason;
                                     //                                  //
            SEND_CHAT_MSG    = 0x40, // chat_id, chat_message_content    // ACCEPTED; DECLINED: reason;
            REQUEST_CHAT_MSG = 0x41, // chat_id, datetime                // ACCEPTED: message_pack; DECLINED: reason;
            MARK_SEEN        = 0x42, // chat_id, timestamp               // ACCEPTED; DECLINED: reason;
            GET_SEEN         = 0x43, // chat_id                          // ACCEPTED; DECLINED: reason;
            REMOVE_MSG       = 0x44, // message_id                       // ACCEPTED; DECLINED: reason;
            DELETE_MSG       = 0x45, // message_id                       // ACCEPTED; DECLINED: reason;
        };

        enum to_client : uint8_t {
          // NAME          // = BYTE  // CONTENT                          // ANSWER
          // ------------- // = ----- // -------------------------------- // ------------------------------------------------------------------------------- //
            MSG_NOTIFY        = 0x20, // message_content                  //
            SESSION_DATA      = 0x21, // session_id, session_hash         // ACCEPTED; DECLINED
            DEAUTHORIZED      = 0x22, // -                                // ACCEPTED

        };

    };

}

#endif //ASIOAPPLICATION_PURPOSE_HPP
