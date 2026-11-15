module d_ff(input clk, rst, d, output reg q);
    always @(posedge clk or posedge rst) begin
        if (rst)
            q <= 1'b0;
        else
            q <= d;
    end
endmodule

module siso_4bit(input clk, rst, sin, output sout);
    wire q0, q1, q2, q3;

    d_ff FF0(clk, rst, sin, q0);
    d_ff FF1(clk, rst, q0,  q1);
    d_ff FF2(clk, rst, q1,  q2);
    d_ff FF3(clk, rst, q2,  q3);

    assign sout = q3;
endmodule
