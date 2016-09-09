/**
 * Author......: Jens Steube <jens.steube@gmail.com>
 * License.....: MIT
 */

#if defined _MD4_ || defined _DCC2_ || defined _NETNTLMV2_ || defined _KRB5PA_ || defined _MS_DRSR_ || defined _KRB5TGS_

#define MD4_F_S(x,y,z)  (((x) & (y)) | ((~(x)) & (z)))
#define MD4_G_S(x,y,z)  (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define MD4_H_S(x,y,z)  ((x) ^ (y) ^ (z))

#ifdef IS_NV
#define MD4_F(x,y,z)    (((x) & (y)) | ((~(x)) & (z)))
#define MD4_G(x,y,z)    (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define MD4_H(x,y,z)    ((x) ^ (y) ^ (z))
#define MD4_Fo(x,y,z)   (MD4_F((x), (y), (z)))
#define MD4_Go(x,y,z)   (MD4_G((x), (y), (z)))
#endif

#ifdef IS_AMD
#define MD4_F(x,y,z)    (((x) & (y)) | ((~(x)) & (z)))
#define MD4_G(x,y,z)    (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define MD4_H(x,y,z)    ((x) ^ (y) ^ (z))
#define MD4_Fo(x,y,z)   (bitselect ((z), (y), (x)))
#define MD4_Go(x,y,z)   (bitselect ((x), (y), ((x) ^ (z))))
#endif

#ifdef IS_GENERIC
#define MD4_F(x,y,z)    (((x) & (y)) | ((~(x)) & (z)))
#define MD4_G(x,y,z)    (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define MD4_H(x,y,z)    ((x) ^ (y) ^ (z))
#define MD4_Fo(x,y,z)   (MD4_F((x), (y), (z)))
#define MD4_Go(x,y,z)   (MD4_G((x), (y), (z)))
#endif

#define MD4_STEP_S(f,a,b,c,d,x,K,s) \
{                                   \
  a += K;                           \
  a += x;                           \
  a += f (b, c, d);                 \
  a  = rotl32_S (a, s);             \
}

#define MD4_STEP(f,a,b,c,d,x,K,s)   \
{                                   \
  a += K;                           \
  a += x;                           \
  a += f (b, c, d);                 \
  a  = rotl32 (a, s);               \
}

#define MD4_STEP0(f,a,b,c,d,K,s)    \
{                                   \
  a += K;                           \
  a += f (b, c, d);                 \
  a  = rotl32 (a, s);               \
}

#endif

#if defined _MD5_ || defined _MD5H_ || defined _SAPB_ || defined _OLDOFFICE01_ || defined _WPA_ || defined _MD5_SHA1_ || defined _SHA1_MD5_ || defined _NETNTLMV2_ || defined _KRB5PA_ || defined _PBKDF2_MD5_ || defined _KRB5TGS_

#define MD5_F_S(x,y,z)  ((z) ^ ((x) & ((y) ^ (z))))
#define MD5_G_S(x,y,z)  ((y) ^ ((z) & ((x) ^ (y))))
#define MD5_H_S(x,y,z)  ((x) ^ (y) ^ (z))
#define MD5_I_S(x,y,z)  ((y) ^ ((x) | ~(z)))

#ifdef IS_NV
#define MD5_F(x,y,z)    ((z) ^ ((x) & ((y) ^ (z))))
#define MD5_G(x,y,z)    ((y) ^ ((z) & ((x) ^ (y))))
#define MD5_H(x,y,z)    ((x) ^ (y) ^ (z))
#define MD5_I(x,y,z)    ((y) ^ ((x) | ~(z)))
#define MD5_Fo(x,y,z)   (MD5_F((x), (y), (z)))
#define MD5_Go(x,y,z)   (MD5_G((x), (y), (z)))
#endif

#ifdef IS_AMD
#define MD5_F(x,y,z)    ((z) ^ ((x) & ((y) ^ (z))))
#define MD5_G(x,y,z)    ((y) ^ ((z) & ((x) ^ (y))))
#define MD5_H(x,y,z)    ((x) ^ (y) ^ (z))
#define MD5_I(x,y,z)    (bitselect (0xffffffffU, (x), (z)) ^ (y))
#define MD5_Fo(x,y,z)   (bitselect ((z), (y), (x)))
#define MD5_Go(x,y,z)   (bitselect ((y), (x), (z)))
#endif

