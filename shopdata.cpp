#include "itemdata.h"

const int	ShopData[MAX_SHOP_NO][MAX_MONTH][MAX_SHOP_ITEM]={
	//掘り出し物屋・ノーマル	0
	{
		{-1},
		{ID_BS+1,ID_BS+1,ID_GLV+5,ID_SHE+2,ID_AME+0,-1},
		{ID_BS+2,ID_BS+2,ID_SPR+3,ID_AXE+3,ID_AME+6,-1},
		{ID_BS+3,ID_BS+3,ID_SWD+5,ID_SLD+2,ID_HED+8,-1},

		{ID_BS+4,ID_BS+4,ID_SPR+WLV_2+3,ID_BOW+8,-1},
		{ID_BS+5,ID_BS+5,ID_SWD+WLV_2+1,ID_GLV+9,ID_SLD+5,ID_HED+9,ID_AMR+ALV_2+4,-1},
		{ID_BS+6,ID_BS+6,ID_AMR+ALV_3+2,ID_AMR+ALV_3+3,-1},
		{ID_BS+7,ID_BS+7,ID_AXE+11,ID_SPR+13,ID_WND+WLV_3+3,-1},

		{ID_BS+8,ID_BS+8,ID_GLV+10,ID_GLV+11,-1},
		{ID_BS+9,ID_BS+9,ID_SHE+12,-1},
		{ID_BS+10,ID_BS+10,ID_AXE+16,-1},
		{ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,
		 ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,-1},

		{-1}
	},
	//掘り出し物屋・ハード	1
	{
		{-1},
		{ID_BS+1,ID_BS+1,ID_BS+1,ID_BS+1	,ID_SLD+1		,ID_SHE+1		,ID_SHE+2,-1},
		{ID_BS+2,ID_BS+2,ID_BS+2,ID_BS+2	,ID_SWD+WLV_1+1	,ID_SPR+WLV_1+2	,ID_WND+WLV_1+3,ID_SLD+4,ID_HED+4,ID_GLV+6,ID_SHE+3,-1},
		{ID_BS+3,ID_BS+3,ID_BS+3,ID_BS+3	,ID_SPR+WLV_1+3	,ID_BOW+WLV_1+1	,ID_AXE+WLV_1+2,ID_SLD+2 ,ID_GLV+3,-1},

		{ID_BS+4,ID_BS+4,ID_BS+4,ID_BS+4	,ID_WND+WLV_2+2	,ID_HED+7	,ID_SHE+4	,-1},
		{ID_BS+5,ID_BS+5,ID_BS+5,ID_BS+5	,ID_AXE+WLV_2+3	,ID_GLV+8	,-1},
		{ID_BS+6,ID_BS+6,ID_BS+6,ID_BS+6	,ID_SPR+WLV_3+3	,ID_SLD+7	,ID_GLV+4	,-1},
		{ID_BS+7,ID_BS+7,ID_BS+7,ID_BS+7	,ID_AXE+WLV_3+1	,ID_SLD+8,ID_HED+10,ID_GLV+12,ID_SHE+9,-1},

		{ID_BS+8,ID_BS+8,ID_BS+8,ID_BS+8	,ID_SLD+10		,ID_HED+12,	-1},
		{ID_BS+9,ID_BS+9,ID_BS+9,ID_BS+9	,ID_BOW+WLV_4+3	,ID_BS+22	,-1},
		{ID_BS+10,ID_BS+10,ID_BS+10,ID_BS+10,ID_WND+WLV_3+3	,ID_SHE+11	,-1},
		{ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,
		 ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_SHE+13,-1},

		{-1}
	},
	//宝石屋		2
	{
		{-1},
		{ID_BS+0,-1},
		{ID_BS+1,-1},
		{ID_BS+2,-1},

		{ID_BS+3,-1},
		{ID_BS+4,-1},
		{ID_BS+5,-1},
		{ID_BS+6,-1},

		{ID_BS+7,-1},
		{ID_BS+8,-1},
		{ID_BS+9,-1},
		{ID_BS+10,-1},

		{ID_BS+11,-1}
	},
	//武器屋		3
	{
		{ID_EQ+0,ID_EQ+18,ID_EQ+36,ID_EQ+54,ID_EQ+72,-1},
		{ID_EQ+1,ID_EQ+19,ID_EQ+37,ID_EQ+55,ID_EQ+73,-1},
		{-1},
		{ID_EQ+2,ID_EQ+20,ID_EQ+38,ID_EQ+56,ID_EQ+74,ID_EQ+75,-1},

		{-1},
		{ID_EQ+6,ID_EQ+24,ID_EQ+42,ID_EQ+43,ID_EQ+60,ID_EQ+61,ID_EQ+78,-1},
		{-1},
		{ID_EQ+10,ID_EQ+28,ID_EQ+46,ID_EQ+64,ID_EQ+65,ID_EQ+82,ID_EQ+83,-1},

		{-1},
		{ID_EQ+14,ID_EQ+32,ID_EQ+50,ID_EQ+68,ID_EQ+86,-1},
		{-1},
		{-1},

		{-1},
	},
	//防具屋		4
	{
		{ID_AMR+ALV_0+0,-1},
		{ID_AMR+ALV_0+1,ID_AMR+ALV_0+2,-1},
		{ID_AMR+ALV_1+0,ID_AMR+ALV_1+1,-1},
		{ID_AMR+ALV_1+2,ID_AMR+ALV_1+3,-1},

		{-1},
		{ID_AMR+ALV_2+0,-1},
		{ID_AMR+ALV_2+1,ID_AMR+ALV_2+2,-1},
		{ID_AMR+ALV_3+0,ID_AMR+ALV_3+1,-1},
		
		{ID_AMR+ALV_3+2,ID_AMR+ALV_3+3,-1},
		{-1},
		{ID_AMR+ALV_4+0,-1},
		{-1},

		{-1},
	},
	//雑貨屋・Lv0	5
	{
		{-1},
		{ID_SLD+0,ID_HED+0,ID_GLV+0,ID_SHE+0,-1},
		{ID_AME+11,ID_AME+12,ID_AME+13,ID_AME+14,ID_AME+15,-1},
		{ID_SLD+4,ID_HED+4,ID_GLV+6,ID_SHE+3,-1},

		{-1},
		{ID_AME+7,ID_AME+8,ID_AME+9,ID_AME+10,-1},
		{-1},
		{-1},

		{ID_SLD+8,ID_HED+10,ID_GLV+12,ID_SHE+9,ID_AME+16,ID_AME+17,ID_AME+18,-1},
		{-1},
		{-1},
		{-1},

		{-1},
	},
	//雑貨屋・Lv1	6
	{
		{ID_SLD+0,ID_HED+0,ID_GLV+0,ID_SHE+0,-1},
		{ID_HED+1,ID_SLD+1,ID_GLV+1,ID_SHE+2,-1},
		{ID_AME+11,ID_AME+12,ID_AME+13,ID_AME+14,ID_AME+15,-1},
		{ID_SLD+4,ID_HED+4,ID_GLV+6,ID_SHE+3,-1},

		{ID_AME+0,ID_AME+1,ID_AME+2,ID_AME+3,ID_AME+4,ID_AME+5,ID_AME+6,-1},
		{ID_AME+7,ID_AME+8,ID_AME+9,ID_AME+10,-1},
		{-1},
		{ID_SLD+8,ID_HED+10,ID_GLV+12,ID_SHE+9,-1},

		{ID_AME+16,ID_AME+17,ID_AME+18,-1},
		{-1},
		{ID_SLD+10,ID_HED+11,ID_GLV+14,ID_SHE+11,-1},
		{-1},

		{-1},
	},
	//雑貨屋・Lv2	7
	{
		{ID_SLD+0,ID_HED+0,ID_GLV+0,ID_SHE+0,-1},
		{ID_HED+1,ID_SLD+1,ID_GLV+1,ID_SHE+2,-1},
		{ID_AME+11,ID_AME+12,ID_AME+13,ID_AME+14,ID_AME+15,-1},
		{ID_SLD+4,ID_HED+4,ID_GLV+6,ID_SHE+3,-1},

		{ID_AME+0,ID_AME+1,ID_AME+2,ID_AME+3,ID_AME+4,ID_AME+5,ID_AME+6,-1},
		{ID_AME+7,ID_AME+8,ID_AME+9,ID_AME+10,ID_AME+16,ID_AME+17,ID_AME+18,-1},
		{ID_SLD+2,ID_SLD+7,ID_HED+2,ID_HED+7,ID_GLV+7,ID_GLV+8,ID_GLV+9,ID_SHE+5,ID_SHE+12,-1},
		{ID_SLD+8,ID_HED+10,ID_GLV+12,ID_SHE+9,-1},

		{-1},
		{ID_SLD+10,ID_HED+11,ID_GLV+14,ID_SHE+11,-1},
		{-1},
		{ID_SLD+11,ID_HED+12,ID_GLV+13,ID_SHE+13,-1},

		{-1},
	},
	//雑貨屋・Lv3	8
	{
		{ID_SLD+0,ID_HED+0,ID_GLV+0,ID_SHE+0,-1},
		{ID_HED+1,ID_SLD+1,ID_GLV+1,ID_SHE+2,-1},
		{ID_AME+11,ID_AME+12,ID_AME+13,ID_AME+14,ID_AME+15,-1},
		{ID_SLD+4,ID_HED+4,ID_GLV+6,ID_SHE+3,-1},

		{ID_AME+0,ID_AME+1,ID_AME+2,ID_AME+3,ID_AME+4,ID_AME+5,ID_AME+6,-1},
		{ID_AME+7,ID_AME+8,ID_AME+9,ID_AME+10,ID_AME+16,ID_AME+17,ID_AME+18,-1},
		{ID_SLD+2,ID_SLD+7,ID_HED+2,ID_HED+7,ID_GLV+7,ID_GLV+8,ID_GLV+9,ID_SHE+5,ID_SHE+12,-1},
		{ID_SLD+8,ID_HED+10,ID_GLV+12,ID_SHE+9,-1},
		
		{-1},
		{ID_SLD+10,ID_HED+11,ID_GLV+14,ID_SHE+11,-1},
		{ID_SLD+11,ID_HED+12,ID_GLV+13,ID_SHE+13,-1},
		{ID_SLD+15,ID_HED+15,ID_GLV+15,ID_SHE+14,-1},

		{-1},
	},
	//妖精の森の行商	9
	{
		{ID_BS+0,ID_BS+1,ID_EQ+1,ID_EQ+19,ID_EQ+37,ID_EQ+55,ID_EQ+72,-1},
		{ID_HED+1,ID_SLD+1,ID_GLV+1,ID_SHE+2,ID_EQ+95,ID_EQ+96,-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
	},
	//妖精の森の行商・木こりの斧入手後	10
	{
		{ID_BS+0,ID_BS+1,ID_EQ+1,ID_EQ+19,ID_EQ+37,ID_EQ+55,ID_EQ+72,-1},
		{ID_HED+1,ID_SLD+1,ID_GLV+1,ID_SHE+2,ID_EQ+95,ID_EQ+96,-1},
		{ID_EQ+41,ID_EQ+90,-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
	},
	//砂漠の店、その1	11
	{
		{ID_SWD+3,ID_SPR+5,ID_AXE+4,ID_BOW+3,ID_WND+5,-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
	},
	//砂漠の店、その2	12
	{
		{ID_SLD+5,ID_HED+8,ID_GLV+5,ID_SHE+4,ID_AME+17,ID_AME+18,-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
	},
	//砂漠の店、その3	13
	{
		{ID_NSUI+0,ID_NSUI+1,ID_NSUI+2,ID_NSUI+3,-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
	},
	//地下水道	14
	{
		{ID_SWD+WLV_2+0,ID_WND+WLV_2+0,ID_AMR+ALV_2+0,ID_AMR+ALV_2+1,ID_AMR+ALV_2+2,-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
	},
	//村落・果物や	15
	{
		{ID_OTH+1,ID_OTH+2,ID_OTH+3,ID_OTH+4,ID_OTH+5,-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
	},
	//村落・宝石や	16
	{
		{-1},
		{-1},
		{-1},
		{-1},
		
		{-1},
		{-1},
		{-1},
		{-1},
		
		{-1},
		{-1},
		{-1},
		{ID_BS+24,ID_BS+11,-1},

		{-1},
	},
	//掘出物に出展されない例外アイテム	MAX_SHOP_NO-1
	{
		{ID_MED+0,ID_MED+1,ID_MED+2,ID_MED+3,-1},
		{ID_OTH+0,ID_OTH+1,ID_OTH+2,ID_OTH+3,ID_OTH+4,ID_OTH+5-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
		{-1},
		{-1},
		{-1},

		{-1},
	},
};