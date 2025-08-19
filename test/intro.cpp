#include"intro1-8.cpp"
#include"intro9-15.cpp"
#include"intro16-30.cpp"
#include"intro31-45.cpp"
#include"intro46-62.cpp"

int get_r(int frame, int i, int j)
{
	switch (frame)
	{
		case 1: return intro_r1[i][j];
		case 2: return intro_r2[i][j];
		case 3: return intro_r3[i][j];
		case 4: return intro_r4[i][j];
		case 5: return intro_r5[i][j];
		case 6: return intro_r6[i][j];
		case 7: return intro_r7[i][j];
		case 8: return intro_r8[i][j];
		case 9:  return intro_r9[i][j];
		case 10: return intro_r10[i][j];
		case 11: return intro_r11[i][j];
		case 12: return intro_r12[i][j];
		case 13: return intro_r13[i][j];
		case 14: return intro_r14[i][j];
		case 15: return intro_r15[i][j];
		case 16: return intro_r16[i][j];
		case 17: return intro_r17[i][j];
		case 18: return intro_r18[i][j];
		case 19: return intro_r19[i][j];
		case 20: return intro_r20[i][j];
		case 21: return intro_r21[i][j];
		case 22: return intro_r22[i][j];
		case 23: return intro_r23[i][j];
		case 24: return intro_r24[i][j];
		case 25: return intro_r25[i][j];
		case 26: return intro_r26[i][j];
		case 27: return intro_r27[i][j];
		case 28: return intro_r28[i][j];
		case 29: return intro_r29[i][j];
		case 30: return intro_r30[i][j];
		case 31: return intro_r31[i][j];
		case 32: return intro_r32[i][j];
		case 33: return intro_r33[i][j];
		case 34: return intro_r34[i][j];
		case 35: return intro_r35[i][j];
		case 36: return intro_r36[i][j];
		case 37: return intro_r37[i][j];
		case 38: return intro_r38[i][j];
		case 39: return intro_r39[i][j];
		case 40: return intro_r40[i][j];
		case 41: return intro_r41[i][j];
		case 42: return intro_r42[i][j];
		case 43: return intro_r43[i][j];
		case 44: return intro_r44[i][j];
		case 45: return intro_r45[i][j];
		case 46: return intro_r46[i][j];
		case 47: return intro_r47[i][j];
		case 48: return intro_r48[i][j];
		case 49: return intro_r49[i][j];
		case 50: return intro_r50[i][j];
		case 51: return intro_r51[i][j];
		case 52: return intro_r52[i][j];
		case 53: return intro_r53[i][j];
		case 54: return intro_r54[i][j];
		case 55: return intro_r55[i][j];
		case 56: return intro_r56[i][j];
		case 57: return intro_r57[i][j];
		case 58: return intro_r58[i][j];
		case 59: return intro_r59[i][j];
		case 60: return intro_r60[i][j];
		case 61: return intro_r61[i][j];
		case 62: return intro_r62[i][j];
		default: return -1;
	}
}

