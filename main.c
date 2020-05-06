/*Name: Mayank Baranwal
  Roll No: 170101084
  Assignment: CS 244 Midsem Assignment - Assembler*/

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

#define op_no 12
#define sysize 10000000

int syno = 0;

//structure to store machine op and repective code
struct mcode{
	char op[5];
	char code[5];
	int occ;
}mc[op_no];

//structure to store labels and repsective positions
struct sycode{
	char sy[10];
	char pos[6];
	// struct sycode* next;
}sy[sysize];

//File pointers to access files
FILE *fp, *fpip, *fpop, *fpsy;

//Convert decimal to binary
void bina(int num, int size){
	//printf("%d\n", num);
	// printf("hello\n");
	char ans[20];
	int sp=0;
	if (size == 16) 	{
		ans[size+1] = '\0';
		sp = 1;
	}
	else
		ans[size] = '\0';	
	
	for (int i=size-1; i>=0; i--){
		int dec = num%2;
		switch (dec){
			case 0: ans[i+sp]='0'; break;
			case 1: ans[i+sp]='1'; break;
		}
		if (i==8){
			ans[i] = ' ';
			sp = 0;
		}
		num /= 2;
	}
	//Print binary number to file
	fprintf(fpop, "%s ", ans);

}

//Convert a hexadecmal number to binary
void hextobin(char code, char ans[21]){
	
	switch(code){
		case '0': strncpy(ans, "0000 ", 5);   break;
		case '1': strncpy(ans, "0001 ", 5);   break;
		case '2': strncpy(ans, "0010 ", 5);   break;
		case '3': strncpy(ans, "0011 ", 5);   break;
		case '4': strncpy(ans, "0100 ", 5);   break;
		case '5': strncpy(ans, "0101 ", 5);   break;
		case '6': strncpy(ans, "0110 ", 5);   break;
		case '7': strncpy(ans, "0111 ", 5);   break;
		case '8': strncpy(ans, "1000 ", 5);   break;
		case '9': strncpy(ans, "1001 ", 5);   break;
		case 'A': strncpy(ans, "1010 ", 5);   break;
		case 'B': strncpy(ans, "1011 ", 5);   break;
		case 'C': strncpy(ans, "1100 ", 5);   break;
		case 'D': strncpy(ans, "1101 ", 5);   break;
		case 'E': strncpy(ans, "1110 ", 5);   break;
		case 'F': strncpy(ans, "1111 ", 5);   break;
	}
	
	ans[5]='\0';
}

//Find the label and print it to the file
void findlbl(char op[7]){
	for (int i=0; i<syno; i++){
		//If label name matches
		if (!strcmp(op, sy[i].sy)){
			char syans[21];
			for (int j=0; j<4; j++){
				hextobin(sy[i].pos[j], syans);
				fprintf(fpop, "%s", syans);
			}
			fprintf(fpop, "\n");
			break;
		}
	}
}

//Find the op code and print it to the machine code file
void findop(char op[6]){
	for (int i=0; i<op_no; i++){
		if (!strcmp(op, mc[i].op)){
			if(!mc[i].occ)			
				fprintf(fpop, "%s %s\n", mc[i].op, mc[i].code);
			mc[i].occ = 1;
			break;
		}
	}
}

//Find the op code and print it to the final output file
void findop2(char op[6]){
	for (int i=0; i<op_no; i++){
		if (!strcmp(op, mc[i].op)){
			fprintf(fpop, "%s ", mc[i].code);
			break;
		}
	}
}

//Read the next 2 operands and increment instruction counter
int next2(){
	int sum = 4;
	char op1[20], op2[20], t, t2[10];
	fscanf (fpip, "%[ ]%[^,]s", t2,op1);
	fscanf (fpip, "%c%s\n", &t, op2);
	// printf("%s %s\n", op1, op2);
	//Check if operands are registers or numbers
	if (op1[0] == 'R')
		sum+=5;
	else
		sum+=16;
	
	if (op2[0] == 'R')
		sum+=5;
	else
		sum+=16;
	return (sum+7);
}

