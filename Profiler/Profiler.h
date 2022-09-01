#pragma once
#include <string>
#include <fstream>
#include <mutex>

class TracerFile;
extern const std::chrono::steady_clock::time_point APP_START;

class Profiler
{
public:
	Profiler(std::string_view name);
	~Profiler();

	void stopTimer();

private:
	std::chrono::steady_clock::time_point _start;
	const std::string_view _name;
	bool _timerStopped;

	static TracerFile s_output;

	static std::mutex s_fileLock;

};

class TracerFile : public std::fstream
{
public:
	~TracerFile();

	bool writeHeader();
	bool writeFooter();

};

#ifdef _DEBUG
#define SCOPED_PROFILE(name) Profiler prof##__LINE__(name)
#define FUNCTION_PROFILE() SCOPED_PROFILE(__FUNCTION__)
#else
#define SCOPED_PROFILE(name)
#define FUNCTION_PROFILE()
#endif
