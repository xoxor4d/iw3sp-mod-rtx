#include "STDInc.hpp"

/// http://www.opensource.apple.com/source/CommonCrypto/CommonCrypto-55010/Source/libtomcrypt/doc/libTomCryptDoc.pdf
namespace Utils
{
	namespace Cryptography
	{
		std::string SHA1::Compute(const std::string& data, bool hex)
		{
			return Compute(reinterpret_cast<const std::uint8_t*>(data.data()), data.size(), hex);
		}

		std::string SHA1::Compute(const std::uint8_t* data, std::size_t length, bool hex)
		{
			std::uint8_t buffer[20]{};

			hash_state state;
			sha1_init(&state);
			sha1_process(&state, data, length);
			sha1_done(&state, buffer);

			std::string hash(reinterpret_cast<char*>(buffer), sizeof(buffer));
			if (!hex) return hash;

			return String::DumpHex(hash, {});
		}
	}
}