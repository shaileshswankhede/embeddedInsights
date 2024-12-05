#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// some references: http://www.graphics.stanford.edu/~seander/bithacks.html
// http://aggregate.org/MAGIC

bool isOdd(uint32_t num)
{
    return num & 1;
}

bool isPowerOfTwo(uint32_t num)
{
    // Discard 0. 2^n have single set bit and (2^n-1) will have all rightmost bits set 
    return num && (!(num & (num - 1)));
}

bool isKthBitSet(uint32_t num, uint8_t k)
{
    return num & (1 << k);
}

void setKthBit(uint32_t *num, uint8_t k)
{
    *num |= (1 << k);
}

void unsetKthBit(uint32_t *num, uint8_t k)
{
    *num &= (!(1 << k));
}

void toggleKthBit(uint32_t *num, uint8_t k)
{
    *num ^= (1 << k);
}

void setKLsbBits(uint32_t *num, uint8_t k)
{
    *num |= ((1U << k) - 1);
}

uint8_t numMod2PowK(uint32_t num, uint8_t k)
{
    // all K-1 bits are set in (1 << k) - 1 making it a mask
    return num & ((1 << k) - 1);
}

void swap(uint32_t *num1, uint32_t *num2)
{
    // if num1 and num2 points to same memory location then below calculation won't work and will make it 0
    // in fact calculation not required in that case
    if (num1 != num2)
    {
        *num1 = *num1 ^ *num2;
        *num2 = *num2 ^ *num1;
        *num1 = *num1 ^ *num2;
    }
}

// or shorter macro
// (num1) ^ (num2) results into 0 when num1, num2 represents same value or address, thus avoiding further calculation without branching
#define SWAP(num1, num2) ((num1) ^ (num2)) && ((num1) ^= (num2)), ((num2) ^= (num1)), ((num1) ^= (num2))

void toggle(uint32_t *x, uint32_t num1, uint32_t num2)
{
    // if(x == num1) x = num2; else if (x == num2) x = num1;
    *x = num1 ^ num2 ^ *x;  
}

int sign(int32_t num)
{
    // return -1 if num < 0, 0 if num = 0 & 1 if num > 0
    // return num != 0 | num < 0
    return (num != 0) | (num >> (sizeof(num) * 8 - 1)); // without using branching. Branching is expensive!
}

bool ifOppositeSign(int32_t num1, int32_t num2)
{
    return (num1 ^ num2) < 0;
}

uint32_t abs(int32_t num)
{
    // >> performs arithmatic shift to preserve sign (implementation defined)
    int const mask = num >> (sizeof(num) * 8 - 1);      // mask = -1 (0xFFFFFF...) for negative and 0 for positive
    return (num ^ mask) - mask;                         // toggle - (-1) --> 2's complement
}

int32_t min(int32_t x, int32_t y)
{
    // without using branching
    return y ^ ((x ^ y) & -(x < y));                    // -(x < y) gives -1 (0xFFFFFF...) --> y ^ x ^ y = x      
}

int32_t max(int32_t x, int32_t y)
{
    return x ^ ((x ^ y) & -(x < y));
}

uint32_t countSetBits(uint32_t num)
{
    uint32_t cnt = 0;
    // worst case loop has to run sizeof(num) * 8 = 32 times
    while(num)
    {
        cnt += (num & 1);
        num >>= 1;
    }
    return cnt;
}

uint32_t fastCountSetBits(uint32_t num)
{
    uint32_t cnt = 0;
    // this loop runs exactly equal to number of set bits
    for (uint32_t cnt = 0; num; cnt++)
    {
        num &= (num - 1);   // clears rightmost bit set       
    }
    return cnt;
}

bool getParity(uint32_t num)
{
    // Even parity (0) if even number of set bits and odd parity (1) if odd number of set bits
    bool parity = false;
    while(num)
    {
        parity = !parity;
        num &= (num - 1);
    }
    return parity;
}

uint32_t lsb1Bit(uint32_t num)
{
    // keep only left significant set bit and clear all others
    // 0b1101000 --> 0b0001000
    return num & (-num);
}

uint32_t swapNibbles(uint8_t num)
{
    return ((num >> 4) & 0xf) | ((num << 4) & 0xf0);
}

uint32_t swapBits(uint32_t num, uint8_t i, uint8_t j)
{
    // swap bits at i & j position in num
    uint32_t x = ((num >> i) ^ (num >> j)) & 1;     // if bits at position i, j are same, no point in swapping. x = 1 only when bits different
    return num ^ ((x << i) | (x << j));             // put xor bit back to position 
}

