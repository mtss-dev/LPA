#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <crypt.h>

//Config padrão:
// int tamanhoMaximo = 3;
// char caracteres[] = "abc";
// char *senhaCriptografada = "$6$7yCakIXevncmT6se$m002Lkf2BK6Qgyhc.c/PxMTvcmBAXYtIo"
//                            "kUKvWwvB5H5zCt5HhhPOlV8ygebOcSsgNqG74whVwN.8UF9WaGfs/";

//Senha criptografada questão 1 (Senha encontrada = 0451)
// int tamanhoMaximo = 4;
// char caracteres[] = "0123456789";
// char *senhaCriptografada = "$6$LrSF5BAseToYYHJ0$SYY1avj8FRoRGpn.1kPXuZ6Xn5WTl2kL3"
//                            "hxc3yMWdDUyz4c/Ac3Av3WO8Q9LciP8o4c9WaeLcgxIXWaHpJMFb.";

//Senha criptografada questão 2 (Senha encontrada = cpp)
// int tamanhoMaximo = 3;
// char caracteres[] = "abcdefghijklmnopqrstuvwxyz";
// char *senhaCriptografada = "$6$rMAk28dVkWjpYoA3$SkWbPYqEB8O/10ryvvjm1qN9BOrkeBOXp"
//                            "JScVSGDL5L88OIs0UCBuP.pnd9TQ6SBx60dLKwR9WAzfnLtvjGvj.";

//Senha criptografada questão 3 (Senha encontrada = )
int tamanhoMaximo = 5;
char caracteres[] = "0123456789";
char *senhaCriptografada = "$6$l2xE4w9twgjtnZBz$9YK9krslZFraLffy5VNiahAfT.xZNvB54"
                           "j91DMCMIoVFvj335ZKxb11qgVMn.KzU2GqVPPyS2FTBqPSciYq761";

int verificaSenha(char* senhaTeste, char* senhaCriptografada) {
    char *senhaTesteCriptografada = crypt(senhaTeste, senhaCriptografada);  
    return strcmp(senhaTesteCriptografada, senhaCriptografada);
}

//Função para a questão 1
// void GenerateAllPasswords(char *pwd, int pos, int siz)
// {   
//     if (pos < siz)
//     {   
//         for (int i = 0; i < tamanhoMaximo; i++)
//         {
//             char* newPwd = malloc(sizeof(char) * tamanhoMaximo);
//             sprintf(newPwd, "%s%c", pwd,caracteres[i]);
//             GenerateAllPasswords(newPwd, pos + 1, siz);
//         }
//     }else{
//         printf("%s\n", pwd);
//     }
// }

//Função para a questão 2
void GenerateAllPasswords(char *pwd, int pos, int siz)
{   
    if (pos < siz)
    {   
        for (int i = 0; i < strlen(caracteres); i++)
        {
            char* newPwd = malloc(sizeof(char) * tamanhoMaximo);
            sprintf(newPwd, "%s%c", pwd,caracteres[i]);
            
            GenerateAllPasswords(newPwd, pos + 1, siz);
            
        }
    }else{
        int aux = verificaSenha(pwd, senhaCriptografada);

        if(aux == 0){
                printf("%s --> sim! Senha encontrada!\n", pwd);
                exit(0);
        }else{
            printf("%s --> não\n",pwd); 
        }

        
    }
}

int main(int argc, char const *argv[])
{
    //for para a questão 1
    // for(int i = 1; i <= tamanhoMaximo; i++){
    //     GenerateAllPasswords("",0, i);
    // }

    //for para a questão 2
    for(int i = 1; i <= tamanhoMaximo; i++){
        GenerateAllPasswords("",0, i);
    }
    
    return 0;
}