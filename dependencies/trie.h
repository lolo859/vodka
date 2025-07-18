#ifndef TRIE_H
#define TRIE_H
#include <cstdint>
#include <vector>
#include <array>
#include <string_view>
static const char* base512_alphabet[512]={"!","#","$","%","&","(",")","*","+",",",".","/","0","1","2","3","4","5","6","7","8","9",":",";","<",">","=","?","@","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","\\","^","_","`","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","{","}","|","~","\"","¡","£","¤","¥","¦","§","¬","¯","°","²","±","µ","´","·","¿","¼","½","¾","À","Á","Â","Ã","Ä","Å","Æ","Ç","È","É","Ê","Ë","Ì","Í","Î","Ï","Ð","Ñ","Ò","Ó","Ô","Õ","Ö","×","Ø","Ù","Ú","Û","Ü","Ý","ß","à","á","â","ã","ä","å","æ","ç","è","é","ê","ë","ì","í","î","ï","ð","ñ","ò","ó","ô","õ","ö","÷","ø","ù","ú","û","ü","ý","þ","ÿ","Ā","ā","Ă","ă","Ą","ą","Ć","ć","Ĉ","ĉ","Ċ","ċ","Č","č","Ď","ď","Đ","đ","Ē","ē","Ĕ","ĕ","Ė","ė","Ę","ę","Ě","ě","Ĝ","ĝ","Ğ","ğ","Ġ","ġ","Ģ","ģ","Ĥ","ĥ","Ħ","ħ","Ĩ","ĩ","Ī","ī","Ĭ","ĭ","Į","į","ı","Ĵ","ĵ","Ķ","ķ","ĸ","Ĺ","ĺ","Ļ","ļ","Ľ","ľ","Ŀ","ŀ","Ł","ł","Ń","ń","Ņ","ņ","Ň","ň","ŉ","Ŋ","ŋ","Ō","ō","Ŏ","ŏ","Ő","ő","Œ","œ","Ŕ","ŕ","Ŗ","ŗ","Ř","ř","Ś","ś","Ŝ","ŝ","Ş","ş","Š","š","Ţ","ţ","Ť","ť","Ŧ","ŧ","Ũ","ũ","Ū","ū","Ŭ","ŭ","Ů","ů","Ű","ű","Ų","ų","Ŵ","ŵ","Ŷ","ŷ","Ÿ","Ź","ź","Ż","ż","Ž","ž","ſ","ƀ","Ɓ","Ƃ","ƃ","Ƅ","ƅ","Ɔ","Ƈ","ƈ","Ɖ","Ɗ","Ƌ","ƌ","ƍ","Ǝ","Ə","Ɛ","Ƒ","ƒ","Ɠ","Ɣ","ƕ","Ɨ","Ƙ","ƙ","ƚ","ƛ","Ɯ","Ɲ","ƞ","Ɵ","Ơ","ơ","ƣ","Ƥ","ƥ","Ʀ","Ƨ","ƨ","Ʃ","ƪ","ƫ","Ƭ","ƭ","Ʈ","Ư","ư","Ʊ","Ʋ","Ƴ","ƴ","Ƶ","ƶ","Ʒ","Ƹ","ƺ","ƻ","Ƽ","ƽ","ƾ","ƿ","ǁ","ǂ","Ǎ","ǎ","Ǐ","ǐ","Ǒ","ǒ","Ǔ","ǔ","Ǖ","ǖ","Ǘ","ǘ","Ǚ","ǚ","Ǜ","ǜ","ǝ","Ǟ","ǟ","Ǡ","ǡ","Ǣ","ǣ","Ǥ","ǥ","Ǧ","ǧ","Ǩ","ǩ","Ǫ","ǫ","Ǭ","ǭ","Ǯ","ǯ","ǰ","Ǵ","ǵ","Ƕ","Ƿ","Ǹ","ǹ","Ǻ","ǻ","Ǽ","ǽ","Ǿ","ǿ","Ȁ","ȁ","Ȃ","ȃ","Ȅ","ȅ","Ȇ","ȇ","Ȉ","ȉ","Ȋ","ȋ","Ȍ","ȍ","Ȏ","ȏ","Ȑ","ȑ","Ȓ","ȓ","Ȕ","ȕ","Ȗ","ȗ","Ș","ș","Ț","ț","Ȝ","ȝ","Ȟ","ȟ","Ƞ","ȡ","Ȣ","Ȥ","ȥ","Ȧ","ȧ","Ȩ","ȩ","Ȫ","ȫ","Ȭ","ȭ","Ȯ","ȯ","Ȱ","ȱ","Ȳ","ȳ","ȴ","ȵ","ȶ","ȷ","ȸ","ȹ","Ⱥ","Ȼ","ȼ","Ƚ","Ⱦ","ȿ","ɀ","Ɂ","ɂ","Ƀ","Ʉ","Ʌ","Ɇ","ɇ","Ɉ","ɉ","Ɋ","ɋ","Ɍ","ɍ","Ɏ","ɏ","ɐ","ɒ","ɓ","ɔ","ɕ","ɖ","ɗ","ə","ɚ","ɛ","ɜ","ɝ","ɞ","ɟ","ɠ","ɡ","ɢ","ɣ","ɤ","ɥ","ɦ","ɧ","ɮ","ɯ","ɰ","ɱ"};
namespace trie {
    struct TrieNode {
        std::array<TrieNode*,256> children; 
        bool is_end_of_symbol;
        uint16_t symbol_value; 
        TrieNode():is_end_of_symbol(false),symbol_value(0) {
            children.fill(nullptr);
        }
    };
    class DecodeTrie {
        public:
        DecodeTrie():root(nullptr),next_node_idx(0) {
            init_trie();
        }
        DecodeTrie(const DecodeTrie&)=delete;
        DecodeTrie& operator=(const DecodeTrie&)=delete;
        ~DecodeTrie() {
            for (TrieNode* node:node_pool) {
                delete[] node;
            }
            node_pool.clear();
        }
        std::pair<uint16_t,size_t> find_symbol(const char* input,size_t max_len) const {
            TrieNode* current=root;
            uint16_t found_value=0;
            size_t consumed_bytes=0;
            size_t longest_match_len=0;
            bool found_match=false;
            for (size_t i=0;i<max_len;++i) {
                unsigned char idx=static_cast<unsigned char>(input[i]);
                if (current->children[idx]==nullptr) {
                    break; 
                }
                current=current->children[idx];
                if (current->is_end_of_symbol) {
                    found_value=current->symbol_value;
                    longest_match_len=i+1;
                    found_match=true;
                }
            }
            if (found_match) {
                return {found_value,longest_match_len};
            } else {
                return {0,0};
            }
        }
        void insert(std::string_view symbol,uint16_t value) {
            TrieNode* current=root;
            for (char ch_byte:symbol) {
                unsigned char idx=static_cast<unsigned char>(ch_byte);
                if (current->children[idx]==nullptr) {
                    current->children[idx]=allocate_node();
                }
                current=current->children[idx];
            }
            current->is_end_of_symbol=true;
            current->symbol_value=value;
        }
        private:
            TrieNode* root;
            std::vector<TrieNode*> node_pool;
            size_t next_node_idx;
            const size_t NODE_POOL_BLOCK_SIZE=1024;
            TrieNode* allocate_node() {
                if (node_pool.empty() || next_node_idx>=NODE_POOL_BLOCK_SIZE) {
                    node_pool.push_back(new TrieNode[NODE_POOL_BLOCK_SIZE]);
                    next_node_idx=0;
                }
                return &node_pool.back()[next_node_idx++];
            }
            void init_trie() {
                node_pool.reserve(512/NODE_POOL_BLOCK_SIZE+2);
                root=allocate_node();
                for (uint16_t i=0;i<512;++i) {
                    insert(base512_alphabet[i],i);
                }
            }
    };
}
#endif