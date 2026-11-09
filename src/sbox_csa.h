// sbox_csa.h

// DECLARAION DE USO DE IA: Este codigo (SOLO ESTE HEADER) esta generado por IA. Sinceramente, era un coñazo hacerlo de cero.

// Codigo generado entero por IA, luego pegado por mi en uno.

#ifndef SBOX_CSA_H
#define SBOX_CSA_H

#include <stdint.h>
#include <string.h>

// ============================================================================
// EL CODIGO EN ESTA ZONA ESTA GENERADO POR IA (DVB-CSA Y S-BOXES)
// ============================================================================

#define PACKET_LANES 64

static const int kSbox1[32] = {2,0,1,1,2,3,3,0, 3,2,2,0,1,1,0,3,
							  0,3,3,0,2,2,1,1, 2,2,0,3,1,1,3,0};
static const int kSbox2[32] = {3,1,0,2,2,3,3,0, 1,3,2,1,0,0,1,2,
							  3,1,0,3,3,2,0,2, 0,0,1,2,2,1,3,1};
static const int kSbox3[32] = {2,0,1,2,2,3,3,1, 1,1,0,3,3,0,2,0,
							  1,3,0,1,3,0,2,2, 2,0,1,2,0,3,3,1};
static const int kSbox4[32] = {3,1,2,3,0,2,1,2, 1,2,0,1,3,0,0,3,
							  1,0,3,1,2,3,0,3, 0,3,2,0,1,2,2,1};
static const int kSbox5[32] = {2,0,0,1,3,2,3,2, 0,1,3,3,1,0,2,1,
							  2,3,2,0,0,3,1,1, 1,0,3,2,3,1,0,2};
static const int kSbox6[32] = {0,1,2,3,1,2,2,0, 0,1,3,0,2,3,1,3,
							  2,3,0,2,3,0,1,1, 2,1,1,2,0,3,3,0};
static const int kSbox7[32] = {0,3,2,2,3,0,0,1, 3,0,1,3,1,2,2,1,
							  1,0,3,3,0,1,1,2, 2,3,1,0,2,3,0,2};

static const uint8_t keyPerm[64] = {
	0x12, 0x24, 0x09, 0x07, 0x2A, 0x31, 0x1D, 0x15,
	0x1C, 0x36, 0x3E, 0x32, 0x13, 0x21, 0x3B, 0x40,
	0x18, 0x14, 0x25, 0x27, 0x02, 0x35, 0x1B, 0x01,
	0x22, 0x04, 0x0D, 0x0E, 0x39, 0x28, 0x1A, 0x29,
	0x33, 0x23, 0x34, 0x0C, 0x16, 0x30, 0x1E, 0x3A,
	0x2D, 0x1F, 0x08, 0x19, 0x17, 0x2F, 0x3D, 0x11,
	0x3C, 0x05, 0x38, 0x2B, 0x0B, 0x06, 0x0A, 0x2C,
	0x20, 0x3F, 0x2E, 0x0F, 0x03, 0x26, 0x10, 0x37
};

static const uint8_t blockSbox[256] = {
	0x3A,0xEA,0x68,0xFE,0x33,0xE9,0x88,0x1A, 0x83,0xCF,0xE1,0x7F,0xBA,0xE2,0x38,0x12,
	0xE8,0x27,0x61,0x95,0x0C,0x36,0xE5,0x70, 0xA2,0x06,0x82,0x7C,0x17,0xA3,0x26,0x49,
	0xBE,0x7A,0x6D,0x47,0xC1,0x51,0x8F,0xF3, 0xCC,0x5B,0x67,0xBD,0xCD,0x18,0x08,0xC9,
	0xFF,0x69,0xEF,0x03,0x4E,0x48,0x4A,0x84, 0x3F,0xB4,0x10,0x04,0xDC,0xF5,0x5C,0xC6,
	0x16,0xAB,0xAC,0x4C,0xF1,0x6A,0x2F,0x3C, 0x3B,0xD4,0xD5,0x94,0xD0,0xC4,0x63,0x62,
	0x71,0xA1,0xF9,0x4F,0x2E,0xAA,0xC5,0x56, 0xE3,0x39,0x93,0xCE,0x65,0x64,0xE4,0x58,
	0x6C,0x19,0x42,0x79,0xDD,0xEE,0x96,0xF6, 0x8A,0xEC,0x1E,0x85,0x53,0x45,0xDE,0xBB,
	0x7E,0x0A,0x9A,0x13,0x2A,0x9D,0xC2,0x5E, 0x5A,0x1F,0x32,0x35,0x9C,0xA8,0x73,0x30,
	0x29,0x3D,0xE7,0x92,0x87,0x1B,0x2B,0x4B, 0xA5,0x57,0x97,0x40,0x15,0xE6,0xBC,0x0E,
	0xEB,0xC3,0x34,0x2D,0xB8,0x44,0x25,0xA4, 0x1C,0xC7,0x23,0xED,0x90,0x6E,0x50,0x00,
	0x99,0x9E,0x4D,0xD9,0xDA,0x8D,0x6F,0x5F, 0x3E,0xD7,0x21,0x74,0x86,0xDF,0x6B,0x05,
	0x8E,0x5D,0x37,0x11,0xD2,0x28,0x75,0xD6, 0xA7,0x77,0x24,0xBF,0xF0,0xB0,0x02,0xB7,
	0xF8,0xFC,0x81,0x09,0xB1,0x01,0x76,0x91, 0x7D,0x0F,0xC8,0xA0,0xF2,0xCB,0x78,0x60,
	0xD1,0xF7,0xE0,0xB5,0x98,0x22,0xB3,0x20, 0x1D,0xA6,0xDB,0x7B,0x59,0x9F,0xAE,0x31,
	0xFB,0xD3,0xB6,0xCA,0x43,0x72,0x07,0xF4, 0xD8,0x41,0x14,0x55,0x0D,0x54,0x8B,0xB9,
	0xAD,0x46,0x0B,0xAF,0x80,0x52,0x2C,0xFA, 0x8C,0x89,0x66,0xFD,0xB2,0xA9,0x9B,0xC0
};

