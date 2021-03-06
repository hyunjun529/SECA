#ifndef SECA_UTIL_LOG_H_
#define SECA_UTIL_LOG_H_

#include <memory>
#include <spdlog/spdlog.h>

namespace seca
{
	namespace util{
		class Log
		{
		// singleton
		private:
			static Log* m_instance;
			static std::once_flag m_initFlag;

		public:
			static Log* getInstance()
			{
				std::call_once(m_initFlag, []() { m_instance = new Log(); });
				return m_instance;
			}

		// function
		private:
			std::shared_ptr<spdlog::logger>	m_loggerConsole;

		public:
			Log()
			{
				m_loggerConsole = spdlog::stdout_color_mt("console");
			}

			template <typename... Args>
			void info(const char* message, const Args&... args)
			{
				m_loggerConsole->info(message, args...);
			}

			template <typename... Args>
			void warn(const char* message, const Args&... args)
			{
				m_loggerConsole->warn(message, args...);
			}

			template <typename... Args>
			void error(const char* message, const Args&... args)
			{
				m_loggerConsole->error(message, args...);
			}
		};

		// macro
		template <typename... Args>
		void Info(const char* message, const Args&... args)
		{
			Log::getInstance()->info(message, args...);
		}
		template <typename... Args>
		void warn(const char* message, const Args&... args)
		{
			Log::getInstance()->warn(message, args...);
		}
		template <typename... Args>
		void error(const char* message, const Args&... args)
		{
			Log::getInstance()->error(message, args...);
		}
	}
}

// define macro
#define SECA_CONSOLE_INFO(message, ...)\
	seca::util::Info(message, ##__VA_ARGS__)

#define SECA_CONSOLE_WARN(message, ...)\
	seca::util::warn(message, ##__VA_ARGS__)

#define SECA_CONSOLE_ERROR(message, ...)\
	seca::util::error(message, ##__VA_ARGS__)

#endif // SECA_UTIL_LOG_H_