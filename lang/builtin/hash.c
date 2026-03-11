#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "./lib/hash.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo HASH_MODULE[] = {
    {MODULE_HASH, HASH_MD5, "__hash_md5__", 1, 1},
    {MODULE_HASH, HASH_SHA1, "__hash_sha1__", 1, 1},
    {MODULE_HASH, HASH_SHA256, "__hash_sha256__", 1, 1},
    {MODULE_HASH, HASH_SHA512, "__hash_sha512__", 1, 1},
};

const int HASH_MODULE_COUNT = sizeof(HASH_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Helpers
//=========================================================

static const char *get_str(Data *d, const char *funcname)
{
   if (!d || d->type == TYPE_NONE || d->type != TYPE_STR)
   {
      throw_error(ERROR_TYPE, "%s() argument must be a string", funcname);
      return NULL;
   }
   return d->str;
}

static void bytes_to_hex(const uint8_t *bytes, int len, char *out)
{
   for (int i = 0; i < len; i++)
      snprintf(out + i * 2, 3, "%02x", bytes[i]);
   out[len * 2] = '\0';
}

//=========================================================
//  MD5
//=========================================================

#define MD5_ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

static const uint32_t MD5_S[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

static const uint32_t MD5_K[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
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
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

static void md5_compute(const uint8_t *msg, size_t len, uint8_t digest[16])
{
   uint32_t a0 = 0x67452301, b0 = 0xefcdab89,
            c0 = 0x98badcfe, d0 = 0x10325476;

   // Padding
   size_t padded_len = ((len + 8) / 64 + 1) * 64;
   uint8_t *padded = calloc(padded_len, 1);
   memcpy(padded, msg, len);
   padded[len] = 0x80;
   uint64_t bit_len = (uint64_t)len * 8;
   memcpy(padded + padded_len - 8, &bit_len, 8);

   for (size_t offset = 0; offset < padded_len; offset += 64)
   {
      uint32_t *M = (uint32_t *)(padded + offset);
      uint32_t A = a0, B = b0, C = c0, D = d0;

      for (int i = 0; i < 64; i++)
      {
         uint32_t F, g;
         if (i < 16)
         {
            F = (B & C) | (~B & D);
            g = i;
         }
         else if (i < 32)
         {
            F = (D & B) | (~D & C);
            g = (5 * i + 1) % 16;
         }
         else if (i < 48)
         {
            F = B ^ C ^ D;
            g = (3 * i + 5) % 16;
         }
         else
         {
            F = C ^ (B | ~D);
            g = (7 * i) % 16;
         }

         F = F + A + MD5_K[i] + M[g];
         A = D;
         D = C;
         C = B;
         B = B + MD5_ROTL(F, MD5_S[i]);
      }

      a0 += A;
      b0 += B;
      c0 += C;
      d0 += D;
   }

   free(padded);
   memcpy(digest, &a0, 4);
   memcpy(digest + 4, &b0, 4);
   memcpy(digest + 8, &c0, 4);
   memcpy(digest + 12, &d0, 4);
}

//=========================================================
//  SHA1
//=========================================================

#define SHA1_ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

static void sha1_compute(const uint8_t *msg, size_t len, uint8_t digest[20])
{
   uint32_t h0 = 0x67452301, h1 = 0xEFCDAB89,
            h2 = 0x98BADCFE, h3 = 0x10325476, h4 = 0xC3D2E1F0;

   size_t padded_len = ((len + 8) / 64 + 1) * 64;
   uint8_t *padded = calloc(padded_len, 1);
   memcpy(padded, msg, len);
   padded[len] = 0x80;

   // Big-endian bit length
   uint64_t bit_len = (uint64_t)len * 8;
   for (int i = 0; i < 8; i++)
      padded[padded_len - 8 + i] = (bit_len >> (56 - i * 8)) & 0xff;

   for (size_t offset = 0; offset < padded_len; offset += 64)
   {
      uint32_t w[80];
      for (int i = 0; i < 16; i++)
      {
         uint8_t *b = padded + offset + i * 4;
         w[i] = ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) | ((uint32_t)b[2] << 8) | (uint32_t)b[3];
      }
      for (int i = 16; i < 80; i++)
         w[i] = SHA1_ROTL(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);

      uint32_t a = h0, b = h1, c = h2, d = h3, e = h4;

      for (int i = 0; i < 80; i++)
      {
         uint32_t f, k;
         if (i < 20)
         {
            f = (b & c) | (~b & d);
            k = 0x5A827999;
         }
         else if (i < 40)
         {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
         }
         else if (i < 60)
         {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
         }
         else
         {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
         }

         uint32_t tmp = SHA1_ROTL(a, 5) + f + e + k + w[i];
         e = d;
         d = c;
         c = SHA1_ROTL(b, 30);
         b = a;
         a = tmp;
      }

      h0 += a;
      h1 += b;
      h2 += c;
      h3 += d;
      h4 += e;
   }

   free(padded);

   // Big-endian output
   uint32_t hh[] = {h0, h1, h2, h3, h4};
   for (int i = 0; i < 5; i++)
      for (int j = 0; j < 4; j++)
         digest[i * 4 + j] = (hh[i] >> (24 - j * 8)) & 0xff;
}

//=========================================================
//  SHA256
//=========================================================

static const uint32_t SHA256_K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

#define SHA256_ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
#define SHA256_CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define SHA256_MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SHA256_S0(x) (SHA256_ROTR(x, 2) ^ SHA256_ROTR(x, 13) ^ SHA256_ROTR(x, 22))
#define SHA256_S1(x) (SHA256_ROTR(x, 6) ^ SHA256_ROTR(x, 11) ^ SHA256_ROTR(x, 25))
#define SHA256_G0(x) (SHA256_ROTR(x, 7) ^ SHA256_ROTR(x, 18) ^ ((x) >> 3))
#define SHA256_G1(x) (SHA256_ROTR(x, 17) ^ SHA256_ROTR(x, 19) ^ ((x) >> 10))

static void sha256_compute(const uint8_t *msg, size_t len, uint8_t digest[32])
{
   uint32_t h[] = {
       0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
       0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

   size_t padded_len = ((len + 8) / 64 + 1) * 64;
   uint8_t *padded = calloc(padded_len, 1);
   memcpy(padded, msg, len);
   padded[len] = 0x80;

   uint64_t bit_len = (uint64_t)len * 8;
   for (int i = 0; i < 8; i++)
      padded[padded_len - 8 + i] = (bit_len >> (56 - i * 8)) & 0xff;

   for (size_t offset = 0; offset < padded_len; offset += 64)
   {
      uint32_t w[64];
      for (int i = 0; i < 16; i++)
      {
         uint8_t *b = padded + offset + i * 4;
         w[i] = ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) | ((uint32_t)b[2] << 8) | (uint32_t)b[3];
      }
      for (int i = 16; i < 64; i++)
         w[i] = SHA256_G1(w[i - 2]) + w[i - 7] + SHA256_G0(w[i - 15]) + w[i - 16];

      uint32_t a = h[0], b = h[1], c = h[2], d = h[3],
               e = h[4], f = h[5], g = h[6], hh = h[7];

      for (int i = 0; i < 64; i++)
      {
         uint32_t t1 = hh + SHA256_S1(e) + SHA256_CH(e, f, g) + SHA256_K[i] + w[i];
         uint32_t t2 = SHA256_S0(a) + SHA256_MAJ(a, b, c);
         hh = g;
         g = f;
         f = e;
         e = d + t1;
         d = c;
         c = b;
         b = a;
         a = t1 + t2;
      }

      h[0] += a;
      h[1] += b;
      h[2] += c;
      h[3] += d;
      h[4] += e;
      h[5] += f;
      h[6] += g;
      h[7] += hh;
   }

   free(padded);
   for (int i = 0; i < 8; i++)
      for (int j = 0; j < 4; j++)
         digest[i * 4 + j] = (h[i] >> (24 - j * 8)) & 0xff;
}

//=========================================================
//  SHA512
//=========================================================

#define SHA512_ROTR(x, n) (((x) >> (n)) | ((x) << (64 - (n))))
#define SHA512_CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define SHA512_MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SHA512_S0(x) (SHA512_ROTR(x, 28) ^ SHA512_ROTR(x, 34) ^ SHA512_ROTR(x, 39))
#define SHA512_S1(x) (SHA512_ROTR(x, 14) ^ SHA512_ROTR(x, 18) ^ SHA512_ROTR(x, 41))
#define SHA512_G0(x) (SHA512_ROTR(x, 1) ^ SHA512_ROTR(x, 8) ^ ((x) >> 7))
#define SHA512_G1(x) (SHA512_ROTR(x, 19) ^ SHA512_ROTR(x, 61) ^ ((x) >> 6))

static const uint64_t SHA512_K[] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
    0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
    0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
    0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
    0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
    0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
    0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
    0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
    0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
    0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
    0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
    0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL};

static void sha512_compute(const uint8_t *msg, size_t len, uint8_t digest[64])
{
   uint64_t h[] = {
       0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
       0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
       0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
       0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL};

   size_t padded_len = ((len + 16) / 128 + 1) * 128;
   uint8_t *padded = calloc(padded_len, 1);
   memcpy(padded, msg, len);
   padded[len] = 0x80;

   uint64_t bit_len = (uint64_t)len * 8;
   for (int i = 0; i < 8; i++)
      padded[padded_len - 8 + i] = (bit_len >> (56 - i * 8)) & 0xff;

   for (size_t offset = 0; offset < padded_len; offset += 128)
   {
      uint64_t w[80];
      for (int i = 0; i < 16; i++)
      {
         uint8_t *b = padded + offset + i * 8;
         w[i] = ((uint64_t)b[0] << 56) | ((uint64_t)b[1] << 48) | ((uint64_t)b[2] << 40) | ((uint64_t)b[3] << 32) | ((uint64_t)b[4] << 24) | ((uint64_t)b[5] << 16) | ((uint64_t)b[6] << 8) | (uint64_t)b[7];
      }
      for (int i = 16; i < 80; i++)
         w[i] = SHA512_G1(w[i - 2]) + w[i - 7] + SHA512_G0(w[i - 15]) + w[i - 16];

      uint64_t a = h[0], b = h[1], c = h[2], d = h[3],
               e = h[4], f = h[5], g = h[6], hh = h[7];

      for (int i = 0; i < 80; i++)
      {
         uint64_t t1 = hh + SHA512_S1(e) + SHA512_CH(e, f, g) + SHA512_K[i] + w[i];
         uint64_t t2 = SHA512_S0(a) + SHA512_MAJ(a, b, c);
         hh = g;
         g = f;
         f = e;
         e = d + t1;
         d = c;
         c = b;
         b = a;
         a = t1 + t2;
      }

      h[0] += a;
      h[1] += b;
      h[2] += c;
      h[3] += d;
      h[4] += e;
      h[5] += f;
      h[6] += g;
      h[7] += hh;
   }

   free(padded);
   for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++)
         digest[i * 8 + j] = (h[i] >> (56 - j * 8)) & 0xff;
}

