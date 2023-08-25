#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <crypt.h>

int tamanhoMaximo = 3;
char caracteres[] = "abc";
char *senhaCriptografada = "$6$7yCakIXevncmT6se$m002Lkf2BK6Qgyhc.c/PxMTvcmBAXYtIo"
                            "kUKvWwvB5H5zCt5HhhPOlV8ygebOcSsgNqG74whVwN.8UF9WaGfs/";

int verificaSenha(char* senhaTeste, char* senhaCriptografada) {
    char *senhaTesteCriptografada = crypt(senhaTeste, senhaCriptografada);  
    return strcmp(senhaTesteCriptografada, senhaCriptografada);
}

int GenerateAllPasswords(char *pwd, int pos, int siz)
{   
    int verifica = verificaSenha(pwd, senhaCriptografada);

    if(verifica == 0){
        printf("%s --> sim! Senha encontrada!\n", pwd);
        return 0;
    }

    if (pos < siz)
    {   
        for (int i = 0; i < tamanhoMaximo; i++)
        {
            char* newPwd = malloc(sizeof(char) * tamanhoMaximo);
            sprintf(newPwd, "%s%c", pwd,caracteres[i]);
            int aux = verificaSenha(newPwd, senhaCriptografada);

            GenerateAllPasswords(newPwd, pos + 1, siz);
        }
    }else if (pos >= siz && verifica != 0){
        printf("%s --> não\n",pwd);
    }
    
    return 1;
}

int main(int argc, char const *argv[])
{
    for(int i = 1; i <= tamanhoMaximo; i++){
        int verifica = GenerateAllPasswords("",0, i);
        if(verifica == 0){
            break;
        }
    }
}
