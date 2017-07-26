////////////////////////////////////////////////////////////////////////////////
// Catapult Synthesis
//
// Copyright (c) 2003-2011 Calypto Design Systems, Inc.
//       All Rights Reserved
// 
// This document contains information that is proprietary to Calypto Design
// Systems, Inc. The original recipient of this document may duplicate this  
// document in whole or in part for internal business purposes only, provided  
// that this entire notice appears in all copies. In duplicating any part of  
// this document, the recipient agrees to make every reasonable effort to  
// prevent the unauthorized use and distribution of the proprietary information.
// 
////////////////////////////////////////////////////////////////////////////////

module singleport_ram_be ( data_in, addr, re, we, data_out, clk, a_rst, s_rst, en);

parameter ram_id          = 1;
parameter words           = 'd256;
parameter width           = 8;
parameter addr_width      = 8;
parameter [0:0] a_reset_active  = 0;
parameter [0:0] s_reset_active  = 1;
parameter [0:0] enable_active   = 1;
parameter [0:0] re_active   = 1;
parameter [0:0] we_active   = 1;
parameter num_byte_enables = 1;
parameter clock_edge = 1;
parameter num_input_registers         = 1;
parameter num_output_registers        = 0;
parameter no_of_singleport_readwrite_port  = 1;

localparam byte_width = width / num_byte_enables;

  input [(width*no_of_singleport_readwrite_port)-1:0] data_in;
  input [(addr_width*no_of_singleport_readwrite_port)-1:0] addr;
  input [(no_of_singleport_readwrite_port*num_byte_enables)-1:0] re;
  input [(no_of_singleport_readwrite_port*num_byte_enables)-1:0] we;
  output [(width*no_of_singleport_readwrite_port)-1:0] data_out;
  input clk;
  input a_rst;
  input s_rst;
  input en;

  reg  [width-1:0] mem [words-1:0];

  reg [addr_width-1        :0] addr_reg;
  reg [num_byte_enables-1:0] re_reg;

// synopsys translate_off
  integer count;
  initial
  begin
    for (count = 0; count < words; count = count + 1) 
      mem[count] = 0;
  end