//=========================================================
//  Public Functions
//=========================================================

// md5(s) -> str
// Returns the MD5 hash of a string as a hex string.
// Example: hash.md5("hello") -> "5d41402abc4b2a76b9719d911017c592"
Data *__hash_md5__(Data *s)
{
   const char *str = get_str(s, "hash.md5");
   if (!str)
      return createData(TYPE_NONE, NULL);

   uint8_t digest[16];
   md5_compute((const uint8_t *)str, strlen(str), digest);

   char *hex = malloc(33);
   bytes_to_hex(digest, 16, hex);
   return createData(TYPE_STR, hex);
}

// sha1(s) -> str
// Returns the SHA1 hash of a string as a hex string.
// Example: hash.sha1("hello") -> "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d"
Data *__hash_sha1__(Data *s)
{
   const char *str = get_str(s, "hash.sha1");
   if (!str)
      return createData(TYPE_NONE, NULL);

   uint8_t digest[20];
   sha1_compute((const uint8_t *)str, strlen(str), digest);

   char *hex = malloc(41);
   bytes_to_hex(digest, 20, hex);
   return createData(TYPE_STR, hex);
}

// sha256(s) -> str
// Returns the SHA256 hash of a string as a hex string.
// Example: hash.sha256("hello") -> "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824"
Data *__hash_sha256__(Data *s)
{
   const char *str = get_str(s, "hash.sha256");
   if (!str)
      return createData(TYPE_NONE, NULL);

   uint8_t digest[32];
   sha256_compute((const uint8_t *)str, strlen(str), digest);

   char *hex = malloc(65);
   bytes_to_hex(digest, 32, hex);
   return createData(TYPE_STR, hex);
}

// sha512(s) -> str
// Returns the SHA512 hash of a string as a hex string.
// Example: hash.sha512("hello") -> "9b71d224bd62f3785d96d46ad3ea3d..."
Data *__hash_sha512__(Data *s)
{
   const char *str = get_str(s, "hash.sha512");
   if (!str)
      return createData(TYPE_NONE, NULL);

   uint8_t digest[64];
   sha512_compute((const uint8_t *)str, strlen(str), digest);

   char *hex = malloc(129);
   bytes_to_hex(digest, 64, hex);
   return createData(TYPE_STR, hex);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeHashModule(Runtime *mod_rt)
{
   for (int i = 0; i < HASH_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)HASH_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&HASH_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchHashBuiltin(HashType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case HASH_MD5:
      return __hash_md5__(ARG(0));
   case HASH_SHA1:
      return __hash_sha1__(ARG(0));
   case HASH_SHA256:
      return __hash_sha256__(ARG(0));
   case HASH_SHA512:
      return __hash_sha512__(ARG(0));
   default:
      throw_error(ERROR_SYNTAX, "unknown hash builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}