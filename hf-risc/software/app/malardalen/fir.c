/* MDH WCET BENCHMARK SUITE. */

/* 2012/09/28, Jan Gustafsson <jan.gustafsson@mdh.se>
 * Changes:
 *  - Adam Betts wrote: I'm currently looking at the fir benchmark and noticed something
 * peculiar. The "in_data" array has 701 elements (effectively 700 as the
 * last elements acts as a sentinel value) and "fir_filter_int" requires
 * the length of the input/output arrays (passed in the " in_len"
 * argument). So I was expecting 700 to be passed as this parameter. Indeed it was,
 * but it has since been changed. The code now contains the following 2
 * lines:
 *
 * //  fir_filter_int(in_data,output,700,fir_int,35,285); Changed JG/Ebbe
 * fir_filter_int(in_data,output,10,fir_int,35,285);
 *
 * Can you explain why the change?
 *
 ***
 *
 * Jan G wrote: Since neither Ebbe nor I can explain the change, I propose to change it back.
 * =>
 * Action: Fixed.
 */


/* Execute the integer fir filter from "C Algorithms for DSP".
 *
 * Adapted for WCET benchmarking by IJAE May 2000.
 *
 * Features: nested variable-length loops.
 *           if-statement with branches taken a known number of times
 *
 * Added explicit sizes of the arrays to work-around a bug in the
 * NIC-compiler. Christer Sandberg
 */
/* #define LOOPS 720 */
#define LOOPS 1

#include <hf-risc.h>

/*--------------------------------------------------
 *---- INPUT DATA FOR TESTING
 *--------------------------------------------------*/
long fir_int[36]={
0xfffffffe, 0x1, 0x4, 0x3, 0xfffffffe, 0xfffffffc, 0x2, 0x7, 0x0,
0xfffffff7, 0xfffffffc, 0xc, 0xb, 0xfffffff2, 0xffffffe6, 0xf, 0x59, 0x7f,
0x59, 0xf, 0xffffffe6, 0xfffffff2, 0xb, 0xc, 0xfffffffc, 0xfffffff7, 0x0,
0x7, 0x2, 0xfffffffc, 0xfffffffe, 0x3, 0x4, 0x1, 0xfffffffe, 0};