//Read the next 1 operand and increment instruction counter
int next1(char *op){
	int sum = 4;
	char op1[20];
	fscanf (fpip, "%s\n", op1);
	// printf("%s\n", op1);
	if(!strcmp(op,"MUL"))
		sum += 5;
	if (op1[0] == 'R'){
		sum+=5;
	}
	else
		sum+=16;
	return (sum+7);
}

//Read the next 2 operands and convert to machine code
int nextpr2(){
	int sum = 4;
	char op1[20], op2[20], t, t2[10];
	fscanf (fpip, "%[ ]%[^,]s", t2,op1);
	fscanf (fpip, "%c%s\n", &t, op2);
	// printf("%s %s\n", op1, op2);
	int num = 0;
	//Distinguish between register and number
	if (op1[0] == 'R'){
		sum += 5;
		num = (int)(op1[1]-'0');

		if (isdigit(op1[2])){
			num*=10;
			//printf("%c\n", op1[2]);
			num += (int)(op1[2]-'0');
		}
		bina(num, 5);
	}
	else{
		sum+=16;
		int bin = 0;
		// printf("%s\n", op1);
		for (int i=strlen(op1)-1; i>=0; i--){
			if (op1[i] == 'h' || op1[i] =='H')
				break;
			if (op1[i] == 'b' || op1[i] == 'B'){
				bin = 1;
				break;
			}
		}
		// printf("%d\n", bin);
		if (bin == 1){
			char ans[20];
			int i=0;
			for (i=0; i<strlen(op1); i++){
				if (op1[i]=='b' || op1[i]=='B')
					break;
				ans[i] = op1[i];
			}
			ans[i] = '\0';
			fprintf(fpop, "%s ", ans);
		}
		else{
			for (int i=0; i<4; i++){
				if (isdigit(op1[i]))
					num += (int)(op1[i]-'0');
				else
					num += (int)(op1[i]-'A'+10);
				if (i==3) 
					break;
				num *= 16;
			}
			bina(num, 16);
		}
	}
	num = 0;
	//Distinguish between register and number
	if (op2[0] == 'R'){
		sum+=5;
		num = (int)(op2[1]-'0');
		if (isdigit(op2[2])){
			num*=10;
			num += (int)(op2[2]-'0');
		}
		// printf("%d\n", num);
		bina(num, 5);
	}
	else{
		sum+=16;
		int bin = 0;
		// printf("%s\n", op2);
		for (int i=strlen(op2)-1; i>=0; i--){
			if (op2[i] == 'h' || op2[i] =='H')
				break;
			if (op2[i] == 'b' || op2[i] == 'B'){
				bin = 1;
				break;
			}
		}
		// printf("%d\n", bin);
		if (bin == 1){
			char ans[20];
			int i=0;
			for (i=0; i<strlen(op2); i++){
				if (op2[i]=='b' || op2[i]=='B')
					break;
				ans[i] = op2[i];
			}
			ans[i] = '\0';
			fprintf(fpop, "%s ", ans);
		}
		else{
			for (int i=0; i<4; i++){
				if (isdigit(op2[i]))
					num += (int)(op2[i]-'0');
				else
					num += (int)(op2[i]-'A'+10);
				// printf("%d\n", num);
				if (i==3) 
					break;
				num *= 16;
			}
			bina(num, 16);
		}
	}
	fprintf(fpop, "\n");
	return (sum+7);
}

