 - Load Results
 - All tasks can be multithreaded by runtime config
   * Multithread mode: 
     * None (Task will run in single thread)
     * Copy Input (The whole memory region is copied per thread)
     * Share Input (The whole memory region is shared by threads)
     * Chunk input (The input will break into smaller chunks copied per thread)
 - Report HTML
   * Task configurations
   * Topology used
   * Results
   * Statistics