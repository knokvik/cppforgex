    #include <iostream>
    #include <fstream>
    #include <string>

    using std::cout;

    void fileHere() {
        cout << "file here";
    }

    // Stream State Flags
    // A stream tracks its internal health status using four distinct state bits: 
    // good(): Returns true if no error flags are raised. Stream is ready for operation.
    // eof(): End-of-File. Returns true if the read pointer has physically hit the absolute end of the file buffer.
    // fail(): Returns true if a logical error occurred (e.g., trying to read a string when an integer was expected, or failing to open a file). The stream is halted but recoverable.
    // bad(): Returns true if a critical physical error occurred (e.g., disk runs out of space, or the hard drive loses connection mid-write). This is usually unrecoverable. 


    void logUrl() {
        std::ofstream outfile("./logs.txt");
        if(!outfile.is_open()) {
            std::cerr << "failed to open or create file";
            return;
        };

        outfile << "https://example.com" << '\n';
        outfile << "https:192.2321.2131.com" << '\n';

        outfile.close();

    }

