/**
 * @file lib_crypt.c
 * @author Kishore
 * @date July 3, 2015, 1:28 PM
 * 
 * @brief This library is a collection of crypt related
 * functions that are used in Class-DB like sha1, base64
 * encoding decoding etc. Any need for crypt functions, 
 * this library will be updated with related fucntion.
 */

#include "headers/headers.h"
#include "headers/lib_utilities.h"
#include "headers/lib_crypt.h"

static uint32_t* __sha1(char *msg);
static void sha1pad(char *msg, char *target);
static inline unsigned char base64_getChar(unsigned char ch);
static void __base64(unsigned char *input, unsigned char *output, int len);

/**
 * Computes base64 of given `input` of length `len` and stores in output.
 * @param input : Input string whose base64 should be computed.
 * @param output : Computed base64 of `input` string.
 * 
 * Here input is ended with '\0' character. Hence there is no need
 * of providing length of input.
 */
void base64(unsigned char *input, unsigned char *output)
{
	__base64(input, output, strlen(input));
}



/**
 * Computes base64 of given `input` of length `len` and stores in output.
 * @param input : Input string whose base64 should be computed.
 * @param output : Computed base64 of `input` string.
 * @param len : Length of input string. Used in case input is binary.
 * 
 * Here input is a binary and hence there is a need of providing length
 * of input.
 */
void base64len(unsigned char *input, unsigned char *output, int len)
{
	__base64(input, output, len);
}



/**
 * Computes SHA1 of given string and returns it.
 * @param msg : character array whose sha1 should be computed.
 * @returns sha1 of the given string.
 * 
 * Compute SHA1 of given message and returns it. The string returned
 * should be immediately used, since the memory location returned
 * for all sha1 calls is same. Hence it is not thread safe function.
 * Also the binary representation of sha1 is only 20 byte length.
 * Here the returned sha1 is in binary representation.
 */
unsigned char* bsha1(char *msg)
{
	unsigned char *hs = (unsigned char*)__sha1(msg);
	static unsigned char bsha[21];
	int i = 0;
	for (i=0; i<20; i++)
	{
		bsha[i] = *(hs+i);
	}
	return bsha;
}



/**
 * Computes SHA1 of given string and returns it.
 * @param msg : character array whose sha1 should be computed.
 * @returns sha1 of the given string.
 * 
 * Compute SHA1 of given message and returns it. The string returned
 * should be immediately used, since the memory location returned
 * for all sha1 calls is same. Hence it is not thread safe function.
 * The sha1 returned is textual information of hexadecimal value of sha1
 * and its length is 40 bytes.
 */
char* sha1(char *msg)
{
	uint32_t *hs = (uint32_t *)__sha1(msg);
	static char shastr[41];
	sprintf(shastr, "%x%x%x%x%x", hs[0], hs[1], hs[2], hs[3], hs[4]);
	
}



/* Static functions that only reside in this library goes down from here.*/
static void sha1pad(char *msg, char *target)
{
	int len = strlen(msg);
	strncpy(target, msg, 64);
	
	int i = 0;
	for (i=len; i<64; i++)
	{
		target[i] = 0;
	}
}


