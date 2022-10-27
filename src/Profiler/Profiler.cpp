#include "Profiler/Profiler.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <fmt/format.h>
#include <fmt/chrono.h>

const std::chrono::steady_clock::time_point APP_START = std::chrono::steady_clock::now();
TracerFile Profiler::s_output;
std::mutex Profiler::s_fileLock;

Profiler::Profiler(std::string_view name) : _name(name), _timerStopped(false)
{
	std::lock_guard lck(s_fileLock);

	if (!s_output.is_open())
	{
		
		const auto wallTime = std::chrono::system_clock::now();
		std::string timeStr = fmt::format("AppProfile {}.json", wallTime);
		s_output.open(timeStr, std::ios::out | std::ios::trunc);
		if (s_output.is_open())
			s_output.writeHeader();
		else
			std::cerr << "Failed to open profiler json file\n";
	}

	_start = std::chrono::steady_clock::now();
}

Profiler::~Profiler()
{
	if (!_timerStopped)
		stopTimer();
}

void Profiler::stopTimer()
{
	auto end = std::chrono::high_resolution_clock::now();

	_timerStopped = true;

	std::lock_guard lck(s_fileLock);

	s_output <<
		"{\n" <<
		"\"name\": \"" << _name << "\",\n" <<
		"\"cat\": \"Function\",\n" <<
		"\"ph\": \"X\",\n" <<
		"\"ts\": " << (_start - APP_START).count() / 1000.0f << ",\n" <<
		"\"dur\": " << (std::chrono::steady_clock::now() - _start).count() / 1000.0f << ",\n" <<
		"\"pid\": 1,\n" <<
		"\"tid\": " << std::this_thread::get_id() << ",\n" <<
		"\"args\": []\n" <<
		"},\n\n";

	s_output.flush();

}

TracerFile::~TracerFile()
{
	if (is_open())
	{
		writeFooter();
		close();
	}
}

bool TracerFile::writeHeader()
{
	if (is_open())
	{
		*this <<
			"{" <<
			"\"displayTimeUnit\": \"ns\", " <<
			"\"traceEvents\": [";

		flush();

		return true;
	}

	return false;
}

bool TracerFile::writeFooter()
{
	if (is_open())
	{
		seekp(-5, std::ios::cur);
		*this << "]}";

		flush();

		return true;
	}

	return false;
}
