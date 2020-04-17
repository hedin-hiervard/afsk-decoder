#include <numeric>

#include <iostream>
#include "MessageDecoder.h"

using namespace std;

Byte MessageDecoder::messageChecksum(const Message& message) {
	Byte result = 0x0;
	for(const auto& byte : message) {
		result = static_cast<Byte>((
			static_cast<DoubleByte>(result) +
			static_cast<DoubleByte>(byte)
		) % (numeric_limits<Byte>().max() + 1));
	}
	return result;
}

void MessageDecoder::reportError(std::string&& message) {
	*m_errorInserter = {
		.position = static_cast<Bytes::size_type>(distance(m_streamBeginIt, m_curByteIt)),
		.message = message,
	};
}

Bytes::size_type MessageDecoder::skipTo(Byte val) {
	Bytes::size_type res = 0;
	while(!streamFinished() && curByte() != val) {
		advanceByte();
		res++;
	}
	return res;
}

void MessageDecoder::decode(
		const Bytes& bytes,
		std::back_insert_iterator<Messages> inserter,
		std::back_insert_iterator<Errors> errorInserter
) {

	m_curByteIt = bytes.begin();
	m_streamEndIt = bytes.end();
	m_streamBeginIt = bytes.begin();

	m_errorInserter = errorInserter;

	while(!streamFinished()) {
		skipTo(0x42);
		if(streamFinished()) {
			reportError("header not found");
			return;
		}
		advanceByte();
		if(streamFinished()) {
			reportError("header not found");
			return;
		}
		if(curByte() == 0x03) {
			advanceByte();
			break; // header found
		}
	}

	while(!streamFinished()) {
		if(distance(m_curByteIt, m_streamEndIt) <= MessageSize) {
			break;
		}
		Message message;
		copy_n(m_curByteIt, MessageSize, message.begin());
		advance(m_curByteIt, MessageSize);
		if(streamFinished()) {
			reportError("no checksum found");
			return;
		}
		auto expectedChecksum = curByte();
		auto actualChecksum = messageChecksum(message);

		advanceByte();
		if(expectedChecksum != actualChecksum) {
			reportError("checksum error: expected " + to_string(expectedChecksum) + ", actual: " + to_string(actualChecksum));
		} else {
			*inserter = message;
		}
	}

	if(!streamFinished()) {
		reportError("unfinished message");
		return;
	}

}