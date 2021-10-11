#include "subnetMaskNumber.h"

int subnetMaskNumber(unsigned char bytes[4]){
  return countOneBits(bytes, 4);
}

#ifdef TEST_SUBNET_MASK_NUMBER
int main(void){

  // test with common network masks and equivalents in slash 
  // notation (e.g. 255.255.255.0 -> /24)

  {
    unsigned char bytes[4] = {255,0,0,0};
    assert(8 == subnetMaskNumber(bytes));
  }

  {
    unsigned char bytes[4] = {255,255,0,0};
    assert(16 == subnetMaskNumber(bytes));
  }

  {
    unsigned char bytes[4] = {255,255,255,0};
    assert(24 == subnetMaskNumber(bytes));
  }

  {
    unsigned char bytes[4] = {255,255,255,128};
    assert(25 == subnetMaskNumber(bytes));
  }

  {
    unsigned char bytes[4] = {255,255,255,192};
    assert(26 == subnetMaskNumber(bytes));
  }

  {
    unsigned char bytes[4] = {255,255,255,224};
    assert(27 == subnetMaskNumber(bytes));
  }

  {
    unsigned char bytes[4] = {255,255,255,240};
    assert(28 == subnetMaskNumber(bytes));
  }

  {
    unsigned char bytes[4] = {255,255,255,248};
    assert(29 == subnetMaskNumber(bytes));
  }

  {
    unsigned char bytes[4] = {255,255,255,252};
    assert(30 == subnetMaskNumber(bytes));
  }

  {
    unsigned char bytes[4] = {255,255,255,254};
    assert(31 == subnetMaskNumber(bytes));
  }

  {
    unsigned char bytes[4] = {255,255,255,255};
    assert(32 == subnetMaskNumber(bytes));
  }

  returnSuccess;

}
#endif

/*
cl /Wall /Fe:test-subnet-mask-number /DTEST_SUBNET_MASK_NUMBER subnetMaskNumber.c countOneBits.c 
*/
