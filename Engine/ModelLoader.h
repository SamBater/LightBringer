#include "Triangle.h"
#include <vector>
#include "YLBFileSystem.h"
#include "Model.h"

static void CalcTangents(ylb:: Model* model);

namespace ylb {
	Model* LoadModel(const char* modelPath);
}