static const int blockPerm[256] = {
	0x00,0x02,0x80,0x82,0x20,0x22,0xA0,0xA2, 0x10,0x12,0x90,0x92,0x30,0x32,0xB0,0xB2,
	0x04,0x06,0x84,0x86,0x24,0x26,0xA4,0xA6, 0x14,0x16,0x94,0x96,0x34,0x36,0xB4,0xB6,
	0x40,0x42,0xC0,0xC2,0x60,0x62,0xE0,0xE2, 0x50,0x52,0xD0,0xD2,0x70,0x72,0xF0,0xF2,
	0x44,0x46,0xC4,0xC6,0x64,0x66,0xE4,0xE6, 0x54,0x56,0xD4,0xD6,0x74,0x76,0xF4,0xF6,
	0x01,0x03,0x81,0x83,0x21,0x23,0xA1,0xA3, 0x11,0x13,0x91,0x93,0x31,0x33,0xB1,0xB3,
	0x05,0x07,0x85,0x87,0x25,0x27,0xA5,0xA7, 0x15,0x17,0x95,0x97,0x35,0x37,0xB5,0xB7,
	0x41,0x43,0xC1,0xC3,0x61,0x63,0xE1,0xE3, 0x51,0x53,0xD1,0xD3,0x71,0x73,0xF1,0xF3,
	0x45,0x47,0xC5,0xC7,0x65,0x67,0xE5,0xE7, 0x55,0x57,0xD5,0xD7,0x75,0x77,0xF5,0xF7,
	0x08,0x0A,0x88,0x8A,0x28,0x2A,0xA8,0xAA, 0x18,0x1A,0x98,0x9A,0x38,0x3A,0xB8,0xBA,
	0x0C,0x0E,0x8C,0x8E,0x2C,0x2E,0xAC,0xAE, 0x1C,0x1E,0x9C,0x9E,0x3C,0x3E,0xBC,0xBE,
	0x48,0x4A,0xC8,0xCA,0x68,0x6A,0xE8,0xEA, 0x58,0x5A,0xD8,0xDA,0x78,0x7A,0xF8,0xFA,
	0x4C,0x4E,0xCC,0xCE,0x6C,0x6E,0xEC,0xEE, 0x5C,0x5E,0xDC,0xDE,0x7C,0x7E,0xFC,0xFE,
	0x09,0x0B,0x89,0x8B,0x29,0x2B,0xA9,0xAB, 0x19,0x1B,0x99,0x9B,0x39,0x3B,0xB9,0xBB,
	0x0D,0x0F,0x8D,0x8F,0x2D,0x2F,0xAD,0xAF, 0x1D,0x1F,0x9D,0x9F,0x3D,0x3F,0xBD,0xBF,
	0x49,0x4B,0xC9,0xCB,0x69,0x6B,0xE9,0xEB, 0x59,0x5B,0xD9,0xDB,0x79,0x7B,0xF9,0xFB,
	0x4D,0x4F,0xCD,0xCF,0x6D,0x6F,0xED,0xEF, 0x5D,0x5F,0xDD,0xDF,0x7D,0x7F,0xFD,0xFF
};

typedef struct {
	uint8_t kk[57];
	uint16_t sp[256];
} BlockCipher;

#define NIBBLE_KEEP_MASK 0xFFFFFFFFFF0ULL

typedef struct {
	uint64_t A, B;
	uint8_t X, Y, Z, D, E, F, p, q, r;
} StreamCipher;

typedef struct {
	BlockCipher blk;
	StreamCipher st;
} CsaCipher;

static void init_block_cipher(BlockCipher *c, const uint8_t *key) {
	int i, j, k;
	int bit[64], newbit[64];
	int kb[8][9];

	for (j = 1; j <= 8; j++)
		kb[7][j] = key[j - 1];

	for (i = 0; i < 7; i++) {
		for (j = 0; j < 8; j++)
			for (k = 0; k < 8; k++) {
				bit[j * 8 + k] = (kb[7 - i][1 + j] >> (7 - k)) & 1;
				newbit[keyPerm[j * 8 + k] - 1] = bit[j * 8 + k];
			}
		for (j = 0; j < 8; j++) {
			kb[6 - i][1 + j] = 0;
			for (k = 0; k < 8; k++)
				kb[6 - i][1 + j] |= newbit[j * 8 + k] << (7 - k);
		}
	}

	for (i = 0; i < 7; i++)
		for (j = 0; j < 8; j++)
			c->kk[1 + i * 8 + j] = (uint8_t)(kb[1 + i][1 + j] ^ i);

	for (i = 0; i < 256; i++)
		c->sp[i] = (uint16_t)((blockPerm[blockSbox[i]] << 8) | blockSbox[i]);
}

#define BLOCK_ROUND(i) do {                                                   \
	unsigned v = c->sp[c->kk[i] ^ R7];                                       \
	unsigned sbox_out = v & 0xFF;                                            \
	unsigned perm_out = v >> 8;                                              \
	unsigned next_R8 = R7;                                                   \
	R7 = (unsigned)(R6 ^ perm_out);                                          \
	R6 = R5;                                                                 \
	R5 = (unsigned)(R4 ^ R8 ^ sbox_out);                                     \
	R4 = (unsigned)(R3 ^ R8 ^ sbox_out);                                     \
	R3 = (unsigned)(R2 ^ R8 ^ sbox_out);                                     \
	R2 = R1;                                                                 \
	R1 = (unsigned)(R8 ^ sbox_out);                                          \
	R8 = next_R8;                                                            \
} while (0)

