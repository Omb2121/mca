module async_counter_3bit(input clk, rst, output reg [2:0] q);
    // Negative-edge triggered ripple counter using JK=1 (toggle mode).
    always @(negedge clk or posedge rst) begin
        if (rst) q[0] <= 1'b0;
        else     q[0] <= ~q[0];
    end

    always @(negedge q[0] or posedge rst) begin
        if (rst) q[1] <= 1'b0;
        else     q[1] <= ~q[1];
    end

    always @(negedge q[1] or posedge rst) begin
        if (rst) q[2] <= 1'b0;
        else     q[2] <= ~q[2];
    end
endmodule