#ifdef IS_GENERIC
#define MD5_F(x,y,z)    ((z) ^ ((x) & ((y) ^ (z))))
#define MD5_G(x,y,z)    ((y) ^ ((z) & ((x) ^ (y))))
#define MD5_H(x,y,z)    ((x) ^ (y) ^ (z))
#define MD5_I(x,y,z)    ((y) ^ ((x) | ~(z)))
#define MD5_Fo(x,y,z)   (MD5_F((x), (y), (z)))
#define MD5_Go(x,y,z)   (MD5_G((x), (y), (z)))
#endif

#define MD5_STEP_S(f,a,b,c,d,x,K,s) \
{                                   \
  a += K;                           \
  a += x;                           \
  a += f (b, c, d);                 \
  a  = rotl32_S (a, s);             \
  a += b;                           \
}

#define MD5_STEP(f,a,b,c,d,x,K,s)   \
{                                   \
  a += K;                           \
  a += x;                           \
  a += f (b, c, d);                 \
  a  = rotl32 (a, s);               \
  a += b;                           \
}

#define MD5_STEP0(f,a,b,c,d,K,s)    \
{                                   \
  a += K;                           \
  a += f (b, c, d);                 \
  a  = rotl32 (a, s);               \
  a += b;                           \
}
#endif

#if defined _SHA1_ || defined _SAPG_ || defined _OFFICE2007_ || defined _OFFICE2010_ || defined _OLDOFFICE34_ || defined _ANDROIDFDE_ || defined _DCC2_ || defined _WPA_ || defined _MD5_SHA1_ || defined _SHA1_MD5_ || defined _PSAFE2_ || defined _LOTUS8_ || defined _PBKDF2_SHA1_ || defined _RAR3_ || defined _SHA256_SHA1_ || defined _ZIP2_ || defined _AXCRYPT_

#ifdef IS_NV
#define SHA1_F0(x,y,z)  ((z) ^ ((x) & ((y) ^ (z))))
#define SHA1_F1(x,y,z)  ((x) ^ (y) ^ (z))
#define SHA1_F2(x,y,z)  (((x) & (y)) | ((z) & ((x) ^ (y))))
#define SHA1_F0o(x,y,z) (SHA1_F0 ((x), (y), (z)))
#define SHA1_F2o(x,y,z) (SHA1_F2 ((x), (y), (z)))
#endif

#ifdef IS_AMD
#define SHA1_F0(x,y,z)  ((z) ^ ((x) & ((y) ^ (z))))
#define SHA1_F1(x,y,z)  ((x) ^ (y) ^ (z))
#define SHA1_F2(x,y,z)  (((x) & (y)) | ((z) & ((x) ^ (y))))
#define SHA1_F0o(x,y,z) (bitselect ((z), (y), (x)))
#define SHA1_F2o(x,y,z) (bitselect ((x), (y), ((x) ^ (z))))
#endif

#ifdef IS_GENERIC
#define SHA1_F0(x,y,z)  ((z) ^ ((x) & ((y) ^ (z))))
#define SHA1_F1(x,y,z)  ((x) ^ (y) ^ (z))
#define SHA1_F2(x,y,z)  (((x) & (y)) | ((z) & ((x) ^ (y))))
#define SHA1_F0o(x,y,z) (SHA1_F0 ((x), (y), (z)))
#define SHA1_F2o(x,y,z) (SHA1_F2 ((x), (y), (z)))
#endif

#define SHA1_STEP_S(f,a,b,c,d,e,x)  \
{                                   \
  e += K;                           \
  e += x;                           \
  e += f (b, c, d);                 \
  e += rotl32_S (a,  5u);           \
  b  = rotl32_S (b, 30u);           \
}

#define SHA1_STEP(f,a,b,c,d,e,x)    \
{                                   \
  e += K;                           \
  e += x;                           \
  e += f (b, c, d);                 \
  e += rotl32 (a,  5u);             \
  b  = rotl32 (b, 30u);             \
}

#define SHA1_STEP0(f,a,b,c,d,e,x)   \
{                                   \
  e += K;                           \
  e += f (b, c, d);                 \
  e += rotl32 (a,  5u);             \
  b  = rotl32 (b, 30u);             \
}

#define SHA1_STEPX(f,a,b,c,d,e,x)   \
{                                   \
  e += x;                           \
  e += f (b, c, d);                 \
  e += rotl32 (a,  5u);             \
  b  = rotl32 (b, 30u);             \
}

