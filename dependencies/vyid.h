//* VYID is under the MIT license.
//* It depends on XXHash (BSD 2-Clause License) and XoshiroCpp (MIT license).
//* Please ensure you comply with their respective licenses.
//* Define VYID_NO_OS_DEPENDENCIES to disable OS functionnalities, such as string outputing and automatic seed generation
#ifndef VYID_H
#define VYID_H
#include <cstdlib>
#include <cstring>
#ifndef VYID_NO_OS_DEPENDENCIES
#include <string>
#include <stdio.h>
#endif
#include <bitset>
#include <random>
#include "XoshiroCpp.hpp"
#include "chacha20.h"
#include "blake3.h"
extern "C" {
    #include "xxhash.h"
}
#include "base85.h"
#include "base512.h"
#define VYID1_MIN_BUFFER_SIZE 62
#define VYID2_MIN_BUFFER_SIZE 182
#define VYID2_MAX_PAYLOAD_SIZE 32
namespace vyid {
    namespace v1 {    
        //* A struct container for random values 
        struct random_values {
            uint64_t rand1;
            uint64_t rand2;
            uint64_t rand3;
        };
        //* Function to generate a VYIDv1, made for single use
        //* gen : the random instance of Xoshiro256StarStar, seeded or no
        //* rand : the random_values struct already created
        //* Return the string containing the VYID
        #ifndef VYID_NO_OS_DEPENDENCIES
        std::string generate_vyid_string_raw(XoshiroCpp::Xoshiro256StarStar& gen,random_values& rand) {
            rand.rand1=gen();
            rand.rand2=gen();
            rand.rand3=gen();
            uint64_t seed=rand.rand1^rand.rand2^rand.rand3;
            struct pair64{uint64_t a,b;};
            struct triplepair64{pair64 a,b,c;};
            triplepair64 tempstruct {{rand.rand2,rand.rand3},{rand.rand1,rand.rand3},{rand.rand1,rand.rand2}};
            auto res=XXH3_128bits_withSeed(&tempstruct,sizeof(tempstruct),seed);
            uint64_t sign1h;
            uint64_t sign2h;
            uint64_t sign3h;
            const uint8_t* ptrres=reinterpret_cast<const uint8_t*>(&res);
            memcpy(&sign1h,&ptrres,8);
            memcpy(&sign2h,&ptrres+4,8);
            memcpy(&sign3h,&ptrres+8,8);
            uint64_t lenc1=rand.rand1^sign1h;
            uint64_t lenc2=rand.rand2^sign2h;
            uint64_t lenc3=rand.rand3^sign3h;
            uint32_t enc1=lenc1>>32;
            uint32_t enc2=lenc2>>32;
            uint32_t enc3=lenc3>>32;
            struct triple64{uint64_t a,b,c;};
            triple64 tempst={enc1,enc2,enc3};
            auto sumtotal=XXH3_64bits_withSeed(&tempst,sizeof(tempst),seed);
            uint32_t trunc1=static_cast<uint32_t>(sumtotal);
            uint32_t trunc2=static_cast<uint32_t>(sumtotal>>32);
            alignas(128) char output[70];
            alignas(8) char grp[6];
            alignas(8) char hash[6];
            alignas(8) char enchar1[6];
            alignas(8) char enchar2[6];
            alignas(8) char enchar3[6];
            alignas(16) char rand1[11];
            alignas(16) char rand2[11];
            alignas(16) char rand3[11];
            base85::uint32_to_b85(trunc1,grp);
            base85::uint32_to_b85(enc1,enchar1);
            base85::uint32_to_b85(enc2,enchar2);
            base85::uint32_to_b85(enc3,enchar3);
            base85::uint64_to_b85(rand.rand1,rand1);
            base85::uint64_to_b85(rand.rand2,rand2);
            base85::uint64_to_b85(rand.rand3,rand3);
            base85::uint32_to_b85(trunc2,hash);
            int offset=0;
            output[offset++]='[';
            memcpy(output+offset,grp,5);
            offset+=5;
            output[offset++]='-';
            memcpy(output+offset,enchar1,5);
            offset+=5;
            memcpy(output+offset,rand1,10);
            offset+=10;
            output[offset++]='-';
            memcpy(output+offset,enchar2,5);
            offset+=5;
            memcpy(output+offset,rand2,10);
            offset+=10;
            output[offset++]='-';
            memcpy(output+offset,enchar3,5);
            offset+=5;
            memcpy(output+offset,rand3,10);
            offset+=10;
            output[offset++]='-';
            memcpy(output+offset,hash,5);
            offset+=5;
            output[offset++]=']';
            output[offset]='\0';
            return std::string(output,output+61);
        }
        #endif
        //* Function to generate a VYIDv1, made for single use
        //* gen : the random instance of Xoshiro256StarStar, seeded or no
        //* rand : the random_values struct already created
        //* output : the const char* to put the VYID, must be at least 62 in size
        //* output_size : the size of the output buffer
        //* Return a int : 0 if sucess, 1 if failure
        int generate_vyid_char_raw(XoshiroCpp::Xoshiro256StarStar& gen,random_values& rand,char* output,size_t output_size) {
            if (__builtin_expect(output_size<VYID1_MIN_BUFFER_SIZE,0)) {
                if (__builtin_expect(output_size>0,0)) {
                    output[0]='\0';
                }
                return 1;
            }
            rand.rand1=gen();
            rand.rand2=gen();
            rand.rand3=gen();
            uint64_t seed=rand.rand1^rand.rand2^rand.rand3;
            struct pair64{uint64_t a,b;};
            struct triplepair64{pair64 a,b,c;};
            triplepair64 tempstruct {{rand.rand2,rand.rand3},{rand.rand1,rand.rand3},{rand.rand1,rand.rand2}};
            auto res=XXH3_128bits_withSeed(&tempstruct,sizeof(tempstruct),seed);
            uint64_t sign1h;
            uint64_t sign2h;
            uint64_t sign3h;
            const uint8_t* ptrres=reinterpret_cast<const uint8_t*>(&res);
            memcpy(&sign1h,&ptrres,8);
            memcpy(&sign2h,&ptrres+4,8);
            memcpy(&sign3h,&ptrres+8,8);
            uint64_t lenc1=rand.rand1^sign1h;
            uint64_t lenc2=rand.rand2^sign2h;
            uint64_t lenc3=rand.rand3^sign3h;
            uint32_t enc1=lenc1>>32;
            uint32_t enc2=lenc2>>32;
            uint32_t enc3=lenc3>>32;
            struct triple64{uint64_t a,b,c;};
            triple64 tempst={enc1,enc2,enc3};
            auto sumtotal=XXH3_64bits_withSeed(&tempst,sizeof(tempst),seed);
            uint32_t trunc1=static_cast<uint32_t>(sumtotal);
            uint32_t trunc2=static_cast<uint32_t>(sumtotal>>32);
            alignas(8) char grp[6];
            alignas(8) char hash[6];
            alignas(8) char enchar1[6];
            alignas(8) char enchar2[6];
            alignas(8) char enchar3[6];
            alignas(16) char rand1[11];
            alignas(16) char rand2[11];
            alignas(16) char rand3[11];
            base85::uint32_to_b85(trunc1,grp);
            base85::uint32_to_b85(enc1,enchar1);
            base85::uint32_to_b85(enc2,enchar2);
            base85::uint32_to_b85(enc3,enchar3);
            base85::uint64_to_b85(rand.rand1,rand1);
            base85::uint64_to_b85(rand.rand2,rand2);
            base85::uint64_to_b85(rand.rand3,rand3);
            base85::uint32_to_b85(trunc2,hash);
            int offset=0;
            output[offset++]='[';
            memcpy(output+offset,grp,5);
            offset+=5;
            output[offset++]='-';
            memcpy(output+offset,enchar1,5);
            offset+=5;
            memcpy(output+offset,rand1,10);
            offset+=10;
            output[offset++]='-';
            memcpy(output+offset,enchar2,5);
            offset+=5;
            memcpy(output+offset,rand2,10);
            offset+=10;
            output[offset++]='-';
            memcpy(output+offset,enchar3,5);
            offset+=5;
            memcpy(output+offset,rand3,10);
            offset+=10;
            output[offset++]='-';
            memcpy(output+offset,hash,5);
            offset+=5;
            output[offset++]=']';
            output[offset]='\0';
            return 0;
        }
        //* Function to generate a VYIDv1
        //* Use this fonction if you have access to random seed provider
        //* Automatically recycle random instance for bulk VYID generation
        //* Return the string containing the VYID
        #ifndef VYID_NO_OS_DEPENDENCIES
        std::string generate_vyid_string() {
            static XoshiroCpp::Xoshiro256StarStar ran=[]() {std::random_device rd;
                std::mt19937_64 seeder(rd());
                std::array<uint64_t,4> seed_data={seeder(),seeder(),seeder(),seeder()};
                return XoshiroCpp::Xoshiro256StarStar(seed_data);
            }();
            static random_values rands;
            return generate_vyid_string_raw(ran,rands);
        }
        //* Function to generate a VYIDv1 with a specific seed
        //* Use this function if you don't have a access to a random seed provider
        //* That function is the one to use for bare metal usage if you want to get the result as a string
        //* Automatically recycle random instance for bulk VYID generation
        //* seed : a array of 4 uint64_t
        //* Return the string containing the VYID
        std::string generate_vyid_string_seed(std::array<uint64_t,4> seed) {
            static XoshiroCpp::Xoshiro256StarStar ran(seed);
            static random_values rands;
            return generate_vyid_string_raw(ran,rands);
        }
        //* Function to generate a VYIDv1
        //* Use this fonction if you have access to random seed provider
        //* Automatically recycle random instance for bulk VYID generation
        //* output : the const char* to put the VYID, must be at least 62 in size
        //* output_size : the size of the output buffer
        //* Return a int : 0 if sucess, 1 if failure
        int generate_vyid_char(char* output,size_t output_size) {
            static XoshiroCpp::Xoshiro256StarStar ran=[]() {std::random_device rd;
                std::mt19937_64 seeder(rd());
                std::array<uint64_t,4> seed_data={seeder(),seeder(),seeder(),seeder()};
                return XoshiroCpp::Xoshiro256StarStar(seed_data);
            }();
            static random_values rands;
            return generate_vyid_char_raw(ran,rands,output,output_size);
        }
        #endif
        //* Function to generate a VYIDv1 with a specific seed
        //* Use this function if you don't have a access to a random seed provider
        //* That function is the one to use for bare metal usage if you want to get the result as a char
        //* Automatically recycle random instance for bulk VYID generation
        //* seed : a array of 4 uint64_t
        //* output : the const char* to put the VYID, must be at least 62 in size
        //* output_size : the size of the output buffer
        //* Return a int : 0 if sucess, 1 if failure
        int generate_vyid_char_seed(std::array<uint64_t,4> seed,char* output,size_t output_size) {
            static XoshiroCpp::Xoshiro256StarStar ran(seed);
            static random_values rands;
            return generate_vyid_char_raw(ran,rands,output,output_size);
        }
        //* That function is the one to verify any VYIDv1 and detecting eventual corruption
        //* This function fully work in a bare metal environnement
        //* vyid : the char* which contain the VYID to verify
        //* vyid_size : the size of the char that contain the VYID
        //* Return a boolean : true if the VYID pass all tests, false if one of the test failed
        bool check_vyid(const char* vyid,size_t vyid_size) {
            if (__builtin_expect(vyid==nullptr,0)) {
                return false;
            }
            if (__builtin_expect(vyid_size<VYID1_MIN_BUFFER_SIZE,0)) {
                return false;
            }
            if (__builtin_expect(vyid[0]!='[' || vyid[VYID1_MIN_BUFFER_SIZE-2]!=']' || vyid[6]!='-' || vyid[22]!='-' || vyid[38]!='-' || vyid[54]!='-',0)) {
                return false;
            }
            alignas(8) char grp[6];
            alignas(8) char hash[6];
            alignas(8) char enchar1[6];
            alignas(8) char enchar2[6];
            alignas(8) char enchar3[6];
            alignas(16) char rand1[11];
            alignas(16) char rand2[11];
            alignas(16) char rand3[11];
            std::memcpy(grp,vyid+1,5);
            grp[5]='\0';
            std::memcpy(enchar1,vyid+7,5);
            enchar1[5]='\0';
            std::memcpy(rand1,vyid+12,10);
            rand1[10]='\0';
            std::memcpy(enchar2,vyid+23,5);
            enchar2[5]='\0';
            std::memcpy(rand2,vyid+28,10);
            rand2[10]='\0';
            std::memcpy(enchar3,vyid+39,5);
            enchar3[5]='\0';
            std::memcpy(rand3,vyid+44,10);
            rand3[10]='\0';
            std::memcpy(hash,vyid+55,5);
            hash[5]='\0';
            uint32_t igrp;
            uint32_t ihash;
            uint32_t encint1;
            uint32_t encint2;
            uint32_t encint3;
            uint64_t irand1;
            uint64_t irand2;
            uint64_t irand3;
            if (__builtin_expect(base85::b85_to_uint32(grp,igrp)!=0,0)) return false;
            if (__builtin_expect(base85::b85_to_uint32(hash,ihash)!=0,0)) return false;
            if (__builtin_expect(base85::b85_to_uint32(enchar1,encint1)!=0,0)) return false;
            if (__builtin_expect(base85::b85_to_uint32(enchar2,encint2)!=0,0)) return false;
            if (__builtin_expect(base85::b85_to_uint32(enchar3,encint3)!=0,0)) return false;
            if (__builtin_expect(base85::b85_to_uint64(rand1,irand1)!=0,0)) return false;
            if (__builtin_expect(base85::b85_to_uint64(rand2,irand2)!=0,0)) return false;
            if (__builtin_expect(base85::b85_to_uint64(rand3,irand3)!=0,0)) return false;
            struct pair64{uint64_t a,b;};
            struct triplepair64{pair64 a,b,c;};
            uint64_t seed=irand1^irand2^irand3;
            triplepair64 tempstruct {{irand2,irand3},{irand1,irand3},{irand1,irand2}};
            auto res=XXH3_128bits_withSeed(&tempstruct,sizeof(tempstruct),seed);
            uint64_t sign1h;
            uint64_t sign2h;
            uint64_t sign3h;
            const uint8_t* ptrres=reinterpret_cast<const uint8_t*>(&res);
            memcpy(&sign1h,&ptrres,8);
            memcpy(&sign2h,&ptrres+4,8);
            memcpy(&sign3h,&ptrres+8,8);
            uint64_t lenc1=irand1^sign1h;
            uint64_t lenc2=irand2^sign2h;
            uint64_t lenc3=irand3^sign3h;
            uint32_t enc1=lenc1>>32;
            uint32_t enc2=lenc2>>32;
            uint32_t enc3=lenc3>>32;
            if (__builtin_expect(encint1!=enc1,0)) {
                return false;
            }
            if (__builtin_expect(encint2!=enc2,0)) {
                return false;
            }
            if (__builtin_expect(encint3!=enc3,0)) {
                return false;
            }
            struct triple64{uint64_t a,b,c;};
            triple64 tempst={enc1,enc2,enc3};
            auto sumtotal=XXH3_64bits_withSeed(&tempst,sizeof(tempst),seed);
            uint32_t trunc1=static_cast<uint32_t>(sumtotal);
            uint32_t trunc2=static_cast<uint32_t>(sumtotal>>32);
            if (__builtin_expect(trunc1!=igrp,0)) {
                return false;
            }
            if (__builtin_expect(trunc2!=ihash,0)) {
                return false;
            }
            return true;
        }
    }
    namespace v2 {
        //* The number identifying the VYID inside the session
        static thread_local uint64_t amount=0;
        //* The session id, generate once per thread
        static thread_local uint64_t session_id=((uint64_t)csprng_get_next_uint32()<<32) | csprng_get_next_uint32();
        //* 512 bits of entropy
        struct random_values {
            uint64_t rand1;
            uint64_t rand2;
            uint64_t rand3;
            uint64_t rand4;
            uint64_t rand5;
            uint64_t rand6;
            uint64_t rand7;
            uint64_t rand8;
        };
        //* The buffer that hold 2048 blocks of 512 bits of entropy
        struct buffer {
            random_values rands[2048];
            size_t actual_offset=2048;
        };
        //* The function to initialize the CSPRNG
        //* MANDATORY TO USE BEFORE GENERATING ANYTHING
        //* If you're are under Linux and don't want to provide a seed, use nullptr as seed
        //* Or give a seed undex the format uint8_t* seed[16]
        //* Return an integer : 0 if sucess, 1 if faillure
        int initialize_csprng(const uint8_t* seed) {
            if (seed!=nullptr) {
                csprng_seed(seed);
            } else {
                FILE* urandom=fopen("/dev/urandom","rb");
                if (urandom==NULL) {
                    return 1;
                }
                uint8_t key[16];
                if (fread(key,1,16,urandom)!=16) {
                    fclose(urandom);
                    return 1;
                }
                fclose(urandom);
                csprng_seed(key);
            }
            return 0;
        }
        //* Function to fill a entropy buffer
        void fill_buffer(buffer* buff) {
            for (int i=0;i<2048;++i) {
                buff->rands[i].rand1=((uint64_t)csprng_get_next_uint32()<<32) | csprng_get_next_uint32();
                buff->rands[i].rand2=((uint64_t)csprng_get_next_uint32()<<32) | csprng_get_next_uint32();
                buff->rands[i].rand3=((uint64_t)csprng_get_next_uint32()<<32) | csprng_get_next_uint32();
                buff->rands[i].rand4=((uint64_t)csprng_get_next_uint32()<<32) | csprng_get_next_uint32();
                buff->rands[i].rand5=((uint64_t)csprng_get_next_uint32()<<32) | csprng_get_next_uint32();
                buff->rands[i].rand6=((uint64_t)csprng_get_next_uint32()<<32) | csprng_get_next_uint32();
                buff->rands[i].rand7=((uint64_t)csprng_get_next_uint32()<<32) | csprng_get_next_uint32();
                buff->rands[i].rand8=((uint64_t)csprng_get_next_uint32()<<32) | csprng_get_next_uint32();
            }
            buff->actual_offset=0;
        }
        //* Function to generate a VYIDv2
        //* Made for single use, compatible with bare metal
        //* THIS FUNCTION WON'T CHECK FOR MIN/MAX SIZE FOR OUTPUT AND PAYLOAD BUFFER. USE THE OFFICIAL API FOR THAT PURPOSE
        //* The amount variable need to be incremented manually if you use directely this function
        //* uint8_t* out : the output buffer, minimum 182 bytes
        //* size_t size_out : the size of the output buffer
        //* uint8_t* payload : the payload to insert, can't be higher than 32 bytes
        //* size_t size_payload : the size of the payload, setting it to 0 will disable the payload insertion, regardless of what is inside the payload buffer
        //* uint8* entropy : the 64 bytes of entropy
        //* blake3_hasher& hasher : the hasher object
        //* Return nothing, output buffer won't be modified if faillure
        void generate_vyid_char_raw(uint8_t* out,size_t size_out,const uint8_t* payload,size_t size_payload,uint8_t* entropy,blake3_hasher& hasher) {
            uint64_t combined_value=amount^session_id;
            uint8_t address_bits;
            uint8_t selector=(uint8_t)combined_value%5;
            if (selector==0) {
                address_bits=64;
            } else if (selector==1) {
                address_bits=96;
            } else if (selector==2) {
                address_bits=128;
            } else if (selector==3) {
                address_bits=160;
            } else if (selector==4) {
                address_bits=192;
            }
            size_t payload_size_bits=size_payload*8;
            uint8_t mainblock_bytes[64]={0};
            memcpy(mainblock_bytes,entropy,64);
            if (payload_size_bits>0) {
                for (size_t i=0;i<payload_size_bits;++i) {
                    size_t payload_byte_idx=i/8;
                    size_t payload_bit_in_byte=i%8;
                    bool bit_value=((payload[payload_byte_idx]> payload_bit_in_byte) & 1);
                    size_t target_bit_idx=address_bits+i;
                    size_t target_byte_idx=target_bit_idx/8;
                    size_t target_bit_in_byte=target_bit_idx%8;
                    if (bit_value) {
                        mainblock_bytes[target_byte_idx]|=(1<<target_bit_in_byte);
                    } else {
                        mainblock_bytes[target_byte_idx]&=~(1<<target_bit_in_byte);
                    }
                }
            }
            uint64_t first_entropy;
            memcpy(&first_entropy,entropy,8);
            uint64_t last_entropy;
            memcpy(&last_entropy,entropy+56,8);
            uint32_t first_param=(uint32_t)(amount>>32);
            uint32_t second_param=(uint32_t)(session_id>>32);
            uint64_t first_seed=((uint64_t)first_param << 32) | second_param;
            uint64_t last_seed=amount^session_id;
            uint8_t seed[32];
            memcpy(seed,&first_seed,8);
            memcpy(seed+8,&last_entropy,8);
            memcpy(seed+16,&first_entropy,8);
            memcpy(seed+24,&last_seed,8);
            blake3_hasher_init_keyed(&hasher,seed);
            blake3_hasher_update(&hasher,mainblock_bytes,sizeof(mainblock_bytes));
            uint8_t output[64];
            blake3_hasher_finalize(&hasher,output,sizeof(output));
            uint64_t integrity;
            memcpy(&integrity,output,sizeof(integrity));
            char session_id_char[17];
            size_t session_id_len=base512::encode_base512(session_id,session_id_char,17);
            char amount_char[17];
            size_t amount_len=base512::encode_base512(amount,amount_char,17);
            char mainblock_char[129]={0};
            char int64_block[17];
            size_t current_mainblock_char_offset=0;
            for (int block=0;block<8;++block) {
                uint64_t extracted_val;
                memcpy(&extracted_val,mainblock_bytes+(block*8),8);
                size_t int64_block_len=base512::encode_base512(extracted_val,int64_block,sizeof(int64_block));
                if (current_mainblock_char_offset+int64_block_len>=sizeof(mainblock_char)) {
                    mainblock_char[sizeof(mainblock_char)-1]='\0'; 
                    break;
                }
                memcpy(mainblock_char+current_mainblock_char_offset,int64_block,int64_block_len);
                current_mainblock_char_offset+=int64_block_len;
            }
            mainblock_char[current_mainblock_char_offset]='\0';
            char integrity_char[17];
            size_t integrity_len=base512::encode_base512(integrity,integrity_char,17);
            size_t idx=0;
            out[idx]='[';
            idx++;
            memcpy(out+idx,session_id_char,session_id_len);
            idx+=session_id_len;
            out[idx]='-';
            idx++;
            memcpy(out+idx,amount_char,amount_len);
            idx+=amount_len;
            out[idx]='-';
            idx++;
            memcpy(out+idx,mainblock_char,current_mainblock_char_offset);
            idx+=current_mainblock_char_offset;
            out[idx]='-';
            idx++;
            memcpy(out+idx,integrity_char,integrity_len);
            idx+=integrity_len;
            out[idx]=']';
            idx++;
            out[idx]='\0';
            return;
        }
        //* The function to generate a VYIDv2, the official API
        //* Made for bulk generation, automaticaly recycle the hasher instance, fill the entropy buffer and create the entropy object
        //* char* out : the output buffer, minimum 182 bytes
        //* size_t size_out : the size of the output buffer
        //* char* payload : the payload to insert, can't be higher than 32 bytes
        //* size_t size_payload : the size of the payload, setting it to 0 will disable the payload insertion, regardless of what is inside the payload buffer
        //* Return an integer : 0 if sucess, 1 if faillure
        int generate_vyid_char(char* out,size_t size_out,const char* payload,size_t size_payload) {
            static thread_local buffer buff;
            if (buff.actual_offset>=2048) {
                fill_buffer(&buff);
            }
            if (size_payload>32) {
                return 1;
            }
            if (size_out<VYID2_MIN_BUFFER_SIZE) {
                return 1;
            }
            static thread_local uint8_t entropy[64];
            memcpy(entropy,&buff.rands[buff.actual_offset],sizeof(random_values));
            buff.actual_offset++;
            static thread_local blake3_hasher hasher;
            uint8_t* out_buffer=reinterpret_cast<uint8_t*>(out);
            const uint8_t* payload_data=reinterpret_cast<const uint8_t*>(payload);
            amount++;
            generate_vyid_char_raw(out_buffer,size_out,payload_data,size_payload,entropy,hasher);
            return 0;
        }
        //* Function to get the number of bytes taken by a utf8 character
        //* Made for internal use
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
            } else if ((byte & 0xF8) == 0xF0) {
                return 4;
            }
            return 0;
        }
        //* Function to extract the payload
        //* Since the size of the inserted payload isn't saved, if your payload isn't 32 bytes, you will end up with some entropy at the end of the extracted payload
        //* It's up to you to know the size of the inserted payload
        //* char* input : the VYIDv2 to extract the payload from, must be at least 182 bytes
        //* size_t input_size : the size of input
        //* char* output : the buffer where the extracted payload is saved, must be at least 32 bytes
        //* size_t output_size : the size of output
        //* Return an integer : 0 if sucess, 1 if size error, 2 if format error, 3 if decoding error, 4 if mainblock error
        int extract_payload(char* input,size_t input_size,char* output,size_t output_size) {
            if (input_size<VYID2_MIN_BUFFER_SIZE) {
                return 1;
            }
            if (output_size<VYID2_MAX_PAYLOAD_SIZE) {
                return 1;
            }
            size_t input_len=strlen(input);
            uint64_t extracted_session_id=0;
            uint64_t extracted_amount=0;
            const char* current_ptr=input;
            size_t remaining_len=input_len;
            if (remaining_len<93) {
                return 1;
            }
            if (*current_ptr!='[') {
                return 2;
            }
            current_ptr++;
            remaining_len--;
            if (current_ptr[remaining_len-1]!=']') {
                return 2;
            }
            const char* session_id_start=current_ptr;
            const char* amount_start=nullptr;
            const char* mainblock_start=nullptr;
            const char* integrity_start=nullptr;
            size_t session_id_len=0;
            size_t amount_len=0;
            size_t mainblock_len=0;
            size_t integrity_len=0;
            int dash_count=0;
            for (size_t i=0;i<remaining_len-1;++i) {
                if (current_ptr[i]=='-') {
                    dash_count++;
                    if (dash_count==1) {
                        session_id_len=i;
                        amount_start=current_ptr+i+1;
                    } else if (dash_count==2) {
                        amount_len=(current_ptr+i)-amount_start;
                        mainblock_start=current_ptr+i+1;
                    } else if (dash_count==3) {
                        mainblock_len=(current_ptr+i)-mainblock_start;
                        integrity_start=current_ptr+i+1;
                        integrity_len=(current_ptr+remaining_len-1)-integrity_start;
                        break;
                    }
                }
            }
            if (dash_count!=3) {
                return 2;
            }
            extracted_session_id=base512::decode_base512(session_id_start,session_id_len);
            if (extracted_session_id==0 && session_id_len>0) {
                return 30;
            }
            extracted_amount=base512::decode_base512(amount_start,amount_len);
            if (extracted_amount==0 && amount_len>0) {
                return 3;
            }
            uint64_t combined_value=amount^session_id;
            uint8_t address_bits;
            uint8_t selector=(uint8_t)combined_value%5;
            if (selector==0) {
                address_bits=64;
            } else if (selector==1) {
                address_bits=96;
            } else if (selector==2) {
                address_bits=128;
            } else if (selector==3) {
                address_bits=160;
            } else if (selector==4) {
                address_bits=192;
            }
            char mainblock_encoded_null_terminated[129];
            if (mainblock_len>=129) {
                return 4;
            }
            memcpy(mainblock_encoded_null_terminated,mainblock_start,mainblock_len);
            mainblock_encoded_null_terminated[mainblock_len]='\0';
            uint8_t mainblock_decoded_bytes[64];
            memset(mainblock_decoded_bytes,0,64);
            int decode_main_result=base512::decode_mainblock_full(mainblock_encoded_null_terminated,mainblock_len,mainblock_decoded_bytes);
            if (decode_main_result!=0) {
                return 3;
            }
            size_t byte_offset=address_bits/8;
            size_t bit_offset=address_bits%8;
            for (size_t i=0;i<VYID2_MAX_PAYLOAD_SIZE;++i) {
                uint8_t byte1=mainblock_decoded_bytes[byte_offset+i];
                uint8_t byte2=0;
                if (byte_offset+i+1<64) {
                    byte2=mainblock_decoded_bytes[byte_offset+i+1];
                }
                output[i]=(byte1<<bit_offset) | (byte2>>(8-bit_offset));
            }
            return 0;
        }
    }
}
#endif