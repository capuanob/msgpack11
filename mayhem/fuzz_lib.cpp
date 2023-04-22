#include "fuzzer/FuzzedDataProvider.h"
#include "msgpack11.hpp"


extern "C" int LLVMFuzzerTestOneInput(uint8_t *data, size_t size) {
    FuzzedDataProvider fdp{data, size};

    std::string orig_string = fdp.ConsumeRemainingBytesAsString();
    std::string err;
    auto objs = msgpack11::MsgPack::parse_multi(orig_string, err);
    for (const auto& obj : objs) {
        obj.dump();
    }
    return 0;
}
