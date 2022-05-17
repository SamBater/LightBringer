#include "Triangle.h"
#include <vector>
#include "YLBFileSystem.h"
namespace ylb {
	std::vector<ylb::Triangle>* LoadObj(const char* modelPath);
}