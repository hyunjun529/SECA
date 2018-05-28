#ifndef _STDAFX_H_
#define _STDAFX_H_

#define _SCL_SECURE_NO_WARNINGS
#define _SECURE_SCL 0

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "util/Logger.h"
// define singleton
seca::util::Log* seca::util::Log::m_instance;
std::once_flag seca::util::Log::m_initFlag;

#endif