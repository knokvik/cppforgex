void trigger_leak() {
    int* leak_ptr = new int[100]; // Allocated but never deleted
}// Refactored variables
