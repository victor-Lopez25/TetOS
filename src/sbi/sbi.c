#include "sbi.h"

#ifdef SBI_USE_FUNCTIONS
struct sbiret sbi_get_spec_version(void)
{
  return sbi_call(0, 0, 0, 0, 0, 0, 0x0, 0x10);
}
struct sbiret sbi_get_impl_id(void)
{
  return sbi_call(0, 0, 0, 0, 0, 0, 1, 0x10);
}
struct sbiret sbi_get_impl_version(void)
{
  return sbi_call(0, 0, 0, 0, 0, 0, 2, 0x10);
}
struct sbiret sbi_probe_extension(void)
{
  return sbi_call(0, 0, 0, 0, 0, 0, 3, 0x10);
}
struct sbiret sbi_get_mvendorid(void)
{
  return sbi_call(0, 0, 0, 0, 0, 0, 4, 0x10);
}
struct sbiret sbi_get_marchid(void)
{
  return sbi_call(0, 0, 0, 0, 0, 0, 5, 0x10);
}
struct sbiret sbi_get_mimpid(void)
{
  return sbi_call(0, 0, 0, 0, 0, 0, 6, 0x10);
}
#endif
