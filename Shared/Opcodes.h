#ifndef FHENIX_OPCODES_HPP
#define FHENIX_OPCODES_HPP

/* CMSG_XXXX_XXXX = Client Message only
 * SMSG_XXXX_XXXX = Server Message only
 * MSG_XXXX_XXXX = Server and Client Message only
 */

namespace Fhenix
{
        enum Opcodes
        {
            // Client Message
            CMSG_CONNECT,
            CMSG_GET_CHANNEL_LIST,
            CMSG_GET_CLIENT_LIST,
            CMSG_GET_SERVER_INFO,
            CMSG_SWITCH_CHANNEL,
            CMSG_CREATE_UDP_CONNECTION,
            // Server and Client Message
            MSG_DISCONNECT,
            MSG_SEND_MESSAGE,
            MSG_VOICE_DATA,
            MSG_UDP_TEST,
            // Server Message
            SMSG_CHANNEL_LIST,
            SMSG_CLIENT_LIST,
            SMSG_CONNECT_RESULT,
            SMSG_SERVER_INFO,
            SMSG_CLIENT_CHANNEL_SWITCHED,
            SMSG_NEW_CLIENT_CONNECTED,
            SMSG_CLIENT_DISCONNECTED,
            SMSG_UDP_CONNECION_OK,
            SMSG_CLIENT_SPEAKING,
        };

        /* MSG_SEND_MESSAGE
         * Args1 : msg (QString)
         * Args2 : Target (User, Server, Group or Channel)
         * Args3 : Channel id (User id, Server id, group id, or channel id)
         * Args4 (Only Server) : sender (Client id or -1 if is the server)
         */

        /* CMSG_CLIENT_LOGIN
         * Arg1 : NickName
         * Arg2 (optionnal): Password
         */

        /* CMSG_CLIENT_DISCONNECT
        */

        /* SMSG_CLIENT_CONNECTED
         * Arg1: client id
         * Arg2: Nickname
         */

        /* CMSG_UPDATE_CLIENT_INFO
         * Arg1: client id
         */

        /* SMSG_UPDATE_CLIENT_INFO
         * Arg1: client id
         * Arg2: client name
         * Arg3: description
         */






        // Client Deconnexion
        // Client Connexion
        // Client join channel
        // Client leave channel
        // Client Ban client
        // Client Kick client
        // Client Move Channel
        // Client Send Message
        // Client


}

#endif
