`define POS_CLK_POS_ASYNC 0
`define POS_CLK_NEG_ASYNC 1
`define NEG_CLK_POS_ASYNC 2
`define NEG_CLK_NEG_ASYNC 3
`define POS_CLK_NO_ASYNC 4
`define NEG_CLK_NO_ASYNC 5
`define N_COMBS 6

module mgc_generic_reg (d, clk, en, a_rst, s_rst, q);
   parameter width = 8;
   parameter ph_clk = 1;//clock polarity, 1=rising_edge
   parameter ph_en = 1;
   parameter ph_a_rst = 1;  // 0 to 1 IGNORED
   parameter ph_s_rst = 1;  // 0 to 1 IGNORED
   parameter a_rst_used = 1;
   parameter s_rst_used = 0;
   parameter en_used = 0;

   input  [width-1:0] d;
   input              clk;
   input              en;
   input              a_rst;
   input              s_rst;
   output [width-1:0] q;

   reg [width-1:0] q_temp [`N_COMBS-1:0];

   always@(posedge a_rst or posedge clk)
   begin
     if (a_rst)
       q_temp[`POS_CLK_POS_ASYNC] <= {width{1'b0}};
     else if (s_rst == ph_s_rst)
       q_temp[`POS_CLK_POS_ASYNC] <= {width{1'b0}};
     else begin
       if ((ph_en & en) | ((ph_en==0) & (en==0))) begin
         q_temp[`POS_CLK_POS_ASYNC] <= d;
       end
     end
   end

   always@(negedge a_rst or posedge clk)
   begin
     if (!a_rst)
       q_temp[`POS_CLK_NEG_ASYNC] <= {width{1'b0}};
     else if (s_rst == ph_s_rst)
       q_temp[`POS_CLK_NEG_ASYNC] <= {width{1'b0}};
     else begin
       if ((ph_en & en) | ((ph_en==0) & (en==0))) begin
         q_temp[`POS_CLK_NEG_ASYNC] <= d;
       end
     end
   end

   always@(posedge a_rst or negedge clk)
   begin
     if (a_rst)
       q_temp[`NEG_CLK_POS_ASYNC] <= {width{1'b0}};
     else if (s_rst == ph_s_rst)
       q_temp[`NEG_CLK_POS_ASYNC] <= {width{1'b0}};
     else begin
       if ((ph_en & en) | ((ph_en==0) & (en==0))) begin
         q_temp[`NEG_CLK_POS_ASYNC] <= d;
       end
     end
   end

   always@(negedge a_rst or negedge clk)
   begin
     if (!a_rst)
       q_temp[`NEG_CLK_NEG_ASYNC] <= {width{1'b0}};
     else if (s_rst == ph_s_rst)
       q_temp[`NEG_CLK_NEG_ASYNC] <= {width{1'b0}};
     else begin
       if ((ph_en & en) | ((ph_en==0) & (en==0))) begin
         q_temp[`NEG_CLK_NEG_ASYNC] <= d;
       end
     end
   end

   assign q = ph_clk ?
		   (ph_a_rst ?
		     q_temp[`POS_CLK_POS_ASYNC]: q_temp[`POS_CLK_NEG_ASYNC]) :
		   (ph_a_rst ?
		     q_temp[`NEG_CLK_POS_ASYNC]: q_temp[`NEG_CLK_NEG_ASYNC]);
endmodule
