// Testbench for cordic.v

module cordic_testbench;
// Inputs
reg signed [1:-16] target_angle;
reg init;
reg clk;

// Outputs
wire signed [1:-16] cosine;
wire signed [1:-16] sine;
wire signed [1:-16] cosine_diff;
wire signed [1:-16] sine_diff;
wire done;

// Constans
reg signed [1:-16] test_value;
reg signed [1:-16] expected_sine;
reg signed [1:-16] expected_cosine;

assign cosine_diff = (expected_cosine - cosine);
assign sine_diff = (expected_sine - sine);

CORDIC cordic_module(cosine, sine, done, target_angle, init, clk);
initial
    begin
	// TEST#2: -0.345566
	$display("--------------------------------");
	$display("Test 1: -0.345566 rad");
	$display("--------------------------------");
	test_value = 18'b111010011110001001;
	expected_sine = 18'b111010100101001000;
	expected_cosine = 18'b001111000011011110;
	init = 0;
	clk = 0;
	target_angle = test_value;
	#3;
	init = 1;
	#10;
	init = 0;
	#300;

	// TEST#1: 1.12 rad
	$display("--------------------------------");
	$display("Test 2: 1.12 rad");
	$display("--------------------------------");
	test_value = 18'b010011001100110011;
	expected_sine = 18'b001110111010011001;
	expected_cosine = 18'b000101110011000001;
	init = 0;
	target_angle = test_value;
	#3;
	init = 1;
	#10;
	init = 0;
	#300;


	// TEST#3: 1.13097 rad
	$display("--------------------------------");
	$display("Test 3: 1.13097 rad");
	$display("--------------------------------");
	test_value = 18'b010010000110000111;
	expected_sine = 18'b001110011110100010;
	expected_cosine = 18'b000110110011111110;
	init = 0;
	target_angle = test_value;
	#3;
	init = 1;
	#10;
	init = 0;
	#300;

	// TEST#4: -1.57079 rad
	$display("--------------------------------");
	$display("Test 4: -1.57079 rad");
	$display("--------------------------------");
	test_value = 18'b100110110111100001;
	expected_sine = 18'b110000000000000011;
	expected_cosine = 18'b111111111111111100;
	init = 0;
	target_angle = test_value;
	#3;
	init = 1;
	#10;
	init = 0;
	#300;
	end

always
	#5 clk = !clk;

endmodule