int get_g(int frame, int i, int j)
{
	switch (frame)
	{
		case 1: return intro_g1[i][j];
		case 2: return intro_g2[i][j];
		case 3: return intro_g3[i][j];
		case 4: return intro_g4[i][j];
		case 5: return intro_g5[i][j];
		case 6: return intro_g6[i][j];
		case 7: return intro_g7[i][j];
		case 8: return intro_g8[i][j];
		case 9:  return intro_g9[i][j];
		case 10: return intro_g10[i][j];
		case 11: return intro_g11[i][j];
		case 12: return intro_g12[i][j];
		case 13: return intro_g13[i][j];
		case 14: return intro_g14[i][j];
		case 15: return intro_g15[i][j];
		case 16: return intro_g16[i][j];
		case 17: return intro_g17[i][j];
		case 18: return intro_g18[i][j];
		case 19: return intro_g19[i][j];
		case 20: return intro_g20[i][j];
		case 21: return intro_g21[i][j];
		case 22: return intro_g22[i][j];
		case 23: return intro_g23[i][j];
		case 24: return intro_g24[i][j];
		case 25: return intro_g25[i][j];
		case 26: return intro_g26[i][j];
		case 27: return intro_g27[i][j];
		case 28: return intro_g28[i][j];
		case 29: return intro_g29[i][j];
		case 30: return intro_g30[i][j];
		case 31: return intro_g31[i][j];
		case 32: return intro_g32[i][j];
		case 33: return intro_g33[i][j];
		case 34: return intro_g34[i][j];
		case 35: return intro_g35[i][j];
		case 36: return intro_g36[i][j];
		case 37: return intro_g37[i][j];
		case 38: return intro_g38[i][j];
		case 39: return intro_g39[i][j];
		case 40: return intro_g40[i][j];
		case 41: return intro_g41[i][j];
		case 42: return intro_g42[i][j];
		case 43: return intro_g43[i][j];
		case 44: return intro_g44[i][j];
		case 45: return intro_g45[i][j];
		case 46: return intro_g46[i][j];
		case 47: return intro_g47[i][j];
		case 48: return intro_g48[i][j];
		case 49: return intro_g49[i][j];
		case 50: return intro_g50[i][j];
		case 51: return intro_g51[i][j];
		case 52: return intro_g52[i][j];
		case 53: return intro_g53[i][j];
		case 54: return intro_g54[i][j];
		case 55: return intro_g55[i][j];
		case 56: return intro_g56[i][j];
		case 57: return intro_g57[i][j];
		case 58: return intro_g58[i][j];
		case 59: return intro_g59[i][j];
		case 60: return intro_g60[i][j];
		case 61: return intro_g61[i][j];
		case 62: return intro_g62[i][j];
		default: return -1;
	}
}

int get_b(int frame, int i, int j)
{
	switch (frame)
	{
		case 1: return intro_b1[i][j];
		case 2: return intro_b2[i][j];
		case 3: return intro_b3[i][j];
		case 4: return intro_b4[i][j];
		case 5: return intro_b5[i][j];
		case 6: return intro_b6[i][j];
		case 7: return intro_b7[i][j];
		case 8: return intro_b8[i][j];
		case 9:  return intro_b9[i][j];
		case 10: return intro_b10[i][j];
		case 11: return intro_b11[i][j];
		case 12: return intro_b12[i][j];
		case 13: return intro_b13[i][j];
		case 14: return intro_b14[i][j];
		case 15: return intro_b15[i][j];
		case 16: return intro_b16[i][j];
		case 17: return intro_b17[i][j];
		case 18: return intro_b18[i][j];
		case 19: return intro_b19[i][j];
		case 20: return intro_b20[i][j];
		case 21: return intro_b21[i][j];
		case 22: return intro_b22[i][j];
		case 23: return intro_b23[i][j];
		case 24: return intro_b24[i][j];
		case 25: return intro_b25[i][j];
		case 26: return intro_b26[i][j];
		case 27: return intro_b27[i][j];
		case 28: return intro_b28[i][j];
		case 29: return intro_b29[i][j];
		case 30: return intro_b30[i][j];
		case 31: return intro_b31[i][j];
		case 32: return intro_b32[i][j];
		case 33: return intro_b33[i][j];
		case 34: return intro_b34[i][j];
		case 35: return intro_b35[i][j];
		case 36: return intro_b36[i][j];
		case 37: return intro_b37[i][j];
		case 38: return intro_b38[i][j];
		case 39: return intro_b39[i][j];
		case 40: return intro_b40[i][j];
		case 41: return intro_b41[i][j];
		case 42: return intro_b42[i][j];
		case 43: return intro_b43[i][j];
		case 44: return intro_b44[i][j];
		case 45: return intro_b45[i][j];
		case 46: return intro_b46[i][j];
		case 47: return intro_b47[i][j];
		case 48: return intro_b48[i][j];
		case 49: return intro_b49[i][j];
		case 50: return intro_b50[i][j];
		case 51: return intro_b51[i][j];
		case 52: return intro_b52[i][j];
		case 53: return intro_b53[i][j];
		case 54: return intro_b54[i][j];
		case 55: return intro_b55[i][j];
		case 56: return intro_b56[i][j];
		case 57: return intro_b57[i][j];
		case 58: return intro_b58[i][j];
		case 59: return intro_b59[i][j];
		case 60: return intro_b60[i][j];
		case 61: return intro_b61[i][j];
		case 62: return intro_b62[i][j];
		default: return -1;
	}
}