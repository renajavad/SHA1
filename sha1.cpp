#include <iostream>
#include <cstdint>
#include <vector>
#include <bitset>
#include <iomanip>

std::string sha1(const std::string& input) {
    std::vector<uint8_t> message(input.begin(), input.end());
    uint64_t data_len = message.size() * 8;

    message.push_back(0x80);

    while ((message.size() * 8) % 512 != 448) {
        message.push_back(0x00);
    }
	message.push_back(data_len >> 56);
	message.push_back(data_len >> 48);
	message.push_back(data_len >> 40);
	message.push_back(data_len >> 32);
	message.push_back(data_len >> 24);
	message.push_back(data_len >> 16);
	message.push_back(data_len >> 8);
	message.push_back(data_len);
	
    std::vector<uint32_t> H = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
    
	for (uint64_t chunks = 0; chunks*64 < message.size(); chunks++) {
	std::vector<uint32_t> W(80);
	std::vector<uint32_t> M(16);
	for(int i = 0; i < 16; i++){
		M[i] = 0;
		for(int j = 0; j < 4; j++){
			M[i] = (M[i] << 8) | message[chunks*64 + 4 * i + j];
		}
	}
	for (int i = 0; i < 16; i++) { 
		W[i] = M[i]; 
	}

	for (int i = 16; i < 80; i++) {
		W[i] = (W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16]);
		W[i] = (W[i] << 1) | (W[i] >> 31);
	}

	uint32_t A = H[0];
	uint32_t B = H[1];
	uint32_t C = H[2];
	uint32_t D = H[3];
	uint32_t E = H[4];

	for (int i = 0; i < 80; i++) {
		uint32_t Ft, Kt;

		if (i < 20) {
			Ft = (B & C) | ((~B) & D);
			Kt = 0x5A827999;
		} else if (i < 40) {
			Ft = B ^ C ^ D;
			Kt = 0x6ED9EBA1;
		} else if (i < 60) {
			Ft = (B & C) | (B & D) | (C & D);
			Kt = 0x8F1BBCDC;
		} else {
			Ft = B ^ C ^ D;
			Kt = 0xCA62C1D6;
		}

		uint32_t temp = ((A << 5) | (A >> 27)) + Ft + E + Kt + W[i]; 
		E = D;
		D = C;
		C = (B << 30) | (B >> 2);
		B = A;
		A = temp;
	}

	H[0] += A;
	H[1] += B;
	H[2] += C;
	H[3] += D;
	H[4] += E;
}
	std::stringstream hexstream;
	hexstream << std::hex << H[0] << " " << H[1] << " " << H[2] << " " << H[3] << " " << H[4] << "\n";
    std::string result(hexstream.str());
    return result;
}

int main() {
    std::string input;
    std::cout << "Enter input string: ";
    std::getline(std::cin, input);

    std::string hashed = sha1(input);

    std::cout << "SHA-1 hash" << ": " << hashed << "\n";

    return 0;
}
