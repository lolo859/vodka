#include <stdint.h>
namespace base85 {
    constexpr char Z85_CHARS[]=
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        ".~:+=^!/*?&<>()_|{}@%$#";
    inline void uint32_to_b85(uint32_t value,char* out) {
        constexpr uint32_t d1=52200625;
        constexpr uint32_t d2=614125;
        constexpr uint32_t d3=7225;  
        constexpr uint32_t d4=85;    
        uint32_t q1=value/d1;
        value-=q1*d1;
        uint32_t q2=value/d2;
        value-=q2*d2;
        uint32_t q3=value/d3;
        value-=q3*d3;
        uint32_t q4=value/d4;
        value-=q4*d4;
        out[0]=Z85_CHARS[q1];
        out[1]=Z85_CHARS[q2];
        out[2]=Z85_CHARS[q3];
        out[3]=Z85_CHARS[q4];
        out[4]=Z85_CHARS[value];
        out[5]='\0';
    }
    inline void uint64_to_b85(uint64_t value,char* out) {
        uint32_to_b85(static_cast<uint32_t>(value>>32),out);
        uint32_to_b85(static_cast<uint32_t>(value & 0xFFFFFFFF),out+5);
        out[10]='\0';
    }
}