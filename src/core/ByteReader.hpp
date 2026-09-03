#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <stdexcept>

namespace gb {

// Little Endian
inline uint32_t readU32(std::span<const std::byte> data, size_t offset) {
    if (offset > data.size() || data.size() - offset < sizeof(uint32_t)) {
        throw std::out_of_range("readU32: offset out of bounds");
    }

    return std::to_integer<uint32_t>(data[offset])               //
           | std::to_integer<uint32_t>(data[offset + 1]) << 8    //
           | std::to_integer<uint32_t>(data[offset + 2]) << 16   //
           | std::to_integer<uint32_t>(data[offset + 3]) << 24;  //
}

inline int32_t readI32(std::span<const std::byte> data, std::size_t offset) {
    return static_cast<std::int32_t>(readU32(data, offset));
}

inline uint16_t readU16(std::span<const std::byte> data, std::size_t offset) {
    if (offset > data.size() || data.size() - offset < sizeof(uint16_t)) {
        throw std::out_of_range("readU16: offset out of bounds");
    }

    return std::to_integer<uint16_t>(data[offset])              //
           | std::to_integer<uint16_t>(data[offset + 1]) << 8;  //
}

inline int16_t readI16(std::span<const std::byte> data, std::size_t offset) {
    return static_cast<std::int16_t>(readU16(data, offset));
}

inline uint16_t readU8(std::span<const std::byte> data, std::size_t offset) {
    if (offset > data.size() || data.size() - offset < sizeof(uint8_t)) {
        throw std::out_of_range("readU8: offset out of bounds");
    }

    return std::to_integer<uint8_t>(data[offset]);
}

template <size_t N>
inline void readArray(std::span<const std::byte> data, std::size_t offset, uint8_t (&dest)[N]) {
    if (offset > data.size() || data.size() - offset < N) {
        throw std::out_of_range("readArray: offset out of bounds");
    }

    std::memcpy(dest, data.data() + offset, N);
}

// Big Endian
inline uint32_t readU32BE(std::span<const std::byte> data, size_t offset) {
    if (offset > data.size() || data.size() - offset < sizeof(uint32_t)) {
        throw std::out_of_range("readU32BE: offset out of bounds");
    }

    return std::to_integer<uint32_t>(data[offset]) << 24        //
           | std::to_integer<uint32_t>(data[offset + 1]) << 16  //
           | std::to_integer<uint32_t>(data[offset + 2]) << 8   //
           | std::to_integer<uint32_t>(data[offset + 3]);       //
}

inline int32_t readI32BE(std::span<const std::byte> data, std::size_t offset) {
    return static_cast<std::int32_t>(readU32BE(data, offset));
}

inline uint16_t readU16BE(std::span<const std::byte> data, std::size_t offset) {
    if (offset > data.size() || data.size() - offset < sizeof(uint16_t)) {
        throw std::out_of_range("readU16BE: offset out of bounds");
    }

    return std::to_integer<uint16_t>(data[offset]) << 8    //
           | std::to_integer<uint16_t>(data[offset + 1]);  //
}

inline int16_t readI16BE(std::span<const std::byte> data, std::size_t offset) {
    return static_cast<std::int16_t>(readU16BE(data, offset));
}

}  // namespace gb