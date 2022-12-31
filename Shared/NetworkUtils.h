#ifndef FHENIX_NETWORKUTILS_H
#define FHENIX_NETWORKUTILS_H

#include "Opcodes.h"
#include "DataStream.h"

enum class Protocol
{
    TCP,
    UDP,
};

namespace Fhenix
{
    namespace Shared
    {

        template<typename ...Params>
        QByteArray makePacket(const Opcodes opcode, Params&&... params)
        {
            QByteArray packet;

            DataStream out(&packet, QIODevice::WriteOnly);

            out << (quint16)0;

            out.pack(static_cast<quint8>(opcode), params...);
            //pack(&out, opcodes, params...);
            out.device()->seek(0);
            out << (quint16)(packet.size() - sizeof(quint16));

            return packet;

            // out.pack(Opcodes, std::forward<Params>(param)...);


            // quint size;
            //
        }


    }

}

#endif // NETWORKUTILS_H