#define SHA1_STEP_PE(f,a,b,c,d,e,x) \
{                                   \
  e += x;                           \
  e += f (b, c, d);                 \
  e += rotl32 (a,  5u);             \
}

#define SHA1_STEP_PB(f,a,b,c,d,e,x) \
{                                   \
  e += K;                           \
  b  = rotl32 (b, 30u);             \
}
#endif

#if defined _SHA256_ || defined _PDF17L8_ || defined _SEVEN_ZIP_ || defined _ANDROIDFDE_ || defined _CLOUDKEY_ || defined _SCRYPT_ || defined _PBKDF2_SHA256_  || defined _SHA256_SHA1_ || defined _MS_DRSR_ || defined _ANDROIDFDE_SAMSUNG_ || defined _RAR5_ || defined _KEEPASS_

#define SHIFT_RIGHT_32(x,n) ((x) >> (n))

#define SHA256_S0_S(x) (rotl32_S ((x), 25u) ^ rotl32_S ((x), 14u) ^ SHIFT_RIGHT_32 ((x),  3u))
#define SHA256_S1_S(x) (rotl32_S ((x), 15u) ^ rotl32_S ((x), 13u) ^ SHIFT_RIGHT_32 ((x), 10u))
#define SHA256_S2_S(x) (rotl32_S ((x), 30u) ^ rotl32_S ((x), 19u) ^ rotl32_S ((x), 10u))
#define SHA256_S3_S(x) (rotl32_S ((x), 26u) ^ rotl32_S ((x), 21u) ^ rotl32_S ((x),  7u))

#define SHA256_S0(x) (rotl32 ((x), 25u) ^ rotl32 ((x), 14u) ^ SHIFT_RIGHT_32 ((x),  3u))
#define SHA256_S1(x) (rotl32 ((x), 15u) ^ rotl32 ((x), 13u) ^ SHIFT_RIGHT_32 ((x), 10u))
#define SHA256_S2(x) (rotl32 ((x), 30u) ^ rotl32 ((x), 19u) ^ rotl32 ((x), 10u))
#define SHA256_S3(x) (rotl32 ((x), 26u) ^ rotl32 ((x), 21u) ^ rotl32 ((x),  7u))

#ifdef IS_NV
#define SHA256_F0(x,y,z)  (((x) & (y)) | ((z) & ((x) ^ (y))))
#define SHA256_F1(x,y,z)  ((z) ^ ((x) & ((y) ^ (z))))
#define SHA256_F0o(x,y,z) (SHA256_F0 ((x), (y), (z)))
#define SHA256_F1o(x,y,z) (SHA256_F1 ((x), (y), (z)))
#endif

#ifdef IS_AMD
#define SHA256_F0(x,y,z)  (((x) & (y)) | ((z) & ((x) ^ (y))))
#define SHA256_F1(x,y,z)  ((z) ^ ((x) & ((y) ^ (z))))
#define SHA256_F0o(x,y,z) (bitselect ((x), (y), ((x) ^ (z))))
#define SHA256_F1o(x,y,z) (bitselect ((z), (y), (x)))
#endif

#ifdef IS_GENERIC
#define SHA256_F0(x,y,z)  (((x) & (y)) | ((z) & ((x) ^ (y))))
#define SHA256_F1(x,y,z)  ((z) ^ ((x) & ((y) ^ (z))))
#define SHA256_F0o(x,y,z) (SHA256_F0 ((x), (y), (z)))
#define SHA256_F1o(x,y,z) (SHA256_F1 ((x), (y), (z)))
#endif

#define SHA256_STEP_S(F0,F1,a,b,c,d,e,f,g,h,x,K)  \
{                                                 \
  h += K;                                         \
  h += x;                                         \
  h += SHA256_S3_S (e);                           \
  h += F1 (e,f,g);                                \
  d += h;                                         \
  h += SHA256_S2_S (a);                           \
  h += F0 (a,b,c);                                \
}

#define SHA256_EXPAND_S(x,y,z,w) (SHA256_S1_S (x) + y + SHA256_S0_S (z) + w)

#define SHA256_STEP(F0,F1,a,b,c,d,e,f,g,h,x,K)    \
{                                                 \
  h += K;                                         \
  h += x;                                         \
  h += SHA256_S3 (e);                             \
  h += F1 (e,f,g);                                \
  d += h;                                         \
  h += SHA256_S2 (a);                             \
  h += F0 (a,b,c);                                \
}

#define SHA256_EXPAND(x,y,z,w) (SHA256_S1 (x) + y + SHA256_S0 (z) + w)

