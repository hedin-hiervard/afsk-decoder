#pragma once

#include <vector>
#include <string>
#include <optional>

#include "data_types.h"

class ByteDetector {
public:
	struct Error {
		Bits::size_type position;
		std::string message;
		std::string bitContext;
	};

	using Errors = std::vector<Error>;

	void detect(
		const Bits& bits,
		std::back_insert_iterator<Bytes> inserter,
		std::back_insert_iterator<Errors> errorInserter
	);
private:
	Bits::size_type skipTo(Bit value);
	inline bool streamEnded() const { return m_curBitIt == m_endOfStreamIt; }
	inline const Bit curBit() const { return *m_curBitIt; }
	inline void advanceOneBit() { m_curBitIt++; }
	inline Bits::size_type bitsRead() const { return std::distance(m_startOfStreamIt, m_curBitIt); }
	void reportError(std::string&& message);
	std::string currentContext() const;

	Bits::const_iterator m_startOfStreamIt;
	Bits::const_iterator m_curBitIt;
	Bits::const_iterator m_endOfStreamIt;
	std::optional<std::back_insert_iterator<Errors>> m_errorInserter;
	static constexpr Bits::size_type ContextRadius = 10;
};