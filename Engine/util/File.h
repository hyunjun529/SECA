#ifndef KATA_UTIL_FILE_H_
#define KATA_UTIL_FILE_H_

#include <iostream>
#include <memory>
#include <thread>
#include <mutex>


namespace kata
{
	namespace util{
		class File
		{
		// singleton
		private:
			static File* m_instance;
			static std::once_flag m_initFlag;

		public:
			static File* getInstance()
			{
				std::call_once(m_initFlag, []() { m_instance = new File(); });
				return m_instance;
			}

		// function
		private:

		public:
			File()
			{
			}
		};
	}
}

#endif // KATA_UTIL_FILE_H_