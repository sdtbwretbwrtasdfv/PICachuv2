import sys
import argparse

def hash_string(char_codes, salt):
    hash_value = int(salt)
    for char_code in char_codes:
        hash_value = ((hash_value << 5) + hash_value) + char_code
        hash_value = hash_value & 0xFFFFFFFF
    return hash_value

def to_upper_string(temp):
    return temp.upper()

def remove_dll_extension(string):
    return string.upper().replace(".DLL", "")

def main():
    parser = argparse.ArgumentParser(description='Process string and hash it.')
    parser.add_argument('string', type=str, help='String to hash')
    parser.add_argument('-s', '--salt', type=int, default=5381, help='Salt for hash function (default: 5381)')
    parser.add_argument('-t', '--type', type=str, required=True, choices=['func', 'dll'], help='Type of input (function or dll)')

    args = parser.parse_args()

    string_to_hash = args.string

    if args.type == 'func':
        string_to_hash = to_upper_string(string_to_hash)
        char_codes = [c for c in string_to_hash.encode()]
        hashed_value = hash_string(char_codes, args.salt)
        print(f"#define H_FUNC_{args.string.upper()} 0x{hashed_value:x}")
    if args.type == 'dll':
        string_to_hash = to_upper_string(string_to_hash)
        char_codes = [c for c in string_to_hash.encode()]
        hashed_value = hash_string(char_codes, args.salt)
        define_name = remove_dll_extension(args.string)
        print(f"#define H_LIB_{define_name} 0x{hashed_value:x}")

if __name__ == "__main__":
    main()