long in_data[701]={
0x0, 0x0, 0x0, 0x0, 0x7f, 0x79, 0x72, 0x79, 0xd, 0xd,
0x0, 0x3, 0x5, 0x2, 0x3, 0x7f, 0x7f, 0x2, 0x7e, 0x0,
0x1, 0x7e, 0x1, 0x1, 0x7f, 0x0, 0x7f, 0x0, 0x2, 0x1,
0x1, 0x3, 0x1, 0x7f, 0x1, 0x0, 0x1, 0x1, 0x7d, 0x7b,
0x73, 0x6a, 0x77, 0x10, 0xe, 0x1, 0x5, 0x5, 0x5, 0x5,
0x7d, 0x0, 0x2, 0x7d, 0x0, 0x0, 0x7e, 0x1, 0x7e, 0x7f,
0x3, 0x7c, 0x7e, 0x6, 0x0, 0x7e, 0x3, 0x2, 0x7f, 0x7e,
0x7f, 0x2, 0x1, 0x7f, 0x1, 0x1, 0x0, 0x3, 0x0, 0x7f,
0x2, 0x0, 0x7f, 0x3, 0x1, 0x0, 0x0, 0x7d, 0x0, 0x3,
0x0, 0x7e, 0x7f, 0x2, 0x1, 0x7e, 0x0, 0x3, 0x7f, 0x7d,
0x1, 0x1, 0x1, 0x7f, 0x0, 0x5, 0x0, 0x7f, 0x2, 0x7e,
0x7f, 0x2, 0x1, 0x0, 0x7e, 0x0, 0x5, 0x0, 0x7f, 0x0,
0x7e, 0x1, 0x0, 0x7d, 0x1, 0x3, 0x7f, 0x0, 0x0, 0x7e,
0x2, 0x3, 0x7e, 0x7d, 0x72, 0x68, 0x71, 0x5, 0xc, 0x7,
0x2, 0x6, 0xd, 0x5, 0x7d, 0x3, 0x2, 0x7f, 0x0, 0x79,
0x7a, 0x3, 0x7e, 0x7d, 0x0, 0x7d, 0x2, 0x1, 0x7d, 0x8,
0x3, 0x7c, 0x6, 0x0, 0x7a, 0x6, 0x2, 0x7c, 0x3, 0x7e,
0x79, 0x6, 0x5, 0x74, 0x7f, 0xd, 0x7a, 0x78, 0x6, 0x5,
0x1, 0x0, 0x7d, 0x1, 0x4, 0x7c, 0x7f, 0x3, 0x7f, 0x5,
0x3, 0x7a, 0x6, 0xa, 0x76, 0x7c, 0xa, 0x7c, 0x7f, 0x6,
0x79, 0x3, 0xc, 0x75, 0x78, 0xa, 0x0, 0x79, 0x3, 0x7e,
0x7c, 0x6, 0x0, 0x79, 0x2, 0x7e, 0x7f, 0x6, 0x76, 0x7f,
0xd, 0x79, 0x7f, 0x6, 0x79, 0x6, 0x3, 0x71, 0x6, 0xa,
0x73, 0x7f, 0xa, 0x0, 0x7f, 0x7a, 0x7c, 0xa, 0x0, 0x75,
0x7f, 0xc, 0xa, 0x7c, 0x79, 0x9, 0xd, 0x7d, 0x7a, 0x5,
0xb, 0xa, 0x79, 0x7c, 0x16, 0x3, 0x72, 0xd, 0x7, 0x79,
0xc, 0x7, 0x7a, 0xb, 0x7, 0x7a, 0xa, 0x7, 0x79, 0xa,
0x5, 0x75, 0x6, 0x5, 0x79, 0x5, 0x6, 0x1, 0x6, 0x0,
0x7a, 0x2, 0x7, 0x3, 0x7d, 0x1, 0xa, 0x7, 0x2, 0x7f,
0x7f, 0x9, 0x7, 0x79, 0x79, 0x6, 0x8, 0x7d, 0x7a, 0x6,
0xc, 0x6, 0x7d, 0x7f, 0xd, 0x7, 0x79, 0x1, 0x6, 0x7f,
0x7f, 0x2, 0x3, 0x1, 0x7e, 0x1, 0x1, 0x7d, 0x1, 0x0,
0x7d, 0x6, 0x3, 0x7d, 0x5, 0x7, 0x7f, 0x7c, 0x1, 0x6,
0x6, 0x7c, 0x7a, 0x7, 0xa, 0x0, 0x78, 0x1, 0x8, 0x0,
0x79, 0x7a, 0x4, 0xa, 0x0, 0x78, 0x1, 0x6, 0x7a, 0x75,
0x7a, 0x0, 0x0, 0x79, 0x76, 0x7f, 0x7, 0x0, 0x7a, 0x7d,
0x2, 0x4, 0x7c, 0x7a, 0x2, 0x5, 0x7c, 0x7a, 0x7d, 0x7f,
0x0, 0x78, 0x75, 0x7f, 0x0, 0x79, 0x78, 0x79, 0x1, 0x3,
0x79, 0x79, 0x0, 0x0, 0x7f, 0x7f, 0x79, 0x7f, 0x2, 0x7a,
0x7c, 0x7d, 0x7c, 0x7f, 0x7d, 0x79, 0x7d, 0x0, 0x79, 0x7a,
0x7c, 0x7d, 0x0, 0x7d, 0x7d, 0x0, 0x0, 0x0, 0x0, 0x7d,
0x7d, 0x0, 0x7d, 0x7e, 0x0, 0x7e, 0x3, 0x3, 0x7d, 0x1,
0x5, 0x0, 0x7e, 0x7d, 0x7f, 0x3, 0x7d, 0x79, 0x1, 0x2,
0x7d, 0x7f, 0x1, 0x0, 0x0, 0x7f, 0x7f, 0x7e, 0x7f, 0x0,
0x7f, 0x0, 0x7c, 0x7d, 0x0, 0x79, 0x78, 0x7c, 0x7c, 0x7b,
0x7b, 0x7d, 0x7f, 0x0, 0x0, 0x7f, 0x0, 0x1, 0x2, 0x0,
0x7f, 0x0, 0x0, 0x0, 0x7f, 0x7e, 0x0, 0x0, 0x7f, 0x0,
0x2, 0x1, 0x2, 0x6, 0x5, 0x3, 0x6, 0x8, 0x5, 0x2,
0x1, 0x1, 0x3, 0x0, 0x7d, 0x7f, 0x0, 0x7f, 0x7e, 0x0,
0x2, 0x3, 0x2, 0x1, 0x2, 0x3, 0x1, 0x7c, 0x7d, 0x0,
0x0, 0x7e, 0x7c, 0x7f, 0x1, 0x0, 0x7e, 0x7c, 0x7f, 0x1,
0x0, 0x7e, 0x7f, 0x2, 0x3, 0x1, 0x0, 0x4, 0x6, 0x5,
0x6, 0x7, 0xa, 0xa, 0x4, 0x2, 0x5, 0x8, 0x9, 0x8,
0x7, 0xc, 0x14, 0x14, 0x10, 0xe, 0x14, 0x15, 0xf, 0x9,
0x7, 0x4, 0x7e, 0x76, 0x64, 0x41, 0x48, 0x7d, 0x6c, 0x3d,
0x67, 0x10, 0x6, 0x7d, 0x75, 0x7, 0x1d, 0x0, 0x6c, 0x2,
0x7d, 0x78, 0x77, 0x6f, 0x77, 0x1, 0x0, 0x2, 0x7, 0xa,
0x1c, 0x1c, 0x17, 0x23, 0x2f, 0x41, 0x43, 0x4f, 0x55, 0x58,
0x7e, 0x2, 0x4c, 0x10, 0x69, 0x2c, 0xd, 0x74, 0x2a, 0x74,
0x63, 0x29, 0x7c, 0x5e, 0x21, 0x35, 0x46, 0x24, 0x67, 0x35,
0x3c, 0x3c, 0x26, 0x26, 0x2f, 0x47, 0x64, 0x4, 0x13, 0x18,
0x27, 0x2b, 0x30, 0x1b, 0x7f, 0x78, 0x72, 0x68, 0x5c, 0x5a,
0x68, 0x7c, 0x3, 0xd, 0x26, 0x41, 0x51, 0x5a, 0x6a, 0x6c,
0x54, 0x78, 0x9, 0x45, 0x79, 0x1f, 0xb, 0x2e, 0x60, 0xb,
0x66, 0x7f, 0x68, 0x77, 0x4e, 0x46, 0x4a, 0x3b, 0x12, 0x5b,
0x37, 0x31, 0x21, 0xb, 0x12, 0x2e, 0x57, 0x7e, 0x19, 0x22,
0x2b, 0x3f, 0x3a, 0x25, 0xb, 0x79, 0x71, 0x68, 0x61, 0x5c,
0x66, 0x72, 0x6, 0x16, 0x29, 0x41, 0x5e, 0x6d, 0x66, 0x60,
0x6e, 0x17, 0x48, 0x36, 0x12, 0x17, 0x2f, 0x63, 0x78, 0x5c,
0x77, 0x6c, 0x75, 0x41, 0x49, 0x4f, 0x3b, 0xb, 0x54, 0x37,
0 };

