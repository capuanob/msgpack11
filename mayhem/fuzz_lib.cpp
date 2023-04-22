#include "fuzzer/FuzzedDataProvider.h"
#include "msgpack11.hpp"


msgpack11::MsgPack::array make_array(FuzzedDataProvider& fdp) {
    msgpack11::MsgPack::array result;
    std::size_t sz = fdp.ConsumeIntegralInRange(0, 100);
    result.reserve(sz);
    for (std::size_t i = 0; i < sz; ++i) {
        result.emplace_back(fdp.ConsumeIntegral<int>());
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput(uint8_t *data, size_t size) {
    FuzzedDataProvider fdp(data, size);

    std::vector<std::pair<std::string, msgpack11::MsgPack>> vec;
    std::size_t sz = fdp.ConsumeIntegralInRange(0, 100);
    vec.reserve(sz);
    for (std::size_t i = 0; i < sz; i++) {
        vec.emplace_back(fdp.ConsumeRandomLengthString(), make_array(fdp));
    }
    msgpack11::MsgPack obj = msgpack11::MsgPack::object{vec.begin(), vec.end()};

    std::string dumped_obj = obj.dump();
    std::string err;
    auto parsed = msgpack11::MsgPack::parse(dumped_obj, err);

    if (parsed != obj) {
        abort();
    }

    return 0;
}
