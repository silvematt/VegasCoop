#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

#include <vector>

#include "Packet.h"

namespace NECRO
{
    //-----------------------------------------------------------------------------------------------------------
    // Higher-level view packets, used for Network transmission.
    //-----------------------------------------------------------------------------------------------------------
    class NetworkMessage
    {
    private:
        size_t m_rpos;          // Read Pos
        size_t m_wpos;          // Write Pos 
        // ReadPos in the NetworkMessage can be viewed as "consumed" pos, 
        // if it's > 0 it means we've consumed the data until there, so it's probably a good idea to move the remaining data at beginning of the buffer with CompactData()

        std::vector<uint8_t>    m_data;          // Raw Data

    public:
        // Move constructor
        NetworkMessage(NetworkMessage&& other) noexcept :
            m_rpos(other.m_rpos),
            m_wpos(other.m_wpos),
            m_data(std::move(other.m_data))                // Move the vector
        {
        }

        // NetworkMessage Constructor
        // data is resized (not reserved) because we'll need it as soon as this is created, and probably we'll need exactly the reservedSize amount
        NetworkMessage() : m_rpos(0), m_wpos(0)
        {
            m_data.resize(Packet::DEFAULT_PCKT_SIZE);
        }

        NetworkMessage(size_t reservedSize) : m_rpos(0), m_wpos(0)
        {
            m_data.resize(reservedSize);
        }

        // Wraps a packet in a NetworkMessage
        NetworkMessage(const Packet& p)
        {
            m_data.resize(p.Size());
            Write(p.GetContentToRead(), p.Size());
        }

        //-----------------------------------------------------------------------------------------------------------
        // Clears data array and write/read pos
        //-----------------------------------------------------------------------------------------------------------
        void Clear()
        {
            m_data.clear();
            m_rpos = m_wpos = 0;
        }

        //-----------------------------------------------------------------------------------------------------------
        // Resets read/write pos without clearing the array, allowing to reuse memory
        //-----------------------------------------------------------------------------------------------------------
        void SoftClear()
        {
            m_rpos = m_wpos = 0;
        }

        size_t  Size()  const { return m_data.size(); }
        bool    Empty() const { return m_data.empty(); }

        // Functions to easily access data locations
        uint8_t* GetBasePointer() { return m_data.data(); }
        uint8_t* GetReadPointer() { return GetBasePointer() + m_rpos; }
        uint8_t* GetWritePointer() { return GetBasePointer() + m_wpos; }

        // Useful information
        size_t GetActiveSize() const { return m_wpos - m_rpos; }
        size_t GetRemainingSpace() const { return m_data.size() - m_wpos; }

        //-----------------------------------------------------------------------------------------------------------------
        // When data will be processed by the socket read handler, it will have to call this function to update the rpos
        //-----------------------------------------------------------------------------------------------------------------
        void ReadCompleted(size_t bytes)
        {
            m_rpos += bytes;
        }

        //-----------------------------------------------------------------------------------------------------------------
        // When data will be written on the buffer by the recv, it will have to call this function to update the wpos
        //-----------------------------------------------------------------------------------------------------------------
        void WriteCompleted(size_t bytes)
        {
            m_wpos += bytes;
        }


        //-----------------------------------------------------------------------------------------------------------------
        // If data was consumed, shifts the remaining (unconsumed) data to the beginning of the data buffer
        //-----------------------------------------------------------------------------------------------------------------
        void CompactData()
        {
            if (m_rpos > 0)
            {
                if (m_rpos != m_wpos) // if there's data to shift
                    memmove(GetBasePointer(), GetReadPointer(), GetActiveSize());

                m_wpos = m_wpos - m_rpos; // adjust wpos accordingly
                m_rpos = 0;
            }
        }

        //-----------------------------------------------------------------------------------------------------------------
        // Writes data on the current WritePointer
        //-----------------------------------------------------------------------------------------------------------------
        void Write(void const* bytes, std::size_t size)
        {
            if (size > 0)
            {
                // Check for space
                if (GetRemainingSpace() < size)
                {
                    // Check if compacting data is enough
                    CompactData();

                    // If not, we need to resize
                    if (GetRemainingSpace() < size)
                    {
                        size_t newSize = (Size() + size);
                        newSize += (newSize / 2); // give an extra 50% of storage
                        m_data.resize(newSize);
                    }
                }

                memcpy(GetWritePointer(), bytes, size);
                m_wpos += size;
            }
        }

        //---------------------------------------------------------------------------------------------------------------------------
        // Increases the data buffer size by 50% of the current size if it's currently full, useful to the inBuffer of the Sockets,
        // if done before a read it can avoid having to queue packets
        //---------------------------------------------------------------------------------------------------------------------------
        void EnlargeBufferIfNeeded()
        {
            if (GetRemainingSpace() == 0)
                m_data.resize(Size() + (Size() / 2));
        }

    };
}
#endif
