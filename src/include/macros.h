#ifndef MACROS_H
#define MACROS_H

#define DISALLOW_COPY(type_name)            \
    type_name(const type_name &) = delete;  \
    type_name& operator=(const type_name &) = delete;

#define DISALLOW_MOVE(type_name)            \
    type_name(type_name &&) = delete;  \
    type_name& operator=(type_name &&) = delete;

#endif //MACROS_H
