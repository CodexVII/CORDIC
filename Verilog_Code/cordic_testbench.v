// Testbench for cordic.v

module cordic_testbench;
// Inputs
reg signed [1:-16] target_angle;
reg init;
reg clk;

// Outputs
wire signed [1:-16] cosine;
wire signed [1:-16] sine;
wire done;

// Constans
reg [1:-16] test_value;
reg [1:-16] expected_sine;
reg [1:-16] expected_cosine ;

CORDIC cordic_module(cosine, sine, done, target_angle, init, clk);
initial
    begin
	$dumpfile("cordic_testbench.vcd");
	$dumpvars(0, cordic_testbench);
	test_value = 18'b010011001100110011;
	expected_sine = 18'b001110111010011001;
	expected_cosine = 18'b000101110011000001;
	init = 0;
	clk = 0;
	target_angle = test_value;
	#1;
	init = 1;
	#2;
	init = 0;
	#30;
	
	end

always
begin
	#2 clk = ~clk;
end

endmodule