long out_data[720]={
0x3, 0xfffffffa, 0xfffffffd, 0x1d, 0x58, 0x89, 0x87, 0x56, 0x20, 0x7,
0x7, 0x4, 0xfffffff9, 0x0, 0x28, 0x5b, 0x6b, 0x4f, 0x2b, 0x21,
0x2d, 0x30, 0x27, 0x27, 0x37, 0x47, 0x42, 0x27, 0x8, 0xfffffff4,
0xfffffff5, 0xd, 0x2e, 0x3b, 0x25, 0x0, 0xfffffff8, 0x1d, 0x59, 0x83,
0x87, 0x6f, 0x4e, 0x2f, 0x12, 0xffffffff, 0xfffffffb, 0x4, 0x15, 0x23,
0x2d, 0x31, 0x2f, 0x29, 0x26, 0x2a, 0x36, 0x48, 0x58, 0x5f,
0x5a, 0x4f, 0x46, 0x41, 0x32, 0x1b, 0x17, 0x37, 0x69, 0x7b,
0x59, 0x2f, 0x24, 0x30, 0x2a, 0x8, 0xfffffff6, 0x7, 0x24, 0x31,
0x2f, 0x33, 0x32, 0x1e, 0x4, 0x7, 0x23, 0x33, 0x21, 0xe,
0x1e, 0x4a, 0x61, 0x4b, 0x21, 0xe, 0x22, 0x49, 0x5e, 0x4d,
0x25, 0xb, 0x18, 0x32, 0x33, 0x15, 0x5, 0x29, 0x64, 0x76,
0x4d, 0x16, 0x9, 0x26, 0x37, 0x23, 0xb, 0x15, 0x3c, 0x52,
0x40, 0x23, 0x1d, 0x2d, 0x36, 0x2d, 0x24, 0x29, 0x32, 0x2c,
0x21, 0x2b, 0x50, 0x7b, 0x8d, 0x73, 0x47, 0x22, 0xf, 0x7,
0xffffffff, 0x0, 0x13, 0x2d, 0x36, 0x2b, 0x23, 0x32, 0x4e, 0x5c,
0x55, 0x4f, 0x55, 0x5c, 0x50, 0x34, 0x20, 0x22, 0x32, 0x38,
0x2f, 0x25, 0x2a, 0x35, 0x32, 0x23, 0x1f, 0x36, 0x57, 0x60,
0x4c, 0x31, 0x2d, 0x40, 0x57, 0x67, 0x67, 0x4c, 0x21, 0x4,
0x8, 0x20, 0x30, 0x2c, 0x33, 0x4e, 0x61, 0x56, 0x39, 0x26,
0x26, 0x2b, 0x2e, 0x38, 0x4a, 0x57, 0x58, 0x5c, 0x5f, 0x50,
0x31, 0x1d, 0x31, 0x58, 0x5d, 0x37, 0x16, 0x23, 0x55, 0x71,
0x56, 0x28, 0x18, 0x30, 0x51, 0x60, 0x5c, 0x52, 0x4f, 0x54,
0x5e, 0x62, 0x57, 0x45, 0x3a, 0x35, 0x26, 0x17, 0x23, 0x47,
0x5d, 0x48, 0x27, 0x30, 0x61, 0x79, 0x5a, 0x31, 0x2d, 0x45,
0x4f, 0x41, 0x3e, 0x48, 0x48, 0x3a, 0x3d, 0x53, 0x55, 0x2f,
0xd, 0x1f, 0x55, 0x69, 0x47, 0x1e, 0x1c, 0x32, 0x3c, 0x31,
0x28, 0x2d, 0x34, 0x32, 0x2e, 0x2e, 0x2f, 0x2d, 0x2f, 0x32,
0x2f, 0x26, 0x23, 0x30, 0x3d, 0x2c, 0x3, 0xffffffef, 0xa, 0x34,
0x39, 0x18, 0xa, 0x28, 0x42, 0x28, 0xfffffffb, 0xfffffffe, 0x37, 0x61,
0x53, 0x32, 0x35, 0x4b, 0x4c, 0x36, 0x36, 0x4e, 0x56, 0x33,
0xe, 0x1b, 0x4e, 0x69, 0x51, 0x22, 0xd, 0x24, 0x4b, 0x5e,
0x4d, 0x2a, 0x12, 0x16, 0x29, 0x35, 0x33, 0x2a, 0x25, 0x26,
0x2f, 0x38, 0x31, 0x1d, 0x1d, 0x42, 0x68, 0x58, 0x1a, 0xffffffff,
0x2a, 0x63, 0x5f, 0x27, 0xa, 0x22, 0x34, 0x1e, 0xb, 0x27,
0x58, 0x5a, 0x2e, 0x10, 0x1b, 0x28, 0x23, 0x31, 0x60, 0x7c,
0x56, 0x1b, 0x1d, 0x5d, 0x81, 0x5c, 0x29, 0x2c, 0x4e, 0x51,
0x35, 0x33, 0x4d, 0x53, 0x32, 0x24, 0x50, 0x86, 0x85, 0x5a,
0x46, 0x5d, 0x6b, 0x5b, 0x4f, 0x63, 0x71, 0x54, 0x2a, 0x2c,
0x50, 0x56, 0x30, 0x1e, 0x4d, 0x8d, 0x90, 0x5b, 0x3a, 0x55,
0x80, 0x89, 0x78, 0x7b, 0x8a, 0x7d, 0x53, 0x3e, 0x5b, 0x83,
0x7f, 0x59, 0x4a, 0x5b, 0x5e, 0x2e, 0xfffffff4, 0xfffffff3, 0x2d, 0x5f,
0x61, 0x50, 0x54, 0x5e, 0x50, 0x30, 0x26, 0x34, 0x32, 0x18,
0x9, 0x27, 0x5b, 0x74, 0x6d, 0x5e, 0x52, 0x40, 0x2d, 0x34,
0x54, 0x5c, 0x31, 0x0, 0xa, 0x56, 0x9c, 0x96, 0x59, 0x2e,
0x38, 0x57, 0x5e, 0x4b, 0x46, 0x5e, 0x78, 0x7c, 0x77, 0x80,
0x8d, 0x7d, 0x4f, 0x2b, 0x2b, 0x33, 0x1e, 0x0, 0x6, 0x28,
0x37, 0x1d, 0x9, 0x24, 0x53, 0x5d, 0x3d, 0x1f, 0x21, 0x29,
0x18, 0xfffffffc, 0xfffffff5, 0x6, 0x12, 0x9, 0xfffffffd, 0x1, 0xf, 0xc,
0xfffffffa, 0xfffffff2, 0x9, 0x32, 0x4d, 0x56, 0x5c, 0x62, 0x53, 0x27,
0x0, 0xfffffffc, 0xc, 0x8, 0xfffffff0, 0xfffffff9, 0x36, 0x6a, 0x55, 0x1c,
0x1b, 0x60, 0x8e, 0x61, 0x15, 0x14, 0x5e, 0x8c, 0x61, 0x1d,
0x1a, 0x52, 0x6b, 0x3d, 0xfffffffb, 0xffffffe8, 0x1, 0x15, 0xc, 0xfffffffe,
0x0, 0xd, 0x11, 0x9, 0x1, 0x1, 0x7, 0xc, 0xb, 0x7,
0x6, 0xd, 0x16, 0x17, 0x10, 0xc, 0x13, 0x1c, 0x13, 0x0,
0x0, 0x26, 0x5f, 0x7b, 0x68, 0x48, 0x48, 0x68, 0x7d, 0x60,
0x2d, 0x19, 0x37, 0x5c, 0x5a, 0x31, 0xf, 0x13, 0x31, 0x4c,
0x5e, 0x71, 0x83, 0x7f, 0x58, 0x20, 0xfffffffd, 0xfffffff8, 0x9, 0x18,
0x19, 0x16, 0x19, 0x27, 0x37, 0x3d, 0x42, 0x4f, 0x62, 0x65,
0x4f, 0x33, 0x2c, 0x36, 0x3d, 0x38, 0x34, 0x40, 0x54, 0x5d,
0x5b, 0x58, 0x59, 0x53, 0x41, 0x32, 0x33, 0x3f, 0x4a, 0x4b,
0x43, 0x33, 0x25, 0x28, 0x3d, 0x4f, 0x44, 0x23, 0xe, 0x16,
0x29, 0x2c, 0x28, 0x39, 0x63, 0x84, 0x7d, 0x5f, 0x56, 0x69,
0x73, 0x56, 0x24, 0xc, 0x1e, 0x40, 0x56, 0x60, 0x69, 0x6e,
0x63, 0x4e, 0x42, 0x44, 0x47, 0x3b, 0x2a, 0x22, 0x2d, 0x45,
0x5d, 0x70, 0x77, 0x6d, 0x5b, 0x4a, 0x3f, 0x37, 0x32, 0x39,
0x43, 0x3d, 0x20, 0x5, 0x10, 0x3f, 0x64, 0x5a, 0x34, 0x21,
0x2e, 0x3e, 0x33, 0x22, 0x30, 0x59, 0x75, 0x71, 0x60, 0x61,
0x67, 0x55, 0x2d, 0x12, 0x1d, 0x43, 0x65, 0x71, 0x6c, 0x5f,
0x53, 0x47, 0x39, 0x29, 0x1c, 0x1e, 0x35, 0x56, 0x6f, 0x74,
0x6f, 0x6a, 0x66, 0x5c, 0x4b, 0x3a, 0x33, 0x36, 0x38, 0x2c,
0 };

