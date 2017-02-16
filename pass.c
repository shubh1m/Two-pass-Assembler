//Code for generating binary expression of assembler code.

#include<stdio.h>
#include<stdlib.h>

static FILE *inp, *pass1, *op, *label;
char  *r, *in, *rr, *opcode, *lbl;
int zz;

void openfiles();
void initialize();
int length(char *);
int check_equal(char *, char *);
void print_binary_opcode();
void print_binary_number(char *);
void print_binary_label(char *);
void print_binary_register(char *);
void closefiles();

void openfiles()
{
	char i[50];
	printf("Enter the input file (code1.txt OR code2.txt)\n");
	scanf("%s",i);
	int x = 0;
	zz = 2;
	while(i[x] != '\0')
	{
		if(i[x] == '1')
			zz = 1;
		x++;
	}
    inp = fopen(i, "r");
    if(i[4] == '1')
    {
        pass1 = fopen("binary1.txt", "w");
        printf("Output is generated in binary1.txt\n");
    }
    else
    {
        pass1 = fopen("binary2.txt", "w");
        printf("Output is generated in binary2.txt\n");
    }
}

void initialize()
{
    in = (char *)malloc(100 * sizeof(char));
    rr = (char *)malloc(50 * sizeof(char));
    opcode = (char *)malloc(50 * sizeof(char));
    lbl = (char *)malloc(20 * sizeof(char));
    r = (char *)malloc(20 * sizeof(char));
}

int length(char *s)
{
    int l = 0;
    while(s[l] != '\0')
        l++;
    return l;
}

int check_equal(char *a, char *b)
{
    if(length(a) != length(b))
        return 0;
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0')
    {
        if(a[i] != b[i])
            return 0;
        i++;
    }
    return 1;
}

void print_binary_opcode(char *opc)
{
    op = fopen("opcode.txt", "r");

    int fl, c = 1;
    while(fscanf(op, "%s", opcode) == 1)
    {
        if(c&1)
            fl = check_equal(opc, opcode);
        else if(fl)
        {
            fprintf(pass1, "%s", opcode);
            fclose(op);
            return ;
        }
        c++;
    }

    fclose(op);
}

void print_binary_number(char *k)
{
    int l = length(k), x = 0, i = 1;
    while(i < l)
    {
        x = x * 10 + (k[i] - '0');
        i++;
    }
    char temp[10];
    i = 0;
    while(x)
    {
        if(x&1)
            temp[i++] = '1';
        else
            temp[i++] = '0';
        x /= 2;
    }
    while(i < 8)
        temp[i++] = '0';
    while(i)
        fprintf(pass1, "%c", temp[--i]);
}

void print_binary_label(char *x)
{
	FILE *label;
	if(zz == 2)
    	label = fopen("label2.txt", "r");
	else
		label = fopen("label1.txt", "r");

    int fl = 0, c = 1;
    while(fscanf(label, "%s", lbl) == 1)
    {
        if(c&1)
            fl = check_equal(lbl, x);
        else if(fl)
        {
            print_binary_number(lbl);
            fclose(label);
            return ;
        }
        c++;
    }

    fclose(label);
}

void print_binary_register(char *ch)
{
    FILE *reg = fopen("reg.txt", "r");

    int fl, c = 1;
    while(fscanf(reg, "%s", rr) == 1)
    {
        if(c&1)
            fl = check_equal(rr, ch);
        else if(fl)
        {
            fprintf(pass1, "%s", rr);
            fclose(reg);
            return ;
        }
        c++;
    }
    fclose(reg);
}

int main(int argc, char *argv[])
{
    openfiles();
    initialize();

    in = (char *)malloc(100 * sizeof(char));

    char *opc = (char *)malloc(8 * sizeof(char));

    while(fscanf(inp, "%s", in) == 1)
    {
        int flag_num = 0, i = 0, flag_reg = 0;
        while(in[i] != '\0')
        {
            if(in[i] == '$')
                flag_num = 1;

            if(in[i] == '.' || in[i] == ',')
                if(in[i + 1] == 'R')
                    flag_reg++;
            i++;
        }

        int l = length(in);
        if(in[l - 1] == ':')
        {
            in[--l] = '\0';
            print_binary_opcode(in);
            fprintf(pass1, "00\n");
            continue;
        }
        int o = 0;
        while(in[o] != '\0' && in[o] != '.')
            opc[o] = in[o++];
        opc[o] = '\0';

        print_binary_opcode(opc);

        if(flag_num)
            fprintf(pass1, "11");
        else if(flag_reg == 2)
            fprintf(pass1, "01");
        else
            fprintf(pass1, "00");

        if(in[o] == '\0')
        {
            fprintf(pass1, "\n");
            continue;
        }

        o++;
        i = 0;
        while(in[o] != '\0' && in[o] != ',')
            r[i++] = in[o++];
        r[i] = '\0';

        if(r[0] == '@')
        {
            int l = length(r), z;
            for(z = 1;z < l;z++)
                r[z - 1] = r[z];
            r[l - 1] = '\0';
            print_binary_label(r);
        }
        else if(r[0] == '$')
            print_binary_number(r);
        else
            print_binary_register(r);

        if(in[o] == '\0')
        {
            fprintf(pass1, "\n");
            continue;
        }

        o++;
        i = 0;
        while(in[0] != '\0')
            r[i++] = in[o++];

        if(r[0] == '$')
            print_binary_number(r);
        else
            print_binary_register(r);

        fprintf(pass1, "\n");
    }

    closefiles();
    return 0;
}

void closefiles()
{
    fclose(inp);
    fclose(pass1);
}
