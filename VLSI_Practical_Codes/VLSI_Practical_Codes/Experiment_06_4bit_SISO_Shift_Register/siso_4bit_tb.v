module siso_4bit_tb;
    reg clk, rst, sin;
    wire sout;

    siso_4bit uut(clk, rst, sin, sout);
    always #5 clk = ~clk;

    initial begin
        clk = 0; rst = 1; sin = 0;
        #12 rst = 0;

        sin = 1; @(posedge clk); #1;
        sin = 0; @(posedge clk); #1;
        sin = 1; @(posedge clk); #1;
        sin = 1; @(posedge clk); #1;

        if (sout !== 1'b1) begin
            $display("FAIL: expected first input bit at SOUT after 4 clocks.");
            $finish;
        end

        sin = 0; @(posedge clk); #1;
        if (sout !== 1'b0) begin
            $display("FAIL: expected second input bit at SOUT after 5 clocks.");
            $finish;
        end

        sin = 0; @(posedge clk); #1;
        if (sout !== 1'b1) begin
            $display("FAIL: expected third input bit at SOUT after 6 clocks.");
            $finish;
        end

        sin = 0; @(posedge clk); #1;
        if (sout !== 1'b1) begin
            $display("FAIL: expected fourth input bit at SOUT after 7 clocks.");
            $finish;
        end

        $display("PASS: 4-bit SISO verified; input sequence 1,0,1,1 appears at output after 4 clocks.");
        $finish;
    end
endmodule