#endif

#if defined _SHA384_ || defined _PDF17L8_

#define SHIFT_RIGHT_64(x,n) ((x) >> (n))

#define SHA384_S0(x) (rotr64 ((x), 28) ^ rotr64 ((x), 34) ^ rotr64 ((x), 39))
#define SHA384_S1(x) (rotr64 ((x), 14) ^ rotr64 ((x), 18) ^ rotr64 ((x), 41))
#define SHA384_S2(x) (rotr64 ((x),  1) ^ rotr64 ((x),  8) ^ SHIFT_RIGHT_64 ((x), 7))
#define SHA384_S3(x) (rotr64 ((x), 19) ^ rotr64 ((x), 61) ^ SHIFT_RIGHT_64 ((x), 6))

#define SHA384_F0(x,y,z) ((z) ^ ((x) & ((y) ^ (z))))
#define SHA384_F1(x,y,z) (((x) & (y)) | ((z) & ((x) ^ (y))))

#ifdef IS_NV
#define SHA384_F0o(x,y,z) (SHA384_F0 ((x), (y), (z)))
#define SHA384_F1o(x,y,z) (SHA384_F1 ((x), (y), (z)))
#endif

#ifdef IS_AMD
#define SHA384_F0o(x,y,z) (bitselect ((z), (y), (x)))
#define SHA384_F1o(x,y,z) (bitselect ((x), (y), ((x) ^ (z))))
#endif

#ifdef IS_GENERIC
#define SHA384_F0o(x,y,z) (SHA384_F0 ((x), (y), (z)))
#define SHA384_F1o(x,y,z) (SHA384_F1 ((x), (y), (z)))
#endif

#define SHA384_STEP(F0,F1,a,b,c,d,e,f,g,h,x,K)  \
{                                               \
  h += K;                                       \
  h += x;                                       \
  h += SHA384_S1 (e);                           \
  h += F0 (e, f, g);                            \
  d += h;                                       \
  h += SHA384_S0 (a);                           \
  h += F1 (a, b, c);                            \
}

#define SHA384_EXPAND(x,y,z,w) (SHA384_S3 (x) + y + SHA384_S2 (z) + w)
#endif

#if defined _SHA512_ || defined _CLOUDKEY_ || defined _OFFICE2013_ || defined _PDF17L8_ || defined _PBKDF2_SHA512_

#define SHIFT_RIGHT_64(x,n) ((x) >> (n))

#define SHA512_S0_S(x) (rotr64_S ((x), 28) ^ rotr64_S ((x), 34) ^ rotr64_S ((x), 39))
#define SHA512_S1_S(x) (rotr64_S ((x), 14) ^ rotr64_S ((x), 18) ^ rotr64_S ((x), 41))
#define SHA512_S2_S(x) (rotr64_S ((x),  1) ^ rotr64_S ((x),  8) ^ SHIFT_RIGHT_64 ((x), 7))
#define SHA512_S3_S(x) (rotr64_S ((x), 19) ^ rotr64_S ((x), 61) ^ SHIFT_RIGHT_64 ((x), 6))

#define SHA512_S0(x) (rotr64 ((x), 28) ^ rotr64 ((x), 34) ^ rotr64 ((x), 39))
#define SHA512_S1(x) (rotr64 ((x), 14) ^ rotr64 ((x), 18) ^ rotr64 ((x), 41))
#define SHA512_S2(x) (rotr64 ((x),  1) ^ rotr64 ((x),  8) ^ SHIFT_RIGHT_64 ((x), 7))
#define SHA512_S3(x) (rotr64 ((x), 19) ^ rotr64 ((x), 61) ^ SHIFT_RIGHT_64 ((x), 6))

#define SHA512_F0(x,y,z) ((z) ^ ((x) & ((y) ^ (z))))
#define SHA512_F1(x,y,z) (((x) & (y)) | ((z) & ((x) ^ (y))))

#ifdef IS_NV
#define SHA512_F0o(x,y,z) (SHA512_F0 ((x), (y), (z)))
#define SHA512_F1o(x,y,z) (SHA512_F1 ((x), (y), (z)))
#endif

#ifdef IS_AMD
#define SHA512_F0o(x,y,z) (bitselect ((z), (y), (x)))
#define SHA512_F1o(x,y,z) (bitselect ((x), (y), ((x) ^ (z))))
#endif