static void decipher_block(const BlockCipher *c, const uint8_t *ib, uint8_t *bd) {
	unsigned R1 = ib[0], R2 = ib[1], R3 = ib[2], R4 = ib[3];
	unsigned R5 = ib[4], R6 = ib[5], R7 = ib[6], R8 = ib[7];
	int i;

	for (i = 56; i > 0; i -= 14) {
		BLOCK_ROUND(i);
		BLOCK_ROUND(i - 1);
		BLOCK_ROUND(i - 2);
		BLOCK_ROUND(i - 3);
		BLOCK_ROUND(i - 4);
		BLOCK_ROUND(i - 5);
		BLOCK_ROUND(i - 6);
		BLOCK_ROUND(i - 7);
		BLOCK_ROUND(i - 8);
		BLOCK_ROUND(i - 9);
		BLOCK_ROUND(i - 10);
		BLOCK_ROUND(i - 11);
		BLOCK_ROUND(i - 12);
		BLOCK_ROUND(i - 13);
	}

	bd[0] = (uint8_t)R1; bd[1] = (uint8_t)R2;
	bd[2] = (uint8_t)R3; bd[3] = (uint8_t)R4;
	bd[4] = (uint8_t)R5; bd[5] = (uint8_t)R6;
	bd[6] = (uint8_t)R7; bd[7] = (uint8_t)R8;
}

static void init_stream_cipher(StreamCipher *s, const uint8_t *key) {
	s->A = 0; s->B = 0;

	s->A |= (uint64_t)((key[0] >> 4) & 0x0F) << 4;
	s->A |= (uint64_t)(key[0] & 0x0F) << 8;
	s->A |= (uint64_t)((key[1] >> 4) & 0x0F) << 12;
	s->A |= (uint64_t)(key[1] & 0x0F) << 16;
	s->A |= (uint64_t)((key[2] >> 4) & 0x0F) << 20;
	s->A |= (uint64_t)(key[2] & 0x0F) << 24;
	s->A |= (uint64_t)((key[3] >> 4) & 0x0F) << 28;
	s->A |= (uint64_t)(key[3] & 0x0F) << 32;

	s->B |= (uint64_t)((key[4] >> 4) & 0x0F) << 4;
	s->B |= (uint64_t)(key[4] & 0x0F) << 8;
	s->B |= (uint64_t)((key[5] >> 4) & 0x0F) << 12;
	s->B |= (uint64_t)(key[5] & 0x0F) << 16;
	s->B |= (uint64_t)((key[6] >> 4) & 0x0F) << 20;
	s->B |= (uint64_t)(key[6] & 0x0F) << 24;
	s->B |= (uint64_t)((key[7] >> 4) & 0x0F) << 28;
	s->B |= (uint64_t)(key[7] & 0x0F) << 32;

	s->X = s->Y = s->Z = s->D = s->E = s->F = 0;
	s->p = s->q = s->r = 0;
}

#define STREAM_ROUND(ina, inb) do {                                           \
	int s1 = kSbox1[((((A) >> 16) & 1) << 4) | ((((A) >> 6) & 1) << 3) |   \
					((((A) >> 25) & 1) << 2) | ((((A) >> 31) & 1) << 1) | \
					((((A) >> 36) & 1))];                                 \
	int s2 = kSbox2[((((A) >> 9) & 1) << 4) | ((((A) >> 14) & 1) << 3) |   \
					((((A) >> 27) & 1) << 2) | ((((A) >> 28) & 1) << 1) | \
					((((A) >> 37) & 1))];                                 \
	int s3 = kSbox3[((((A) >> 7) & 1) << 4) | ((((A) >> 8) & 1) << 3) |    \
					((((A) >> 21) & 1) << 2) | ((((A) >> 23) & 1) << 1) | \
					((((A) >> 26) & 1))];                                 \
	int s4 = kSbox4[((((A) >> 15) & 1) << 4) | ((((A) >> 5) & 1) << 3) |   \
					((((A) >> 11) & 1) << 2) | ((((A) >> 18) & 1) << 1) | \
					((((A) >> 32) & 1))];                                 \
	int s5 = kSbox5[((((A) >> 22) & 1) << 4) | ((((A) >> 19) & 1) << 3) |  \
					((((A) >> 24) & 1) << 2) | ((((A) >> 33) & 1) << 1) | \
					((((A) >> 38) & 1))];                                 \
	int s6 = kSbox6[((((A) >> 13) & 1) << 4) | ((((A) >> 17) & 1) << 3) |  \
					((((A) >> 20) & 1) << 2) | ((((A) >> 30) & 1) << 1) | \
					((((A) >> 39) & 1))];                                 \
	int s7 = kSbox7[((((A) >> 10) & 1) << 4) | ((((A) >> 12) & 1) << 3) |  \
					((((A) >> 29) & 1) << 2) | ((((A) >> 34) & 1) << 1) | \
					((((A) >> 35) & 1))];                                 \
	int eB = ((((B) >> 12) & 1) ^ (((B) >> 25) & 1) ^                    \
			  (((B) >> 30) & 1) ^ (((B) >> 39) & 1)) << 3;                \
	eB |= ((((B) >> 24) & 1) ^ (((B) >> 33) & 1) ^                       \
		   (((B) >> 15) & 1) ^ (((B) >> 18) & 1)) << 2;                   \
	eB |= ((((B) >> 23) & 1) ^ (((B) >> 34) & 1) ^                       \
		   (((B) >> 16) & 1) ^ (((B) >> 21) & 1)) << 1;                   \
	eB |= ((((B) >> 38) & 1) ^ (((B) >> 27) & 1) ^                       \
		   (((B) >> 13) & 1) ^ (((B) >> 32) & 1));                        \
	int next_A1 = (int)(((A) >> 40) & 0x0F) ^ X ^ (ina);                  \
	int next_B1 = (int)(((B) >> 28) & 0x0F) ^                            \
				  (int)(((B) >> 40) & 0x0F) ^ Y ^ (inb);                  \
	int nrot = ((next_B1 << 1) | ((next_B1 >> 3) & 1)) & 0x0F;            \
	next_B1 = p ? nrot : next_B1;                                         \
	D = (uint8_t)(E ^ Z ^ eB);                                            \
	int next_E = F;                                                       \
	int esum = Z + E + r;                                                 \
	int new_r = (esum >> 4) & 1;                                          \
	F = (uint8_t)(q ? (esum & 0x0F) : E);                                 \
	r = (uint8_t)(q ? new_r : r);                                         \
	E = (uint8_t)next_E;                                                  \
	A = ((A) << 4 | ((uint64_t)next_A1 << 4)) & NIBBLE_KEEP_MASK;          \
	B = ((B) << 4 | ((uint64_t)next_B1 << 4)) & NIBBLE_KEEP_MASK;          \
	X = (uint8_t)(((s4 & 1) << 3) | ((s3 & 1) << 2) | (s2 & 2) |          \
				  ((s1 & 2) >> 1));                                       \
	Y = (uint8_t)(((s6 & 1) << 3) | ((s5 & 1) << 2) | (s4 & 2) |          \
				  ((s3 & 2) >> 1));                                       \
	Z = (uint8_t)(((s2 & 1) << 3) | ((s1 & 1) << 2) | (s6 & 2) |          \
				  ((s5 & 2) >> 1));                                       \
	p = (uint8_t)((s7 & 2) >> 1);                                         \
	q = (uint8_t)(s7 & 1);                                                \
	op = (uint8_t)((op << 2) ^                                              \
		 (((((D ^ (D >> 1)) >> 1) & 2) | ((D ^ (D >> 1)) & 1))));          \
} while (0)

