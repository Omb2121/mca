module async_counter_3bit_tb;
    reg clk, rst;
    wire [2:0] q;
    integer i;

    async_counter_3bit uut(clk, rst, q);
    always #5 clk = ~clk;

    initial begin
        clk = 0; rst = 1;
        #12 rst = 0;
        for (i = 1; i <= 8; i = i + 1) begin
            @(negedge clk);
            #1;
            if (q !== (i % 8)) begin
                $display("FAIL: count=%0d q=%b expected=%b",
                         i, q, (i % 8));
                $finish;
            end
        end
        $display("PASS: 3-bit asynchronous counter verified (0 to 7, repeat).");
        $finish;
    end
endmodule