uint32_t swapBitsSet(uint32_t num, uint8_t i, uint8_t j, uint8_t cnt)
{
    // swap cnt bits starting at i & j position in num
    // function will fail if sets are overlapping
    // if num  = 0001 1100, i = 1, i = 4, cnt = 3 --> 
    // swapped = 0110 0010  
    uint32_t x = ((num >> i) ^ (num >> j)) & ((1U << cnt) - 1); // XOR temporary
    return num ^ ((x << i) | (x << j));
}

uint32_t reverseBits(uint32_t num)
{
    uint32_t rev = num;
    uint8_t s = sizeof(num) * 8 - 1;
    // get lsb of num, right to rev. num bits moves in right direction while rev bits move in left direction
    while(num)
    {
        rev <<= 1;
        num >>= 1;
        rev |= (num & 1);
        s--;
    }
    // now s represents additional zeros at left side of left most set bit in original num
    // extra shift by s is needed in order to get 32 bit reverse num
    rev <<= s;
    return rev;
}

uint8_t log2(uint32_t num)
{
    // pow(2, x) = num. Return x
    // using obvious bit shift, keep dividing number by 2 till it becomes 0 and count number of shifts
    // its essentially the position of most significant bit set
    uint8_t cnt = 0;
    while(num >>= 1)
    {
        cnt++;
    }
    return cnt;
}

uint8_t log2LutMethod(uint32_t num)
{
    #define repeat(n)         n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
    const int8_t lut[256] = {-1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
                            repeat(4), repeat(5), repeat(5), repeat(6), repeat(6), repeat(6), repeat(6),
                            repeat(7), repeat(7), repeat(7), repeat(7), repeat(7), repeat(7), repeat(7), repeat(7) };
    // log(0) is written as -1 since its invalid
    // this lut table can be generated programatically using below code
    // lut[0] = -1, lut[1] = 0;
    // for (uint8_t i = 2; i <= 255; i++)
    // {
    //    lut[i] = 1 + lut[i/2];
    // }
    // log(a*b) = log(a) + log(b) --> if we break 32bit number into 8bit chunks, we can shift higher chunk to lower 8 bits
    // and add 8, 16 or 24 based on chunk position
    uint8_t chunk8, ret;
    if(chunk8 = num >> 24)
    {
        ret = 24 + lut[chunk8];
    }
    else if(chunk8 = num >> 16)
    {
        ret = 16 + lut[chunk8];
    }
    else if(chunk8 = num >> 8)
    {
        ret = 8 + lut[chunk8];
    }
    else
    {
        ret = lut[num];
    }

    return ret;
}

uint8_t log10(uint32_t num)
{
    // log10(10) = 1, log10(100) = 2, log10(1000) = 3, ... and so on.
    // for rest of the values log10 value in integer doesn't change
    // This method works well when the input is uniformly distributed over 32-bit values because 76% of the inputs 
    // are caught by the first compare, 21% are caught by the second compare, 2% are caught by the third, and so on 
    // As a result, less than 2.6 operations are needed on average.
    // no brackets required for multiple ternary comparision because of operator precedence
    // if making function macro, follow putting bracket around num like (num) so that it will work for even expression passed to num
    return num >= 100000000 ? 9 : num >= 100000000 ? 8 : num >= 10000000 ? 7 : num > 1000000 ? 6 : num > 100000 ? \
           5 : num > 10000 ? 4 : num > 1000 ? 3 : num > 100 ? 2 : num > 10 ? 1 : 0;
}

uint8_t countTrailingZeros(uint32_t num)
{
    // this method is simillar to simple right shifting number, masking with 0x1 and incrementing count till masked result is 0
    // but this method avoids masking in every iteration. So little bit faster
    uint8_t cnt = 0;
    if (num)
    {
        num = (num ^ (num - 1)) >> 1;  // Set v's trailing 0s to 1s and zero rest
        for (; num; cnt++)
        {
            num >>= 1;
        }
    }
    else
    {
        cnt = 8 * sizeof(num);
    }
    return cnt;
}

