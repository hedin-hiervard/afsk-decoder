#pragma once

#include <array>
#include <vector>
#include <string>
#include <optional>

#include "data_types.h"

class MessageDecoder
{
public:
	static constexpr size_t MessageSize = 30;

	using Message = std::array<Byte, MessageSize>;
	using Messages = std::vector<Message>;

	struct Error {
		Bytes::size_type position;
		std::string message;
	};
	using Errors = std::vector<Error>;

	void decode(
		const Bytes& bytes,
		std::back_insert_iterator<Messages> inserter,
		std::back_insert_iterator<Errors> errorInserter
	);
private:
	inline bool streamFinished() const { return m_curByteIt == m_streamEndIt; }
	inline Byte curByte() const { return *m_curByteIt; }
	inline void advanceByte() { m_curByteIt++; }

	static Byte messageChecksum(const Message& message);

	void reportError(std::string&& message);

	Bytes::size_type skipTo(Byte val);

	Bytes::const_iterator m_curByteIt;
	Bytes::const_iterator m_streamEndIt;
	Bytes::const_iterator m_streamBeginIt;

	std::optional<std::back_insert_iterator<Messages>> m_inserter;
	std::optional<std::back_insert_iterator<Errors>> m_errorInserter;
};