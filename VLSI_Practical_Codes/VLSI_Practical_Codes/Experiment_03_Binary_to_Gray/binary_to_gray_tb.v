module binary_to_gray_tb;
    reg [3:0] binary;
    wire [3:0] gray;
    reg [3:0] expected;
    integer i;

    binary_to_gray uut(binary, gray);

    initial begin
        for (i = 0; i < 16; i = i + 1) begin
            binary = i;
            expected = binary ^ (binary >> 1);
            #5;
            if (gray !== expected) begin
                $display("FAIL: binary=%b gray=%b expected=%b",
                         binary, gray, expected);
                $finish;
            end
        end
        $display("PASS: Binary-to-Gray converter verified for all 16 inputs.");
        $finish;
    end
endmodule
