`timescale 1ns/1ns

// Testbench for 8x8 Booth multiplier
// Runs through all possible combinations from -128 to 127
module tbBooth8x8();
	logic [7:0] dut_a,dut_b;
	logic [15:0] dut_product;
	
	// Instantiate an 8x8 booth multiplier
	boothMultipliernxn #(.n(8)) DUT
	(
		.a(dut_a),.b(dut_b),
		.product(dut_product)
	);
	
	// Run throuhg all inputs and test that the product is correct
	initial begin
		for(integer a=-128; a<128; a++) begin
			for(integer b=-128; b<128; b++) begin
				logic [15:0]real_product;
				real_product = a*b;
				
				dut_a = a[7:0];
				dut_b = b[7:0];
				#5;
				
				if(dut_product !== real_product) begin
					$display("Mismatch: %d x %d = %d, but got %d",a,b,real_product,dut_product);
					$stop();
				end
			end
		end
		
		$display("All test cases passed");
		$stop();
	end
endmodule
