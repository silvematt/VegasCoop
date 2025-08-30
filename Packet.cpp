#include "Packet.h"


#include <sstream>
#include <iomanip>

namespace NECRO
{
	//--------------------------------------------------------------------------------------------
	// Appends bytes to the data
	//--------------------------------------------------------------------------------------------
	void Packet::Append(uint8_t const* src, size_t cnt)
	{
		// Calculate the new size of this packet
		size_t const newSize = m_wpos + cnt;

		// Check if the capacity (reserved bytes) isn't enough 
		if (m_data.capacity() < newSize)
		{
			m_data.reserve(newSize + (newSize / 2));	// Increase the capacity to the needed capacity + an extra 50%
		}

		// Resize the vector if the space is not enough
		if (m_data.size() < newSize)
			m_data.resize(newSize);

		std::memcpy(&m_data[m_wpos], src, cnt);
		m_wpos = newSize;
	}

	//--------------------------------------------------------------------------------------------
	// Prints the packet in hex
	//--------------------------------------------------------------------------------------------
	void Packet::Print() const
	{
		//LOG_INFO("Packet size {}", Size());

		// Hex print of the packet content
		std::ostringstream oss;  // Output string stream

		for (size_t i = 0; i < m_data.size(); ++i)
		{
			// Print each byte in hexadecimal format into the string stream
			oss << std::hex << std::setfill('0') << std::setw(2)
				<< static_cast<int>(m_data[i]) << " ";

			// Group in 16 bytes
			if ((i + 1) % 16 == 0) {
				oss << std::endl;
			}
		}

		oss << std::endl;

		//LOG_INFO("Packet content: \n{}", oss.str().c_str());
	}

}
