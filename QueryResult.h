#pragma once

#include <memory>

namespace dc {

template<typename TState>
struct QueryResult {
    enum class Status {
        Ready,
        NotReady,
        UnknownIndex,
    };

    Status status;
    std::shared_ptr<const TState> snapshot; // non-null only when status == Ready
};

} // namespace dc
