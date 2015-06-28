#include "WorkUnit.h"
#include "Utils.h"

WorkUnit::WorkUnit() {
    _data_dir = getExecutableDir() + "/../share/fahbench/workunits";
}
