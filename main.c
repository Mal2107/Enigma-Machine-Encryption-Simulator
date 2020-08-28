#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define r1   "EKMFLGDQVZNTOWYHXUSPAIBRCJ"
#define r2   "AJDKSIRUXBLHWTMCQGZNPYFVOE"
#define r3   "BDFHJLCPRTXVZNYEIWGAKMUSQO"
#define r4   "ESOVPZJAYQUIRHXLNFTGKDCMWB"
#define r5   "VZBRGITYUPSDNHLXAWMJQOFECK"
#define refA "EJMZALYXVBWFCRQUONTSPIKHGD"
#define refB "YRUHQSLDPXNGOKMIEBFZCWVJAT"
#define MAX_LEN 100

FILE *fptr;
struct rotors
{
    char rotorWiring[27];
    int offsetOfRotor;
};

struct rotors rot[3];
char alpha[]  = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char ref[27]  = "";
int offset=0;
char plugBoardConnection[5][3];

char passThroughRotors(char);
char plugBoardEncryption(char ch);
void writeInFile(char*);
void readFile();


int convertAlphaToNumeric(char ch)
{
    int ascii_val = (int)ch;
    return ascii_val-65;
}

void setupEnigma()
{
    int choice;
    printf("\nEnter the NAME of rotors and their STARTING POSITION [rotor position from right to left] ! \n");
    for(int i = 2; i>=0; i--)
    {
        printf("Please select rotor %d ( I - 1 ,  II - 2 , III - 3 , IV - 4 , V - 5): ",i+1);
        scanf("%d",&choice);
        switch(choice)
        {
        case 1:
            strcpy(rot[i].rotorWiring,r1);
            break;
        case 2:
            strcpy(rot[i].rotorWiring,r2);
            break;
        case 3:
            printf("Ëxecuted the case");
            strcpy(rot[i].rotorWiring,r3);
            break;
        case 4:
            strcpy(rot[i].rotorWiring,r4);
            break;
        case 5:
            strcpy(rot[i].rotorWiring,r5);
            break;

        }
        fflush(stdin);
        printf("Enter the starting pos. of rotor %d (A-Z): ",i+1);
        char startingPos;
        startingPos = getchar();
        fflush(stdin);
        rot[i].offsetOfRotor = convertAlphaToNumeric(startingPos);

    }

    printf("\nPlease enter reflector (1=A , 2=B): ");
    scanf("%d",&choice);
    switch(choice)
    {
    case 1:
        strcpy(ref,refA);
        break;
    case 2:
        strcpy(ref,refB);
        break;
    default:
        strcpy(ref,refB);
    }
    fflush(stdin);
}

void getPlugBoardEncryptions()
{
    for(int i=0; i<5; i++)
    {
        char plugBoardConnections[3]="";
        printf("\nEnter plug board connection (In the format char1char2 [Eg - QA])%d: ",(i+1));
        scanf("%s",plugBoardConnections);
        fflush(stdin);
        plugBoardConnection[i][0] = plugBoardConnections[0];
        plugBoardConnection[i][1] = plugBoardConnections[1];
    }
    printf("\n\n\nENCRYPTION STEPS");
    printf("\n----------------\n\n");

}

