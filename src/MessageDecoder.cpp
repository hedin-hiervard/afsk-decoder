#include "MessageDecoder.h"

using namespace std;

void MessageDecoder::reportError(std::string&& message) {
	*m_errorInserter = {
		.position = static_cast<Bytes::size_type>(distance(m_streamBeginIt, m_curByteIt)),
		.message = message,
	};
}

Bytes::size_type MessageDecoder::skipTo(Byte val) {
	Bytes::size_type res = 0;
	while(!streamFinished() && curByte() != val) {
		m_curByteIt++;
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

	/* skip leading 0xff */
	while(!streamFinished() && curByte() == 0xff) {
		advanceByte();
	}

	if(streamFinished() || curByte() != 0x42) {
		reportError("header error");
		return;
	}

	advanceByte();

	if(streamFinished() || curByte() != 0x03) {
		reportError("header error");
		return;
	}

	while(!streamFinished()) {
		if(distance(m_curByteIt, m_streamEndIt) <= MessageSize) {
			break;
		}
		Message message;
		copy_n(m_curByteIt, MessageSize, message.begin());
		advance(m_curByteIt, MessageSize);
		*inserter = message;
	}

	if(streamFinished()) {
		reportError("unfinished message");
		return;
	}


}