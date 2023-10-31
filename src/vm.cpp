#include "vm.h"

uint8_t NumbVm::fetch()
{
    return bytecode[++m_pc];
}

int64_t NumbVm::read_qword()
{
    int64_t qword = 0;

    for (int i = 0; i < sizeof(qword); i++) {
        qword |= static_cast<int64_t>(fetch()) << (i * 8);
    }

    return qword;
}

int32_t NumbVm::read_dword()
{
    int32_t dword = 0;

    for (int i = 0; i < sizeof(dword); i++) {
        dword |= static_cast<int32_t>(fetch()) << (i * 8);
    }

    return dword;
}