#ifdef IS_GENERIC
#define SHA512_F0o(x,y,z) (SHA512_F0 ((x), (y), (z)))
#define SHA512_F1o(x,y,z) (SHA512_F1 ((x), (y), (z)))
#endif

#define SHA512_STEP_S(F0,F1,a,b,c,d,e,f,g,h,x,K)  \
{                                                 \
  h += K;                                         \
  h += x;                                         \
  h += SHA512_S1_S (e);                           \
  h += F0 (e, f, g);                              \
  d += h;                                         \
  h += SHA512_S0_S (a);                           \
  h += F1 (a, b, c);                              \
}

#define SHA512_EXPAND_S(x,y,z,w) (SHA512_S3_S (x) + y + SHA512_S2_S (z) + w)

#define SHA512_STEP(F0,F1,a,b,c,d,e,f,g,h,x,K)  \
{                                               \
  h += K;                                       \
  h += x;                                       \
  h += SHA512_S1 (e);                           \
  h += F0 (e, f, g);                            \
  d += h;                                       \
  h += SHA512_S0 (a);                           \
  h += F1 (a, b, c);                            \
}

#define SHA512_EXPAND(x,y,z,w) (SHA512_S3 (x) + y + SHA512_S2 (z) + w)
#endif

#ifdef _RIPEMD160_

#ifdef IS_NV
#define RIPEMD160_F(x,y,z)    ((x) ^ (y) ^ (z))
#define RIPEMD160_G(x,y,z)    ((z) ^ ((x) & ((y) ^ (z)))) /* x ? y : z */
#define RIPEMD160_H(x,y,z)    (((x) | ~(y)) ^ (z))
#define RIPEMD160_I(x,y,z)    ((y) ^ ((z) & ((x) ^ (y)))) /* z ? x : y */
#define RIPEMD160_J(x,y,z)    ((x) ^ ((y) | ~(z)))
#define RIPEMD160_Go(x,y,z)   (RIPEMD160_G ((x), (y), (z)))
#define RIPEMD160_Io(x,y,z)   (RIPEMD160_I ((x), (y), (z)))
#endif

#ifdef IS_AMD
#define RIPEMD160_F(x,y,z)    ((x) ^ (y) ^ (z))
#define RIPEMD160_G(x,y,z)    ((z) ^ ((x) & ((y) ^ (z)))) /* x ? y : z */
#define RIPEMD160_H(x,y,z)    (((x) | ~(y)) ^ (z))
#define RIPEMD160_I(x,y,z)    ((y) ^ ((z) & ((x) ^ (y)))) /* z ? x : y */
#define RIPEMD160_J(x,y,z)    ((x) ^ ((y) | ~(z)))
#define RIPEMD160_Go(x,y,z)   (bitselect ((z), (y), (x)))
#define RIPEMD160_Io(x,y,z)   (bitselect ((y), (x), (z)))
#endif

#ifdef IS_GENERIC
#define RIPEMD160_F(x,y,z)    ((x) ^ (y) ^ (z))
#define RIPEMD160_G(x,y,z)    ((z) ^ ((x) & ((y) ^ (z)))) /* x ? y : z */
#define RIPEMD160_H(x,y,z)    (((x) | ~(y)) ^ (z))
#define RIPEMD160_I(x,y,z)    ((y) ^ ((z) & ((x) ^ (y)))) /* z ? x : y */
#define RIPEMD160_J(x,y,z)    ((x) ^ ((y) | ~(z)))
#define RIPEMD160_Go(x,y,z)   (RIPEMD160_G ((x), (y), (z)))
#define RIPEMD160_Io(x,y,z)   (RIPEMD160_I ((x), (y), (z)))
#endif

#define RIPEMD160_STEP(f,a,b,c,d,e,x,K,s) \
{                                         \
  a += K;                                 \
  a += x;                                 \
  a += f (b, c, d);                       \
  a  = rotl32 (a, s);                     \
  a += e;                                 \
  c  = rotl32 (c, 10u);                   \
}

#define ROTATE_LEFT_WORKAROUND_BUG(a,n) ((a << n) | (a >> (32 - n)))

#define RIPEMD160_STEP_WORKAROUND_BUG(f,a,b,c,d,e,x,K,s)  \
{                                         \
  a += K;                                 \
  a += x;                                 \
  a += f (b, c, d);                       \
  a  = ROTATE_LEFT_WORKAROUND_BUG (a, s); \
  a += e;                                 \
  c  = rotl32 (c, 10u);                   \
}

#endif
