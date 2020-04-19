#include <algorithm>

#include <iostream>
#include "ByteDetector.h"

using namespace std;

Bits::size_type ByteDetector::skipTo(Bit valueToStopAt) {
	Bits::size_type skipped = 0;
	while(!streamEnded() && curBit() != valueToStopAt) {
		advanceOneBit();
		skipped++;
	}
	return skipped;
}

std::string ByteDetector::currentContext() const {
	std::string context;

	auto contextStart = distance(m_startOfStreamIt, m_curBitIt) > ContextRadius ? (m_curBitIt - ContextRadius) : m_startOfStreamIt;
	auto contextEnd = distance(m_curBitIt, m_endOfStreamIt) > ContextRadius ? (m_curBitIt + ContextRadius) : m_endOfStreamIt;

	for(auto it = contextStart; it != contextEnd; it++) {
		if(it == m_curBitIt) {
			context += ">";
		}
		context = context + to_string(*it);
		if(it == m_curBitIt) {
			context += "<";
		}
	}
	return context;
}

void ByteDetector::reportError(std::string&& message) {
	Bits contextBits;

	*m_errorInserter = { bitsRead(), message, currentContext() };
}

void ByteDetector::detect(
		const Bits& bits,
		back_insert_iterator<Bytes> inserter,
		back_insert_iterator<Errors> errorInserter
	) {

	m_curBitIt = bits.begin();
	m_startOfStreamIt = bits.begin();
	m_endOfStreamIt = bits.end();
	m_errorInserter = errorInserter;

	bool encounteredFFByte = false;

	/* read byte-by-byte */
	while(!streamEnded()) {
		skipTo(0);

		if(streamEnded()) {
			return;
		}

		Byte currentByte = 0;

		/* the pointer is at the start-bit */
		for(short x = 0; x < 8; x++) {
			advanceOneBit();
			if(streamEnded()) {
				reportError("stream ended before byte was read");
				return;
			}
			currentByte |= (curBit() << x);
		}

		/* the pointer is at the last bit */

		advanceOneBit();

		/* the pointer is at first stop-bit */

		bool wellFormedByte = true;
		for(short x = 0; x < 2; x++) {
			if(streamEnded()) {
				reportError("stream ended before stop-bit");
				return;
			}
			if(curBit() != 1) {
				wellFormedByte = false;
				break;
			}
			advanceOneBit();
		}

		if(!wellFormedByte) {
			continue;
		}
		if(!encounteredFFByte && currentByte == 0xff) {
			encounteredFFByte = true;
		}

		if(!encounteredFFByte) {
			continue;
		}

		inserter = currentByte;
	}
}