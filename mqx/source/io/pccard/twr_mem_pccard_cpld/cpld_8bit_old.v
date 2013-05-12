//*******************************************************************************************************************************************
  // Minibus CPLD
  // 8-bit Non-Muxed Mode
//*******************************************************************************************************************************************
//Top module

module cpld_8bit( 
					  address,
					  data,
					  reset,
					  ale,
					  cs0_b,
					  rw_b,
					  clk,
					  oe,
					  cf_ce_b,				  //Active low Card select signal
					  cf_oe,                  //Active low Output enable strobe
                      cf_reset,               //Reset the compact flash card
                      cf_reg_1,               //Low during I/O operations,in memory mode used to distinguish betwen common and attribute memory 
                      cf_we,                  //Active low signal used for writing configuration registers

					  cf_address,
					  cf_data
			                   );            
		
		output [10:0] cf_address;
		output [7:0] cf_data;
				
		input [19:0] address;
		inout [7:0] data;
		input reset;
		input ale;
		input cs0_b;
		input rw_b;
		input clk;
		input oe;
		output cf_ce_b;
		output cf_we;
		output cf_reg_1;
		output cf_oe;
		output cf_reset;
	
		
		
		wire h_ready;
		wire ready_int;
		wire hint_int;
		wire hint_n0;
		wire [10:0] h_address;
		
		//reg  [10:0] cf_address;
		reg  [7:0]  data_reg;
		reg  cf_we;
		reg  cf_reg_1;
		//reg  cf_oe;
		reg  cf_ce_b;
		reg  cf_reset;

		
initial
      begin
		  //cf_address=11'b00000000000;
          data_reg=8'b00000000;
          cf_ce_b=1'b0;
		  cf_reg_1=1'b1;
 //         cf_oe=1'b0;
          cf_we=1'b1;
	  end
    
always @(posedge ale) begin     //Latch data on rising CS signal  (ALE = CS1)
   if (rw_b==1'b0) begin          //write
       cf_ce_b  =1'b0;
	   cf_reg_1    =1'b1;
	  // oe=1'b1;
	   cf_we=1'b0;
	  end 
   else	begin						//read
	   cf_ce_b =1'b0;
       cf_reg_1   =1'b1;
     //  oe=1'b0;
       cf_we=1'b1; 
   end
end

//Output data when OE is low and CPLD is selected (ALE = CS1)
//assign data = (~oe & ~ale) ? data_reg : 8'bz; 
assign data = 8'bz; 
assign cf_oe = oe;
assign cf_address = address[10:0];

//Debug signals on J19
assign cf_data[0]=cs0_b;
assign cf_data[1]=ale;
assign cf_data[2]=oe;
assign cf_data[3]=rw_b;
assign cf_data[4]=clk;
assign cf_data[5]=address[0];
assign cf_data[7:6]=2'b11;

endmodule

	
	