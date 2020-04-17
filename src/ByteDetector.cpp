#include <algorithm>

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

void ByteDetector::reportError(std::string&& message) {
	Bits contextBits;
	std::string context;
	for(auto it = m_curBitIt - 5; it != m_curBitIt + 5; it++) {
		if(it == m_curBitIt) {
			context += ">";
		}
		context = context + to_string(*it);
		if(it == m_curBitIt) {
			context += "<";
		}
	}
	*m_errorInserter = { bitsRead(), message, context };
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

	/* skip leading tone */
	Bits::size_type leadingBits = skipTo(0);

	/* read byte-by-byte */
	while(!streamEnded()) {
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

		for(short x = 0; x < 2; x++) {
			if(streamEnded()) {
				reportError("stream ended before stop-bit");
				return;
			}
			if(curBit() != 1) {
				reportError("missing stop-bit");
				break;
			}
			advanceOneBit();
		}
		inserter = currentByte;
		skipTo(0);
	}
}