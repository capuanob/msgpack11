#include "fuzzer/FuzzedDataProvider.h"
#include "msgpack11.hpp"


extern "C" int LLVMFuzzerTestOneInput(uint8_t *data, size_t size) {
    FuzzedDataProvider fdp{data, size};

    std::string orig_string = fdp.ConsumeRemainingBytesAsString();
    std::string err;
    auto deserialized = msgpack11::MsgPack::parse(orig_string, err);
    std::string serialized = deserialized.dump();
    return 0;
}
