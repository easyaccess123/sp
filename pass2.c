#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
    char mnemonic[15],code[15],a[10], ad[10], label[10], opcode[10], operand[10], symbol[10];
    int start, diff, i, address, add, len, actual_len, finaddr, prevaddr;

    FILE *f1, *f2, *f3, *f4, *f5;
    f1 = fopen("out.txt", "w");
    f2 = fopen("symtab.txt", "r");
    f3 = fopen("in.txt", "r");
    f4 = fopen("objectcode.txt", "w");
    f5 = fopen("optab.txt","r");

    fscanf(f3, "%s\t%s\t%s", label, opcode, operand);
    start=atoi(operand);
    while (strcmp(opcode, "END") != 0)
    {
        prevaddr = address;
        fscanf(f3, "%d%s%s%s", &address, label, opcode, operand);
    }
    finaddr = address;

    fclose(f3);
    f3 = fopen("in.txt", "r");

    fscanf(f3, "\t%s\t%s\t%s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0)
    {
        fprintf(f1, "\t%s\t%s\t%s\n", label, opcode, operand);
        fprintf(f4, "H^%s^00%s^00%d\n", label, operand, finaddr-start);
        fscanf(f3, "%d%s%s%s", &address, label, opcode, operand);
        start = address;
        diff = prevaddr - start;
        fprintf(f4, "T^00%d^%d", address, diff);
    }

    while (strcmp(opcode, "END") != 0)
    {
        if (strcmp(opcode, "BYTE") == 0)
        {
            fprintf(f1, "%d\t%s\t%s\t%s\t", address, label, opcode, operand);
            len = strlen(operand);
            actual_len = len - 3;
            fprintf(f4, "^");
            for (i = 2; i < (actual_len + 2); i++)
            {
                itoa(operand[i], ad, 16);
                fprintf(f1, "%s", ad);
                fprintf(f4, "%s", ad);
            }
            fprintf(f1, "\n");
        }

        else if (strcmp(opcode, "WORD") == 0)
        {
            len = strlen(operand);
            itoa(atoi(operand), a, 10);
            fprintf(f1, "%d\t%s\t%s\t%s\t00000%s\n", address, label, opcode, operand, a);
            fprintf(f4, "^00000%s", a);
        }

        else if ((strcmp(opcode, "RESB") == 0) || (strcmp(opcode, "RESW") == 0)) {
            fprintf(f1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
        }

        else
        {
            fscanf(f5,"%s%s",mnemonic,code);
            while (strcmp(opcode, mnemonic) != 0)
                fscanf(f5,"%s%s",mnemonic,code);
                //j++;
            if (strcmp(operand, "COPY") == 0)
                fprintf(f1, "%d\t%s\t%s\t%s\t%s0000\n", address, label, opcode, operand, code);
            else
            {
                rewind(f2);
                fscanf(f2, "%s%d", symbol, &add);
                while (strcmp(operand, symbol) != 0)
                    fscanf(f2, "%s%d", symbol, &add);
                fprintf(f1, "%d\t%s\t%s\t%s\t%s%d\n", address, label, opcode, operand, code, add);
                fprintf(f4, "^%s%d", code, add);
            }
        }

        fscanf(f3, "%d%s%s%s", &address, label, opcode, operand);
    }

    fprintf(f1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
    fprintf(f4, "\nE^00%d", start);

    fclose(f4);
    fclose(f3);
    fclose(f2);
    fclose(f1);
    fclose(f5);

    char ch;
    printf("\n");
    f4 = fopen("objectcode.txt", "r");
    ch = fgetc(f4);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = fgetc(f4);
    }
    fclose(f4);
    printf("\n");
}