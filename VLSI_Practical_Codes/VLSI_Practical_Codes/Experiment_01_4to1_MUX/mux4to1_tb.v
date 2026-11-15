module mux4to1_tb;
    reg [3:0] in;
    reg [1:0] sel;
    wire y;
    integer i, j;

    mux4to1 uut(in, sel, y);

    initial begin
        for (i = 0; i < 16; i = i + 1) begin
            in = i;
            for (j = 0; j < 4; j = j + 1) begin
                sel = j;
                #5;
                if (y !== in[sel]) begin
                    $display("FAIL: in=%b sel=%b y=%b", in, sel, y);
                    $finish;
                end
            end
        end
        $display("PASS: 4:1 MUX verified for all input/select combinations.");
        $finish;
    end
endmodule