// To match size of input
#define OUTSIZE 720



/*--------------------------------------------------
 *--- Prototypes
 *--------------------------------------------------*/


void fir_filter_int(long* in,long* out,long in_len,
                    long* coef,long coef_len,
                    long scale);


/*--------------------------------------------------
 *--- Main Function
 *--------------------------------------------------*/
int _main()
{
  long  output[OUTSIZE];

  fir_filter_int(in_data,output,700,fir_int,35,285); 
//  fir_filter_int(in_data,output,10,fir_int,35,285);Changed JG/Ebbe

  /* Verify results */
  /*for(i=0;i<700;i++)*/
  /* if (output[i]!=out_data[i])*/
  /*{ printf("Error: index %d, data %x != %x\n",*/
  /* i, output[i], out_data[i]); */
  /* break; */
  /* }*/
  return 0;
}

/**************************************************************************
fir_filter_int - Filters int data array based on passed int coefficients.

The length of the input and output arrays are equal
and are allocated by the calller.
The length of the coefficient array is passed.
An integer scale factor (passed) is used to divide the accumulation result.

void fir_filter_int(int *in,int *out,int in_len,
                                  int *coef,int coef_len,int scale)

    in          integer pointer to input array
    out         integer pointer to output array
    in_len      length of input and output arrays
    coef        integer pointer to coefficient array
    coef_len    length of coeffient array
    scale       scale factor to divide after accumulation

No return value.

*************************************************************************/

