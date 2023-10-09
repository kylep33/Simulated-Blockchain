#include "page_table.h"
#include <iostream>

int main() {
    // Define page table properties
    const int LEVEL_COUNT = 3;
    uint32_t bitmask[LEVEL_COUNT] = {0x1ff, 0x1ff000, 0x3fe00000};
    uint32_t bitshift[LEVEL_COUNT] = {0, 9, 18};
    int entry_count[LEVEL_COUNT] = {512, 512, 512};

    // Create root level
    Level* root_level = new Level(0, bitmask[0], bitshift[0], entry_count[0]);

    // Create child levels
    for (int i = 1; i < LEVEL_COUNT; i++) {
        Level* level = new Level(i, bitmask[i], bitshift[i], entry_count[i]);
        root_level->set_child(0, level);
    }

    std::cout << "Empty page table created." << std::endl;

    // TODO: Perform page table operations

    // Free memory
    delete root_level;

    return 0;
}