static uint32_t* __sha1(char *msg)
{
	static char sha1str[41];
	
	uint32_t h0 = 0x67452301;
	uint32_t h1 = 0xEFCDAB89;
	uint32_t h2 = 0x98BADCFE;
	uint32_t h3 = 0x10325476;
	uint32_t h4 = 0xC3D2E1F0;

	// Message length should be stored in 64 bit to be used in algorithm.
	int msg_len = strlen(msg);
	uint64_t message_length = (uint64_t)(msg_len*8);
	
	char chunk[320] = "";
	
	int total_chunks = (msg_len+8+1)/64 + 1;
	int current_chunk_index = 0;
	int append1_chunk = (msg_len+1)/64;
	
	while (1)
	{
		// If the whole string is computed break.
		if (current_chunk_index == (total_chunks))
			break;
		
		
		if (current_chunk_index*64 < msg_len)
			sha1pad(current_chunk_index*64 + msg, chunk);
		else
			sha1pad("", chunk);

		// Message is divided into chunks of 512 bits and the below process
		// is done to all of them. Last chunk should be padded out for which
		// sha1pad function was written.
		if (current_chunk_index == (total_chunks - 1)){
			message_length = to_bigendian64(message_length);
			char *ml = (char *)&message_length;
			
			int i=0;
			for(i=0; i<8; i++)
				chunk[56+i] = ml[i];
		}
		
		if (current_chunk_index == append1_chunk)
		{
			int len = strlen(msg + current_chunk_index*64);
			chunk[len] = 0x80;
		}
		// Reference chunk with `w` for simplicity.
		char *w = chunk;

		int i=0;

		for (i=0; i<16; i++)
		{
			uint32_t *e = (uint32_t *)(w + i*4);
			// Convert all the words of 512 bit chunks to big endian system
			// to avoid inconsistencies in little endian architecture.
			*e = to_bigendian32(*e);
		}
		for (i=16; i<80; i++)
		{
			uint32_t a, b, c, d;
			a = *((uint32_t *)((w+(i-3)*4)));
			b = *((uint32_t *)((w+(i-8)*4)));
			c = *((uint32_t *)((w+(i-14)*4)));
			d = *((uint32_t *)((w+(i-16)*4)));

			uint32_t *e = (uint32_t *)(w + i*4);
			*e = a ^ b ^ c ^ d;
			*e = lrotate32(*e, 1);			
		}
		
		uint32_t a, b, c, d, e;
		
		a = h0;
		b = h1;
		c = h2;
		d = h3;
		e = h4;
		
		for (i=0; i<80; i++)
		{
			uint32_t f,k;
			if (i<20)
			{
				f = (b & c) | ((~b) & d);
				k = 0x5A827999;
			}
			else if (i<40)
			{
				f = b ^ c ^ d;
				k = 0x6ED9EBA1;
			}
			else if (i<60)
			{
				f = (b & c) | (b & d) | (c & d);
				k = 0x8F1BBCDC;
			}
			else
			{
				f = b ^ c ^ d;
				k = 0xCA62C1D6;
			}
			uint32_t wi = (uint32_t)*((uint32_t *)(w+i*4));
			
			if (i==0){
				uint32_t la =  lrotate32(a, 5);

			}
			uint32_t temp = lrotate32(a, 5) + f + k + e + wi;
			e = d;
			d = c;
			c = lrotate32(b, 30);
			b = a;
			a = temp;
		}
		h0 = h0 + a;
		h1 = h1 + b;
		h2 = h2 + c;
		h3 = h3 + d;
		h4 = h4 + e;
		
		current_chunk_index++;
	}
	// h0 -- h4 the result we got is in little endian.
	// To get the answer we need Big endian numbers. Hence we
	// convert them to big endian numbers.
	static uint32_t hs[4];
	hs[0] = to_bigendian32(h0);
	hs[1] = to_bigendian32(h1);
	hs[2] = to_bigendian32(h2);
	hs[3] = to_bigendian32(h3);
	hs[4] = to_bigendian32(h4);
	return hs;
}

static inline unsigned char base64_getChar(unsigned char ch)
{
	if (ch<26)
		return 'A' + ch;
	else if (ch<52)
		return 'a' + ch - 26;
	else if (ch<62)
		return '0' + ch - 52;
	else if (ch == 62)
		return '+';
	else if (ch==63)
		return '/';
	else{
		assert(ch<64);
	}
}


static void __base64(unsigned char *input, unsigned char *output, int len)
{
	int i, op_len = 0;
	for (i=0; (i<len); i+=3)
	{
		if ((len-i)<3)
			break;
		unsigned char	ch0 = input[i]>>2,
				ch1 = ((input[i] & 0x3)<<4) | (input[i+1]>>4),
				ch2 = ((input[i+1] & 0x0F)<<2) | ((input[i+2])>>6),
				ch3 = input[i+2] & 0x3f;
		output[op_len++] = base64_getChar(ch0);
		output[op_len++] = base64_getChar(ch1);
		output[op_len++] = base64_getChar(ch2);
		output[op_len++] = base64_getChar(ch3);
	}
	if ((len-i)==1){
		output[op_len++] = base64_getChar(input[i]>>2);
		output[op_len++] = base64_getChar(((input[i] & 0x3)<<4) | (input[i+1]>>4));
		output[op_len++] = '=';
		output[op_len++] = '=';
	}
	else if ((len-i)==2){
		output[op_len++] = base64_getChar(input[i]>>2);
		output[op_len++] = base64_getChar(((input[i] & 0x3)<<4) | (input[i+1]>>4));
		output[op_len++] = base64_getChar(((input[i+1] & 0x0F)<<2) | ((input[i+2])>>6));
		output[op_len++] = '=';
	}
	output[op_len] = '\0';
}