//Read the next 1 operand and convert to machine code
int nextpr1(char *op){
	int sum = 4;
	char op1[20];
	fscanf (fpip, "%s\n", op1);
	// printf("%s\n", op1);
	int num = 0;
	//Distinguish between register and number
	if(!strcmp(op,"MUL"))
		sum += 5;
	if (op1[0] == 'R'){
		sum+=5;
		
		num = (int)(op1[1]-'0');
		if (isdigit(op1[2])){
			num*=10;
			num += (int)(op1[2]-'0');
		}
		//printf("%d\n", num);
		
		bina(num, 5);
	}
	else{
		sum+=16;
		int bin = 0;
		// printf("%s\n", op1);
		for (int i=strlen(op1)-1; i>=0; i--){
			if (op1[i] == 'h' || op1[i] =='H')
				break;
			if (op1[i] == 'b' || op1[i] == 'B'){
				bin = 1;
				break;
			}
		}
		// printf("%d\n", bin);
		if (bin == 1){
			char ans[20];
			int i=0;
			for (i=0; i<strlen(op1); i++){
				if (op1[i]=='b' || op1[i]=='B')
					break;
				ans[i] = op1[i];
			}
			ans[i] = '\0';
			fprintf(fpop, "%s ", ans);
		}
		else{
			for (int i=0; i<4; i++){
				if (isdigit(op1[i]))
					num += (int)(op1[i]-'0');
				else
					num += (int)(op1[i]-'A'+10);
				if (i==3) 
					break;
				num *= 16;
			}
			bina(num, 16);
		}
	}
	fprintf(fpop, "\n");
	return (sum+7);
}

// char pos[7];


//Convert from decimal to hex and send with H
void hexa (int cnt, char pos[7]){
	int big = 0;
	// if (cnt >= 40960)
	// 	big = 1;

	pos[5+big] = '\0';
	pos[4+big] = 'H';
	for (int i=3; i>=0; i--){
		int no = cnt%16;
		switch(no){
			case 0: pos[i+big]='0'; break;
			case 1: pos[i+big]='1'; break;
			case 2: pos[i+big]='2'; break;
			case 3: pos[i+big]='3'; break;
			case 4: pos[i+big]='4'; break;
			case 5: pos[i+big]='5'; break;
			case 6: pos[i+big]='6'; break;
			case 7: pos[i+big]='7'; break;
			case 8: pos[i+big]='8'; break;
			case 9: pos[i+big]='9'; break;
			case 10: pos[i+big]='A'; break;
			case 11: pos[i+big]='B'; break;
			case 12: pos[i+big]='C'; break;
			case 13: pos[i+big]='D'; break;
			case 14: pos[i+big]='E'; break;
			case 15: pos[i+big]='F'; break;
		}
		cnt /= 16;
	}
	if (big)
		pos[0] = '0';
}

//Convert from decimal to hex 
void hexa2 (int cnt, char pos[7]){
	int big = 0;
	// if (cnt >= 40960)
	// 	big = 1;

	pos[5+big-1] = '\0';
	for (int i=3; i>=0; i--){
		int no = cnt%16;
		switch(no){
			case 0: pos[i+big]='0'; break;
			case 1: pos[i+big]='1'; break;
			case 2: pos[i+big]='2'; break;
			case 3: pos[i+big]='3'; break;
			case 4: pos[i+big]='4'; break;
			case 5: pos[i+big]='5'; break;
			case 6: pos[i+big]='6'; break;
			case 7: pos[i+big]='7'; break;
			case 8: pos[i+big]='8'; break;
			case 9: pos[i+big]='9'; break;
			case 10: pos[i+big]='A'; break;
			case 11: pos[i+big]='B'; break;
			case 12: pos[i+big]='C'; break;
			case 13: pos[i+big]='D'; break;
			case 14: pos[i+big]='E'; break;
			case 15: pos[i+big]='F'; break;
		}
		cnt /= 16;
	}
	if (big)
		pos[0] = '0';
}


