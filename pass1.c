// Code for generating label table which are generated in PASS '1' of assembler.

#include<stdio.h>

static FILE *inp, *pass1;
static char in[100];

void openfiles();
void closefiles();

void openfiles()
{
    char i[100],o[100];
    printf("Enter the input file(code1.txt OR code2.txt)\n");
    scanf("%s",i);
    inp = fopen(i, "r");
    if(i[4] == '1')
    {
        pass1 = fopen("label1.txt", "w");
        printf("Output is generated in label1.txt\n");
    }
    else
    {
        pass1 = fopen("label2.txt", "w");
        printf("Output is generated in label2.txt\n");
    }
}

int main(int argc, char *argv[])
{
    openfiles();

    int c = 1;
    while(fscanf(inp, "%s", in) == 1)
    {
        int l = 0;
        for(l = 0;in[l] != '\0';)
            l++;
        if(l > 1 && in[l - 1] == ':')
        {
            in[l - 1] = '\0';
            fprintf(pass1, "%s $%d\n",in,c + 1);
        }
        c++;
    }

    closefiles();
}

void closefiles()
{
    fclose(inp);
    fclose(pass1);
}
