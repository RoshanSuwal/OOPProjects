#pragma once
enum class PacketType {
	ChatMessage,
	FileTransferRequestFile,
	FileTransferEndofFile,
	FileTransferByteBuffer,
	FileTransferRequestNextBuffer
};