static void seed_stream_cipher(StreamCipher *s, const uint8_t *sb) {
	uint64_t A = s->A, B = s->B;
	uint8_t X = s->X, Y = s->Y, Z = s->Z, D = s->D, E = s->E, F = s->F;
	uint8_t p = s->p, q = s->q, r = s->r, op = 0;

	for (int i = 0; i < 8; i++) {
		int in1 = (sb[i] >> 4) & 0x0F;
		int in2 = sb[i] & 0x0F;
		op = 0;
		STREAM_ROUND(D ^ in1, in2);
		STREAM_ROUND(D ^ in2, in1);
		STREAM_ROUND(D ^ in1, in2);
		STREAM_ROUND(D ^ in2, in1);
	}

	s->A = A; s->B = B;
	s->X = X; s->Y = Y; s->Z = Z; s->D = D; s->E = E; s->F = F;
	s->p = p; s->q = q; s->r = r;
}

static void xor_stream_bytes(StreamCipher *s, uint8_t *data, size_t n) {
	uint64_t A = s->A, B = s->B;
	uint8_t X = s->X, Y = s->Y, Z = s->Z, D = s->D, E = s->E, F = s->F;
	uint8_t p = s->p, q = s->q, r = s->r, op;

	for (size_t i = 0; i < n; i++) {
		op = 0;
		STREAM_ROUND(0, 0);
		STREAM_ROUND(0, 0);
		STREAM_ROUND(0, 0);
		STREAM_ROUND(0, 0);
		data[i] ^= op;
	}

	s->A = A; s->B = B;
	s->X = X; s->Y = Y; s->Z = Z; s->D = D; s->E = E; s->F = F;
	s->p = p; s->q = q; s->r = r;
}

static void loadControlWord(CsaCipher *c, const uint8_t *cw, int full) {
	uint8_t key[8];

	memcpy(key, cw, 8);
	if (!full) {
		key[3] = (uint8_t)(key[0] + key[1] + key[2]);
		key[7] = (uint8_t)(key[4] + key[5] + key[6]);
	}

	init_block_cipher(&c->blk, key);
	init_stream_cipher(&c->st, key);
}

static void decipherBlockChain(const BlockCipher *blk, uint8_t *data, size_t size) {
	size_t nblocks = size / 8;
	uint8_t *prev;

	if (!nblocks) { return; }

	decipher_block(blk, data, data);

	prev = data;
	for (size_t i = 1; i < nblocks; i++) {
		uint8_t *cur = data + 8 * i;

		for (size_t k = 0; k < 8; k++)
			prev[k] ^= cur[k];
		decipher_block(blk, cur, cur);
		prev = cur;
	}
}

static void decipher_payload(const CsaCipher *c, uint8_t *data, size_t size) {
	StreamCipher sc;

	if (size < 8) { return; }

	sc = c->st;
	seed_stream_cipher(&sc, data);
	xor_stream_bytes(&sc, data + 8, size - 8);
	decipherBlockChain(&c->blk, data, size);
}

#define ALL_BITS (~(uint64_t)0)

static inline void bs_sbox1(uint64_t x0, uint64_t x1, uint64_t x2,
							uint64_t x3, uint64_t x4,
							uint64_t *r0, uint64_t *r1) {
	uint64_t p1_05 = x2 & x4;
	uint64_t p1_09 = x1 & x4;
	uint64_t p1_0a = x1 & x3;
	uint64_t p1_0b = p1_0a & x4;
	uint64_t p1_11 = x0 & x4;
	uint64_t p1_18 = x0 & x1;
	uint64_t p1_1a = p1_18 & x3;
	uint64_t p1_1c = p1_18 & x2;
	uint64_t p1_1d = p1_1c & x4;
	uint64_t p1_03 = x3 & x4;
	uint64_t p1_06 = x2 & x3;
	uint64_t p1_0c = x1 & x2;
	uint64_t p1_0d = p1_0c & x4;
	uint64_t p1_0e = p1_0c & x3;
	uint64_t p1_12 = x0 & x3;
	uint64_t p1_13 = p1_12 & x4;
	uint64_t p1_14 = x0 & x2;
	uint64_t p1_16 = p1_14 & x3;
	uint64_t p1_1b = p1_1a & x4;
	uint64_t p1_1e = p1_1c & x3;
	*r0 = (((((((((x3 ^ p1_05) ^ x1) ^ p1_09) ^ p1_0b) ^ p1_11) ^ p1_18) ^ p1_1a) ^ p1_1c) ^ p1_1d);
	*r1 = (((((((((((((((((((ALL_BITS ^ x4) ^ x3) ^ p1_03) ^ p1_05) ^ p1_06) ^ p1_09) ^ p1_0a) ^ p1_0c) ^ p1_0d) ^ p1_0e) ^ x0) ^ p1_13) ^ p1_14) ^ p1_16) ^ p1_18) ^ p1_1a) ^ p1_1b) ^ p1_1c) ^ p1_1e);
}

