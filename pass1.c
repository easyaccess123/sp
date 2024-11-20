#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void main(){
    FILE *f1,*f2,*f3,*f4;//file pointers
    int start,location_counter,length,f=0;
    char label[10],opcode[10],operand[10],mnemonic[10],code[10],extra[10];
    //input file,symbol table and intermediate file
    f1=fopen("input.txt","r");
    f3=fopen("symtab.txt","w");
    f4=fopen("output.txt","w");
    printf("SYMTAB IS:\n");
    fscanf(f1,"%s\t%s\t%s",label,opcode,operand);
    if (strcmp(opcode,"START")==0)
    {
        start=atoi(operand);
        location_counter=start;
        fprintf(f4,"\t%s\t%s\t%s\n",label,opcode,operand);
        fscanf(f1,"%s\t%s\t%s",label,opcode,operand);
    }
    else
    {
        location_counter=0;
    }
    while(!feof(f1)){//end of input filr
        fprintf(f4,"\t%d",location_counter);
        if(strcmp(label,"--")!=0){
            fprintf(f3,"%d\t%s\n",location_counter,label);
            printf("\t%d\t%s\n",location_counter,label);
        }
        f2=fopen("optab.txt","r");
        fscanf(f2,"%s\t%s",code,mnemonic);
        while (!feof(f2))//end of optab
        {
            if(strcmp(code,opcode)==0){
                location_counter+=3;
                break;
            }
            fscanf(f2,"%s\t%s",code,mnemonic);
        }
        fclose(f2);
        if(strcmp(opcode,"WORD")==0)
            location_counter+=3;
        else if(strcmp(opcode,"RESW")==0)
            location_counter+=(3*(atoi(operand)));
        else if(strcmp(opcode,"RESB")==0)
        {
            location_counter+=(atoi(operand));
        }
       else if(strcmp(opcode,"BYTE")==0) 
        {
            location_counter+=strlen(operand);
        }
        fprintf(f4,"\t%s\t%s\n",label,opcode,operand);//write to file
        fscanf(f1,"%s\t%s\t%s",label,opcode,operand);//read to file
    }
    fprintf(f4,"\t%d\t%s\t%s",location_counter,label,opcode,operand);
    //compute length of input source code and display it
    length=location_counter-start;
    printf("Length of the input source code is: %d",length);
    fclose(f1);
    fclose(f3);
    fclose(f4);
}