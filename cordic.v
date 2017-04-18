/** CORDIC module defined in cordic.v
 *
 *  Dylan O' Connor Desmond - 13154117
 *  Ian Lodovica            - 13131567
 **/
module CORDIC(cosine, sine, done, target_angle, init, clk);
// Number of bits for fixed point
parameter BIT_SIZE = 18;

// Inputs
input signed [1:-16] target_angle;
input init;
input clk;

// Outputs
output signed [1:-16] cosine;
output signed [1:-16] sine;
output done;

// Input values in registers
reg signed [1:-16] target_angle_reg;

// Output values in registers
reg signed [1:-16] cosine_reg;
reg signed [1:-16] sine_reg;
reg done_reg;

// Assign registers to outputs
assign cosine = cosine_reg;
assign sine = sine_reg;
assign done = done_reg;

// Registers for x, y, current angle, sign bit and counter
reg signed [1:-16] current_x_reg;
reg signed [1:-16] current_y_reg;
reg signed [1:-16] current_z_reg;
reg signed [1:-16] next_x_reg;
reg signed [1:-16] next_y_reg;
reg signed [1:-16] next_z_reg;
reg sign_bit;
reg [4:0] counter;

// Scale to compensate for CORDIC gain
reg signed [1:-16] scaler = 18'b00_1001_1011_0111_0100;

// Wire array to hold the arctan values
wire signed [1:-16] atan_table [0:17];

assign atan_table[00] = 18'b00_1100_1001_0000_1111;
assign atan_table[01] = 18'b00_0111_0110_1011_0001;
assign atan_table[02] = 18'b00_0011_1110_1011_0110;
assign atan_table[03] = 18'b00_0001_1111_1101_0101;
assign atan_table[04] = 18'b00_0000_1111_1111_1010;
assign atan_table[05] = 18'b00_0000_0111_1111_1111;
assign atan_table[06] = 18'b00_0000_0011_1111_1111;
assign atan_table[07] = 18'b00_0000_0001_1111_1111;
assign atan_table[08] = 18'b00_0000_0000_1111_1111;
assign atan_table[09] = 18'b00_0000_0000_0111_1111;
assign atan_table[10] = 18'b00_0000_0000_0011_1111;
assign atan_table[11] = 18'b00_0000_0000_0001_1111;
assign atan_table[12] = 18'b00_0000_0000_0000_1111;
assign atan_table[13] = 18'b00_0000_0000_0000_0111;
assign atan_table[14] = 18'b00_0000_0000_0000_0011;
assign atan_table[15] = 18'b00_0000_0000_0000_0001;
assign atan_table[16] = 18'b00_0000_0000_0000_0000;
assign atan_table[17] = 18'b00_0000_0000_0000_0000;

// On a positive clock edge
always @(posedge clk)
begin
	if(init == 1)
	begin
		// Initialise the registers
		current_x_reg = scaler;
		current_y_reg = 0;
		current_z_reg = target_angle;
		done_reg = 0;
		counter = 0;
	end
	else if(done_reg == 0)
	// Iterate through CORDIC algorithm
	begin
		// Get the sign of the current angle
		sign_bit = current_z_reg >> (BIT_SIZE - 1);

		if(sign_bit == 0)
		// If sign bit is positive
		begin
			// Calculate next x, y and current angle accordingly
			next_x_reg = current_x_reg - (current_y_reg >>> counter);
			next_y_reg = current_y_reg + (current_x_reg >>> counter);
			next_z_reg = current_z_reg - atan_table[counter];
		end
		else if(sign_bit == 1)
		// If sign bit is negative
		begin
			// Calculate next x, y and current angle accordingly
			next_x_reg = current_x_reg + (current_y_reg >>> counter);
			next_y_reg = current_y_reg - (current_x_reg >>> counter);
			next_z_reg = current_z_reg + atan_table[counter];
		end

		// Update x, y and current angle
		current_x_reg = next_x_reg;
		current_y_reg = next_y_reg;
		current_z_reg = next_z_reg;
		
		// Increment counter
		counter = counter + 1;

		if(counter == BIT_SIZE)
		// Iterations completed
		begin
			// Get sine and cosine of input angle
			cosine_reg = current_x_reg;
			sine_reg = current_y_reg;

			// Set done flag
			done_reg = 1;
		end
	end
end

endmodule
