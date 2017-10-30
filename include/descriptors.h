struct Descr{
    uint8_t bytes[8];
 
    // --- accessor methods
    uint8_t type_attr(){ return bytes[5]; };
};

struct SegDescr{
    uint16_t limit_1;   // limit, bits 0..15
    uint16_t base_1;    // base, bits 0..15
    uint8_t base_2;     // base, bits 16..23
    uint8_t type_attr;  // type_attr
    uint8_t lim_attr;
      //^ bits 0..3: limit, bits 16..19
      //^ bits 4..7: additional data/code attributes
    uint8_t base_3;     // base, bits 24..31
};


