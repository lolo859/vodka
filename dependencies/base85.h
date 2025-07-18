#include <stdint.h>
#include <cstring>
#include <array>
namespace base85 {
    constexpr char Z85_CHARS[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        ".~:+=^!/*?&<>()_|{}@%$#";

    constexpr uint32_t POW85[5]={
        52200625, // 85^4
        614125,   // 85^3
        7225,     // 85^2
        85,       // 85^1
        1         // 85^0
    };

    // Générée statiquement pour accélérer les décodages
    constexpr int8_t make_decode_map(unsigned char c) {
        for (int i=0; i < 85; ++i)
            if (Z85_CHARS[i] == c)
                return static_cast<int8_t>(i);
        return -1;
    }

    constexpr int8_t build_map_byte(unsigned char c) {
        return make_decode_map(c);
    }

    // Génère une table fixe au lieu de faire la recherche à chaque appel
    constexpr auto build_decode_table() {
        std::array<int8_t, 256> arr={};
        for (size_t i=0; i < 256; ++i) {
            arr[i]=build_map_byte(static_cast<unsigned char>(i));
        }
        return arr;
    }

    constexpr auto Z85_DECODE_MAP_ARRAY=build_decode_table();

    inline void uint32_to_b85(uint32_t value, char* out) {
        for (int i=0; i < 5; ++i) {
            uint32_t div=POW85[i];
            uint32_t q=value / div;
            out[i]=Z85_CHARS[q];
            value -= q * div;
        }
        out[5]='\0';
    }

    inline void uint64_to_b85(uint64_t value, char* out) {
        uint32_to_b85(static_cast<uint32_t>(value >> 32), out);
        uint32_to_b85(static_cast<uint32_t>(value), out + 5);
        out[10]='\0';
    }

    inline int b85_to_uint32_internal(const char* in, uint32_t& value) {
        uint32_t result=0;
        for (int i=0; i < 5; ++i) {
            int8_t v=Z85_DECODE_MAP_ARRAY[static_cast<unsigned char>(in[i])];
            if (v < 0) {
                value=0;
                return 1;
            }
            result += static_cast<uint32_t>(v) * POW85[i];
        }
        value=result;
        return 0;
    }

    inline int b85_to_uint32(const char* in, uint32_t& out) {
        return b85_to_uint32_internal(in, out);
    }

    inline int b85_to_uint64(const char* in, uint64_t& out) {
        uint32_t high, low;
        int error=0;
        error |= b85_to_uint32_internal(in, high);
        error |= b85_to_uint32_internal(in + 5, low);
        out=(static_cast<uint64_t>(high) << 32) | low;
        return error;
    }
}