static inline void bs_sbox2(uint64_t x0, uint64_t x1, uint64_t x2,
							uint64_t x3, uint64_t x4,
							uint64_t *r0, uint64_t *r1) {
	uint64_t p2_05 = x2 & x4;
	uint64_t p2_0a = x1 & x3;
	uint64_t p2_0b = p2_0a & x4;
	uint64_t p2_0c = x1 & x2;
	uint64_t p2_0d = p2_0c & x4;
	uint64_t p2_12 = x0 & x3;
	uint64_t p2_13 = p2_12 & x4;
	uint64_t p2_14 = x0 & x2;
	uint64_t p2_18 = x0 & x1;
	uint64_t p2_1a = p2_18 & x3;
	uint64_t p2_1b = p2_1a & x4;
	uint64_t p2_1c = p2_18 & x2;
	uint64_t p2_1d = p2_1c & x4;
	uint64_t p2_06 = x2 & x3;
	uint64_t p2_07 = p2_06 & x4;
	uint64_t p2_16 = p2_14 & x3;
	uint64_t p2_19 = p2_18 & x4;
	*r0 = ((((((((((ALL_BITS ^ x3) ^ x2) ^ p2_05) ^ p2_0b) ^ p2_0d) ^ p2_13) ^ p2_14) ^ p2_18) ^ p2_1b) ^ p2_1d);
	*r1 = (((((((((((ALL_BITS ^ x4) ^ x3) ^ p2_05) ^ p2_06) ^ p2_07) ^ x1) ^ p2_16) ^ p2_19) ^ p2_1a) ^ p2_1b) ^ p2_1c);
}

static inline void bs_sbox3(uint64_t x0, uint64_t x1, uint64_t x2,
							uint64_t x3, uint64_t x4,
							uint64_t *r0, uint64_t *r1) {
	uint64_t p3_03 = x3 & x4;
	uint64_t p3_05 = x2 & x4;
	uint64_t p3_06 = x2 & x3;
	uint64_t p3_07 = p3_06 & x4;
	uint64_t p3_09 = x1 & x4;
	uint64_t p3_0a = x1 & x3;
	uint64_t p3_0b = p3_0a & x4;
	uint64_t p3_0c = x1 & x2;
	uint64_t p3_0e = p3_0c & x3;
	uint64_t p3_12 = x0 & x3;
	uint64_t p3_13 = p3_12 & x4;
	uint64_t p3_14 = x0 & x2;
	uint64_t p3_15 = p3_14 & x4;
	uint64_t p3_16 = p3_14 & x3;
	uint64_t p3_17 = p3_16 & x4;
	uint64_t p3_18 = x0 & x1;
	uint64_t p3_19 = p3_18 & x4;
	uint64_t p3_1c = p3_18 & x2;
	uint64_t p3_1e = p3_1c & x3;
	*r0 = ((((x3 ^ p3_03) ^ p3_05) ^ x1) ^ x0);
	*r1 = (((((((((((((((((((((ALL_BITS ^ x4) ^ x3) ^ p3_05) ^ p3_06) ^ p3_07) ^ x1) ^ p3_09) ^ p3_0a) ^ p3_0b) ^ p3_0c) ^ p3_0e) ^ x0) ^ p3_12) ^ p3_13) ^ p3_14) ^ p3_15) ^ p3_16) ^ p3_17) ^ p3_19) ^ p3_1c) ^ p3_1e);
}

static inline void bs_sbox4(uint64_t x0, uint64_t x1, uint64_t x2,
							uint64_t x3, uint64_t x4,
							uint64_t *r0, uint64_t *r1) {
	uint64_t p4_03 = x3 & x4;
	uint64_t p4_09 = x1 & x4;
	uint64_t p4_0a = x1 & x3;
	uint64_t p4_0b = p4_0a & x4;
	uint64_t p4_0c = x1 & x2;
	uint64_t p4_11 = x0 & x4;
	uint64_t p4_12 = x0 & x3;
	uint64_t p4_14 = x0 & x2;
	uint64_t p4_16 = p4_14 & x3;
	uint64_t p4_17 = p4_16 & x4;
	uint64_t p4_18 = x0 & x1;
	uint64_t p4_19 = p4_18 & x4;
	uint64_t p4_1a = p4_18 & x3;
	uint64_t p4_1b = p4_1a & x4;
	uint64_t p4_1c = p4_18 & x2;
	uint64_t p4_1e = p4_1c & x3;
	uint64_t p4_06 = x2 & x3;
	uint64_t p4_07 = p4_06 & x4;
	uint64_t p4_0e = p4_0c & x3;
	*r0 = ((((((((((((((ALL_BITS ^ x3) ^ p4_03) ^ x2) ^ p4_09) ^ p4_0b) ^ p4_0c) ^ p4_11) ^ p4_12) ^ p4_17) ^ p4_18) ^ p4_19) ^ p4_1b) ^ p4_1c) ^ p4_1e);
	*r1 = (((((((((((((((ALL_BITS ^ x4) ^ p4_03) ^ x2) ^ p4_07) ^ x1) ^ p4_0e) ^ x0) ^ p4_11) ^ p4_12) ^ p4_17) ^ p4_18) ^ p4_19) ^ p4_1b) ^ p4_1c) ^ p4_1e);
}

