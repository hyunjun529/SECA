#include "Log.h"

// define singleton
seca::util::Log* seca::util::Log::m_instance;
std::once_flag seca::util::Log::m_initFlag;