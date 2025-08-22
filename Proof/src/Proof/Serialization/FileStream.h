#pragma once
#include "StreamReadAndWrite.h"
#include <fstream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
namespace Proof
{
	class FileStreamWriter : public StreamWriter
	{
	public:
		FileStreamWriter(const std::filesystem::path& path)
			:m_Path(path)
		{
			m_Stream = std::ofstream(path, std::ifstream::out | std::ifstream::binary);
		}

		virtual ~FileStreamWriter() override 
		{ 
			if (m_Stream.is_open())
				m_Stream.close(); 
		}

		bool IsStreamGood() const override { return m_Stream.good(); }
		uint64_t GetStreamPosition() override { return (uint64_t)m_Stream.tellp(); }
		void SetStreamPosition(uint64_t pos) override { m_Stream.seekp(pos); }
		bool WriteData(const char* data, size_t size) override { m_Stream.write(data, size); return m_Stream.good(); }


	private:
		std::ofstream m_Stream;
		std::filesystem::path m_Path;
	};


	class FileStreamReader : public StreamReader
	{
	public:
		FileStreamReader(const std::filesystem::path& path)
			:m_Path(path)
		{
			m_Stream = std::ifstream(path, std::ifstream::in | std::ifstream::binary);
		}

		virtual ~FileStreamReader() override
		{
			if (m_Stream.is_open())
				m_Stream.close();
		}

		bool IsStreamGood() const override { return m_Stream.good(); }
		uint64_t GetStreamPosition() override { return static_cast<uint64_t>(m_Stream.tellg()); }
		void SetStreamPosition(uint64_t pos) override { m_Stream.seekg(pos); }
		bool ReadData(char* dst, size_t size) override { m_Stream.read(dst, size); return m_Stream.good(); }

	private:
		std::ifstream m_Stream;
		std::filesystem::path m_Path;
	};
}