static inline void bs_sbox5(uint64_t x0, uint64_t x1, uint64_t x2,
							uint64_t x3, uint64_t x4,
							uint64_t *r0, uint64_t *r1) {
	uint64_t p5_03 = x3 & x4;
	uint64_t p5_05 = x2 & x4;
	uint64_t p5_06 = x2 & x3;
	uint64_t p5_07 = p5_06 & x4;
	uint64_t p5_09 = x1 & x4;
	uint64_t p5_0a = x1 & x3;
	uint64_t p5_0c = x1 & x2;
	uint64_t p5_0d = p5_0c & x4;
	uint64_t p5_11 = x0 & x4;
	uint64_t p5_14 = x0 & x2;
	uint64_t p5_15 = p5_14 & x4;
	uint64_t p5_16 = p5_14 & x3;
	uint64_t p5_17 = p5_16 & x4;
	uint64_t p5_18 = x0 & x1;
	uint64_t p5_19 = p5_18 & x4;
	uint64_t p5_1a = p5_18 & x3;
	uint64_t p5_1b = p5_1a & x4;
	uint64_t p5_0b = p5_0a & x4;
	uint64_t p5_0e = p5_0c & x3;
	uint64_t p5_12 = x0 & x3;
	uint64_t p5_1c = p5_18 & x2;
	uint64_t p5_1d = p5_1c & x4;
	uint64_t p5_1e = p5_1c & x3;
	*r0 = (((((((((((((((p5_03 ^ x2) ^ p5_05) ^ p5_07) ^ p5_09) ^ p5_0a) ^ p5_0d) ^ p5_11) ^ p5_14) ^ p5_15) ^ p5_16) ^ p5_17) ^ p5_18) ^ p5_19) ^ p5_1a) ^ p5_1b);
	*r1 = ((((((((((((((((((((ALL_BITS ^ x4) ^ x3) ^ p5_03) ^ p5_05) ^ p5_06) ^ p5_07) ^ x1) ^ p5_09) ^ p5_0b) ^ p5_0d) ^ p5_0e) ^ p5_11) ^ p5_12) ^ p5_14) ^ p5_16) ^ p5_17) ^ p5_19) ^ p5_1a) ^ p5_1d) ^ p5_1e);
}

static inline void bs_sbox6(uint64_t x0, uint64_t x1, uint64_t x2,
							uint64_t x3, uint64_t x4,
							uint64_t *r0, uint64_t *r1) {
	uint64_t p6_06 = x2 & x3;
	uint64_t p6_07 = p6_06 & x4;
	uint64_t p6_0a = x1 & x3;
	uint64_t p6_0c = x1 & x2;
	uint64_t p6_0e = p6_0c & x3;
	uint64_t p6_12 = x0 & x3;
	uint64_t p6_13 = p6_12 & x4;
	uint64_t p6_14 = x0 & x2;
	uint64_t p6_16 = p6_14 & x3;
	uint64_t p6_17 = p6_16 & x4;
	uint64_t p6_18 = x0 & x1;
	uint64_t p6_1a = p6_18 & x3;
	uint64_t p6_1b = p6_1a & x4;
	uint64_t p6_1c = p6_18 & x2;
	uint64_t p6_1e = p6_1c & x3;
	uint64_t p6_05 = x2 & x4;
	uint64_t p6_0b = p6_0a & x4;
	uint64_t p6_0d = p6_0c & x4;
	uint64_t p6_19 = p6_18 & x4;
	*r0 = (((((((((((x4 ^ x2) ^ p6_06) ^ p6_07) ^ p6_0a) ^ p6_0c) ^ p6_0e) ^ p6_13) ^ p6_16) ^ p6_17) ^ p6_1b) ^ p6_1e);
	*r1 = (((((((x3 ^ p6_05) ^ p6_0b) ^ p6_0c) ^ p6_0d) ^ x0) ^ p6_13) ^ p6_19);
}

static inline void bs_sbox7(uint64_t x0, uint64_t x1, uint64_t x2,
							uint64_t x3, uint64_t x4,
							uint64_t *r0, uint64_t *r1) {
	uint64_t p7_03 = x3 & x4;
	uint64_t p7_06 = x2 & x3;
	uint64_t p7_07 = p7_06 & x4;
	uint64_t p7_0c = x1 & x2;
	uint64_t p7_18 = x0 & x1;
	uint64_t p7_1a = p7_18 & x3;
	uint64_t p7_1b = p7_1a & x4;
	uint64_t p7_0a = x1 & x3;
	uint64_t p7_0b = p7_0a & x4;
	uint64_t p7_11 = x0 & x4;
	uint64_t p7_12 = x0 & x3;
	uint64_t p7_13 = p7_12 & x4;
	uint64_t p7_14 = x0 & x2;
	uint64_t p7_16 = p7_14 & x3;
	uint64_t p7_17 = p7_16 & x4;
	uint64_t p7_1c = p7_18 & x2;
	uint64_t p7_1e = p7_1c & x3;
	*r0 = (((((((((x4 ^ p7_03) ^ x2) ^ p7_06) ^ p7_07) ^ x1) ^ p7_0c) ^ x0) ^ p7_1a) ^ p7_1b);
	*r1 = ((((((((((((x4 ^ x3) ^ p7_03) ^ x2) ^ x1) ^ p7_0b) ^ p7_11) ^ p7_13) ^ p7_14) ^ p7_16) ^ p7_17) ^ p7_1b) ^ p7_1e);
}

typedef struct {
	uint64_t A[11][4];
	uint64_t B[11][4];
	uint64_t X[4], Y[4], Z[4], D[4], E[4], F[4];
	uint64_t p, q, r;
} BatchCipher;

