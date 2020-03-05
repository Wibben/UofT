`timescale 1ns/1ns

// Testbench for 8x8 Wallace tree multiplier
// Runs through all possible combinations from 0 to 255
module tbWallaceTree8x8();
	logic [7:0] dut_a,dut_b;
	logic [15:0] dut_product;
	
	// Instantiate an 8x8 booth multiplier
	wallaceTree8x8 DUT
	(
		.a(dut_a),.b(dut_b),
		.product(dut_product)
	);
	
	// Run throuhg all inputs and test that the product is correct
	initial begin
		for(integer a=0; a<256; a++) begin
			for(integer b=0; b<256; b++) begin
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