uint32_t roundUpPowerOfTwo(uint32_t num)
{
    // Using SWAR method that recursively folds upper bits into lower bits.
    // This yields all 1's below most significant bit set. Adding 1 to that will give next power of 2
    // if number is already power of 2; same number should be returned, thus we can first subtract number by 1 and then use SWAR
    num += (num == 0);      //to handle special case where num = 0, making num initially 1; otherwise solution returns 0 and 0 is not power of 2
    num--;
    num |= (num >> 1);
    num |= (num >> 2);
    num |= (num >> 4);
    num |= (num >> 8);
    num |= (num >> 16);
    return num + 1;
}

uint32_t alignUp2PowK(uint32_t num, uint32_t alignment)
{
    // find number >= 'num' such that number is multiple of 'alignment' 
    // if alignment is power of 2, then we simply add (alignment - 1) and chop off extra bits as per trailing zeros in alignment
    return (num + alignment - 1) & (-alignment);    //-alignment is same as ~alignment + 1 (2's complement)
}

uint32_t interleaveBits(uint16_t x, uint16_t y)
{
    // interleave bits of x and y so that all of the bits of x are in even position and that of y in odd position
    // this interleaving produces morton number and has applications in linear algebra, texture mapping etc
    uint32_t z = 0;
    for (uint32_t i = 0; i < sizeof(x) * 8; i++)
    {
        z |= ((x & (1U << i)) << i) | ((y & (1U << i)) << (i + 1));
    }
    return z;
}

uint32_t interleaveBitsByMagicNumber(uint32_t x, uint32_t y)
{
    static const uint32_t B[] = { 0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF };
    static const uint32_t S[] = { 1, 2, 4, 8 };
    // x and y must be initially fit in uint16_t i.e less than 65536

    x |= ((x << S[3]) & B[3]);
    x |= ((x << S[2]) & B[2]);
    x |= ((x << S[1]) & B[1]);
    x |= ((x << S[0]) & B[0]);

    y |= ((y << S[3]) & B[3]);
    y |= ((y << S[2]) & B[2]);
    y |= ((y << S[1]) & B[1]);
    y |= ((y << S[0]) & B[0]);

    return x | (y << 1);
}

uint32_t ceilDivision(uint32_t a, uint32_t b)
{
    return (a + (b - 1)) / b;
}

uint32_t roundToNearestDivision(uint32_t a, uint32_t b)
{
    return (a + (float)(b/2)) / b;
}

// https://www.geeksforgeeks.org/gray-to-binary-and-binary-to-gray-conversion/
uint32_t binaryToGray(uint32_t binary)
{
    return binary ^ (binary >> 1);
}

uint32_t grayToBinary(uint32_t gray)
{
    uint32_t binary = gray;
    while(gray)
    {
        gray >>= 1;
        binary ^= gray;
    }
    return binary;
}

uint32_t grayToBinaryFast(uint32_t gray)
{
    // using SWAR technique
    gray ^= (gray >> 16);
    gray ^= (gray >> 8);
    gray ^= (gray >> 4);
    gray ^= (gray >> 2);
    gray ^= (gray >> 1);
    return gray;
}

int main()
{
    uint32_t num1 = 12436456;
    uint32_t num2 = 97659809;
    swap(&num1, &num2);
    printf("After swapping: num1 %u num2 %u\n", num1, num2);

    uint32_t x = num1;
    toggle(&x, num1, num2);
    printf("value of x after toggling changed from num1(%u) to %u\n", num1, x);
    toggle(&x, num1, num2);
    printf("Now value of x after toggling changed back to %u\n", x);

    int32_t num = -656787343;
    printf("abs of %d = %u\n", num, abs(num));

    num = 0b00011100;
    printf("result of swapping bits at idx 1 and 3 = 0x%x\n", swapBits(num, 1, 3));
    printf("result of swapping 3 bits starting at idx 1 and 4 = 0x%x\n", swapBitsSet(num, 1, 4, 3));

    num1 = 1177317493;
    printf("log2 of %u = %u\n", num1, log2LutMethod(num1));
    printf("log10 of %u = %u\n", num1, log10(num1));

    num = 0b1101000;
    printf("trailing zeros = %u\n", countTrailingZeros(num));
    printf("Rounded up power of 2 = 0x%x\n", roundUpPowerOfTwo(num));

    num = 530;
    printf("After aligning num to 32 bytes, num = %u\n", alignUp2PowK(num, 32));

    num = 0b100110;
    uint32_t gray = binaryToGray(num);
    printf("binaryToGray = 0x%x\n", gray);
    uint32_t bin = grayToBinary(gray);
    printf("grayToBinary = 0x%x\n", bin);

    return 0;
}
