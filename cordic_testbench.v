/** Testbench for cordic.v
 *
 *  Dylan O' Connor Desmond - 13154117
 *  Ian Lodovica            - 13131567
 **/

module cordic_testbench;
// Inputs
reg signed [1:-16] target_angle;
reg init;
reg clk;

// Outputs
wire signed [1:-16] cosine;
wire signed [1:-16] sine;
wire done;

// Registers for testing
reg signed [1:-16] expected_cosine;
reg signed [1:-16] expected_sine;
reg signed [1:-16] cosine_error;
reg signed [1:-16] sine_error;

// Plus half pi values
reg signed [1:-16] half_pi_positive = 18'b01_1001_0010_0010_0000;
reg signed [1:-16] half_pi_positive_expected_sine = 18'b01_0000_0000_0000_0000;
reg signed [1:-16] half_pi_positive_expected_cosine = 18'b00_0000_0000_0000_0000;

// Plus quarter pi values
reg signed [1:-16] quarter_pi_positive = 18'b00_1100_1001_0001_0000;
reg signed [1:-16] quarter_pi_positive_expected_sine = 18'b00_1011_0101_0000_0100;
reg signed [1:-16] quarter_pi_positive_expected_cosine = 18'b00_1011_0101_0000_0100;

// Zero values
reg signed [1:-16] zero = 18'b00_0000_0000_0000_0000;
reg signed [1:-16] zero_expected_sine = 18'b00_0000_0000_0000_0000;
reg signed [1:-16] zero_expected_cosine = 18'b01_0000_0000_0000_0000;

// Minus quarter pi values
reg signed [1:-16] quarter_pi_negative = 18'b11_0011_0110_1111_0000;
reg signed [1:-16] quarter_pi_negative_expected_sine = 18'b11_0100_1010_1111_1100;
reg signed [1:-16] quarter_pi_negative_expected_cosine = 18'b00_1011_0101_0000_0100;

// Minus half pi values
reg signed [1:-16] half_pi_negative = 18'b10_0110_1101_1110_0000;
reg signed [1:-16] half_pi_negative_expected_sine = 18'b11_0000_0000_0000_0000;
reg signed [1:-16] half_pi_negative_expected_cosine = 18'b00_0000_0000_0000_0000;

