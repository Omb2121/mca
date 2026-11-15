module t_flip_flop_tb;
    reg clk, rst, t;
    wire q;

    t_flip_flop uut(clk, rst, t, q);
    always #5 clk = ~clk;

    initial begin
        clk = 0; rst = 1; t = 0;
        #12 rst = 0;

        t = 0; #20; // Hold
        t = 1; #40; // Toggle on each rising edge
        t = 0; #20; // Hold

        $display("T flip-flop test completed. Final q=%b", q);
        $finish;
    end
endmodule
