/*
 * siple.hpp
 *
 *  A simple, portable, generic SipHash implementation
 *
 *  System requirements:
 *  - C++89 or later
 *  - long long int
 *  - 8-bit char
 */

#ifndef SIPLE_HPP_
#define SIPLE_HPP_

typedef unsigned long long sip_word;

static inline void
rotl64(sip_word &word, int count)
{
  word = (word << count) | ((word & 0xffffffffffffffffull) >> (64 - count));
}

static inline sip_word
read64(const unsigned char *p)
{
  return(((sip_word)p[0]) |
    ((sip_word)p[1] << 8) |
    ((sip_word)p[2] << 16) |
    ((sip_word)p[3] << 24) |
    ((sip_word)p[4] << 32) |
    ((sip_word)p[5] << 40) |
    ((sip_word)p[6] << 48) |
    ((sip_word)p[7] << 56));
}

static inline void
mix(sip_word &state0, sip_word &state1, sip_word &state2, sip_word &state3)
{
  state0 += state1; state2 += state3;
  rotl64(state1, 13); rotl64(state3, 16);
  state1 ^= state0; state3 ^= state2;
  rotl64(state0, 32);
  state2 += state1; state0 += state3;
  rotl64(state1, 17); rotl64(state3, 21);
  state1 ^= state2; state3 ^= state0;
  rotl64(state2, 32);
}

#define siphash24 siphash<2, 4>
#define siphash48 siphash<4, 8>

template<const int rounds, const int finalisation_rounds>
static inline sip_word
siphash(const void *in, const size_t insize, sip_word key0 = 0, sip_word key1 = 0)
{
  sip_word state0 = key0 ^ 0x736f6d6570736575ull;
  sip_word state1 = key1 ^ 0x646f72616e646f6dull;
  sip_word state2 = key0 ^ 0x6c7967656e657261ull;
  sip_word state3 = key1 ^ 0x7465646279746573ull;

  const unsigned char *char_in = (const unsigned char*)in;
  const int tail_size = insize & 7;
  const unsigned char *main_loop_end = char_in + insize - tail_size;

  // The main loop
  for (; char_in != main_loop_end; char_in += 8)
  {
    sip_word current_word = read64(char_in);

    state3 ^= current_word;
    for (int i = 0; i < rounds; ++i)
      mix(state0, state1, state2, state3);
    state0 ^= current_word;
  }

  // We're left with 0..7 bytes.
  sip_word tail_word = ((sip_word)insize) << 56;

  switch (tail_size)
  {
  case 7: tail_word |= (sip_word)char_in[6] << 48;
  case 6: tail_word |= (sip_word)char_in[5] << 40;
  case 5: tail_word |= (sip_word)char_in[4] << 32;
  case 4: tail_word |= (sip_word)char_in[3] << 24;
  case 3: tail_word |= (sip_word)char_in[2] << 16;
  case 2: tail_word |= (sip_word)char_in[1] << 8;
  case 1: tail_word |= (sip_word)char_in[0];
  case 0: break;
  }

  // Mix-in the tail block.
  state3 ^= tail_word;
  for (int i = 0; i < rounds; ++i)
    mix(state0, state1, state2, state3);
  state0 ^= tail_word;

  // The final mix
  state2 ^= 0xff;
  for (int i = 0; i < finalisation_rounds; ++i)
    mix(state0, state1, state2, state3);

  return (state0 ^ state1 ^ state2  ^ state3) & 0xffffffffffffffffull;
}

#endif /* SIPLE_HPP_ */