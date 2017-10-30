#include<descriptors.h>

uint32_t SegDescr::base(){
 return
   base_1 |
   (base_2<<16) |
   (base_3<<24);
};
 
uint32_t SegDescr::limit(){
 return
  limit_1 |
  ((lim_attr&0xf)<<16);
};