char rotorEncryption(char ch,int rotorNumber,bool encrypt)
{
    char rotor[27]="";
    switch(rotorNumber)
    {
    case 1:
        strcpy(rotor,rot[0].rotorWiring);
        offset = rot[0].offsetOfRotor;
        break;
    case 2:
        strcpy(rotor,rot[1].rotorWiring);
        if((rot[1].offsetOfRotor==4)&&encrypt==true)
        {
            rot[1].offsetOfRotor+=1;
            rot[0].offsetOfRotor+=1;
        }
        else if(rot[2].offsetOfRotor==22 && encrypt==true)
        {
            rot[1].offsetOfRotor+=1;
        }
        offset = rot[1].offsetOfRotor;
        break;
    case 3:
        strcpy(rotor,rot[2].rotorWiring);
        if(encrypt==true)
        {
            rot[2].offsetOfRotor+=1;
        }
        offset = rot[2].offsetOfRotor;
        break;
    }

    if(offset>25)
    {
        offset = offset-26;
    }
    int inputSignal = convertAlphaToNumeric(ch);
    int inputContact = inputSignal+offset;
    if(inputContact>25)
    {
        inputContact = inputContact-26;
    }
    int outputContact;
    if(encrypt==true)
    {
        outputContact = convertAlphaToNumeric(rotor[inputContact]);
    }
    else
    {
        //printf("\nThe rotor wiring is %s",rotor);
        char inverse = (char)(inputContact+65);
        for(int i = 0; i<26; i++)
        {
            //printf("\n%c",rotor[i]);
            if(inverse==rotor[i])
            {
                outputContact = i;
                break;
            }
        }
    }
    int outPutSignal = outputContact-offset;
    if(outPutSignal<0)
    {
        outPutSignal=outPutSignal+26;
    }
    char encryptedByRotor = (char)(outPutSignal+65);


    return encryptedByRotor;
}

char reflection(char input)
{
    int inputSignal = convertAlphaToNumeric(input);
    return ref[inputSignal];
}

char plugBoardEncryption(char ch)
{
    for(int i =0; i<5; i++)
    {
        if(ch==plugBoardConnection[i][0])
        {
            return plugBoardConnection[i][1];
        }
        else if(ch==plugBoardConnection[i][1])
        {
            return plugBoardConnection[i][0];
        }
    }
    return ch;
}
char* encryptString(char line[])
{
    char encrypted[MAX_LEN] = "";
    for(int i = 0; line[i]!='\0'; i++)
    {
        if(isalpha(line[i])==1)
        {
            encrypted[i] = passThroughRotors(line[i]);
        }
        else
        {
            encrypted[i] = "-";
        }
    }
    printf("\n\n\nThe encrypted string is %s\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n",encrypted);
    return *encrypted;
}

char passThroughRotors(char ch)
{
    printf("\nCHAR to be encrypted : %c",ch);

    char plugBoardencrypt = plugBoardEncryption(ch);
    printf("\nPlugBoard Encryptions: %c",plugBoardencrypt);

    char rotor3encryption = rotorEncryption(plugBoardencrypt,3,true);
    printf("\nWheel 3 Encryption   : %c",rotor3encryption);

    char rotor2encryption = rotorEncryption(rotor3encryption,2,true);
    printf("\nWheel 2 Encryption   : %c",rotor2encryption);

    char rotor1encryption = rotorEncryption(rotor2encryption,1,true);
    printf("\nWheel 1 Encryption   : %c",rotor3encryption);

    char refEncryption    = reflection     (rotor1encryption       );
    printf("\nReflector Encryption : %c",refEncryption);

    rotor1encryption = rotorEncryption(refEncryption,   1,false);
    printf("\nWheel 1 Encryption   : %c",rotor3encryption);

    rotor2encryption = rotorEncryption(rotor1encryption,2,false);
    printf("\nWheel 2 Encryption   : %c",rotor2encryption);

    rotor3encryption = rotorEncryption(rotor2encryption,3,false);
    printf("\nWheel 3 Encryption   : %c",rotor3encryption);

    plugBoardencrypt = plugBoardEncryption(rotor3encryption);
    printf("\nPlugBoard Encryptions: %c",plugBoardencrypt);

    printf("\nOUTPUT               : %c",plugBoardencrypt);

    printf("\n");

    return plugBoardencrypt;
}
void readFile()
{
    int num;
    fptr = fopen("file1.txt","r");

    char string[MAX_LEN + 1]; // One extra byte needed
    // for the null character

    while(fgets(string, sizeof string, fptr) != NULL)
    {
        encryptString(string);
    }
    fclose(fptr);

}
int main()
{
    setupEnigma();
    getPlugBoardEncryptions();
    readFile();
    //rotorEncryption('A',3,true);
    //passThroughRotors('A');
    //printf("%s",encryptString("HELLOWORLD"));

    return 0;
}