void fir_filter_int(long* in,long* out,long in_len,
                    long* coef,long coef_len,
                    long scale)
{
  long i,j,coef_len2,acc_length;
  long acc;
  long *in_ptr,*data_ptr,*coef_start,*coef_ptr,*in_end;

  /* set up for coefficients */
  coef_start = coef;
  coef_len2 = (coef_len + 1) >> 1;

  /* set up input data pointers */
  in_end = in + in_len - 1;
  in_ptr = in + coef_len2 - 1;

  /* initial value of accumulation length for startup */
  acc_length = coef_len2;

  for(i = 0 ; i < in_len ; i++) {

    /* set up pointer for accumulation */
    data_ptr = in_ptr;
    coef_ptr = coef_start;

    /* do accumulation and write result with scale factor */

    acc = (long)(*coef_ptr++) * (*data_ptr--);
    for(j = 1 ; j < acc_length ; j++)
      acc += (long)(*coef_ptr++) * (*data_ptr--);
    *out++ = (int)(acc/scale);

    /* check for end case */

    if(in_ptr == in_end) {
      acc_length--;       /* one shorter each time */
      coef_start++;       /* next coefficient each time */
    }

    /* if not at end, then check for startup, add to input pointer */

    else {
      if(acc_length < coef_len) acc_length++;
      in_ptr++;
    }
  }
}

void main(void){
	volatile unsigned int cycles;

	printf("\nFIR benchmark");
	cycles = TIMER0;
	_main();
	cycles = TIMER0 - cycles;
	printf("\nWCET: %d cycles\n", cycles);
	
}

