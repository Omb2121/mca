module full_adder_ha_tb;
    reg a, b, cin;
    wire sum, cout;
    reg [1:0] expected;
    integer i;

    full_adder_ha uut(a, b, cin, sum, cout);

    initial begin
        for (i = 0; i < 8; i = i + 1) begin
            {a, b, cin} = i;
            #5;
            expected = a + b + cin;
            if ({cout, sum} !== expected) begin
                $display("FAIL: a=%b b=%b cin=%b sum=%b cout=%b",
                         a, b, cin, sum, cout);
                $finish;
            end
        end
        $display("PASS: Full adder verified for all 8 input combinations.");
        $finish;
    end
endmodule
