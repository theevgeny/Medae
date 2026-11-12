#include "BinaryFileHandler.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ios>
#include <string>
#include <utility>
#include <vector>

#include <spdlog/spdlog.h>

using namespace Medae::Assets;

BinaryFileHandler::BinaryFileHandler(std::fstream&& file, const std::string& path) noexcept
	: m_file(std::move(file)), m_path(path)
{
	m_file.exceptions(std::ios::goodbit);

	if (!m_file.is_open()) {
		spdlog::error("BinaryFileHandler: stream is not open: {}", m_path);
		return;
	}

	m_file.seekg(0, std::ios::end);
	if (!m_file) {
		spdlog::error("BinaryFileHandler: cannot determine file size: {}", m_path);
		m_file.clear();
		return;
	}

	m_size = static_cast<std::size_t>(m_file.tellg());

	m_file.seekg(0, std::ios::beg);
	if (!m_file) {
		spdlog::error("BinaryFileHandler: cannot seek to start: {}", m_path);
		m_file.clear();
		m_size = 0;
		return;
	}

	m_valid = true;
}

bool BinaryFileHandler::IsValid() const noexcept
{
	return m_valid;
}

void BinaryFileHandler::ReadRaw(void* dst, std::size_t size) noexcept
{
	if (size == 0 || dst == nullptr) {
		return;
	}

	const std::size_t available = m_readPos < m_size ? m_size - m_readPos : 0;
	const std::size_t toRead = std::min(size, available);
	const std::size_t offsetBefore = m_readPos;

	if (toRead > 0) {
		m_file.read(static_cast<char*>(dst), static_cast<std::streamsize>(toRead));
		const auto got = m_file.gcount();
		m_readPos += static_cast<std::size_t>(got);
		if (got != static_cast<std::streamsize>(toRead)) {
			spdlog::error("BinaryFileHandler: read failed at offset {} in: {} (got {} of {} byte(s))", offsetBefore,
				m_path, got, toRead);
			m_file.clear();
		}
	}

	if (toRead < size) {
		// Tail is already zero-initialized by the caller.
		spdlog::error("BinaryFileHandler: requested {} byte(s) at offset {} but only {} available, "
					  "zero-padding {} byte(s): {}",
			size, offsetBefore, toRead, size - toRead, m_path);
	}
}

std::uint8_t BinaryFileHandler::GetByte() noexcept
{
	std::uint8_t byte = 0;
	ReadRaw(&byte, sizeof(byte));
	return byte;
}

std::vector<std::uint8_t> BinaryFileHandler::GetBytes(std::size_t size) noexcept
{
	std::vector<std::uint8_t> buffer(size, 0);
	ReadRaw(buffer.data(), size);
	return buffer;
}

std::vector<std::uint8_t> BinaryFileHandler::GetBytesToEnd() noexcept
{
	if (m_readPos >= m_size) {
		return {};
	}

	const std::size_t remaining = m_size - m_readPos;
	std::vector<std::uint8_t> buffer(remaining, 0);
	ReadRaw(buffer.data(), remaining);
	return buffer;
}

std::vector<std::uint8_t> BinaryFileHandler::GetAllBytes() noexcept
{
	m_file.clear();
	m_file.seekg(0, std::ios::beg);
	if (!m_file) {
		spdlog::error("BinaryFileHandler: cannot seek to start: {}", m_path);
		m_file.clear();
		return {};
	}
	m_readPos = 0;

	std::vector<std::uint8_t> buffer(m_size, 0);
	ReadRaw(buffer.data(), m_size);
	return buffer;
}

void BinaryFileHandler::ToStart() noexcept
{
	m_file.clear();
	m_file.seekg(0, std::ios::beg);
	if (!m_file) {
		spdlog::error("BinaryFileHandler: cannot seek to start: {}", m_path);
		m_file.clear();
		return;
	}
	m_readPos = 0;
}

std::size_t BinaryFileHandler::GetReadPos() const noexcept
{
	return m_readPos;
}

const std::string& BinaryFileHandler::GetPath() const noexcept
{
	return m_path;
}

std::size_t BinaryFileHandler::GetSize() const noexcept
{
	return m_size;
}