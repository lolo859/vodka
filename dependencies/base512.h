#ifndef BASE512_H
#define BASE512_H
#include <cstdint>
#include <iostream>
#include <bitset>
#include <cstring>
#include <string>
#include <string_view>
#include "utf8.h"
#include "trie.h"
#define BASE512_INPUT_OUTPUT_MIN_SIZE 17
using namespace trie;
namespace base512 {
    static const char* base512_alphabet[512]={"!","#","$","%","&","(",")","*","+",",",".","/","0","1","2","3","4","5","6","7","8","9",":",";","<",">","=","?","@","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","\\","^","_","`","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","{","}","|","~","\"","¡","£","¤","¥","¦","§","¬","¯","°","²","±","µ","´","·","¿","¼","½","¾","À","Á","Â","Ã","Ä","Å","Æ","Ç","È","É","Ê","Ë","Ì","Í","Î","Ï","Ð","Ñ","Ò","Ó","Ô","Õ","Ö","×","Ø","Ù","Ú","Û","Ü","Ý","ß","à","á","â","ã","ä","å","æ","ç","è","é","ê","ë","ì","í","î","ï","ð","ñ","ò","ó","ô","õ","ö","÷","ø","ù","ú","û","ü","ý","þ","ÿ","Ā","ā","Ă","ă","Ą","ą","Ć","ć","Ĉ","ĉ","Ċ","ċ","Č","č","Ď","ď","Đ","đ","Ē","ē","Ĕ","ĕ","Ė","ė","Ę","ę","Ě","ě","Ĝ","ĝ","Ğ","ğ","Ġ","ġ","Ģ","ģ","Ĥ","ĥ","Ħ","ħ","Ĩ","ĩ","Ī","ī","Ĭ","ĭ","Į","į","ı","Ĵ","ĵ","Ķ","ķ","ĸ","Ĺ","ĺ","Ļ","ļ","Ľ","ľ","Ŀ","ŀ","Ł","ł","Ń","ń","Ņ","ņ","Ň","ň","ŉ","Ŋ","ŋ","Ō","ō","Ŏ","ŏ","Ő","ő","Œ","œ","Ŕ","ŕ","Ŗ","ŗ","Ř","ř","Ś","ś","Ŝ","ŝ","Ş","ş","Š","š","Ţ","ţ","Ť","ť","Ŧ","ŧ","Ũ","ũ","Ū","ū","Ŭ","ŭ","Ů","ů","Ű","ű","Ų","ų","Ŵ","ŵ","Ŷ","ŷ","Ÿ","Ź","ź","Ż","ż","Ž","ž","ſ","ƀ","Ɓ","Ƃ","ƃ","Ƅ","ƅ","Ɔ","Ƈ","ƈ","Ɖ","Ɗ","Ƌ","ƌ","ƍ","Ǝ","Ə","Ɛ","Ƒ","ƒ","Ɠ","Ɣ","ƕ","Ɨ","Ƙ","ƙ","ƚ","ƛ","Ɯ","Ɲ","ƞ","Ɵ","Ơ","ơ","ƣ","Ƥ","ƥ","Ʀ","Ƨ","ƨ","Ʃ","ƪ","ƫ","Ƭ","ƭ","Ʈ","Ư","ư","Ʊ","Ʋ","Ƴ","ƴ","Ƶ","ƶ","Ʒ","Ƹ","ƺ","ƻ","Ƽ","ƽ","ƾ","ƿ","ǁ","ǂ","Ǎ","ǎ","Ǐ","ǐ","Ǒ","ǒ","Ǔ","ǔ","Ǖ","ǖ","Ǘ","ǘ","Ǚ","ǚ","Ǜ","ǜ","ǝ","Ǟ","ǟ","Ǡ","ǡ","Ǣ","ǣ","Ǥ","ǥ","Ǧ","ǧ","Ǩ","ǩ","Ǫ","ǫ","Ǭ","ǭ","Ǯ","ǯ","ǰ","Ǵ","ǵ","Ƕ","Ƿ","Ǹ","ǹ","Ǻ","ǻ","Ǽ","ǽ","Ǿ","ǿ","Ȁ","ȁ","Ȃ","ȃ","Ȅ","ȅ","Ȇ","ȇ","Ȉ","ȉ","Ȋ","ȋ","Ȍ","ȍ","Ȏ","ȏ","Ȑ","ȑ","Ȓ","ȓ","Ȕ","ȕ","Ȗ","ȗ","Ș","ș","Ț","ț","Ȝ","ȝ","Ȟ","ȟ","Ƞ","ȡ","Ȣ","Ȥ","ȥ","Ȧ","ȧ","Ȩ","ȩ","Ȫ","ȫ","Ȭ","ȭ","Ȯ","ȯ","Ȱ","ȱ","Ȳ","ȳ","ȴ","ȵ","ȶ","ȷ","ȸ","ȹ","Ⱥ","Ȼ","ȼ","Ƚ","Ⱦ","ȿ","ɀ","Ɂ","ɂ","Ƀ","Ʉ","Ʌ","Ɇ","ɇ","Ɉ","ɉ","Ɋ","ɋ","Ɍ","ɍ","Ɏ","ɏ","ɐ","ɒ","ɓ","ɔ","ɕ","ɖ","ɗ","ə","ɚ","ɛ","ɜ","ɝ","ɞ","ɟ","ɠ","ɡ","ɢ","ɣ","ɤ","ɥ","ɦ","ɧ","ɮ","ɯ","ɰ","ɱ"};
    static const int base512_values_bytes[512]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
    static DecodeTrie decode_trie;
    size_t get_utf8_char_byte_length(const char* c) {
        if (c==NULL || *c=='\0') {
            return 0;
        }
        unsigned char byte=(unsigned char)*c;
        if ((byte & 0x80)==0) {
            return 1;
        } else if ((byte & 0xE0)==0xC0) {
            return 2;
        } else if ((byte & 0xF0)==0xE0) {
            return 3;
        } else if ((byte & 0xF8)==0xF0) {
            return 4;
        }
        return 0;
    }
    inline size_t encode_base512(uint64_t input,char* output,size_t output_size) {
        if (output_size==0) {return 0;}
        uint8_t buf[9]={0};
        for (int i=0;i<8;++i) {
            buf[i]=static_cast<uint8_t>(input>>(8*(7-i)));
        }
        size_t out_off=0;
        for (int seg=0;seg<8;++seg) {
            const int bit_index=seg*9;
            const int byte_index=bit_index>>3;
            const int bit_offset=bit_index & 7;
            uint16_t window=(static_cast<uint16_t>(buf[byte_index])<<8) | static_cast<uint16_t>(buf[byte_index+1]);
            uint16_t value=(window>>(16-9-bit_offset)) & 0x1FF;
            const char* sym=base512_alphabet[value];
            size_t sym_len=base512_values_bytes[value];
            if (out_off+sym_len>=output_size) {
                output[0]='\0';
                return 0;
            }
            memcpy(output+out_off,sym,sym_len);
            out_off+=sym_len;
        }
        output[out_off]='\0';
        return out_off;
    }
    inline uint64_t decode_base512(const char* input,size_t input_size,size_t* consumed_bytes_out) {
        if (!input || input_size==0 || !consumed_bytes_out) {
            *consumed_bytes_out=0;
            return 0;
        }
        const char* p=input;
        const char* end=input+input_size;
        unsigned __int128 accumulated_bits=0;
        size_t bytes_read_for_this_block=0;
        for (int i=0;i<8;++i) {
            std::pair<uint16_t,size_t> result=decode_trie.find_symbol(p,end-p);
            uint16_t symbol_value=result.first;
            size_t current_char_byte_len=result.second;
            if (current_char_byte_len==0) {
                *consumed_bytes_out=0;
                return 0;
            }
            accumulated_bits=(accumulated_bits<<9) | symbol_value;
            p+=current_char_byte_len;
            bytes_read_for_this_block+=current_char_byte_len;
        }
        *consumed_bytes_out=bytes_read_for_this_block;
        uint64_t result_val=static_cast<uint64_t>(accumulated_bits>>8);
        return result_val;
    }
    inline int decode_mainblock_full(const char* input_encoded,size_t input_encoded_len,uint8_t* output_decoded) {
        if (!input_encoded || !output_decoded) {
            return 1;
        }
        const char* current_ptr=input_encoded;
        for (int i=0;i<8;++i) {
            size_t consumed_segment_bytes=0;
            uint64_t decoded_val=decode_base512(current_ptr,input_encoded_len-(current_ptr-input_encoded),&consumed_segment_bytes);
            if (consumed_segment_bytes==0) {
                return 1;
            }
            memcpy(output_decoded+(i*sizeof(uint64_t)),&decoded_val,sizeof(uint64_t));
            current_ptr+=consumed_segment_bytes;
        }
        if (current_ptr-input_encoded!=input_encoded_len) {
            return 1;
        }
        return 0;
    }
    inline uint64_t decode_base512(const char* input,size_t input_size) {
        size_t temp_consumed_bytes;
        return decode_base512(input,input_size,&temp_consumed_bytes);
    }
}
#endif