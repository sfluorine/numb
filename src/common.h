#pragma once

#define TRY(opt)              \
    ({                        \
        auto&& __ref = (opt); \
        if (!__ref) {         \
            return {};        \
        }                     \
        *__ref;               \
    })
