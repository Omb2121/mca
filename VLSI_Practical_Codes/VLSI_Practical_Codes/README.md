# VLSI Design Laboratory – Practical Verilog Codes

Each experiment has its own folder. Design code and testbench code are separate.
The programs follow the uploaded VLSI laboratory manuals.

## Experiments included
1. 4:1 Multiplexer
2. Full Adder using two Half Adders
3. 4-bit Binary-to-Gray Converter
4. JK Flip-Flop and T Flip-Flop (separate design and testbench files)
5. 3-bit Asynchronous Up Counter
6. 4-bit SISO Shift Register using D Flip-Flops

## Run in ModelSim
For each experiment:
1. Create/open a ModelSim project.
2. Add the design `.v` file(s) and the corresponding `_tb.v` file.
3. Compile all files.
4. Start simulation using the testbench module name (the module name is the filename without `.v`, except where noted).
5. In the Transcript window, run `run -all`.
6. Use `add wave *` before `run -all` if you want to see signals in the Wave window.

Example for Experiment 1:
```tcl
vlib work
vlog mux4to1.v mux4to1_tb.v
vsim work.mux4to1_tb
add wave *
run -all
```

For the other folders, replace the filenames and testbench top module with the corresponding names.

## Important notes
- Experiment 4 has two independent circuits: JK and T flip-flops. Simulate each testbench separately.
- Experiment 5 is implemented as a negative-edge-triggered ripple up-counter, with each stage clocked by the preceding stage's output, matching the asynchronous-counter structure in the manual.
- Experiment 6 contains the D flip-flop and SISO shift-register modules in one design file, as in the manual. The testbench uses a faster clock for practical simulation.
- Experiment 6's manual also describes a Basys 3 clock divider/top module and XDC constraints. Those board-specific files are not needed for the basic ModelSim functional simulation; they can be added separately for FPGA implementation.
- Testbenches print PASS/FAIL messages for the combinational experiments and the asynchronous counter/SISO sequence. JK/T testbenches exercise their truth-table operations and print the final output.
