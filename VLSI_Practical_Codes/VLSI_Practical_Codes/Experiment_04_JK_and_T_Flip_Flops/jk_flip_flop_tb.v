module jk_flip_flop_tb;
    reg clk, rst, j, k;
    wire q;

    jk_flip_flop uut(clk, rst, j, k, q);
    always #5 clk = ~clk;

    initial begin
        clk = 0; rst = 1; j = 0; k = 0;
        #12 rst = 0;

        j = 0; k = 0; #10; // Hold
        j = 1; k = 0; #10; // Set
        j = 0; k = 0; #10; // Hold
        j = 0; k = 1; #10; // Reset
        j = 1; k = 1; #10; // Toggle
        j = 1; k = 1; #10; // Toggle

        $display("JK flip-flop test completed. Final q=%b", q);
        $finish;
    end
endmodule
