#include <testing/qa_pfb_resampler.h>
#include <testing/qa_pfb_channelizer.h>
#include <complex>
#include <exception>
#include <iostream>
#include <vector>

int main()
{
  std::cout << "Starting Test of SAN-CAFE" << std::endl;

  int result = 0;
  //result |= pfb_resampler::test_1();
  //std::cout << "\n";
  result |= pfb_channelizer::test_1();

  if(!result) {
    std::cout << "All checks have passed\n";
  } else {
    std::cout << "Some checks failed\n";
  }

  return result;
}
