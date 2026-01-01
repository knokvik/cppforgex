.global _main
.align 4

_main:
    // ----------------------------------------------------
    // STEP 1: Read up to 64 bytes from standard input (stdin)
    // ----------------------------------------------------
    mov     x0, #0              // File descriptor 0 = stdin
    adrp    x1, buffer@PAGE     // Load address of our text buffer
    add     x1, x1, buffer@PAGEOFF
    mov     x2, #64             // Buffer size limit
    mov     x16, #3             // macOS system call number 3 = sys_read
    svc     #05                 // Call the kernel
    mov     x10, x0             // x10 now holds the number of bytes actually read

    // Initialize variables
    adrp    x1, buffer@PAGE     // x1 points to the start of string data
    add     x1, x1, buffer@PAGEOFF
    add     x2, x1, x10         // x2 points to the absolute end of data
    mov     x3, #0              // x3 = running total sum
    mov     x4, #0              // x4 = counter for numbers processed (0 to 2)

parse_loop:
    cmp     x1, x2              // Check if we hit the end of the input buffer
    b.hs    print_result        // If out of characters, go to printing
    cmp     x4, #3              // Have we already handled 3 valid numbers?
    b.eq    print_result        // If yes, stop parsing

    // Skip spaces or newlines
    ldrb    w5, [x1]            // Load current byte into register w5
    cmp     w5, #' '
    b.eq    skip_char
    cmp     w5, #'\n'
    b.eq    skip_char
    cmp     w5, #'\r'
    b.eq    skip_char

    // If it's a digit, extract the full integer
    mov     x6, #0              // x6 will store the parsed value
digit_loop:
    cmp     x1, x2              // Check boundary inside extraction
    b.hs    check_constraints
    ldrb    w5, [x1]
    cmp     w5, #'0'
    b.lo    check_constraints   // Not a number digit anymore
    cmp     w5, #'9'
    b.hi    check_constraints

    // val = val * 10 + (char - '0')
    mov     x7, #10
    mul     x6, x6, x7          // multiply current value by 10
    sub     w5, w5, #'0'        // convert ASCII text digit to raw integer
    add     x6, x6, x5          // add to total
    add     x1, x1, #1          // move memory pointer forward
    b       digit_loop

check_constraints:
    // Validate constraint: 0 < val < 1001
    cmp     x6, #0
    b.ls    next_number         // Skip if value <= 0
    mov     x7, #1001
    cmp     x6, x7
    b.hs    next_number         // Skip if value >= 1001

    add     x3, x3, x6          // Valid! Add to running sum
    add     x4, x4, #1          // Increment tracking count
    b       parse_loop

skip_char:
    add     x1, x1, #1          // Advance pointer over whitespace
    b       parse_loop

next_number:
    add     x4, x4, #1          // Increment count even if number was rejected
    b       parse_loop

// ----------------------------------------------------
// STEP 2: Convert the integer sum back to text ASCII to print
// ----------------------------------------------------
print_result:
    adrp    x1, out_buf@PAGE    // Target buffer for printing output
    add     x1, x1, out_buf@PAGEOFF
    add     x1, x1, #20         // Start filling from the back of buffer
    mov     w5, #'\n'
    strb    w5, [x1]            // Set a trailing newline character
    mov     x7, #10             // Base 10 divisor

num_to_ascii:
    udiv    x8, x3, x7          // x8 = sum / 10
    msub    x9, x8, x7, x3      // x9 = sum % 10 (remainder)
    add     w9, w9, #'0'        // Convert remainder to ASCII digit
    sub     x1, x1, #1          // Shift buffer pointer backward
    strb    w9, [x1]            // Save digit to buffer
    mov     x3, x8              // Update sum to the quotient
    cbnz    x3, num_to_ascii    // Repeat if quotient is not zero

    // Calculate length of text to print
    adrp    x2, out_buf@PAGE
    add     x2, x2, out_buf@PAGEOFF
    add     x2, x2, #21         // Original end boundary
    sub     x2, x2, x1          // x2 now contains byte string length

    // ----------------------------------------------------
    // STEP 3: Trigger System Write to Output Screen
    // ----------------------------------------------------
    mov     x0, #1              // File descriptor 1 = stdout
    mov     x16, #4             // macOS system call number 4 = sys_write
    svc     #05                 // Call kernel

    // ----------------------------------------------------
    // STEP 4: Safe Program Exit
    // ----------------------------------------------------
    mov     x0, #0              // Return status code 0
    mov     x16, #1             // macOS system call number 1 = sys_exit
    svc     #05

.section __DATA,__data
buffer:  .space 64              // 64-byte allocation area for input strings
out_buf: .space 32              // 32-byte allocation area for print formatting
