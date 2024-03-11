# CSE420
This project utilizes the linked list (LL) data structure to compare text files, writing each word in common into a new text file.
Based on the assignment, the length of each input file is unknown, therefore each node in the LL must be dynamically allocated in order to prevent the stack from overflowing.
My solution to this problem was to do the following
1. Combine the linked lists by a Tail->Head pointer
2. Sorting the combined list
3. Traverse the list, comparing each node with the previous node
4. If the two nodes have the same value, write it to the output file