static void set_batch_key(BatchCipher *S, const StreamCipher *st) {
	memset(S, 0, sizeof(*S));

	for (int k = 1; k <= 10; k++) {
		unsigned a = (unsigned)((st->A >> (4 * k)) & 0x0F);
		unsigned b = (unsigned)((st->B >> (4 * k)) & 0x0F);
		for (int j = 0; j < 4; j++) {
			S->A[k][j] = (a & (1u << j)) ? ALL_BITS : 0;
			S->B[k][j] = (b & (1u << j)) ? ALL_BITS : 0;
		}
	}

	S->X[0] = (st->X & 1) ? ALL_BITS : 0;
	S->X[1] = (st->X & 2) ? ALL_BITS : 0;
	S->X[2] = (st->X & 4) ? ALL_BITS : 0;
	S->X[3] = (st->X & 8) ? ALL_BITS : 0;
	S->Y[0] = (st->Y & 1) ? ALL_BITS : 0;
	S->Y[1] = (st->Y & 2) ? ALL_BITS : 0;
	S->Y[2] = (st->Y & 4) ? ALL_BITS : 0;
	S->Y[3] = (st->Y & 8) ? ALL_BITS : 0;
	S->Z[0] = (st->Z & 1) ? ALL_BITS : 0;
	S->Z[1] = (st->Z & 2) ? ALL_BITS : 0;
	S->Z[2] = (st->Z & 4) ? ALL_BITS : 0;
	S->Z[3] = (st->Z & 8) ? ALL_BITS : 0;
	S->D[0] = (st->D & 1) ? ALL_BITS : 0;
	S->D[1] = (st->D & 2) ? ALL_BITS : 0;
	S->D[2] = (st->D & 4) ? ALL_BITS : 0;
	S->D[3] = (st->D & 8) ? ALL_BITS : 0;
	S->E[0] = (st->E & 1) ? ALL_BITS : 0;
	S->E[1] = (st->E & 2) ? ALL_BITS : 0;
	S->E[2] = (st->E & 4) ? ALL_BITS : 0;
	S->E[3] = (st->E & 8) ? ALL_BITS : 0;
	S->F[0] = (st->F & 1) ? ALL_BITS : 0;
	S->F[1] = (st->F & 2) ? ALL_BITS : 0;
	S->F[2] = (st->F & 4) ? ALL_BITS : 0;
	S->F[3] = (st->F & 8) ? ALL_BITS : 0;
	S->p = st->p ? ALL_BITS : 0;
	S->q = st->q ? ALL_BITS : 0;
	S->r = st->r ? ALL_BITS : 0;
}

#define LANE_ROUND_HEAD                                                       \
	uint64_t s1a, s1b, s2a, s2b, s3a, s3b, s4a, s4b;                       \
	uint64_t s5a, s5b, s6a, s6b, s7a, s7b;                                 \
	uint64_t nA[4], nB[4], nrot[4], eb[4], oldF[4], sum[4], nr;            \
	int b;                                                                  \
	bs_sbox1(A[4][0], A[1][2], A[6][1], A[7][3], A[9][0], &s1a, &s1b);     \
	bs_sbox2(A[2][1], A[3][2], A[6][3], A[7][0], A[9][1], &s2a, &s2b);     \
	bs_sbox3(A[1][3], A[2][0], A[5][1], A[5][3], A[6][2], &s3a, &s3b);     \
	bs_sbox4(A[3][3], A[1][1], A[2][3], A[4][2], A[8][0], &s4a, &s4b);     \
	bs_sbox5(A[5][2], A[4][3], A[6][0], A[8][1], A[9][2], &s5a, &s5b);     \
	bs_sbox6(A[3][1], A[4][1], A[5][0], A[7][2], A[9][3], &s6a, &s6b);     \
	bs_sbox7(A[2][2], A[3][0], A[7][1], A[8][2], A[8][3], &s7a, &s7b);     \
	eb[3] = B[3][0] ^ B[6][1] ^ B[7][2] ^ B[9][3];                         \
	eb[2] = B[6][0] ^ B[8][1] ^ B[3][3] ^ B[4][2];                         \
	eb[1] = B[5][3] ^ B[8][2] ^ B[4][0] ^ B[5][1];                         \
	eb[0] = B[9][2] ^ B[6][3] ^ B[3][1] ^ B[8][0];                         \
	for (b = 0; b < 4; b++) {                                               \
		nA[b] = A[10][b] ^ X[b];                                            \
		nB[b] = B[7][b] ^ B[10][b] ^ Y[b];                                  \
	}

#define LANE_ROUND_TAIL                                                       \
	nrot[0] = nB[3]; nrot[1] = nB[0]; nrot[2] = nB[1]; nrot[3] = nB[2];    \
	for (b = 0; b < 4; b++)                                                 \
		nB[b] ^= (nrot[b] ^ nB[b]) & p;                                     \
	for (b = 0; b < 4; b++)                                                 \
		D[b] = E[b] ^ Z[b] ^ eb[b];                                        \
	{                                                                       \
		uint64_t carry = r;                                                 \
		for (b = 0; b < 4; b++) {                                           \
			uint64_t x = Z[b] ^ E[b];                                       \
			sum[b] = x ^ carry;                                             \
			carry = (Z[b] & E[b]) | (carry & x);                            \
		}                                                                   \
		nr = carry;                                                         \
	}                                                                       \
	for (b = 0; b < 4; b++) oldF[b] = F[b];                                 \
	for (b = 0; b < 4; b++)                                                 \
		F[b] = E[b] ^ ((sum[b] ^ E[b]) & q);                                \
	r ^= (nr ^ r) & q;                                                      \
	for (b = 0; b < 4; b++) E[b] = oldF[b];                                 \
	for (int k = 10; k >= 2; k--) {                                         \
		for (b = 0; b < 4; b++) {                                           \
			A[k][b] = A[k - 1][b];                                          \
			B[k][b] = B[k - 1][b];                                          \
		}                                                                   \
	}                                                                       \
	for (b = 0; b < 4; b++) { A[1][b] = nA[b]; B[1][b] = nB[b]; }          \
	X[3] = s4a; X[2] = s3a; X[1] = s2b; X[0] = s1b;                        \
	Y[3] = s6a; Y[2] = s5a; Y[1] = s4b; Y[0] = s3b;                        \
	Z[3] = s2a; Z[2] = s1a; Z[1] = s6b; Z[0] = s5b;                        \
	p = s7b; q = s7a;                                                       \
	for (b = 7; b >= 2; b--) op[b] = op[b - 2];                            \
	op[0] = D[0] ^ D[1];                                                    \
	op[1] = D[2] ^ D[3];

