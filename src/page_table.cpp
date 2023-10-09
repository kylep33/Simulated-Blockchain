#include "page_table.h"
#include <cstring>

Level::Level(int level, uint32_t mask, uint32_t shift, int num_entries) {
    this->level = level;
    this->mask = mask;
    this->shift = shift;
    this->num_entries = num_entries;

    if (level == 0) {
        mappings = new Mapping[num_entries];
        std::memset(mappings, 0, num_entries * sizeof(Mapping));
    } else {
        children.resize(num_entries, nullptr);
    }
}

Level::~Level() {
    if (level == 0) {
        delete[] mappings;
    } else {
        for (Level* child : children) {
            delete child;
        }
    }
}

Level* Level::get_child(uint32_t vpn) {
    if (level == 0) {
        // Error: leaf node has no children
        return nullptr;
    }
    uint32_t index = (vpn & mask) >> shift;
    return children[index];
}

void Level::set_child(uint32_t vpn, Level* child) {
    if (level == 0) {
        // Error: cannot set child on a leaf node
        return;
    }
    uint32_t index = (vpn & mask) >> shift;
    children[index] = child;
}

void Level::set_mapping(uint32_t vpn, uint32_t pfn) {
    if (level != 0) {
        // Error: cannot set mapping on a non-leaf node
        return;
    }
    uint32_t index = vpn & mask;
    mappings[index].vpn = vpn;
    mappings[index].pfn = pfn;
}

uint32_t Level::get_mapping(uint32_t vpn) {
    if (level != 0) {
        // Error: cannot get mapping from a non-leaf node
        return 0;
    }
    uint32_t index = vpn & mask;
    return mappings[index].pfn;
}