#pragma once
enum class PacketType {
	ChatMessage,
	FileTransferProposal,
	FileTransferRequestFile,
	FileTransferEndofFile,
	FileTransferByteBuffer,
	FileTransferRequestNextBuffer
};