CORDIC cordic_module(cosine, sine, done, target_angle, init, clk);
initial
    begin
	// Set up initial conditions
	init = 0;
	clk = 0;

	// Test case 1
	$display("Test case 1: Input angle = + pi/2");
	target_angle = half_pi_positive;
	expected_cosine = half_pi_positive_expected_cosine;
	expected_sine = half_pi_positive_expected_sine;
	cosine_error = 18'b0;
	sine_error = 18'b0;
	#3;
	init = 1;
	#10;
	init = 0;
	#180;
	cosine_error = cosine - expected_cosine;
	sine_error = sine - expected_sine;
	// Absolute error
	if(cosine_error < zero)
	begin
		// Get 2's complement
		cosine_error = ~cosine_error + 1;
	end
	if(sine_error < zero)
	begin
		// Get 2's complement
		sine_error = ~sine_error + 1;
	end
	$display("Expected   sine:  0x%x", expected_sine);
	$display("CORDIC     sine:  0x%x", sine);
	$display("Expected cosine:  0x%x", expected_cosine);
	$display("CORDIC   cosine:  0x%x", cosine);
	$display("Sine error     :  0x%x", sine_error);
	$display("Cosine: error  :  0x%x", cosine_error);
	$display("");
	#50;

	// Test case 2
	$display("Test case 2: Input angle = + pi/4");
	target_angle = quarter_pi_positive;
	expected_cosine = quarter_pi_positive_expected_cosine;
	expected_sine = quarter_pi_positive_expected_sine;
	#3;
	init = 1;
	#10;
	init = 0;
	#180;
	cosine_error = cosine - expected_cosine;
	sine_error = sine - expected_sine;
	// Absolute error
	if(cosine_error < zero)
	begin
		// Get 2's complement
		cosine_error = ~cosine_error + 1;
	end
	if(sine_error < zero)
	begin
		// Get 2's complement
		sine_error = ~sine_error + 1;
	end
	$display("Expected   sine:  0x%x", expected_sine);
	$display("CORDIC     sine:  0x%x", sine);
	$display("Expected cosine:  0x%x", expected_cosine);
	$display("CORDIC   cosine:  0x%x", cosine);
	$display("Sine error     :  0x%x", sine_error);
	$display("Cosine: error  :  0x%x", cosine_error);
	$display("");
	#50;

	// Test case 3
	$display("Test case 3: Input angle = 0");
	target_angle = zero;
	expected_cosine = zero_expected_cosine;
	expected_sine = zero_expected_sine;
	#3;
	init = 1;
	#10;
	init = 0;
	#180;
	cosine_error = cosine - expected_cosine;
	sine_error = sine - expected_sine;
	// Absolute error
	if(cosine_error < zero)
	begin
		// Get 2's complement
		cosine_error = ~cosine_error + 1;
	end
	if(sine_error < zero)
	begin
		// Get 2's complement
		sine_error = ~sine_error + 1;
	end
	$display("Expected   sine:  0x%x", expected_sine);
	$display("CORDIC     sine:  0x%x", sine);
	$display("Expected cosine:  0x%x", expected_cosine);
	$display("CORDIC   cosine:  0x%x", cosine);
	$display("Sine error     :  0x%x", sine_error);
	$display("Cosine: error  :  0x%x", cosine_error);
	$display("");
	#50;

	// Test case 4
	$display("Test case 4: Input angle = - pi/4");
	target_angle = quarter_pi_negative;
	expected_cosine = quarter_pi_negative_expected_cosine;
	expected_sine = quarter_pi_negative_expected_sine;
	#3;
	init = 1;
	#10;
	init = 0;
	#180;
	cosine_error = cosine - expected_cosine;
	sine_error = sine - expected_sine;
	// Absolute error
	if(cosine_error < zero)
	begin
		// Get 2's complement
		cosine_error = ~cosine_error + 1;
	end
	if(sine_error < zero)
	begin
		// Get 2's complement
		sine_error = ~sine_error + 1;
	end
	$display("Expected   sine:  0x%x", expected_sine);
	$display("CORDIC     sine:  0x%x", sine);
	$display("Expected cosine:  0x%x", expected_cosine);
	$display("CORDIC   cosine:  0x%x", cosine);
	$display("Sine error     :  0x%x", sine_error);
	$display("Cosine: error  :  0x%x", cosine_error);
	$display("");
	#50;

	// Test case 5
	$display("Test case 5: Input angle = - pi/2");
	target_angle = half_pi_negative;
	expected_cosine = half_pi_negative_expected_cosine;
	expected_sine = half_pi_negative_expected_sine;
	#3;
	init = 1;
	#10;
	init = 0;
	#180;
	cosine_error = cosine - expected_cosine;
	sine_error = sine - expected_sine;
	// Absolute error
	if(cosine_error < zero)
	begin
		// Get 2's complement
		cosine_error = ~cosine_error + 1;
	end
	if(sine_error < zero)
	begin
		// Get 2's complement
		sine_error = ~sine_error + 1;
	end
	$display("Expected   sine:  0x%x", expected_sine);
	$display("CORDIC     sine:  0x%x", sine);
	$display("Expected cosine:  0x%x", expected_cosine);
	$display("CORDIC   cosine:  0x%x", cosine);
	$display("Sine error     :  0x%x", sine_error);
	$display("Cosine: error  :  0x%x", cosine_error);
	$display("");
	#50;

	end

always
begin
	// Clock signal
	#5 clk = ~clk;
end

endmodule
