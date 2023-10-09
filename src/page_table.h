#include <vector>
#include <cstdint>

struct Mapping
{
    uint32_t vpn;
    uint32_t pfn;
};

class Level
{
public:
    Level(int level, uint32_t mask, uint32_t shift, int num_entries);
    ~Level();

    Level *get_child(uint32_t vpn);
    void set_child(uint32_t vpn, Level *child);
    void set_mapping(uint32_t vpn, uint32_t pfn);
    uint32_t get_mapping(uint32_t vpn);

    // required variables
    // int levelCount //Number of levels
    //  – bitmask [i]: bit mask for level i
    //  – bitShift [i]: # of bits to shift level i page bits
    //  – entryCount [i]: # of possible pages for level i

    // current level

private:
    int level;
    uint32_t mask;
    uint32_t shift;
    int num_entries;
    std::vector<Level *> children;
    Mapping *mappings;
};