#define LANE_ROUND_IV(in1, in2) do {                                        \
	LANE_ROUND_HEAD                                                           \
	for (b = 0; b < 4; b++) {                                               \
		nA[b] ^= D[b] ^ ((in1)[b]);                                         \
		nB[b] ^= ((in2)[b]);                                                \
	}                                                                       \
	LANE_ROUND_TAIL                                                           \
} while (0)

#define LANE_ROUND_NORMAL do {                                                    \
	LANE_ROUND_HEAD                                                           \
	LANE_ROUND_TAIL                                                           \
} while (0)

static void seedBatchCipher(BatchCipher *S, const uint64_t iv[8][8]) {
	uint64_t A[11][4], B[11][4], X[4], Y[4], Z[4], D[4], E[4], F[4], op[8];
	uint64_t p = S->p, q = S->q, r = S->r;

	memcpy(A, S->A, sizeof(A));
	memcpy(B, S->B, sizeof(B));
	memcpy(X, S->X, sizeof(X)); memcpy(Y, S->Y, sizeof(Y));
	memcpy(Z, S->Z, sizeof(Z)); memcpy(D, S->D, sizeof(D));
	memcpy(E, S->E, sizeof(E)); memcpy(F, S->F, sizeof(F));
	memset(op, 0, sizeof(op));

	for (int i = 0; i < 8; i++) {
		uint64_t in1[4], in2[4];
		in1[0] = iv[i][4]; in1[1] = iv[i][5];
		in1[2] = iv[i][6]; in1[3] = iv[i][7];
		in2[0] = iv[i][0]; in2[1] = iv[i][1];
		in2[2] = iv[i][2]; in2[3] = iv[i][3];
		LANE_ROUND_IV(in1, in2);
		LANE_ROUND_IV(in2, in1);
		LANE_ROUND_IV(in1, in2);
		LANE_ROUND_IV(in2, in1);
	}

	memcpy(S->A, A, sizeof(A));
	memcpy(S->B, B, sizeof(B));
	memcpy(S->X, X, sizeof(X)); memcpy(S->Y, Y, sizeof(Y));
	memcpy(S->Z, Z, sizeof(Z)); memcpy(S->D, D, sizeof(D));
	memcpy(S->E, E, sizeof(E)); memcpy(S->F, F, sizeof(F));
	S->p = p; S->q = q; S->r = r;
}

static inline void transpose_bits8(uint8_t *a) {
	unsigned int x, y, t;

	x = ((unsigned)a[0] << 24) | ((unsigned)a[1] << 16) |
		((unsigned)a[2] << 8) | a[3];
	y = ((unsigned)a[4] << 24) | ((unsigned)a[5] << 16) |
		((unsigned)a[6] << 8) | a[7];

	t = (x ^ (x >> 7)) & 0x00AA00AAu; x = x ^ t ^ (t << 7);
	t = (y ^ (y >> 7)) & 0x00AA00AAu; y = y ^ t ^ (t << 7);
	t = (x ^ (x >> 14)) & 0x0000CCCCu; x = x ^ t ^ (t << 14);
	t = (y ^ (y >> 14)) & 0x0000CCCCu; y = y ^ t ^ (t << 14);
	t = (x ^ (y >> 4)) & 0x0F0F0F0Fu; x = x ^ t; y = y ^ (t << 4);

	a[0] = (uint8_t)(x >> 24); a[1] = (uint8_t)(x >> 16);
	a[2] = (uint8_t)(x >> 8);  a[3] = (uint8_t)x;
	a[4] = (uint8_t)(y >> 24); a[5] = (uint8_t)(y >> 16);
	a[6] = (uint8_t)(y >> 8);  a[7] = (uint8_t)y;
}

static inline void extract_batch_bytes(const uint64_t *op, uint8_t *ks) {
	for (int g = 0; g < 8; g++) {
		uint8_t a[8];
		for (int b = 0; b < 8; b++)
			a[b] = (uint8_t)(op[7 - b] >> (8 * g));
		transpose_bits8(a);
		for (int j = 0; j < 8; j++)
			ks[8 * g + j] = a[7 - j];
	}
}

static void xor_batch_bytes(BatchCipher *S, uint8_t *const pkt[64], size_t off,
							 size_t nbytes, int lanes) {
	uint64_t A[11][4], B[11][4], X[4], Y[4], Z[4], D[4], E[4], F[4], op[8];
	uint64_t p = S->p, q = S->q, r = S->r;

	memcpy(A, S->A, sizeof(A));
	memcpy(B, S->B, sizeof(B));
	memcpy(X, S->X, sizeof(X)); memcpy(Y, S->Y, sizeof(Y));
	memcpy(Z, S->Z, sizeof(Z)); memcpy(D, S->D, sizeof(D));
	memcpy(E, S->E, sizeof(E)); memcpy(F, S->F, sizeof(F));

	for (size_t i = 0; i < nbytes; i++) {
		memset(op, 0, sizeof(op));
		LANE_ROUND_NORMAL;
		LANE_ROUND_NORMAL;
		LANE_ROUND_NORMAL;
		LANE_ROUND_NORMAL;

		uint8_t ks[64];
		extract_batch_bytes(op, ks);

		for (int k = 0; k < lanes; k++)
			pkt[k][off + i] ^= ks[k];
	}

	memcpy(S->A, A, sizeof(A));
	memcpy(S->B, B, sizeof(B));
	memcpy(S->X, X, sizeof(X)); memcpy(S->Y, Y, sizeof(Y));
	memcpy(S->Z, Z, sizeof(Z)); memcpy(S->D, D, sizeof(D));
	memcpy(S->E, E, sizeof(E)); memcpy(S->F, F, sizeof(F));
	S->p = p; S->q = q; S->r = r;
}

// ============================================================================
// FIN DE LA ZONA GENERADA POR IA, el resto del codigo es 100% humano (O copiado de internet)
// ============================================================================

#endif