int main(){

	// struct sycode** sy2 = (struct sycode**)malloc(sizeof(struct sycode**));
	// sy->head = (sycode*)malloc(sizeof(sycode));

	fp = fopen("mccode.txt", "r");
	if (fp == NULL){
		printf("Unable to open mcode.txt. Aborting");
		return 0;
	}
	char str[5];
	char cd[5];
	//Reading machines codes and values and stroing them in structure
	for (int i=0; i<op_no; i++){
		fscanf(fp, "%s %s\n", str, cd);
		strcpy(mc[i].op, str);
		strcpy(mc[i].code, cd);
		mc[i].occ = 0;
	}
	fclose(fp);

	// for (int i=0; i<op_no; i++){
	// 	printf("%s %s\n", mc[i].op, mc[i].code);
	// }
	//Rading input assembler code  --> First pass
	fpip = fopen("input.asm", "r");
	fpop = fopen("opTable.txt", "w");
	fpsy = fopen("symTable.txt", "w");
	//Exception handling
	if (fpip == NULL){
		printf("Unable to open input.asm. Aborting");
		return 0;
	}
	if (fpop == NULL){
		printf("Unable to open opTable.txt. Aborting");
		return 0;
	}
	if (fpsy == NULL){
		printf("Unable to open symTable.txt. Aborting");
		return 0;
	}
	int done = 0;
	int cnt = 0;
	while(!feof(fpip)){
		char op[1000], op1[20], op2[20];
		//Read first word of a line and decide course of action
		fscanf(fpip, "%s", op);
		for (int i=0; i<strlen(op); i++)
			op[i] = toupper(op[i]);
		int sum = 0;
		//Continue again
		if (!strcmp(op, "START")){
			// printf("\n");
			continue;
		}
		//End reading
		else if (!strcmp(op, "END")){
			// printf("\n");
			break;
		}
		//Read next 2 operations
		else if (!strcmp(op, "MOV")){
			findop(op);
			cnt += next2()/8;
			// break;
		}
		//Read next 2 operations
		else if (!strcmp(op, "ADD")){
			findop(op);
			cnt += next2()/8;
		}
		//Read next 2 operations
		else if (!strcmp(op, "SUB")){
			findop(op);
			cnt += next2()/8;
		}
		//Read next 1 operation
		else if (!strcmp(op, "MUL")){
			findop(op);
			cnt += next1(op)/8;
		}
		//Read next 2 operations
		else if (!strcmp(op, "CMP")){
			findop(op);
			cnt += next2()/8;
		}
		//Read next 2 operations
		else if (!strcmp(op, "AND")){
			findop(op);
			cnt += next2()/8;
		}
		//Read next 2 operations
		else if (!strcmp(op, "OR")){
			findop(op);
			cnt += next2()/8;
		}
		//Read next 1 operation
		else if (!strcmp(op, "NOT")){
			findop(op);
			cnt += next1(op)/8;
		}
		//Read next 1 operation
		else if (!strcmp(op, "JMP")){
			findop(op);
			fscanf (fpip, "%s\n", op1);
			cnt += 3;
		}
		//Read next 1 operation
		else if (!strcmp(op, "LOOP")){
			// findop(op);
			fscanf (fpip, "%s\n", op1);
			// printf("%s\n", op1);
			cnt += 7;
			//Mention SUB and JNZ in op code table
			findop("SUB");
			findop("JNZ");
		}
		else if (!strcmp(op, "HLT")){
			findop(op);
			// printf("\n");
			cnt += 1;
		}
		//Read next 1 operation
		else if (!strcmp(op, "JNZ")){
			findop(op);
			fscanf (fpip, "%s\n", op1);
			// printf("%s\n", op1);
			cnt += 3;
		}
		else{
			// fscanf (fpip, "%s", op1);
			// printf("%s ", op1);
			for (int i=0; i<strlen(op); i++){
				if (op[i] == ':')
					op[i] = '\0';
			}
			if (!strlen(op))
				continue;
			strcpy(sy[syno].sy, op);
			char pos[7];
			hexa(cnt, pos);
			strcpy(sy[syno].pos, pos);
			syno++;
			continue;
		}
		// printf("\n");
	}

	//Write all the labels and respective positions in file
	for (int i=0; i<syno; i++){
		fprintf(fpsy, "%s: %s\n", sy[i].sy, sy[i].pos);
	}

	fclose(fpip);
	fclose(fpop);
	fclose(fpsy);
	//End of first pass

//-----------------------------------------------------------------------------

	//Start of second pass
	cnt = 0;
	fpip = fopen("input.asm", "r");
	fpop = fopen("output.o", "w");
	fprintf(fpop, "ADDRESS\t\t\t\tINSTRUCTION\n");
	while(!feof(fpip)){
		char op[6], op1[20], op2[20];
		fscanf(fpip, "%s", op);
		for (int i=0; i<strlen(op); i++)
			op[i] = toupper(op[i]);
		int sum = 0;
		// printf("%s ", op);
		if (!strcmp(op, "START")){
			// printf("\n");
			// break;
			continue;
		}
		else if (!strcmp(op, "END")){
			// printf("\n");
			break;
		}
		char pos[7], ans1[20], ans2[20];
		hexa2(cnt, pos);
		
		//Read next 2 operations and convert to machine code
		if (!strcmp(op, "MOV")){
			fprintf(fpop, "%s\t\t\t\t", pos);
			findop2(op);
			cnt += nextpr2()/8;
			// break;
		}
		//Read next 2 operations and convert to machine code
		else if (!strcmp(op, "ADD")){
			fprintf(fpop, "%s\t\t\t\t", pos);
			findop2(op);
			cnt += nextpr2()/8;
		}
		//Read next 2 operations and convert to machine code
		else if (!strcmp(op, "SUB")){
			fprintf(fpop, "%s\t\t\t\t", pos);
			findop2(op);
			cnt += nextpr2()/8;
		}
		//Read next operation and convert to machine code
		else if (!strcmp(op, "MUL")){
			fprintf(fpop, "%s\t\t\t\t", pos);
			findop2(op);
			fprintf(fpop, "00001 ");
			// printf("%s ", op);
			
			cnt += nextpr1(op)/8;
		}
		//Read next 2 operations and convert to machine code
		else if (!strcmp(op, "CMP")){
			fprintf(fpop, "%s\t\t\t\t", pos);
			findop2(op);
			cnt += nextpr2()/8;
		}
		//Read next 2 operations and convert to machine code
		else if (!strcmp(op, "AND")){
			fprintf(fpop, "%s\t\t\t\t", pos);
			findop2(op);
			cnt += nextpr2()/8;
		}
		//Read next 2 operations and convert to machine code
		else if (!strcmp(op, "OR")){
			fprintf(fpop, "%s\t\t\t\t", pos);
			findop2(op);
			cnt += nextpr2()/8;
		}
		//Read next  operation and convert to machine code
		else if (!strcmp(op, "NOT")){
			fprintf(fpop, "%s\t\t\t\t", pos);
			findop2(op);
			cnt += nextpr1(op)/8;
		}
		//Read next operation and convert to machine code
		else if (!strcmp(op, "JMP")){
			fprintf(fpop, "%s\t\t\t\t", pos);
			findop2(op);
			fscanf (fpip, "%s\n", op1);
			//Convert label to machine code
			findlbl(op1);
			cnt += 3;
		}
		//Read next operation and convert to machine code
		else if (!strcmp(op, "LOOP")){
			//Loop broken into subtraction and jumping
			fprintf(fpop, "%s\t\t\t\t", pos);
			findop2("SUB");
			//Subtract 1 from counter register
			fprintf(fpop, "11111 00000000 00000001\n");
			cnt+=4;
			hexa2(cnt, pos);
			fprintf(fpop, "%s\t\t\t\t", pos);
			// printf("%s\t\t\t\t", pos);
			findop2("JNZ");
			fscanf (fpip, "%s\n", op1);
			findlbl(op1);
			cnt += 3;
		}
		else if (!strcmp(op, "HLT")){
			fprintf(fpop, "%s\t\t\t\t", pos);
			findop2(op);
			fprintf(fpop, "\n");
			cnt += 1;
			continue;
		}
		//Read next operation and convert to machine code
		else if (!strcmp(op, "JNZ")){
			fprintf(fpop, "%s\t\t\t\t", pos);
			findop2(op);
			fscanf (fpip, "%s\n", op1);
			findlbl(op1);
			cnt += 3;
		}
		else{
			continue;
		}
	}

	fclose(fpip);
	//End of second pass
	//----------------------------------------------------------------------
	return 0;
}