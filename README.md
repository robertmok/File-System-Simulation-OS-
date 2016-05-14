# File-System-Simulation-OS-

###Part 1###



-------------------------------------------------------------------------

###Part 2###

The data structure I used for the file list, file operations, contiguous block allocation table is a multi-dimension char pointer array.  The contiguous block allocation memory is a integer array. The FAT file system will be used with linked list block allocation. It will use the same file list, file operation structure as contigous block allocation.

**File List Structure:**

The file list structure stores all the existing files information. Each file will have a file number, file name, the number of blocks of that file and the size of that file stored in the structure. 

**File Operations Structure:**

The file operations stores the list of generated file operations. It stores the file requested number and the file operation to do on that file.

**Contiguous Block Allocation Table Structure:**

The table stores entries that files are allocated in the memory. Each entry stores the start block location of that file in memory and the file's blocks length. The index to this table is related to the file number. For example, file number 1 will have entry in the table of index 1.

**Contiguous Block Allocation Memory Structure:**

The memory stores the file number in all the allocated blocks given for a file.

**Assumptions**

I assumed that the writing a file that already exists will  always be the same size as before. File size does not change
on write. I also assumed that when the operation is a delete, the memory blocks allocated to that file will be freed after  the file has been deleted.






