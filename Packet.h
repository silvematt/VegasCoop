#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <string>
#include <stdexcept>

namespace NECRO
{
    //----------------------------------------------------------------------------------------------------------------
    // Low-level definition of packet, suited for creation and manipulation of packets to then send as NetworkMessages
    //----------------------------------------------------------------------------------------------------------------
    class Packet
    {
    private:
        size_t                  m_rpos;     // Read Pos
        size_t                  m_wpos;     // Write Pos
        std::vector<uint8_t>    m_data;     // Raw Data

    public:
        constexpr static size_t DEFAULT_PCKT_SIZE = 0x1000;

        // Packet Constructor
        Packet() : m_rpos(0), m_wpos(0)
        {
            // Data is reserved, not allocated. Reserving data allows to avoid stress on the allocator when appending bytes to the packet.
            // This means when we'll send the packet to the network, we'll still use 'data.size()' which will only contain the bytes we've actually written and need to send.
            m_data.reserve(DEFAULT_PCKT_SIZE);
        }

        Packet(size_t reservedSize) : m_rpos(0), m_wpos(0)
        {
            m_data.reserve(reservedSize);
        }

        void Clear()
        {
            m_data.clear();
            m_wpos = m_rpos = 0;
        }

        uint8_t* GetContent()
        {
            if (m_data.empty())
                throw std::out_of_range("Trying to GetContent of an empty packet!");

            return m_data.data();
        }

        const uint8_t* GetContentToRead() const
        {
            if (m_data.empty())
                throw std::out_of_range("Trying to GetContent of an empty packet!");

            return m_data.data();
        }

        size_t  Size()  const { return m_data.size(); }
        bool    Empty() const { return m_data.empty(); }


        // Base append function
        void Append(uint8_t const* src, size_t cnt);

        // Template allows us to override operators and append different data types
        template <typename T> void Append(T value)
        {
            Append((uint8_t*)&value, sizeof(value));
        }

        void Print() const;

#pragma region OPERATORS_OVERLOAD

        Packet& operator<<(bool value)
        {
            Append<uint8_t>(value ? 1 : 0);
            return *this; // allows for method chaining
        }

        Packet& operator<<(uint8_t value)
        {
            Append<uint8_t>(value);
            return *this;
        }

        Packet& operator<<(uint16_t value)
        {
            Append<uint16_t>(value);
            return *this;
        }

        Packet& operator<<(uint32_t value)
        {
            Append<uint32_t>(value);
            return *this;
        }

        Packet& operator<<(uint64_t value)
        {
            Append<uint64_t>(value);
            return *this;
        }

        Packet& operator<<(int8_t value)
        {
            Append<int8_t>(value);
            return *this;
        }

        Packet& operator<<(int16_t value)
        {
            Append<int16_t>(value);
            return *this;
        }

        Packet& operator<<(int32_t value)
        {
            Append<int32_t>(value);
            return *this;
        }

        Packet& operator<<(int64_t value)
        {
            Append<int64_t>(value);
            return *this;
        }

        Packet& operator<<(float value)
        {
            Append<float>(value);
            return *this;
        }

        Packet& operator<<(double value)
        {
            Append<double>(value);
            return *this;
        }

        Packet& operator<<(std::string const& value)
        {
            size_t length = value.length();
            Append(reinterpret_cast<const uint8_t*>(value.data()), length);
            //Append(static_cast<uint8_t>(0)); // '\0' DO NOT APPEND NULL TERMINATOR!
            return *this;
        }

        Packet& operator<<(const char* value)
        {
            size_t length = std::strlen(value);
            Append(reinterpret_cast<const uint8_t*>(value), length);
            Append(static_cast<uint8_t>(0)); // '\0'
            return *this;
        }


#pragma endregion

    };

}

#endif
