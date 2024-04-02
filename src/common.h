#ifndef __COMMON_H_
#define __COMMON_H_

#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#define LOG_BUFSIZE 1 << 10
#define CLASS_PTR(className) \
class className; \
using className ## UPtr = std::unique_ptr<className>; \
using className ## Ptr = std::shared_ptr<className>; \
using className ## WPtr = std::weak_ptr<className>;

std::optional<std::string> LoadTextFile(const std::string& filename);

#endif // __COMMON_H_