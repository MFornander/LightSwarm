#pragma once

namespace LightSwarm {

#define INFO(...)  LightSwarm::Debug::GetSingleton().Info(__VA_ARGS__)
#define WARN(...)  LightSwarm::Debug::GetSingleton().Warn(__VA_ARGS__)
#define ERRX(...) LightSwarm::Debug::GetSingleton().Error(__VA_ARGS__)

#define INFO_IF(cond, ...)  (!cond) ? (void)0 : INFO(__VA_ARGS__)
#define WARN_IF(cond, ...)  (!cond) ? (void)0 : WARN(__VA_ARGS__)
#define ERRX_IF(cond, ...) (!cond) ? (void)0 : ERRX(__VA_ARGS__)


class Debug final
{
public:
	Debug();

	void SetLed(bool on = true);

	void Info(const char* format, ...);
	void Warn(const char* format, ...);
	void Error(const char* format, ...);

	static Debug& GetSingleton();
};
} // namespace