// synopsys translate_on

  reg [width-1:0] data_in_inreg [((num_input_registers > 1) ? num_input_registers - 2 : 0): 0];
  reg [addr_width-1:0] addr_inreg [((num_input_registers > 1) ? num_input_registers - 2 : 0): 0];
  reg [num_byte_enables-1:0] re_inreg [((num_input_registers > 1) ? num_input_registers - 2 : 0): 0];
  reg [num_byte_enables-1:0] we_inreg [((num_input_registers > 1) ? num_input_registers - 2 : 0): 0];

  genvar i;
  generate
  begin : pipe
    if (num_input_registers > 1)
    begin
      for( i=num_input_registers-2; i >= 1; i = i - 1)
      begin : SHIFT_MSBs // Verilog 2000 syntax requires that GENERATE_LOOP_STATEMENT be a named block
                         // Modelsim seems to allow it though it should be a bug
        wire [width-1:0] data_in_i;
        wire [addr_width-1:0] addr_i;
        wire [num_byte_enables-1:0] re_i;
        wire [num_byte_enables-1:0] we_i;
  
        mgc_generic_reg #(width,      clock_edge, enable_active, a_reset_active, s_reset_active, 1'b0, 1'b0, 1'b0) i_data_in_inreg (data_in_inreg[i-1], clk, en, a_rst, s_rst, data_in_i);
        mgc_generic_reg #(addr_width, clock_edge, enable_active, a_reset_active, s_reset_active, 1'b0, 1'b0, 1'b0) i_addr_inreg    (addr_inreg[i-1],    clk, en, a_rst, s_rst, addr_i);
        mgc_generic_reg #(num_byte_enables, clock_edge, enable_active, a_reset_active, s_reset_active, 1'b0, 1'b0, 1'b0) i_re_inreg      (re_inreg[i-1],      clk, en, a_rst, s_rst, re_i);
        mgc_generic_reg #(num_byte_enables, clock_edge, enable_active, a_reset_active, s_reset_active, 1'b0, 1'b0, 1'b0) i_we_inreg      (we_inreg[i-1],      clk, en, a_rst, s_rst, we_i);
  
        always@(data_in_i)
          data_in_inreg[i] = data_in_i;
  
        always@(addr_i)
          addr_inreg[i] = addr_i;
  
        always@(re_i)
          re_inreg[i] = re_i;
        always@(we_i)
          we_inreg[i] = we_i;
      end // end for loop

      // SHIFT for the first stage
      wire [width-1:0] data_in_init;
      wire [addr_width-1:0] addr_init;
      wire [num_byte_enables-1:0] re_init;
      wire [num_byte_enables-1:0] we_init;
    
      mgc_generic_reg #(width,      clock_edge, enable_active, a_reset_active, s_reset_active, 1'b0, 1'b0, 1'b1) i_data_in_inreg_i (data_in, clk, en, a_rst, s_rst, data_in_init);
      mgc_generic_reg #(addr_width, clock_edge, enable_active, a_reset_active, s_reset_active, 1'b0, 1'b0, 1'b1) i_addr_inreg_i    (addr,    clk, en, a_rst, s_rst, addr_init);
      mgc_generic_reg #(num_byte_enables, clock_edge, enable_active, a_reset_active, s_reset_active, 1'b0, 1'b0, 1'b1) i_re_inreg_i      (re,      clk, en, a_rst, s_rst, re_init);
      mgc_generic_reg #(num_byte_enables, clock_edge, enable_active, a_reset_active, s_reset_active, 1'b0, 1'b0, 1'b1) i_we_inreg_i      (we,      clk, en, a_rst, s_rst, we_init);
    
      always@(data_in_init)
        data_in_inreg[0] = data_in_init;
    
      always@(addr_init)
        addr_inreg[0] = addr_init;
    
      always@(re_init)
        re_inreg[0] = re_init;

      always@(we_init)
        we_inreg[0] = we_init;
    end
  end
  endgenerate

  wire [width-1:0]      data_in_f = (num_input_registers > 1) ? data_in_inreg[num_input_registers-2] : data_in;
  wire [addr_width-1:0] addr_f    = (num_input_registers > 1) ? addr_inreg[num_input_registers-2]    : addr;
  wire [num_byte_enables-1:0]            re_f      = (num_input_registers > 1) ? re_inreg[num_input_registers-2]      : re;
  wire [num_byte_enables-1:0]            we_f      = (num_input_registers > 1) ? we_inreg[num_input_registers-2]      : we;

  reg addr_NotDontCare;
  reg addr_NotDontCare2;
  integer j, k, l, m, n, o, p, q;

  generate
    if (clock_edge == 1'b1)
      begin: POSEDGE_BLK
        always @(posedge clk)
        begin
          if ( en == enable_active )
          begin
            addr_NotDontCare = 1'b1;
            begin: blk_X
              for (j = 0; j < addr_width; j = j + 1)
              begin
                if ((addr_f[j] === 1'bX) || (addr_f[j] === 1'bZ))
                begin
                  addr_NotDontCare = 1'b0;
                  for (k = 0; k < num_byte_enables; k = k + 1)
                  begin
                    if (we_f[k] == we_active) begin
                      $display($time, " Error: Dont-care addresses for pipelined singleport memory write");
                      $finish;
                    end
                  end
                  disable blk_X;
                end
              end
            end
                                                                                        
            addr_reg <= addr_f;
            re_reg   <= re_f;

            if (addr_NotDontCare)
            begin
              if ((addr_f < words) && (addr_f >= 0)) begin
                for (j = 0; j < num_byte_enables; j = j + 1)
                begin
                  if (we_f[j] == we_active) begin
                    // mem[addr_f] <= data_in_f;
                    mem[addr_f][((j*byte_width)+byte_width)-1-:byte_width] <= data_in_f[((j*byte_width)+byte_width)-1-:byte_width];
                  end
                end
              end else begin
                for (j = 0; j < num_byte_enables; j = j + 1)
                begin
                  if (we_f[j] == we_active) begin
                    $display($time, " Error: Out-of-bound pipelined singleport memory-write");
                    $finish;
                  end
                end
              end
            end
          end
        end
      end
    else
      begin: NEGEDGE_BLK
        always @(negedge clk)
        begin
          if ( en == enable_active )
          begin
            addr_NotDontCare = 1'b1;
            begin: blk_X
              for (n = 0; n < addr_width; n = n + 1)
              begin
                if ((addr_f[n] === 1'bX) || (addr_f[n] === 1'bZ))
                begin
                  addr_NotDontCare = 1'b0;
                  for (o = 0; o < num_byte_enables; o = o + 1)
                  begin
                    if (we_f[o] == we_active) begin
                      $display($time, " Error: Dont-care addresses for pipelined singleport memory write");
                      $finish;
                    end
                  end
                  disable blk_X;
                end
              end
            end
                                                                                        
            addr_reg <= addr_f;
            re_reg   <= re_f;

            if (addr_NotDontCare)
            begin
              if ((addr_f < words) && (addr_f >= 0)) begin
                for (n = 0; n < num_byte_enables; n = n + 1)
                begin
                  if (we_f[n] == we_active) begin
                    // mem[addr_f] <= data_in_f;
                    mem[addr_f][((n*byte_width)+byte_width)-1-:byte_width] <= data_in_f[((n*byte_width)+byte_width)-1-:byte_width];
                  end
                end
              end else begin
                for (n = 0; n < num_byte_enables; n = n + 1)
                begin
                  if (we_f[n] == we_active) begin
                    $display($time, " Error: Out-of-bound pipelined singleport memory-write");
                    $finish;
                  end
                end
              end
            end
          end
        end
      end
  endgenerate

  reg [width-1:0] data_out_f;

  integer r; //TODO: added k to match vhdl
  always@(
          mem[addr_reg]
	       or addr_reg
          //TODO: commented difference// or addr_NotDontCare
	        or re_reg
          )
  begin
   //TODO: added to match VHDL :
    addr_NotDontCare2 = 1'b1;

    for(k= 0; k < addr_width-1; k = k+ 1)
    begin
      if ((addr_reg[k] === 1'bX) || (addr_reg[k] === 1'bZ)) begin
        addr_NotDontCare2 = 1'b0 ; 
      end
    end //loop

    if (addr_NotDontCare2 )
    //begin
      if ((addr_reg < words) && (addr_reg >= 0))
        for (r = 0; r < num_byte_enables; r = r + 1)
        begin
	        if ( re_reg[r] == re_active )
            data_out_f[((r*byte_width) + byte_width) - 1 -: byte_width] <= mem[addr_reg][((r*byte_width) + byte_width) - 1 -: byte_width];
          else
            data_out_f[((r*byte_width) + byte_width) - 1 -: byte_width] <= {(byte_width){1'bX}};
        end //end for loop
      else
        data_out_f <= {width{1'bX}};
    else
      data_out_f <= {width{1'bX}};
  end


  reg [width-1:0] data_out_outreg [(num_output_registers > 0 ? num_output_registers - 1 : 0): 0];

  genvar t;
  generate
  begin : outblk
    if (num_output_registers > 0)
    begin
      for( t = num_output_registers-1; t >= 1; t = t - 1)
      begin : SHIFT_MSBs // Verilog 2000 syntax requires that GENERATE_LOOP_STATEMENT be a named block
                         // Modelsim seems to allow it though it should be a bug
        wire [width-1:0] data_out_i;
        mgc_generic_reg #(width, clock_edge, enable_active, a_reset_active, s_reset_active, 1'b0, 1'b0, 1'b1) i_data_out_outreg (data_out_outreg[t-1], clk, en, a_rst, s_rst, data_out_i);
  
        always@(data_out_i)
          data_out_outreg[t] = data_out_i;
      end // end for loop

      // SHIFT for the first stage
      wire [width-1:0] data_out_init;
      mgc_generic_reg #(width, clock_edge, enable_active, a_reset_active, s_reset_active, 1'b0, 1'b0, 1'b1) i_data_out_outreg_i (data_out_f, clk, en, a_rst, s_rst, data_out_init);

      always@(data_out_init)
        data_out_outreg[0] = data_out_init;
    end
  end
  endgenerate

  assign data_out = (num_output_registers > 0) ? data_out_outreg[num_output_registers-1] : data_out_f;

endmodule

