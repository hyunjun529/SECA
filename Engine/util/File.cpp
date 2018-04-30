#include "Log.h"

// define singleton
kata::util::Log* kata::util::Log::m_instance;
std::once_flag kata::util::Log::m_initFlag;