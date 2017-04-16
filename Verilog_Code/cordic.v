module CORDIC(cosine, sine, done, target_angle, init, clk);
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
reg [1:-16] target_angle_reg;

// Output values in registers
reg [1:-16] cosine_reg;
reg [1:-16] sine_reg;
reg done_reg;

// Assign registers to outputs
assign cosine = cosine_reg;
assign sine = sine_reg;
assign done = done_reg;

// Current angle
reg [1:-16] current_angle_reg;
reg [1:-16] current_x_reg;
reg [1:-16] current_y_reg;
reg [1:-16] current_z_reg;
reg [1:-16] next_x_reg;
reg [1:-16] next_y_reg;
reg [1:-16] next_z_reg;
reg sign_bit;
reg [7:0] counter;

// Scale to compensate for CORDIC gain
wire [1:-16] scaler;
assign scaler = 18'b001001101101110100;

// Wire array to hold the arctan values
wire signed [1:-16] atan_table [0:17];

assign atan_table[00] = 18'b001100100100001111;
assign atan_table[01] = 18'b000111011010110001;
assign atan_table[02] = 18'b000011111010110110;
assign atan_table[03] = 18'b000001111111010101;
assign atan_table[04] = 18'b000000111111111010;
assign atan_table[05] = 18'b000000011111111111;
assign atan_table[06] = 18'b000000001111111111;
assign atan_table[07] = 18'b000000000111111111;
assign atan_table[08] = 18'b000000000011111111;
assign atan_table[09] = 18'b000000000001111111;
assign atan_table[10] = 18'b000000000000111111;
assign atan_table[11] = 18'b000000000000011111;
assign atan_table[12] = 18'b000000000000001111;
assign atan_table[13] = 18'b000000000000000111;
assign atan_table[14] = 18'b000000000000000011;
assign atan_table[15] = 18'b000000000000000001;
assign atan_table[16] = 18'b000000000000000000;
assign atan_table[17] = 18'b000000000000000000;


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
	begin
		// Iterate through CORDIC algorithm
		sign_bit = current_z_reg >> (BIT_SIZE - 1);

		if(sign_bit == 0)
		begin
			next_x_reg = current_x_reg - (current_y_reg >> counter);
			next_y_reg = current_y_reg + (current_x_reg >> counter);
			next_z_reg = current_z_reg - atan_table[counter];
		end
		else
		begin
			next_x_reg = current_x_reg + (current_y_reg >> counter);
			next_y_reg = current_y_reg - (current_x_reg >> counter);
			next_z_reg = current_z_reg + atan_table[counter];
		end

		current_x_reg = next_x_reg;
		current_y_reg = next_y_reg;
		current_z_reg = next_z_reg;
		
		// Increment counter
		counter = counter + 1;

		if(counter == 18)
		begin
			cosine_reg = current_x_reg;
			sine_reg = current_y_reg;
			done_reg = 1;
		end
	end
end

endmodule
