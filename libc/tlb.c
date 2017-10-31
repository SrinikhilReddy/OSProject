// Abstract model of a TLB.
 
typedef uintptr_t vaddr_t;
typedef uintptr_t paddr_t;
 
// Flag to mark an entry in the modelled hardware TLB as having been set for use as a valid translation.
#define TLB_ENTRY_FLAGS_INUSE
 
struct tlb_cache_record_t
{
   vaddr_t entry_virtual_address;
   paddr_t relevant_physical_address;
   uint16_t permissions;
};
 
// Instance of a hardware Translation Lookaside Buffer.
struct tlb_cache_record_t   hw_tlb[CPU_MODEL_MAX_TLB_ENTRIES];


// Model routine for a TLB lookup.
 
int tlb_lookup(vaddr_t v, paddr_t *p)
{
   for (int i=0; i<CPU_MODEL_MAX_TLB_ENTRIES; i++)
   {
      if (hw_tlb[i].flags & TLB_ENTRY_FLAGS_INUSE && hw_tlb[i].entry_virtual_address == v)
      {
         *p = hw_tlb[i].relevant_physical_address;
         return 1;
      };
   };
   return 0;
}

__asm__ volatile ("TLBFLSH   %0\n\t"::"r" (virtual_address));

// Modelled function for a flush of the TLB modelled earlier on.
 
void tlb_flush_single(vaddr_t v)
{
   for (int i=0; i<CPU_MODEL_MAX_TLB_ENTRIES; i++)
   {
      if (hw_tlb[i].flags & TLB_ENTRY_FLAGS_INUSE && hw_tlb[i].entry_virtual_address == v)
      {
         ht_tlb[i].flags &= ~TLB_ENTRY_FLAGS_INUSE;
         return;
      };
   };
}


