#include <stdio.h>
#include <stdlib.h>

unsigned int* md(unsigned int* hash, unsigned int len, char* buf){
	unsigned int s[64] = {7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
		5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
		4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
		6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };
	unsigned int K[64] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };
	unsigned int a0 = 0x67452301;   // A
	unsigned int b0 = 0xefcdab89;   // B
	unsigned int c0 = 0x98badcfe;   // C
	unsigned int d0 = 0x10325476;   // D
	unsigned int plen = 64*((len+8)/64+1);
	void* padd = malloc(plen*sizeof(char));
	unsigned char* padded = padd;
	unsigned int i = 0;
	for(i=0;i<len;i++){
		padded[i] = buf[i];
	}
	padded[i] = 0x80;
	for(i=len+1;i<plen;i++){
		padded[i] = 0;
	}
	((unsigned long long*)(padd))[plen/8-1] = 8*len;
	for(unsigned int j=0;j<plen/64;j++){
		unsigned int A = a0;
		unsigned int B = b0;
		unsigned int C = c0;
		unsigned int D = d0;
		for(i=0;i<64;i++){
			unsigned int F, g;
			if(i<16){
				F = (B&C)|((~B)&D);
				g = i;
			}
			else if(i>=16 && i<32){
				F = (D&B)|((~D)&C);
				g = (5*i+1)%16;
			}
			else if(i>=32 && i<48){
				F = B ^ C ^ D;
				g = (3*i+5)%16;
			}
			else if (i>=48 && i<64){
				F = C ^ (B|(~D));
				g = (7*i)%16;
			}
			else printf("Error\n");
			unsigned int mg = ((unsigned int*)(padd))[16*j+g];
			F = F + A + K[i] + mg;
			A = D;
			D = C;
			C = B;
			B = B + ((F<<s[i]) | (F>>(32-s[i])));
		}
		a0 += A;
		b0 += B;
		c0 += C;
		d0 += D;
	}
	hash[0] = a0; hash[1] = b0; hash[2] = c0; hash[3] = d0;
	free(padd);
	return hash;
}

int main(int argc, char** argv){

	if(argc>1){
		int len = 0;
		while(argv[1][len]) len++;
		unsigned int a[4];
		md(a, len, argv[1]);
		unsigned char b[16]; 
		for(int i=0;i<4;i++){
			b[(i*4)+0] = (unsigned char)((a[i] & 0x000000FF));
        		b[(i*4)+1] = (unsigned char)((a[i] & 0x0000FF00) >> 8);
        		b[(i*4)+2] = (unsigned char)((a[i] & 0x00FF0000) >> 16);
        		b[(i*4)+3] = (unsigned char)((a[i] & 0xFF000000) >> 24);
		}
		for(int i=0;i<16;i++){
        		printf("%02x", b[i]);
    		}
    		printf("\n");
	}
	return 0;

}
