#include "SVPWM.h"

/**********************************************************************************************************
Park变换，输入电角度、Ualpha和Ubeta，经过Park变换得到Uq、Ud
**********************************************************************************************************/ 
void Park_transit(struct DQ_Aix* DQ,struct AlphaBeta_Aix* AB)       
{ 
    DQ->I_q = -AB->I_Alpha*sin(DQ->theta) + AB->I_Beta*cos(DQ->theta);       /* Park变换 */
    DQ->I_d = AB->I_Alpha*cos(DQ->theta) + AB->I_Beta*sin(DQ->theta);
}

/**********************************************************************************************************
反park变换，输入Uq、Ud得到Ualpha、Ubeta
**********************************************************************************************************/ 
void Rev_Park(struct DQ_Aix* DQ,struct AlphaBeta_Aix* AB)
{
	
    AB->U_Alpha = DQ->U_d*cos(DQ->theta) - DQ->U_q*sin(DQ->theta);
    AB->U_Beta = DQ->U_d*sin(DQ->theta) + DQ->U_q*cos(DQ->theta);	

}

/**********************************************************************************************************
clark变换，输入Ua、Ub、Uc得到Ualpha、Ubeta
**********************************************************************************************************/ 
void clark_transit(struct Three_Phase* abc,struct AlphaBeta_Aix* AB) 
{

    AB->I_Alpha = DIV_2_3*(abc->Ia - DIV_1_2*abc->Ib - DIV_1_2*abc->Ic);   /* 做等幅值变换 */
    AB->I_Beta = DIV_2_3*(Sqrt_3_div_2*abc->Ib - Sqrt_3_div_2*abc->Ic);

}

/**********************************************************************************************************
反clark变换，输入Ualpha、Ubeta得到Ua、Ub、Uc
**********************************************************************************************************/ 
void Rev_clark(struct Three_Phase* abc,struct AlphaBeta_Aix* AB) 
{

    abc->Ua = AB->U_Alpha;        /* 依据Ia+Ib+Ic=0 */
    abc->Ub = -DIV_1_2*AB->U_Alpha + Sqrt_3_div_2*AB->U_Beta;
    abc->Uc = -DIV_1_2*AB->U_Alpha - Sqrt_3_div_2*AB->U_Beta;

}

/* 当前扇区判断/扇区计算 */
void CalculateSector(struct AlphaBeta_Aix AB, struct SVPWM_Control* ptr)
{
		int8_t A = 0;
		int8_t B = 0;
		int8_t C = 0;
		int8_t N = 0;
		
        /* 六个扇区的三个判断条件 */
		float VA = AB.U_Beta;
		float VB = AB.U_Alpha * Sqrt_3_div_2 - DIV_1_2 * AB.U_Beta;
		float VC = 0 - AB.U_Alpha * Sqrt_3_div_2 - DIV_1_2 * AB.U_Beta;

        /* 对X、Y、Z三个参量式赋值 */
		ptr->X = SVPWM_K * AB.U_Beta;
		ptr->Y = SVPWM_K * (AB.U_Alpha * Sqrt_3_div_2 + DIV_1_2 * AB.U_Beta);
		ptr->Z = SVPWM_K * (0 - AB.U_Alpha * Sqrt_3_div_2 + DIV_1_2 * AB.U_Beta);
	
		if(VA > 0)
		{
				A = 1;
		}
		else
		{
				A = 0;
		}
		
		if(VB > 0)
		{
				B = 1;
		}
		else
		{
				B = 0;
		}
		
		if(VC > 0)
		{
				C = 1;
		}
		else
		{
				C = 0;
		}
		
		N = 4*C + 2*B + A;
		
		switch(N)
		{
			case 3:
				ptr->sector = 1;
				break;
			case 1:
				ptr->sector = 2;
				break;
			case 5:
				ptr->sector = 3;
				break;
			case 4:
				ptr->sector = 4;
				break;
			case 6:
				ptr->sector = 5;
				break;
			case 2:
				ptr->sector = 6;
				break;
		}	
}

/* 计算出对应扇区占空比 */
void CalulateDutyCycle(struct SVPWM_Control* ctrl,struct SVPWM_Duty* ptr)
{
        /* 对X、Y、Z三个参量式,在扇区判断的基础上进行扇区计算值的赋值 */
		switch(ctrl->sector)
		{
			case 2:
				ctrl->T_First = ctrl->Z;
				ctrl->T_Second = ctrl->Y;
				break;
			case 6:
				ctrl->T_First = ctrl->Y;
				ctrl->T_Second = 0 - ctrl->X;
				break;
			case 1:
				ctrl->T_First = 0 - ctrl->Z;
				ctrl->T_Second = ctrl->X;
				break;
			case 4:
				ctrl->T_First = 0 - ctrl->X;
				ctrl->T_Second = ctrl->Z;
				break;
			case 3:
				ctrl->T_First = ctrl->X;
				ctrl->T_Second = 0 - ctrl->Y;
				break;
			case 5:
				ctrl->T_First = 0 - ctrl->Y;
				ctrl->T_Second = 0 - ctrl->Z;
				break;
		}
		
		/* 当大于Ts本身时,对T_First和T_Second的大小进行缩放 */
		if((ctrl->T_First + ctrl->T_Second) > Ts)
		{
				ctrl->T_First  = ctrl->T_First  * Ts / (ctrl->T_First + ctrl->T_Second);
				ctrl->T_Second = ctrl->T_Second  * Ts / (ctrl->T_First + ctrl->T_Second);
		}			
		
		ctrl->T0 = (Ts - ctrl->T_First - ctrl->T_Second) / 2;
		
		/* 按照PWM中心对齐模式1、PWM计数模式1来设置三路逆变器PWM的大小 */
//		ctrl->Tc = ctrl->T0 / 2;
//		ctrl->Tb = ctrl->Tc + ctrl->T_Second / 2;
//		ctrl->Ta = ctrl->Tb + ctrl->T_First / 2;

		/* 按照PWM中心对齐模式1、PWM计数模式2来设置三路逆变器PWM的大小 */
		ctrl->Ta = ctrl->T0 / 2;
		ctrl->Tb = ctrl->Ta + ctrl->T_First / 2;
		ctrl->Tc = ctrl->Tb + ctrl->T_Second / 2;
		
		/* 判断对应扇区赋值三路占空比 */
		switch(ctrl->sector)
		{
			case 2:
				ptr->Ta = ctrl->Tb;
				ptr->Tb = ctrl->Ta;
				ptr->Tc = ctrl->Tc;
				break;
			case 6:
				ptr->Ta = ctrl->Ta;
				ptr->Tb = ctrl->Tc;
				ptr->Tc = ctrl->Tb;
				break;
			case 1:
				ptr->Ta = ctrl->Ta;
				ptr->Tb = ctrl->Tb;
				ptr->Tc = ctrl->Tc;
				break;
			case 4:
				ptr->Ta = ctrl->Tc;
				ptr->Tb = ctrl->Tb;
				ptr->Tc = ctrl->Ta;
				break;
			case 3:
				ptr->Ta = ctrl->Tc;
				ptr->Tb = ctrl->Ta;
				ptr->Tc = ctrl->Tb;
				break;
			case 5:
				ptr->Ta = ctrl->Tb;
				ptr->Tb = ctrl->Tc;
				ptr->Tc = ctrl->Ta;
				break;
		}
		
}
