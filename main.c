/*
 * File: main.c
 * Creator: Jimmy Li
 * Created: Tue Dec 7 19:11:24 2017
 * Time-stamp: <Tue Dec 10 23:15:42 EDT 2017 li>
 *
 * Project 5:
 * Boolean Logic and Boolean Circuits
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Circuit.h"

static Circuit* CircuitA(){
	Value* inX = new_Value(false);
	Value* inY = new_Value(false);
	Value* inZ = new_Value(false);
	Gate* not0 = new_Inverter(inY);
	Gate* and0 = new_AndGate(inX, Gate_getOutput(not0));
	Gate* and1 = new_AndGate(inY, inZ);
	Gate* or0 = new_OrGate(Gate_getOutput(and0), Gate_getOutput(and1));

	Value** inputs = new_Value_array(3);
	inputs[0] = inX;
	inputs[1] = inY;
	inputs[2] = inZ;
	Value** outputs = new_Value_array(1);
	outputs[0] = Gate_getOutput(or0);
	Gate** gates = new_Gate_array(4);
	gates[0] = not0;
	gates[1] = and0;
	gates[2] = and1;
	gates[3] = or0;
	return new_Circuit(3, inputs, 1, outputs, 4, gates);
}

static Circuit* CircuitB(){
	Value* inX = new_Value(false);
	Value* inY = new_Value(false);
	Value* inZ = new_Value(false);
	Gate* not0 = new_Inverter(inY);
	Gate* and0 = new_AndGate(inX, Gate_getOutput(not0));
	Gate* not1 = new_Inverter(Gate_getOutput(and0));
	Gate* and1 = new_AndGate(inY, inZ);
	Gate* not2 = new_Inverter(Gate_getOutput(and1));
	Gate* or0 = new_OrGate(Gate_getOutput(not1), Gate_getOutput(not2));
	Gate* not3 = new_Inverter(Gate_getOutput(or0));

	Value** inputs = new_Value_array(3);
	inputs[0] = inX;
	inputs[1] = inY;
	inputs[2] = inZ;
	Value** outputs = new_Value_array(1);
	outputs[0] = Gate_getOutput(not3);
	Gate** gates = new_Gate_array(7);
	gates[0] = not0;
	gates[1] = and0;
	gates[2] = not1;
	gates[3] = and1;
	gates[4] = not2;
	gates[5] = or0;
	gates[6] = not3;
	return new_Circuit(3, inputs, 1, outputs, 7, gates);
}

static Circuit* CircuitC(){
	Value* inX = new_Value(false);
	Value* inY = new_Value(false);
	Gate* and0 = new_AndGate(inX, inY);
	Gate* not0 = new_Inverter(inX);
	Gate* not1 = new_Inverter(inY);
	Gate* and1 = new_AndGate(Gate_getOutput(not0), Gate_getOutput(not1));
	Gate* or0 = new_OrGate(Gate_getOutput(and0), Gate_getOutput(and1));

	Value** inputs = new_Value_array(2);
	inputs[0] = inX;
	inputs[1] = inY;
	Value** outputs = new_Value_array(1);
	outputs[0] = Gate_getOutput(or0);
	Gate** gates = new_Gate_array(5);
	gates[0] = and0;
	gates[1] = not0;
	gates[2] = not1;
	gates[3] = and1;
	gates[4] = or0;
	return new_Circuit(2, inputs, 1, outputs, 5, gates);
}

static Circuit* Circuit_One_Bit_Adder(){
	Value* inX = new_Value(false);
	Value* inY = new_Value(false);
	Value* inC = new_Value(false);
	Gate* not1 = new_Inverter(inX);
	Gate* not2 = new_Inverter(inY);
	Gate* not3 = new_Inverter(inC);
	Gate* and1 = new_And3Gate(Gate_getOutput(not1),Gate_getOutput(not2),inC);
	Gate* and2 = new_And3Gate(Gate_getOutput(not1), inY, Gate_getOutput(not3));
	Gate* and3 = new_And3Gate(Gate_getOutput(not1), inY, inC);
	Gate* and4 = new_And3Gate(inX, Gate_getOutput(not2), Gate_getOutput(not3));
	Gate* and5 = new_And3Gate(inX, Gate_getOutput(not2), inC);
	Gate* and6 = new_And3Gate(Gate_getOutput(not3),inY, inX);
	Gate* and7 = new_And3Gate(inX,inY, inC);
	Gate* or1 = new_Or4Gate(Gate_getOutput(and1),Gate_getOutput(and2), Gate_getOutput(and4), Gate_getOutput(and7));
	Gate* or2 = new_Or4Gate(Gate_getOutput(and3), Gate_getOutput(and5), Gate_getOutput(and6), Gate_getOutput(and7));

	Value** inputs = new_Value_array(3);
	inputs[0] = inX;
	inputs[1] = inY;
	inputs[2] = inC;
	Value** outputs = new_Value_array(2);
	outputs[0] = Gate_getOutput(or1); //Output z, the sum
	outputs[1] = Gate_getOutput(or2); //Output d, the carry-out
	Gate** gates = new_Gate_array(12);
	gates[0] = not1;
	gates[1] = not2;
	gates[2] = not3;
	gates[3] = and1;
	gates[4] = and2;
	gates[5] = and3;
	gates[6] = and4;
	gates[7] = and5;
	gates[8] = and6;
	gates[9] = and7;
	gates[10] = or1;
	gates[11] = or2;
	return new_Circuit(3, inputs, 2, outputs, 12, gates);
}

static char* b2s(bool b) {
	return b ? "T" : "F";
}

static void testAllPossibleCombinations(Circuit* circuit){
	int numInputs = Circuit_numInputs(circuit);
	int powNumInputs = pow(2, numInputs);
	int powNumInputs_copy = powNumInputs;
	int values_index = 0;
	int h, i, j, k, l, m, o, p, q;
	//initialized allCombos 2Darray as -1
	int allCombos[powNumInputs][numInputs];
	for (h = 0; h < powNumInputs; h++){
		for (i = 0; i < numInputs; i++){
			allCombos[h][i] = -1;
		}
	}
	//initialize values array as -1
	int values[10];
	for (j = 0; j < 10; j++){
		values[j] = -1;
	}
	//fill value array with values for each input
	while (powNumInputs_copy > 0){
		values[values_index] = powNumInputs_copy/2;
		values_index++;
		powNumInputs_copy = powNumInputs_copy/2;
	}
	//initialized booleans array as 0
	int booleans[numInputs];
	for (k = 0; k < numInputs; k++){
		booleans[k] = 0;
	}
	int temp_boolean;
	//outermost loop: go through all the numInputs
	for (m = 0; m < numInputs; m++){
		temp_boolean = 1;
		//second outermost loop: go through all the powNumInputs
		for (l = 0; l < powNumInputs; l++){
			//if l%value[m] == 0, toggle boolean;
			if (l%values[m] == 0){
				temp_boolean = (temp_boolean+1)%2;
			}
			allCombos[l][m] = temp_boolean;
		}
	}
	for (o = 0; o < powNumInputs; o++){
		for (p = 0; p < numInputs; p++){
			Circuit_setInput(circuit, p, allCombos[o][p]);
		}
		Circuit_update(circuit);
		bool out0 = Circuit_getOutput(circuit, 0);
		bool out1;
		if (Circuit_numOutputs(circuit)==2) {
			out1 = Circuit_getOutput(circuit, 1);
		}

		for (q = 0; q < numInputs; q++){
			printf("%s ", b2s(allCombos[o][q]));
		}

		if (Circuit_numOutputs(circuit)==2) {
			printf("-> %s", b2s(out0));
			printf(" %s\n", b2s(out1));
		} else {
			printf("-> %s\n", b2s(out0));
		}
	}
}


int main(int argc, char **argv) {
	Circuit* c0 = CircuitA();
	printf("Circuit (a):\n");
	printf("Format: x y z -> output\n");
	testAllPossibleCombinations(c0);
	Circuit* c1 = CircuitB();
	printf("Circuit (b):\n");
	printf("Format: x y z -> output\n");
	testAllPossibleCombinations(c1);
	Circuit* c2 = CircuitC();
	printf("Circuit (c):\n");
	printf("Format: x y -> z\n");
	testAllPossibleCombinations(c2);
	Circuit* c3 = Circuit_One_Bit_Adder();
	printf("Circuit (One-Bit Adder):\n");
	printf("Format: x y c -> d\n");
	testAllPossibleCombinations(c3);
}
