//*******************************************************************************************************************************************
// Minibus CPLD
// 8-bit Non-Muxed Mode
//*******************************************************************************************************************************************
//Top module

module cpld_8bit( 
			address,
			data,
			reset,
			cs,
			rw_b,
			clk,
			oe,
			cf_ce,				 //Active low Card select signal
			cf_oe,				//Active low Output enable strobe
			cf_reset,			//Reset the compact flash card
			cf_reg,				//Low during I/O operations,in memory mode used to distinguish betwen common and attribute memory 
			cf_we,				//Active low signal used for writing configuration registers
			cf_address,
			cf_data,
			cf_wait,
			cf_cd 
			); 
		
		output [10:0] cf_address;
		output [7:0] cf_data;
				
		input [19:0] address;
		input reset;
		input cs;
		input rw_b;
		input clk;
		input oe;
		input [1:0] cf_cd;
		
		output [7:0] data;
		output cf_ce;
		output cf_we;
		output cf_reg;
		output cf_oe;
		output cf_reset;
		output cf_wait;
		
		reg  [7:0] data_reg;
		reg  cf_we;
		reg  cf_ce;
		reg  cf_wait;
		
// initial settings
initial
	begin
		data_reg  = 8'b10101101; // card is not connected (173 dec)
	end	
		
// connect reset directly
assign cf_reset = ~reset; 

// on every cs1 change cf_ce and cf_we generation
always @(cs) begin 
	// if active ce1- then connect rw to cf_we signal
	if ((rw_b == 0) && (cs == 0)) begin   	//write
		cf_we = 0;		
	end else begin 							   //read or cs1 was changed to 1
		cf_we = 1;
	end
	if (!(~cs & rw_b == 1 & address[13])) begin	// in case we are only checking card presence do not propagate CS on CF_CE
		cf_ce = cs;
	end
end

// connect directly oe for (active - zero during read) 
assign cf_oe = oe;

// connect first 11 addr bytes directly on output
assign cf_address = address[10:0];

// connect addr bit n.12 directly on reg cf pin
assign cf_reg = address[12];

// card detection
always @(cf_cd) begin 
	if (~cf_cd[0] & ~cf_cd[1]) begin
		data_reg  = 8'b11100101; // card is connected send (229 dec)
	end else begin
		data_reg  = 8'b10101101; // card is not connected (173 dec)
	end	
end

// addr n.13 is used for card detecting
assign data = (~cs & ~oe & address[13])  ? data_reg : 8'bz;



//Debug signals on J19
assign cf_data[0]=cf_reset;
assign cf_data[1]=cf_oe;
assign cf_data[2]=cf_we;
assign cf_data[3]=cf_ce;
assign cf_data[4]=cf_reg;
assign cf_data[5]=address[12];
assign cf_data[7:6]=2'b11;

endmodule

	
	