import random

alphabet = '0123456789ABCDEFHJKLMNPQRTUVWXYZ'

def b32decode(mapped_alphabet):
    i = 0
    decoded = [0]*(int)(5/8*24)

    for ii in range(0,24):
        for iii in range(0,5):
            Var1 = mapped_alphabet[ii]
            if Var1 & (1 << iii) != 0:
                shift = (i+iii) % 8
                decoded[(int)((i + iii) / 8)] |= (1 << shift)        
        i += 5;
    return decoded

def b32encode(decoded):
    i = 0
    encoded = [0]*24

    for ii in range(0,(int)(5/8*24)):
        for iii in range(0,8):
            Var1 = decoded[ii]
            if Var1 & (1 << iii) != 0:
                shift = (i+iii) % 5
                encoded[(int)((i + iii) / 5)] |= (1 << shift)        
        i += 8;
    return(encoded)

# p = 33759901540733
# q = 34719860683127
# e = 65537
# n = 1172139078170334317196312091
# d = 117398124862438392921820577

def RSA_encrypt(message,key):
	encrypted_text = pow(message,key,1172139078170334317196312091)
	return(encrypted_text)

def RSA_decrypt(message,key):
	decrypted_text = pow(message,65537,1172139078170334317196312091)
	return(decrypted_text)

def lcm(a, b):
	return abs(a*b)

x = lcm(33759901540733-1,34719860683127-1)

key = pow(0x10001, -1, x)
print("RSA Key:")
print(key)
print("RSA Key Hex:")
print(hex(key))

# Perform an XOR operation on 8 characters with the value 0xAEB7037B and bitwise-shift the output 2 places to the right
rand_bytes = random.randint(0,0xFFFFFFFF)
hex_rand_bytes = hex((rand_bytes ^ 0xAEB7037B) >> 2)[2:]
print("XORED Bytes:")
print(hex_rand_bytes)

# Have 7 characters be the value 'PWNADV3' Note: This needs to be backwards due to endianness
static_bytes="3VDANWP".encode("utf-8").hex()
print(static_bytes)
print("Bytes For Keygen:")
int_bytes=int(static_bytes+hex_rand_bytes,16)
print(int_bytes)

# Encrypt the bytes using RSA decryption with the e value 65537, p value 33759901540733 and q value 34719860683127
encrypted=RSA_encrypt(int_bytes,key)
print("Encrypted Bytes For Keygen:")
print(encrypted)

# Encode the encrypted bytes using the custom Base32 routine, and ensure the last 8 characters (4 bytes) are equal to that of the original generated random bytes)
hex_encrypted = bytearray(encrypted.to_bytes(12, byteorder='little'))
print(hex_encrypted)
hex_encrypted[11] |= int.from_bytes(rand_bytes.to_bytes(4, byteorder='little')[:1], byteorder='little')
hex_encrypted += rand_bytes.to_bytes(4, byteorder='little')[1:]
print("Hex Encrypted Bytes For Keygen:")
print(hex_encrypted.hex().upper())
print(hex_encrypted)
DLC_Key_bytes=b32encode(hex_encrypted)
print("DLC Key Bytes (No Checksum)")
print(DLC_Key_bytes)

# With a provided DLC key, using a custom alphabet, confirm the last byte is equal to the sum of the submitted bytes mapped to this alphabet after performing an AND operation with the key '1f'.
checksum = 0
DLC_Key = ""
for value in DLC_Key_bytes:
    DLC_Key+=alphabet[value]
    checksum+=value
print("DLC Key (No Checksum)")
print(DLC_Key)
print("DLC Key (Checksum)")
DLC_Key += alphabet[checksum & 0x1f]
print(DLC_Key[:5] + '-' + DLC_Key[5:10] + '-' + DLC_Key[10:15] + '-' + DLC_Key[15:20] + '-' + DLC_Key[20:25])
