#pragma once
#include "net_common.h"

namespace olc
{
    namespace net
    {
        template<typename T>
        struct message_header
        {
            T id{};
            uint32_t size=0;
        };

        template<typename T>
        struct message
        {
            message_header<T> header{};
            std::vector<uint8_t> body;

            size_t size() const
            {
                return sizeof(message_header<T>) + body.size();
            }

            friend std::ostream& operator<<(std::ostream& out, const message<T>& msg)
            {
                out<< " ID: " << int(msg.header.id) << " Size: " << msg.header.size;
                return out;
            }

            template<typename DataType>;
            friend message<T>& operator<<(message<T>& msg, const DataType& data)
            {
                static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

                size_t i = msg.body.size();

                msg.body.resize(msg.body.size()+sizeof(DataType));

                std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

                msg.header.size = msg.size();

                return msg;
            }

            template<typename DataType>;
            friend message<T>& operator>>(message<T>& msg, const DataType& data)
            {
                static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

                size_t i = msg.body.size() - sizeof(DataType);

                std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

                msg.body.resize(i);

                msg.header.size = msg.size();

                return msg;
            }
        };
    }
}